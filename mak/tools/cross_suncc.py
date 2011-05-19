from waflib.Configure import conf
from waflib import Utils
import re

format = re.compile('^cc: Sun.*C (..?\..?.?) ([A-Za-z]+)_([^ ]+) .*')

@conf
def get_available_suncc(conf):
	conf.env.SUNCC_TARGETS = []
	cc = conf.find_program('suncc', mandatory=False, var='detect_suncc', silent=True)
	cxx = conf.find_program('sunCC', mandatory=False, var='detect_suncxx', silent=True)
	if cc and cxx:
		conf.get_suncc_targets_32(cc, cxx)
		conf.get_suncc_targets_64(cc, cxx)

def to32bits(arch):
	if arch == 'amd64' or arch == 'x86_64' or arch == 'x64':
		return 'x86'
	return arch

def to64bits(arch):
	if arch == 'i386' or arch == 'i486' or arch == 'i586' or arch == 'i686' or arch == 'x86':
		return 'amd64'
	return arch

@conf
def get_suncc_targets(conf, cc, cxx, flag):
	cmd = [cc, '-V', flag]
	try:
		p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
		out = p.communicate()[1]
		retcode = p.returncode
	except Exception:
		return None
	if p.returncode:
		return None
	out = str(out).split('\n')
	line = out[0]
	return format.match(line)

@conf
def get_suncc_targets_32(conf, cc, cxx):
	result = conf.get_suncc_targets(cc, cxx, '-m32')
	if result:
		conf.env.SUNCC_TARGETS.append((cc, cxx, result.group(1), result.group(2), to32bits(result.group(3))))

@conf
def get_suncc_targets_64(conf, cc, cxx):
	result = conf.get_suncc_targets(cc, cxx, '-m64')
	if result:
		conf.env.SUNCC_TARGETS.append((cc, cxx, result.group(1), result.group(2), to64bits(result.group(3))))

def configure(conf):
	conf.load('suncc suncxx')

