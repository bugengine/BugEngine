import os


def build(build_context):
    os.environ['LD_NOVERSION'] = '1'
    environ = getattr(build_context, 'environ', None)
    if environ:
        environ['LD_NOVERSION'] = '1'
