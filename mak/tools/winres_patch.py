from waflib.Tools import c_preproc, winres
import os


exec_command_winrc_old = winres.winrc.exec_command
def exec_command_winrc(self, *k, **kw):
    if os.pathsep == '\\':
        if isinstance(k[0], list):
            k = ([i.replace('\\', '/') for i in k[0]],) + k[1:]
    return exec_command_winrc_old(self, *k, **kw)
winres.winrc.exec_command = exec_command_winrc


def configure(conf):
    conf.load('winres')

