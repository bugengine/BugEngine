from Configure import conftest
import Utils
import re

format = re.compile('^cc: Sun.*C (..?\..?.?) ([A-Za-z]+)_([^ ]+) .*')

@conftest
def find_suncc(conf):
	cc = conf.find_program('suncc', mandatory=False)
	if cc:
		conf.get_suncc_targets(cc)


def to32bits(arch):
	if arch == 'amd64' or arch == 'x86_64' or arch == 'x64':
		return 'x86'
	return arch

def to64bits(arch):
	if arch == 'i386' or arch == 'i486' or arch == 'i586' or arch == 'i686' or arch == 'x86':
		return 'amd64'
	return arch

@conftest
def get_suncc_targets(conf, cc):
	cmd = [cc, '-V', '-m32']
	try:
		p = Utils.pproc.Popen(cmd, stdin=Utils.pproc.PIPE, stdout=Utils.pproc.PIPE, stderr=Utils.pproc.PIPE)
		out = p.communicate()[1]
	except:
		return
	out = str(out).split('\n')
	line = out[0]
	result = format.match(line)
	if result:
		conf.env['SUNCC_TARGETS'].append(result.group(1), result.group(2), to32bits(result.group(3)))
	cmd = [cc, '-V', '-m64']
	try:
		p = Utils.pproc.Popen(cmd, stdin=Utils.pproc.PIPE, stdout=Utils.pproc.PIPE, stderr=Utils.pproc.PIPE)
		out = p.communicate()[1]
	except:
		return
	out = str(out).split('\n')
	line = out[0]
	result = format.match(line)
	if result:
		conf.env['SUNCC_TARGETS'].append(result.group(1), result.group(2), to64bits(result.group(3)))

def configure(conf):
	conf.check_tool('suncc suncxx')

