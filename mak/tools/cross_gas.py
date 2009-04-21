from Configure import conftest
import Options
import cross_gcc


@conftest
def find_cross_gas(conf):
	try: target = Options.options.target
	except: target = None
	if not target:
		target = conf.env['GCC_TARGET']
	version = conf.env['GCC_VERSION']
	if target:
		v = conf.env
		if not v['AS']: v['AS'] = conf.find_program(target+'-as', var='AS', path_list=v['GCC_PATH'])
		if not v['AS']: v['AS'] = conf.find_program(target+'-gas', var='AS', path_list=v['GCC_PATH'])
		if not v['AS'] and target == v['GCC_DEFAULT_TARGET']:
			v['AS'] = conf.find_program('as', var='AS')
			if not v['AS']: v['AS'] = conf.find_program('gas', var='AS')
		if not v['AS']: conf.fatal('unable to find as for target %s' % target)

		if not v['AR']: v['AR'] = conf.find_program(target+'-ar', var='AR', path_list=v['GCC_PATH'])
		if not v['AR'] and target == v['GCC_DEFAULT_TARGET']:
			v['AR'] = conf.find_program('ar', var='AR')
		if not v['AR']: conf.fatal('unable to find ar for target %s' % target)

		if not v['RANLIB']: v['RANLIB'] = conf.find_program(target+'-ranlib', var='RANLIB', path_list=v['GCC_PATH'])
		if not v['RANLIB'] and target == v['GCC_DEFAULT_TARGET']:
			v['RANLIB'] = conf.find_program('ar', var='RANLIB')
		if not v['RANLIB']: conf.fatal('unable to find ranlib for target %s' % target)

	conf.check_tool('gas ar')

detect = '''
get_native_gcc_target
find_cross_gas
'''
