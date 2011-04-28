from Configure import conftest
import Options
import Utils
import re
from TaskGen import feature, before, extension, after
import os
from mak.waflib.Errors import ConfigurationError

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
			  ('spu', 'spu')]
	for gccname,aname in archs:
		if target.find(gccname) != -1:
				return aname

@conftest
def get_available_gcc(conf):
	toolchaindirs=set([])
	conf.env['GCC_TARGETS'] = []
	for dir in os.environ['PATH'].split(':'):
		toolchaindirs.add(os.path.normpath(os.path.join(dir, '..')))
	for toolchaindir in toolchaindirs:
		if not os.path.isdir(os.path.join(toolchaindir,'lib')):
			continue
		for lib in os.listdir(os.path.join(toolchaindir,'lib')):
			if lib == 'gcc':
				libdir = os.path.join(toolchaindir, 'lib', lib)
			elif lib.startswith('gcc'):
				libdir = os.path.join(toolchaindir, 'lib', lib, 'gcc')
			else:
				continue
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

@conftest
def find_cross_gcc(conf):
	target = conf.env['GCC_TARGET']
	version = conf.env['GCC_VERSION']
	versionsmall = '.'.join(version.split('.')[0:2])
	versionverysmall = ''.join(version.split('.')[0:2])
	if target:
		v = conf.env
		for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall]:
			if conf.find_program(target+'-gcc'+name, var='CC', path_list=v['GCC_PATH'], mandatory=False):
				break
		if not v['CC']: conf.fatal('unable to find gcc for target %s' % target)

		for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall, '']:
			if conf.find_program(target+'-g++'+name, var='CXX', path_list=v['GCC_PATH'], mandatory=False):
				break
		if not v['CXX']: conf.fatal('unable to find g++ for target %s' % target)

		for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall, '']:
			if conf.find_program(target+'-cpp'+name, var='CPP', path_list=v['GCC_PATH'], mandatory=False):
				break
		if not v['CPP']:
			for name in ['-'+version, '-'+versionsmall, '-'+versionverysmall, '']:
				if conf.find_program('cpp'+name, var='CPP', path_list=v['GCC_PATH'], mandatory=False):
					break
		if not v['CPP']: conf.fatal('unable to find cpp for target %s' % target)

		if not v['AS']: v['AS'] = v['CC']

		for ar in [target+'-gar', target+'-ar', 'gar', 'ar']:
			if conf.find_program(ar, var='AR', path_list=v['GCC_PATH'], mandatory=False):
				break
		if not v['AR']:
			for ar in ['gar', 'ar']:
				if conf.find_program(ar, var='AR', mandatory=False):
					break
		if not v['AR']: conf.fatal('unable to find ar for target %s' % target)

		for ranlib in [target+'-ranlib', target+'-granlib', 'ranlib', 'granlib']:
			if conf.find_program(ranlib, var='RANLIB', path_list=v['GCC_PATH'], mandatory=False):
				break
		if not v['RANLIB']:
			for ranlib in ['ranlib', 'granlib', 'ranlib', 'granlib']:
				if conf.find_program(ranlib, var='RANLIB', mandatory=False):
					break
		if not v['RANLIB']: conf.fatal('unable to find ranlib for target %s' % target)
	conf.load('gcc gxx gas')

@conftest
def add_standard_gcc_flags(conf):
	v = conf.env
	v.append_unique('ASFLAGS', '-c')

	v['CCFLAGS_warnall'] = ['-std=c99', '-Wall', '-Wextra', '-pedantic', '-Winline', '-Wno-unknown-pragmas', '-Wno-unused-parameter', '-Werror']
	v['CXXFLAGS_warnall'] = ['-Wall', '-Wextra', '-Wno-unknown-pragmas', '-Wno-unused-parameter', '-Werror', '-Wno-sign-compare']

	v['CCFLAGS_debug'] = ['-pipe', '-g', '-D_DEBUG']
	v['CXXFLAGS_debug'] = ['-pipe', '-g', '-D_DEBUG', '-Wno-invalid-offsetof']
	v['LINKFLAGS_debug'] = ['-pipe', '-g']
	v['ASFLAGS_debug'] = ['-pipe', '-g', '-D_DEBUG']

	v['CCFLAGS_profile'] = ['-pipe', '-g', '-DNDEBUG', '-O3']
	v['CXXFLAGS_profile'] = ['-pipe', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions', '-Wno-invalid-offsetof']
	v['ASFLAGS_profile'] = ['-pipe', '-g', '-DNDEBUG', '-O3']
	v['LINKFLAGS_profile'] = ['-pipe', '-g', '-s']

	v['CCFLAGS_final'] = ['-pipe', '-g', '-DNDEBUG', '-O3']
	v['CXXFLAGS_final'] = ['-pipe', '-g', '-DNDEBUG', '-O3', '-fno-rtti', '-fno-exceptions', '-Wno-invalid-offsetof']
	v['ASFLAGS_final'] = ['-pipe', '-g', '-DNDEBUG', '-O3']
	v['LINKFLAGS_final'] = ['-pipe', '-g', '-s']


configure = '''
get_native_gcc_target
find_cross_gcc
add_standard_gcc_flags
'''

