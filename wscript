VERSION = "0.2.0"
APPNAME = "BugEngine"

top = '.'
out = 'bld/.waf'

def options(opt):
    opt.recurse('mak')

def configure(conf):
    conf.recurse('mak')

def build(bld):
    bld.recurse('mak')
    bld.recurse('src/build.py')
    bld.recurse('mak', name='plugins', once=False)

