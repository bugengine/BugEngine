from Configure import conftest


@conftest
def find_opencc(conf):
	v['CC'] = conf.find_program('opencc', var='CC')
	v['CC'] = conf.find_program('openCC', var='CXX')
	conf.check_tool('gcc')

configure = '''
find_opencc
'''
