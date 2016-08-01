def configure(conf):
    conf.recurse('arm', once=False)
    conf.env.append_unique('CFLAGS', ['-march=armv7-a'])
    conf.env.append_unique('CXXFLAGS', ['-march=armv7-a'])
    conf.env.append_unique('LDFLAGS', ['-march=armv7-a'])
