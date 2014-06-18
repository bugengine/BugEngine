import py_bugengine

for prop in dir(py_bugengine):
    print('%s=%s'%(prop, getattr(py_bugengine, prop)))
