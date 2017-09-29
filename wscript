"""
    Main wscript for the BugENgine
"""


VERSION = "0.2.0"
APPNAME = "BugEngine"

top = '.'           #pylint: disable=invalid-name
out = 'bld/.waf'    #pylint: disable=invalid-name


def options(opt):
    "recursively declare options to the parser"
    opt.recurse('mak')


def configure(conf):
    "entry point for build system configuration"
    conf.recurse('mak')


def build(bld):
    "set up build targets and executes the build"
    bld.recurse('mak')
    bld.recurse('src/build.py')
    bld.recurse('mak', name='plugins', once=False)
