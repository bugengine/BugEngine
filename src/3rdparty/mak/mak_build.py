from waflib.TaskGen import feature

def build(bld):
	mak = bld(target = 'build.mak', source = [])
	def add_files(p, path):
		pass
	add_files(mak, bld.bugenginenode.make_node('mak'))
	mak.features.append('Makefile')
	mak.source_nodes = [bld.bugenginenode.make_node('mak')]


@feature('Makefile')
def dummy_makefile(self):
	pass
