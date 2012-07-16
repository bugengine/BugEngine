from waflib.TaskGen import feature, before, extension, after
from waflib.Configure import conf
from waflib.Logs import warn,pprint
from waflib import Options, Utils
import re
import os
import sys
from mak.waflib.Errors import ConfigurationError

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
def get_native_icc_target(conf, icc):
	cmd = [icc, '-V']
	try:
		p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
		out = p.communicate()[1]
	except:
		return (None, None)

	if not isinstance(out, str):
		out = out.decode(sys.stdout.encoding)
	out = out.split('\n')
	target = os.uname()[0].lower()
	arch = 'x86'
	version = None
	for line in out:
		words = line.split()
		while len(words):
			if words[0] == 'Intel(R)' and words[1] == '64':
				arch = 'amd64'
			if words[0] == 'ID:':
				if words[1].startswith('m_cproc_'):
					version = words[1][10:].split('.')[0]
			if words[0] == 'Version':
				version = words[1].split('.')[0]
			words = words[1:]
	return (target+'-'+arch, version)



@conf
def get_native_clang_target(conf, clang):
	cmd = [clang, '-v']
	try:
		p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
		out = p.communicate()[1]
	except:
		return (None, None)

	if not isinstance(out, str):
		out = out.decode(sys.stdout.encoding)
	out = out.split('\n')
	target = None
	version = None

	for line in out:
		if line.startswith('Target:'):
			target = line.split()[1]
		if line.find('clang version ') != -1:
			line = line.split()
			while line[0] != 'clang' and line[1] != 'version':
				line = line[1:]
			version = line[2].split('-')[0]
	return (target, version)


@conf
def get_native_gcc_target(conf):
	if not conf.env['GCC_NATIVE_TARGET']:
		cmd = ['gcc', '-v']
		try:
			p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
			out = p.communicate()[1]
		except:
			return

		out = str(out).split('\n')

		for line in out:
			if line.startswith('Target:'):
				conf.env['GCC_NATIVE_TARGET'] = line.split()[1]
				conf.env['GCC_NATIVE_ARCH'] = parse_gcc_target(conf.env['GCC_NATIVE_TARGET'])
			if line.startswith('gcc version '):
				conf.env['GCC_NATIVE_VERSION'] = line.split()[2]

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
def add_gcc_to_env(conf, version, toolchaindir, gcc_target, flag, gcc, gxx):
	newenv = conf.env
	newenv['GCC_NAME'] = gcc
	newenv['GXX_NAME'] = gxx
	newenv['GCC_VERSION']	= version
	newenv['GCC_TARGET']	= gcc_target
	newenv['GCC_FLAGS']		= flag and [flag] or []
	newenv['GCC_PATH']		= [os.path.abspath(os.path.join(toolchaindir, '..', 'bin')),
							   os.path.abspath(os.path.join(toolchaindir, '..', '..', 'bin')),
							   os.path.abspath(os.path.join(toolchaindir, '..', gcc_target, 'bin')),
							   toolchaindir]
	conf.load('cross_gcc', tooldir='mak/tools')


@conf
def create_gcc_env(conf, version, toolchaindir, target, platform, originalarch, gcc, gxx, add_gcc_flags_to_env, add_platform_flags_to_env):
	worked = False
	tool = gcc.split('-')[0]
	for opt,arch in allarchs(originalarch):
		name = '%s-%s-%s-%s' %(platform, arch, tool, version.replace('-', '_'))
		if name in conf.env['BUILD_VARIANTS']:
			continue
		conf.setenv(name, conf.env.derive())
		try:
			conf.env.ENVNAME = name
			add_gcc_to_env(conf, version, toolchaindir, target, opt, gcc, gxx)
			if platform == 'win32':
				conf.env['WINRC'] = conf.find_program('%s-windres' % target, path_list = conf.env['PATH'], var='WINRC')
				conf.load('winres')
			add_gcc_flags_to_env(conf)
			add_platform_flags_to_env(conf, name, arch)

			conf.recurse(os.path.join(conf.mak, 'target', 'archs', arch), once=False)
			conf.recurse(os.path.join(conf.mak, 'libs'), once=False)

			pprint('GREEN', 'configure for tool %s' % name)
			conf.variant = ''
			conf.env['BUILD_VARIANTS'].append(name)
			worked = True
		except Exception as e:
			conf.variant = ''
	if not worked:
		arch = originalarch
		name = '%s-%s-%s-%s' %(platform, arch, tool, version.replace('-', '_'))
		if name in conf.env['BUILD_VARIANTS']:
			return
		conf.setenv(name, conf.env.derive())
		try:
			conf.env.ENVNAME = name
			add_gcc_to_env(conf, version, toolchaindir, target, '', gcc, gxx)
			if platform == 'win32':
				conf.env['WINRC'] = conf.find_program('%s-windres' % target, path_list = conf.env['PATH'], var='WINRC')
				conf.load('winres')
			add_gcc_flags_to_env(conf)
			add_platform_flags_to_env(conf, name, arch)

			conf.recurse(os.path.join(conf.mak, 'target', 'archs', arch), once=False)
			conf.recurse(os.path.join(conf.mak, 'libs'), once=False)

			pprint('GREEN', 'configure for tool %s' % name)
			conf.variant = ''
			conf.env['BUILD_VARIANTS'].append(name)
		except Exception as e:
			conf.variant = ''

def add_ld_so(conf, file, toolchaindirs):
	f = open(file, 'r')
	for line in f:
		line = line.strip()
		if not line:
			continue
		elif line.startswith('#'):
			continue
		elif line.startswith('include'):
			continue
		else:
			toolchaindirs.add(line)

@conf
def get_available_gcc(conf, paths=[]):
	toolchaindirs=set([])
	conf.env['GCC_TARGETS'] = []
	for dir in paths+[i for i in os.environ['PATH'].split(':') if i not in paths]:
		toolchaindirs.add(os.path.normpath(os.path.join(dir, '..', 'lib')))
		try:
			for f in os.listdir(os.path.join(dir, '..')):
				toolchaindirs.add(os.path.normpath(os.path.join(dir, '..', f, 'lib')))
		except:
			pass
	if os.path.isfile('/etc/ld.so.conf'):
		add_ld_so(conf, '/etc/ld.so.conf', toolchaindirs)
	if os.path.isdir('/etc/ld.so.conf.d'):
		for f in os.listdir('/etc/ld.so.conf.d'):
			add_ld_so(conf, '/etc/ld.so.conf.d/'+f, toolchaindirs)
	for toolchaindir in toolchaindirs:
		if not os.path.isdir(toolchaindir):
			continue
		for lib in os.listdir(os.path.join(toolchaindir)):
			if lib.startswith('gcc'):
				libdir = os.path.join(toolchaindir, lib)
			else:
				continue
			if not os.path.isdir(libdir):
				continue
			for subdir in ['', 'lib/gcc', 'gcc']:
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
						if libdir.find('llvm') != -1:
							gcc, gxx = 'llvm-gcc', 'llvm-g++'
						else:
							gcc, gxx = 'gcc', 'g++'
						conf.env['GCC_TARGETS'].append((version, toolchaindir, target, arch, gcc, gxx))
	conf.env['GCC_TARGETS'].sort(key= lambda x: (x[2], x[3], x[0]))

	for dir in paths+[i for i in os.environ['PATH'].split(':') if i not in paths]:
		if conf.find_program('icc', var='ICC', path_list=[dir], mandatory=False, silent = True):
			toolchaindir = os.path.split(conf.env.ICC)[0]
			target, version = conf.get_native_icc_target(conf.env.ICC)
			arch = parse_gcc_target(target) or 'unknown'
			conf.env['GCC_TARGETS'].append((version, toolchaindir, target, arch, 'icc', 'icpc'))
			del conf.env['ICC']

	if conf.find_program('clang', var='CLANG', mandatory=False, silent = True):
		toolchaindir = os.path.split(conf.env.CLANG)[0]
		target, version = conf.get_native_clang_target(conf.env.CLANG)
		arch = parse_gcc_target(target) or 'unknown'
		conf.env['GCC_TARGETS'].append((version, toolchaindir, target, arch, 'clang', 'clang++'))


@conf
def find_cross_gcc(conf):
	gcc = conf.env['GCC_NAME'] or 'gcc'
	gxx = conf.env['GXX_NAME'] or 'g++'
	target = conf.env['GCC_TARGET']
	version = conf.env['GCC_VERSION']
	conf.env.BROKEN_INITIALIZER = gcc == 'gcc' and version.split('.')[0] == '3'
	versionsmall = '.'.join(version.split('.')[0:2])
	versionverysmall = ''.join(version.split('.')[0:2])
	if target:
		v = conf.env
		for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall, versionverysmall, '']:
			if conf.find_program(target+'-'+gcc+name, var='CC', path_list=v['GCC_PATH'], mandatory=False, silent=True):
				break
		if not v['CC']:
			for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall, versionverysmall, '']:
				if conf.find_program(gcc+name, var='CC', path_list=v['GCC_PATH'], mandatory=False, silent=True):
					break
		if not v['CC']: conf.fatal('unable to find %s for target %s' % (gcc, target))
		cmd = [v['CC']] + conf.env['GCC_FLAGS'] + ['-dM', '-E', '-']
		try:
			p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
			p.stdin.write('\n'.encode())
			out = p.communicate()[0]
			if p.returncode != 0:
				conf.fatal('could not run the compiler %r' % str(cmd))
			out = str(out).split('\n')

			for line in out:
				if line.startswith('Target:'):
					target = line.split()[1]
					if thistarget != target:
						conf.fatal('mismatch target: %s instead of %s' % (thistarget, target))
				if line.find(' version ') != -1:
					versions = line.split()
					while versions[0] != 'version':
						versions = versions[1:]
					version = versions[1]
					if thisversion != version:
						conf.fatal('mismatch version: %s instead of %s' % (thisversion, version))
		except Exception as e:
			conf.fatal('could not run the compiler %r' % str(cmd))


		for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall, versionverysmall, '']:
			if conf.find_program(target+'-'+gxx+name, var='CXX', path_list=v['GCC_PATH'], mandatory=False, silent=True):
				break
		if not v['CXX']:
			for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall, versionverysmall, '']:
				if conf.find_program(gxx+name, var='CXX', path_list=v['GCC_PATH'], mandatory=False, silent=True):
					break
		if not v['CXX']: conf.fatal('unable to find %s for target %s' % (gxx, target))
		conf.find_program(target+'-gdb', var='GDB', path_list=v['GCC_PATH'], mandatory=False, silent=True)

		for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall, versionverysmall, '']:
			if conf.find_program(target+'-cpp'+name, var='CPP', path_list=v['GCC_PATH'], mandatory=False, silent=True):
				break
		if not v['CPP']:
			for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall, versionverysmall, '']:
				if conf.find_program('cpp'+name, var='CPP', path_list=v['GCC_PATH'], mandatory=False, silent=True):
					break
		if not v['CPP']:
			conf.find_program('cpp', var='CPP', mandatory=False, silent=True)
		if not v['CPP']: conf.fatal('unable to find cpp for target %s' % target)

		for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall, versionverysmall, '']:
			if conf.find_program(target+'-objcopy'+name, var='OBJCOPY', path_list=v['GCC_PATH'], mandatory=False, silent=True):
				break
			if conf.find_program(target+'-gobjcopy'+name, var='OBJCOPY', path_list=v['GCC_PATH'], mandatory=False, silent=True):
				break
		if not v['OBJCOPY']:
			for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall, versionverysmall, '']:
				if conf.find_program('objcopy'+name, var='OBJCOPY', path_list=v['GCC_PATH'], mandatory=False, silent=True):
					break
				if conf.find_program('gobjcopy'+name, var='OBJCOPY', path_list=v['GCC_PATH'], mandatory=False, silent=True):
					break

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
			for windres in [target+'-windres', 'mingw32-windres', 'windres']:
				if conf.find_program(windres, var='WINRC', path_list=v['GCC_PATH'], mandatory=False, silent=True):
					break
	if gcc != 'icc':
		conf.load('gcc gxx gas')
	else:
		conf.load('icc icpc')
	conf.find_program('objcopy', var='OBJCOPY', mandatory=False)

@conf
def add_standard_gcc_flags(conf):
	v = conf.env
	v.append_unique('ASFLAGS', '-c')
	if v['GCC_FLAGS']:
		v['CFLAGS'] = v['GCC_FLAGS'][:]
		v['CXXFLAGS'] = v['GCC_FLAGS'][:]
		v['LINKFLAGS'] = v['GCC_FLAGS'][:]

	v['CFLAGS_warnnone'] = ['-w']
	v['CXXFLAGS_warnnone'] = ['-w']
	v['CFLAGS_warnall'] = ['-std=c99', '-Wall', '-Winline', '-Wno-unknown-pragmas', '-Werror']
	v['CXXFLAGS_warnall'] = ['-Wall', '-Wno-unknown-pragmas', '-Werror', '-Wnon-virtual-dtor', '-Woverloaded-virtual', '-Wno-invalid-offsetof']


	v['CFLAGS_debug'] = ['-pipe', '-g', '-D_DEBUG']
	v['CXXFLAGS_debug'] = ['-pipe', '-g', '-D_DEBUG']
	v['ASFLAGS_debug'] = ['-pipe', '-g', '-D_DEBUG']
	v['LINKFLAGS_debug'] = ['-pipe', '-g']

	v['CFLAGS_profile'] = ['-pipe', '-g', '-DNDEBUG', '-O3']
	v['CXXFLAGS_profile'] = ['-pipe', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions']
	v['ASFLAGS_profile'] = ['-pipe', '-g', '-DNDEBUG', '-O3']
	v['LINKFLAGS_profile'] = ['-pipe', '-g']

	v['CFLAGS_final'] = ['-pipe', '-g', '-DNDEBUG', '-O3']
	v['CXXFLAGS_final'] = ['-pipe', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions']
	v['ASFLAGS_final'] = ['-pipe', '-g', '-DNDEBUG', '-O3']
	v['LINKFLAGS_final'] = ['-pipe', '-g']

	if conf.env.GCC_NAME != 'icc':
		v['CFLAGS_warnall'] += ['-Wextra', '-pedantic']
		v['CXXFLAGS_warnall'] += ['-Wextra', '-Wno-sign-compare']
		v['CXXFLAGS_profile'] += ['-Wno-unused-parameter']
		v['CXXFLAGS_final'] += ['-Wno-unused-parameter']
		if int(v['GCC_VERSION'].split('.')[0]) >= 4:
			v['CXXFLAGS'] += ['-fno-threadsafe-statics']
	else:
		v.append_unique('CFLAGS', ['-static-intel'])
		v.append_unique('CXXFLAGS', ['-static-intel'])
		v.append_unique('LINKFLAGS', ['-static-intel'])

	if conf.env.GCC_NAME == 'clang':
		v.append_unique('CFLAGS_warnall', ['-Wno-unknown-warning-option', '-Wno-unneeded-internal-declaration', '-Wno-unused-function'])
		v.append_unique('CXXFLAGS_warnall', ['-Wno-unknown-warning-option', '-Wno-unneeded-internal-declaration', '-Wno-unused-function'])
		v.append_unique('CFLAGS_profile', ['-Wno-unused-function'])
		v.append_unique('CXXFLAGS_profile', ['-Wno-unused-function'])
		v.append_unique('CFLAGS_final', ['-Wno-unused-function'])
		v.append_unique('CXXFLAGS_final', ['-Wno-unused-function'])


configure = '''
get_native_gcc_target
find_cross_gcc
add_standard_gcc_flags
'''

