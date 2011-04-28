from Configure import conftest


@conftest
def find_opencc(conf):
	v['CC'] = conf.find_program('opencc', var='CC')
	v['CXX'] = conf.find_program('openCC', var='CXX')
	conf.load('gcc')

configure = '''
find_opencc
'''
