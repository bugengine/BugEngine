import os

alloptims		= [ 'debug', 'release', 'profile', 'final']
allplatforms	= [ 'win32', 'linux', 'sunos', 'darwin', 'freebsd']
allarchs		= [ 'amd64', 'x86', 'alpha', 'arm', 'arm7', 'arm9', 'hppa', 'ia64', 'mips', 'ppc', 'ppc64', 's390', 's390x', 'sparc' ]
builder			= None
platformaliases = {
	'pc':		['win32', 'linux', 'sunos', 'darwin', 'freebsd'],
	'posix': 	['linux', 'sunos', 'freebsd'],
}

if os.path.isdir('extra'):
	for f in os.listdir('extra'):
		if f in ['.svn', '.cvs']:
			continue
		if os.path.isdir(os.path.join('extra', f)) and os.path.isfile(os.path.join('extra', f, 'wscript')):
			allplatforms.append(f)
