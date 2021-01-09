import os


def setup(conf):
    os.environ['LD_NOVERSION'] = '1'
    environ = getattr(conf, 'environ', None)
    if environ:
        environ['LD_NOVERSION'] = '1'
