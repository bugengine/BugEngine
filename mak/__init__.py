import os

alloptims		= [ 'debug', 'release', 'profile', 'final']
allplatforms	= [ 'win32', 'linux', 'sunos', 'darwin', 'freebsd']
allarchs		= [ 'amd64', 'x86', 'alpha', 'arm', 'arm7', 'arm9', 'hppa', 'ia64', 'mips', 'ppc', 'ppc64', 's390', 's390x', 'sparc' ]
builder			= None
platformaliases = {
	'pc':		['win32', 'linux', 'sunos', 'darwin', 'freebsd'],
	'posix': 	['linux', 'sunos', 'freebsd'],
}

