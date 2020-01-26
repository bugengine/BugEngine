import os


def options(opt):
    pass


def configure(conf):
    environ = getattr(conf, 'environ', os.environ)
    environ['PATH'] = os.pathsep.join([conf.bugenginenode.make_node("mak/host/win32/bin").abspath(), environ['PATH']])


def build(bld):
    environ = getattr(bld, 'environ', os.environ)
    environ['PATH'] = os.pathsep.join([bld.bugenginenode.make_node("mak/host/win32/bin").abspath(), environ['PATH']])


def plugins(bld):
    pass


from waflib.Tools import winres
from waflib import Task

cls = Task.classes.get('winrc', None)
derived_class = type('winrc', (cls, ), {})


def exec_command_rc(self, *k, **kw):
    def shortname(longname):
        def short(name):
            if len(name) > 8:
                return name[0:7] + '~1'
            else:
                return name

        return '/'.join([short(name) for name in longname.split(os.path.sep)])

    l = []
    for a in k[0]:
        if a[0:2] == '-I':
            if a.find(' ') != -1:
                l.append('-I%s' % shortname(a[2:]))
            else:
                l.append(a)
        else:
            l.append(a)
    return super(derived_class, self).exec_command(l, **kw)


def exec_command(self, *k, **kw):
    if self.env['CC_NAME'] != 'msvc':
        return self.exec_command_rc(*k, **kw)
    else:
        return super(derived_class, self).exec_command(*k, **kw)


# Chain-up monkeypatch needed since exec_command() is in base class API
derived_class.exec_command = exec_command
derived_class.exec_command_rc = exec_command_rc
