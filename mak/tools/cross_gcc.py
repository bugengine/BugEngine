from Configure import conftest
import Options

def set_options(opt):
	opt.add_option('--target', action='store', default='', help='select target for GCC')

@conftest
def get_native_gcc_target(conf):
	if not conf.env['GCC_NATIVE_TARGET']:
		cmd = ['gcc', '-v']
		try:
			p = Utils.pproc.Popen(cmd, stdin=Utils.pproc.PIPE, stdout=Utils.pproc.PIPE, stderr=Utils.pproc.PIPE)
			out = p.communicate()[0]
		except:
			return

		out = str(out).split('\n')

		for line in out:
			if line.startswith('Target:'):
				conf.env['GCC_NATIVE_TARGET'] = line.plit()[1]

@conftest
def find_cross_gcc(conf):
	try: target = Options.options.target
	except: target = None
	if not target:
		target = conf.env['GCC_TARGET']
	version = conf.env['GCC_VERSION']
	if target:
		v = conf.env
		if not v['CC']: v['CC'] = conf.find_program(target+'-gcc', var='CC', path_list=v['GCC_PATH'])
		if not v['CC']: conf.fatal('unable to find gcc for target %s', target)

		if not v['CPP']: v['CPP'] = conf.find_program(target+'-cpp', var='CPP', path_list=v['GCC_PATH'])
		if not v['CPP']: conf.fatal('unable to find cpp for target %s', target)

		if not v['AR']: v['AR'] = conf.find_program(target+'-ar', var='AR', path_list=v['GCC_PATH'])
		if not v['AR'] and target == v['GCC_DEFAULT_TARGET']:
			v['AR'] = conf.find_program('ar', var='AR')
		if not v['AR']: conf.fatal('unable to find ar for target %s', target)

		if not v['RANLIB']: v['RANLIB'] = conf.find_program(target+'-ranlib', var='RANLIB', path_list=v['GCC_PATH'])
		if not v['RANLIB'] and target == v['GCC_DEFAULT_TARGET']:
			v['RANLIB'] = conf.find_program('ar', var='RANLIB')
		if not v['RANLIB']: conf.fatal('unable to find ranlib for target %s', target)


	conf.check_tool('gcc')

detect = '''
get_native_gcc_target
find_cross_gcc
'''
