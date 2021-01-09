import os
import sys
import tarfile
import tempfile
from waflib import Logs, Errors
if sys.version_info < (3, ):
    import urllib2 as request
else:
    from urllib import request

HOST_WIN32_TOOLS = 'https://github.com/bugengine/BugEngine/releases/download/v0.0/host-win32.tgz'


def configure(conf):
    path = conf.bldnode.make_node('host/win32')
    if not os.path.isdir(path.abspath()):
        conf.start_msg('setting up Win32 tools')
        path.parent.mkdir()
        try:
            pkg = request.urlopen(HOST_WIN32_TOOLS)
        except Exception as e:
            raise Errors.WafError('failed to download package "%s": %s' % (HOST_WIN32_TOOLS, e))
        with tempfile.TemporaryFile(mode='w+b') as archive_file:
            Logs.pprint('PINK', 'downloading...', sep=' ')
            archive_file.write(pkg.read())
            archive_file.seek(0)
            Logs.pprint('PINK', 'unpacking...', sep=' ')
            archive = tarfile.open(fileobj=archive_file, mode='r')
            root_path = ''
            info = archive.getmembers()[0]
            root_path = info.name
            archive.extractall(path.parent.abspath())
            conf.end_msg('OK')

    environ = getattr(conf, 'environ', os.environ)
    environ['PATH'] = os.pathsep.join([os.path.join(path.abspath(), 'bin'), environ['PATH']])
