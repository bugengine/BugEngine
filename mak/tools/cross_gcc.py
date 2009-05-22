from Configure import conftest
import Options
import Utils
import re

def set_options(opt):
	opt.add_option('--target', action='store', default='', help='select target for GCC')

@conftest
def get_native_gcc_target(conf):
	if not conf.env['GCC_NATIVE_TARGET']:
		cmd = ['gcc', '-v']
		try:
			p = Utils.pproc.Popen(cmd, stdin=Utils.pproc.PIPE, stdout=Utils.pproc.PIPE, stderr=Utils.pproc.PIPE)
			out = p.communicate()[1]
		except:
			return

		out = str(out).split('\n')

		for line in out:
			if line.startswith('Target:'):
				conf.env['GCC_NATIVE_TARGET'] = line.split()[1]

def parse_gcc_target(target):
	os = [ ('mingw', 'win32'),
		   ('linux', 'linux'),
		   ('psp', 'psp'),
		   ('arm-eabi', 'nds'),
		   ('darwin', 'darwin'),
		   ('gekko', 'wii') ]
	archs = [ ('i386', 'x86'),
			  ('i486', 'x86'),
			  ('i586', 'x86'),
			  ('i686', 'x86'),
			  ('arm-eabi', 'arm7'),
			  ('gekko', 'mips'),
			  ('x86_64', 'amd64'),
			  ('powerpc', 'powerpc'),
			  ('psp', 'powerpc')]
	foundpname = None
	foundaname = None
	for gccname,pname in os:
		if target.find(gccname) != -1:
			foundpname = pname
	for gccname,aname in archs:
		if target.find(gccname) != -1:
			foundaname = aname
	return foundpname,foundaname

 
@conftest
def find_cross_gcc(conf):
	try: target = Options.options.target
	except: target = None
	if not target:
		target = conf.env['GCC_TARGET']
	version = conf.env['GCC_VERSION']
	if target:
		v = conf.env
		v['GCC_CONFIGURED_PLATFORM'],v['GCC_CONFIGURED_ARCH'] = parse_gcc_target(target)
		if not v['CC']: v['CC'] = conf.find_program(target+'-gcc-'+version, var='CC', path_list=v['GCC_PATH'])
		if not v['CC']: conf.fatal('unable to find gcc for target %s' % target)

		if not v['CPP']: v['CPP'] = conf.find_program(target+'-cpp-'+version, var='CPP', path_list=v['GCC_PATH'])
		if not v['CPP']: v['CPP'] = conf.find_program(target+'-cpp', var='CPP', path_list=v['GCC_PATH'])
		if not v['CPP']: v['CPP'] = conf.find_program('cpp-'+version, var='CPP', path_list=v['GCC_PATH'])
		if not v['CPP']: v['CPP'] = conf.find_program('cpp-'+version[0:3], var='CPP', path_list=v['GCC_PATH'])
		if not v['CPP']: conf.fatal('unable to find cpp for target %s' % target)

		if not v['AR']: v['AR'] = conf.find_program(target+'-ar', var='AR', path_list=v['GCC_PATH'])
		if not v['AR']:
			v['AR'] = conf.find_program('ar', var='AR', path_list=v['GCC_PATH'])
		if not v['AR']: conf.fatal('unable to find ar for target %s' % target)

		if not v['RANLIB']: v['RANLIB'] = conf.find_program(target+'-ranlib', var='RANLIB', path_list=v['GCC_PATH'])
		if not v['RANLIB']:
			v['RANLIB'] = conf.find_program('ar', var='RANLIB', path_list=v['GCC_PATH'])
		if not v['RANLIB']: conf.fatal('unable to find ranlib for target %s' % target)

	conf.check_tool('gcc')

detect = '''
get_native_gcc_target
find_cross_gcc
'''
