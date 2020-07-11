from waflib import Errors


def build(bld):
    bld.thirdparty('3rdparty.scripting.python37', feature='python')
