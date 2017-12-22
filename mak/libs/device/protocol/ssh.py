import os
import sys
import subprocess
import threading
import base64
import bz2
try:
    import cPickle as pickle
except ImportError:
    import pickle
from . import Protocol


run_cmd_code = """result=None
try:
  import cPickle as pickle
except ImportError:
  import pickle

def run_cmd(command):
  try:
    global result
    result = None
    exec("global result\\n" + command, globals(), locals())
    pickle.dump(result, stdout, protocol=2)
  except Exception as e:
    pickle.dump(e, stdout, protocol=2)
  finally:
    sys.stderr.flush()

"""

class SSHProtocol(Protocol):
    name = 'ssh'

    class Connection:
        def __init__(self, hostname, auth):
            self.hostname = hostname
            self.auth = auth

        def __enter__(self):
            self.process = subprocess.Popen(['ssh', '-y', '-o', 'UserKnownHostsFile=/dev/null',
                                             '-o', 'StrictHostKeyChecking=no', self.hostname,
                                             'python -i -u -c \'import pickle; import sys; sys.ps1=""; sys.ps2=""; stdout=sys.stdout if sys.version_info[0]<=2 else sys.stdout.buffer\''],
                                             stdin=subprocess.PIPE,
                                             stdout=subprocess.PIPE,
                                             stderr=subprocess.PIPE,
                                             bufsize=0)
            self.t = threading.Thread(target = SSHProtocol.Connection._print_stderr, args = (self,))
            self.t.start()
            self.process.stdin.write(run_cmd_code.encode('utf8'))
            self.process.stdin.flush()
            return self

        def __exit__(self, type, value, traceback):
            self.process.stdin.close()
            self.process.wait()
            self.t.join()

        def _print_stderr(self):
            for line in self.process.stderr.readlines():
                if not isinstance(line, str):
                    line = line.decode(sys.stderr.encoding)
                line = line.strip()
                if line:
                    print(line)

        def _forward_stdin(self):
            for line in sys.stdin.readlines():
                self.process.stdin.write(line.encode('utf8'))
                self.process.stdin.flush()
            self.process.stdin.close()


        def _run(self, command):
            # print('running command:\n%s' % command)
            self.process.stdin.write(('run_cmd("""%s""")\n' % command).encode('utf8'))
            self.process.stdin.flush()
            try:
                result = pickle.load(self.process.stdout)
                if isinstance(result, BaseException):
                    raise result
            except Exception as e:
                raise
            return result

        def status(self):
            try:
                result = self._run('import platform\nresult = [platform.system(), platform.machine()]')
            except EOFError:
                return ('offline', None, None, None)
            return ('online', result[0], result[1], [result[1]])

        def mkdir(self, dir):
            self._run('import os\nos.makedirs("%s")' % dir)

        def put(self, file, destination):
            uploaded = 0
            with open(file, 'rb') as file_object:
                data = bz2.compress(file_object.read())
            try:
                self._run('import os\nimport bz2\nglobal decompressor; decompressor = bz2.BZ2Decompressor()\nglobal data; data = b""\n')
                uploaded = 0
                chunk_size = 1024
                while uploaded < len(data):
                    self._run('import base64\nglobal data\ndata += decompressor.decompress(base64.b64decode("%s"))' % (base64.b64encode(data[uploaded:uploaded+chunk_size])))
                    uploaded += chunk_size
                    sys.stdout.write('\r[%3d%%] uploading %s...' % (100*uploaded / len(data), file))
                sys.stdout.write('\n')
            except (BaseException, KeyboardInterrupt):
                print('\naborted; deleting %s' % destination)
                try:
                    self._run('import os\nos.unlink("%s")' % (destination))
                except BaseException as e:
                    pass
                raise
            else:
                self._run('import os\n'
                          'filename="%s"\n'
                          'if os.path.isdir(filename):\n'
                          '  filename=os.path.join(filename, "%s")\n'
                          'with open(filename, "wb") as f:\n'
                          '  f.write(data)' % (destination, os.path.basename(file)))

        def pull(self, file, destination):
            data = b''

            self._run('import os\n'
                      'import bz2\n'
                      'global data\n'
                      'global uploaded\n'
                      'with open("%s", "rb") as f:\n'
                      '  data = bz2.compress(f.read())\n'
                      'uploaded = 0\n' % file)
            while True:
                uploaded, total, d = self._run('global data\n'
                                               'global uploaded\n'
                                               'import base64\n'
                                               'd = base64.b64encode(data[uploaded:uploaded+1024])\n'
                                               'uploaded += 1024\n'
                                               'if uploaded > len(data): uploaded = len(data)\n'
                                               'result = (uploaded, len(data), d)')
                sys.stdout.write('\r[%3d%%] downloading %s...' % (100*uploaded / total, file))
                data += base64.b64decode(d)
                if uploaded == total:
                    break
            sys.stdout.write('\n')

            if os.path.isdir(destination):
                destination = os.path.join(destination, os.path.basename(file))
            with open(destination, 'wb') as file_object:
                file_object.write(bz2.decompress(data))

        def start_shell(self):
            process = subprocess.Popen(['ssh', '-y', '-o', 'UserKnownHostsFile=/dev/null',
                                        '-o', 'StrictHostKeyChecking=no', self.hostname])
            process.communicate()


    def __init__(self, connection):
        try:
            self.auth, self.hostname = connection.split('@')
        except ValueError:
            self.hostname = connection
            self.auth = None
        else:
            pass

    def connect(self):
        return SSHProtocol.Connection(self.hostname, self.auth)

    @classmethod
    def help(self, out):
        out.write('    Connects to a machine through SSH.\n')
        out.write('    The target machine needs to have an SSH server runing.\n')
        out.write('    Syntax: %s://user:password@host\n' % self.name)
        out.write('            %s://id_rsa@host\n' % self.name)
