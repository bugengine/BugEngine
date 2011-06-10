from waflib.TaskGen import feature, before, extension, after
from waflib.Configure import conf
from waflib.Logs import warn,pprint
from waflib import Options, Utils
import re
import os
from mak.waflib.Errors import ConfigurationError

@conf
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

def allarchs(arch):
	if arch == 'x86':
		return [('-m64', 'amd64'), ('-m32', 'x86')]
	elif arch == 'amd64':
		return [('-m32', 'x86'), ('-m64', 'amd64')]
	elif arch == 'arm':
		return [('', 'arm')]
	elif arch == 'mips':
		return [('-m64', 'mips64'), ('-m32', 'mips')]
	elif arch == 'mips64':
		return [('-m32', 'mips'), ('-m64', 'mips64')]
	elif arch == 'mipsel':
		return [('-m64', 'mipsel64'), ('-m32', 'mipsel')]
	elif arch == 'mipsel64':
		return [('-m32', 'mipsel'), ('-m64', 'mipsel64')]
	elif arch == 'powerpc' or arch == 'ppc':
		return [('-m64', arch+'64'), ('-m32', arch)]
	elif arch == 'powerpc64' or arch == 'ppc64':
		return [('-m32', arch[:-2]), ('-m64', arch)]
	elif arch == 'spu':
		return ['', 'spu']
	else:
		warn('unknown architecture : %s' % arch)
		return [('', arch)]


@conf
def add_gcc_to_env(conf, version, toolchaindir, gcc_target, flag):
	newenv = conf.env
	newenv['GCC_VERSION']	= version
	newenv['GCC_TARGET']	= gcc_target
	newenv['GCC_FLAGS']		= [flag]
	newenv['GCC_PATH']		= [os.path.abspath(os.path.join(toolchaindir, 'bin')),
							   os.path.abspath(os.path.join(toolchaindir, gcc_target, 'bin'))]
	conf.load('cross_gcc', tooldir='mak/tools')


@conf
def create_gcc_env(conf, version, toolchaindir, target, platform, originalarch, add_gcc_flags_to_env, add_platform_flags_to_env):
	worked = False
	for opt,arch in allarchs(originalarch):
		name = 'gcc-%s-%s-%s' %(platform, arch, version.replace('-', '_'))
		conf.setenv(name, conf.env.derive())
		try:
			add_gcc_to_env(conf, version, toolchaindir, target, opt)
			if platform == 'win32':
				conf.env['WINRC'] = conf.find_program('%s-windres' % target, path_list = conf.env['PATH'], var='WINRC')
				conf.load('winres')
			add_gcc_flags_to_env(conf)
			add_platform_flags_to_env(conf, name, arch)

			conf.recurse(os.path.join('..', '..', '..', 'target', 'archs', arch), once=False)

			pprint('GREEN', 'configure for tool %s' % name)
			conf.variant = ''
			conf.env['BUILD_VARIANTS'].append(name)
			worked = True
		except Exception as e:
			conf.variant = ''
	if not worked:
		arch = originalarch
		name = 'gcc-%s-%s-%s' %(platform, arch, version.replace('-', '_'))
		conf.setenv(name, conf.env.derive())
		try:
			add_gcc_to_env(conf, version, toolchaindir, target, '')
			if platform == 'win32':
				conf.env['WINRC'] = conf.find_program('%s-windres' % target, path_list = conf.env['PATH'], var='WINRC')
				conf.load('winres')
			add_gcc_flags_to_env(conf)
			add_platform_flags_to_env(conf, name, arch)

			conf.recurse(os.path.join('..', '..', '..', 'target', 'archs', arch), once=False)

			pprint('GREEN', 'configure for tool %s' % name)
			conf.variant = ''
			conf.env['BUILD_VARIANTS'].append(name)
		except Exception as e:
			conf.variant = ''

def parse_gcc_target(target):
	archs = [ ('i686-w64', 'amd64'),
			  ('i386', 'x86'),
			  ('i486', 'x86'),
			  ('i586', 'x86'),
			  ('i686', 'x86'),
			  ('arm-eabi', 'arm'),
			  ('arm', 'arm'),
			  ('mipsel', 'mips'),
			  ('mips', 'mips'),
			  ('Gekko', 'mips'),
			  ('x86_64', 'amd64'),
			  ('x86', 'x86'),
			  ('amd64', 'amd64'),
			  ('powerpc', 'powerpc'),
			  ('psp', 'mips'),
			  ('mingw32', 'x86'),
			  ('ppu', 'powerpc'),
			  ('spu', 'spu'),
			  ('ee', 'mipsel64'),
			  ('iop', 'mipsel')]
	for gccname,aname in archs:
		if target.find(gccname) != -1:
				return aname

@conf
def get_available_gcc(conf):
	toolchaindirs=set([])
	conf.env['GCC_TARGETS'] = []
	for dir in os.environ['PATH'].split(':'):
		toolchaindirs.add(os.path.normpath(os.path.join(dir, '..')))
	for toolchaindir in toolchaindirs:
		if not os.path.isdir(os.path.join(toolchaindir,'lib')):
			continue
		for lib in os.listdir(os.path.join(toolchaindir,'lib')):
			if lib.startswith('gcc'):
				libdir = os.path.join(toolchaindir, 'lib', lib)
			else:
				continue
			if not os.path.isdir(libdir):
				continue
			for subdir in ['', 'gcc']:
				libdir = os.path.join(libdir, subdir)
				if not os.path.isdir(libdir):
					continue
				for target in os.listdir(libdir):
					if not os.path.isdir(os.path.join(libdir, target)):
						continue
					if target in ['.svn', '.cvs']:
						continue
					for version in os.listdir(os.path.join(libdir, target)):
						if not os.path.isdir(os.path.join(libdir, target, version)):
							continue
						if version in ['.svn', '.cvs']:
							continue
						if os.path.islink(os.path.join(libdir, target, version)):
							continue
						if not os.path.isdir(os.path.join(libdir, target, version, 'include')):
							continue
						arch = parse_gcc_target(target) or 'unknown'
						conf.env['GCC_TARGETS'].append((version, toolchaindir, target, arch))
	conf.env['GCC_TARGETS'].sort(key= lambda x: (x[2], x[3], x[0]))


@conf
def find_cross_gcc(conf):
	target = conf.env['GCC_TARGET']
	version = conf.env['GCC_VERSION']
	versionsmall = '.'.join(version.split('.')[0:2])
	versionverysmall = ''.join(version.split('.')[0:2])
	if target:
		v = conf.env
		for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall, '']:
			if conf.find_program(target+'-gcc'+name, var='CC', path_list=v['GCC_PATH'], mandatory=False, silent=True):
				break
		if not v['CC']: conf.fatal('unable to find gcc for target %s' % target)
		cmd = [v['CC']] + conf.env['GCC_FLAGS'] + ['-dM', '-E', '-']
		try:
			p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
			p.stdin.write('\n'.encode())
			out = p.communicate()[0]
			if p.returncode != 0:
				conf.fatal('could not run the compiler %r' % str(cmd))
		except:
			conf.fatal('could not run the compiler %r' % str(cmd))


		for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall, '']:
			if conf.find_program(target+'-g++'+name, var='CXX', path_list=v['GCC_PATH'], mandatory=False, silent=True):
				break
		if not v['CXX']: conf.fatal('unable to find g++ for target %s' % target)

		for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall, '']:
			if conf.find_program(target+'-cpp'+name, var='CPP', path_list=v['GCC_PATH'], mandatory=False, silent=True):
				break
		if not v['CPP']:
			for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall, '']:
				if conf.find_program('cpp'+name, var='CPP', path_list=v['GCC_PATH'], mandatory=False, silent=True):
					break
		if not v['CPP']: conf.fatal('unable to find cpp for target %s' % target)

		if not v['AS']: v['AS'] = v['CC']

		for ar in [target+'-gar', target+'-ar', 'gar', 'ar']:
			if conf.find_program(ar, var='AR', path_list=v['GCC_PATH'], mandatory=False, silent=True):
				break
		if not v['AR']:
			for ar in ['gar', 'ar']:
				if conf.find_program(ar, var='AR', mandatory=False, silent=True):
					break
		if not v['AR']: conf.fatal('unable to find ar for target %s' % target)

		for ranlib in [target+'-ranlib', target+'-granlib', 'ranlib', 'granlib']:
			if conf.find_program(ranlib, var='RANLIB', path_list=v['GCC_PATH'], mandatory=False, silent=True):
				break
		if not v['RANLIB']:
			for ranlib in ['ranlib', 'granlib', 'ranlib', 'granlib']:
				if conf.find_program(ranlib, var='RANLIB', mandatory=False, silent=True):
					break
		if not v['RANLIB']: conf.fatal('unable to find ranlib for target %s' % target)
		if not v['WINRC']:
			if not conf.find_program('mingw32-windres', var='WINRC', path_list=v['GCC_PATH'], mandatory=False, silent=True):
				conf.find_program('windres', var='WINRC', path_list=v['GCC_PATH'], mandatory=False, silent=True)
	conf.load('gcc gxx gas')

@conf
def add_standard_gcc_flags(conf):
	v = conf.env
	v.append_unique('ASFLAGS', '-c')
	v['CFLAGS'] = v['GCC_FLAGS']
	v['CXXFLAGS'] = v['GCC_FLAGS']
	v['LINKFLAGS'] = v['GCC_FLAGS']

	v['CFLAGS_warnnone'] = ['-w']
	v['CXXFLAGS_warnnone'] = ['-w']
	v['CFLAGS_warnall'] = ['-std=c99', '-Wall', '-Wextra', '-pedantic', '-Winline', '-Wno-unknown-pragmas', '-Wno-unused-parameter', '-Werror']
	v['CXXFLAGS_warnall'] = ['-Wall', '-Wextra', '-Wno-unknown-pragmas', '-Wno-unused-parameter', '-Werror', '-Wno-sign-compare']

	v['CFLAGS_debug'] = ['-pipe', '-g', '-D_DEBUG']
	v['CXXFLAGS_debug'] = ['-pipe', '-g', '-D_DEBUG', '-Wno-invalid-offsetof']
	v['LINKFLAGS_debug'] = ['-pipe', '-g']
	v['ASFLAGS_debug'] = ['-pipe', '-g', '-D_DEBUG']

	v['CFLAGS_profile'] = ['-pipe', '-g', '-DNDEBUG', '-O3']
	v['CXXFLAGS_profile'] = ['-pipe', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions', '-Wno-invalid-offsetof']
	v['ASFLAGS_profile'] = ['-pipe', '-g', '-DNDEBUG', '-O3']
	v['LINKFLAGS_profile'] = ['-pipe', '-g', '-s']

	v['CFLAGS_final'] = ['-pipe', '-g', '-DNDEBUG', '-O3']
	v['CXXFLAGS_final'] = ['-pipe', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions', '-Wno-invalid-offsetof']
	v['ASFLAGS_final'] = ['-pipe', '-g', '-DNDEBUG', '-O3']
	v['LINKFLAGS_final'] = ['-pipe', '-g', '-s']


configure = '''
get_native_gcc_target
find_cross_gcc
add_standard_gcc_flags
'''

