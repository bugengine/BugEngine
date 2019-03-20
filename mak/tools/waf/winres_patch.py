from waflib.Tools import c_preproc, winres
import os


exec_command_winrc_old = winres.winrc.exec_command
def exec_command_winrc(self, *k, **kw):
    if os.sep == '\\':
        if isinstance(k[0], list):
            k = ([i.replace('\\', '/') for i in k[0]],) + k[1:]
    return exec_command_winrc_old(self, *k, **kw)
winres.winrc.exec_command = exec_command_winrc


def configure(conf):
    conf.load('winres')
    v = conf.env
    if v.CC_NAME=='msvc':
        v.WINRC_TGT_F='/fo'
        v.WINRC_SRC_F=''
    else:
        v.WINRC_TGT_F='-o'
        v.WINRC_SRC_F='-i'

