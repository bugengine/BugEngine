from waflib.TaskGen import feature

def build(bld):
    mak = bld(target = 'engine.mak', source = [])
    def add_files(p, path):
        pass
    add_files(mak, bld.bugenginenode.make_node('mak'))
    mak.features.append('Makefile')
    mak.source_nodes = [bld.bugenginenode.make_node('mak')]
    mak.module_path = mak.source_nodes[0].path_from(bld.srcnode).replace('/', '.')


@feature('Makefile')
def dummy_makefile(self):
    pass
