from Configure import conftest
import Utils
import re

format = re.compile('^cc: Sun Ceres C (..?\..?.?) SunOS_([^ ]+) .*')

@conftest
def get_suncc_target(conf):
	cmd = [conf.env['CC'], '-V']
	try:
		p = Utils.pproc.Popen(cmd, stdin=Utils.pproc.PIPE, stdout=Utils.pproc.PIPE, stderr=Utils.pproc.PIPE)
		out = p.communicate()[1]
	except:
		conf.fatal('Unable to run SunCC')

	out = str(out).split('\n')
	line = out[0]
	result = format.match(line)
	if result:
		conf.env['SUNCC_TARGET'] = result.group(2)
		conf.env['SUNCC_VERSION'] = result.group(1)
	else:
		conf.fatal('Unable to retrieve SunCC version')

@conftest
def find_suncc(conf):
	conf.check_tool('suncc suncxx')

detect = '''
find_suncc
get_suncc_target
'''
