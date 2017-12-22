import shlex
import subprocess
import sys

class Gdb:
    def __init__(self, gdb_path, options=[]):
        self.process = subprocess.Popen([gdb_path] + options,
                                        stdin=subprocess.PIPE,
                                        stdout=sys.stdout,
                                        stderr=sys.stderr)

    def process_command(self, cmd):
        return cmd

    def run_command(self, cmd):
        cmd = shlex.split(cmd)
        cmd = self.process_command(cmd)
        try:
            self.process.stdin.write(' '.join(cmd))
            self.process.stdin.write('\n')
        except ValueError:
            pass

    def run(self):
        while self.process.poll() is None:
            cmd = sys.stdin.readline()
            if cmd:
                self.run_command(cmd.strip())
            else:
                self.process.communicate()

if __name__ == '__main__':
    Gdb('gdb', ['/bin/ls']).run()
