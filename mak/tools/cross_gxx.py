from Configure import conftest
import Options
import cross_gcc


@conftest
def find_cross_gxx(conf):
	try: target = Options.options.target
	except: target = None
	if not target:
		target = conf.env['GCC_TARGET']
	version = conf.env['GCC_VERSION']
	if target:
		v = conf.env
		if not v['CXX']: v['CXX'] = conf.find_program(target+'-g++', var='CXX', path_list=v['GCC_PATH'])
		if not v['CXX']: v['CXX'] = conf.find_program(target+'-c++', var='CXX', path_list=v['GCC_PATH'])
		if not v['CXX']: conf.fatal('unable to find g++ for target %s' % target)

	conf.check_tool('gxx')

detect = '''
get_native_gcc_target
find_cross_gxx
'''
