import string
try:
	import hashlib
	createMd5=hashlib.md5
except:
	import md5
	createMd5=md5.new

def _hexdigest(s):
	"""Return a string as a string of hex characters.
	"""
	# NOTE:  This routine is a method in the Python 2.0 interface
	# of the native md5 module, but we want SCons to operate all
	# the way back to at least Python 1.5.2, which doesn't have it.
	h = string.hexdigits
	r = ''
	for c in s:
		try:
			i = ord(c)
		except:
			i = c
		r = r + h[(i >> 4) & 0xF] + h[i & 0xF]
	return r

def xmlify(s):
	s = string.replace(s, "&", "&amp;") # do this first
	s = string.replace(s, "'", "&apos;")
	s = string.replace(s, '"', "&quot;")
	return s

def generateGUID(slnfile, name):
	"""This generates a dummy GUID for the sln file to use.  It is
	based on the MD5 signatures of the sln filename plus the name of
	the project.  It basically just needs to be unique, and not
	change with each invocation."""
	solution = _hexdigest(createMd5((str(slnfile)+str(name)).encode()).digest()).upper()
	# convert most of the signature to GUID form (discard the rest)
	solution = "{" + solution[:8] + "-" + solution[8:12] + "-" + solution[12:16] + "-" + solution[16:20] + "-" + solution[20:32] + "}"
	return solution



class Solution:
	def __init__(self, name, filename, version, versionnumber, versionname):
		self.name = name
		self.filename = filename
		self.guid = "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}" #vcproj/vcxproj
		self.file = open(self.filename,'w')
		self.projectlist = []
		self.version = version
		self.versionnumber = versionnumber
		self.versionname = versionname
		self.done = set([])

	def writeHeader(self):
		self.file.write("""Microsoft Visual Studio Solution File, Format Version %s\n""" % (self.versionnumber))
		self.file.write("""# %s\n""" % (self.versionname))

	def addProject(self, project, filename):
		name = project.category+'.'+project.name
		if project in self.done: return
		self.done.add(project)
		projectGUID = generateGUID(filename,name)
		self.projectlist.append((projectGUID, project.category))
		self.file.write("Project(\"%s\") = \"%s\", \"%s\", \"%s\"\n" % (self.guid, name, filename, projectGUID))
		self.file.write('EndProject\n')

	def writeFooter(self, configs):
		if float(self.versionnumber) >= 9.0:
			self.file.write("Global\n\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\n")
			for conf in configs:
				self.file.write("\t\t%s|%s = %s|%s\n" % (conf,'win32',conf,'win32'))
		else:
			self.file.write("Global\n\tGlobalSection(SolutionConfiguration) = preSolution\n")
			for conf in configs:
				self.file.write("\t\t%s = %s\n" % (conf,conf))
		if float(self.versionnumber) >= 9.0:
			self.file.write("""\tEndGlobalSection\n\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\n""")
		else:
			self.file.write("""\tEndGlobalSection\n\tGlobalSection(ProjectConfiguration) = postSolution\n""")
		for proj,type in self.projectlist:
			for conf in configs:
				if float(self.versionnumber) >= 9.0:
					self.file.write("""\t\t%(GUID)s.%(CONF)s|%(PLATFORM)s.ActiveCfg = %(CONF)s|%(PLATFORM)s\n""" % {'GUID':proj,'CONF':conf,'PLATFORM':'win32'})
					if type == 'game':
						self.file.write("""\t\t%(GUID)s.%(CONF)s|%(PLATFORM)s.Build.0 = %(CONF)s|%(PLATFORM)s\n""" % {'GUID':proj,'CONF':conf,'PLATFORM':'win32'})
				else:
					self.file.write("""\t\t%(GUID)s.%(CONF)s.ActiveCfg = %(CONF)s|%(PLATFORM)s\n""" % {'GUID':proj,'CONF':conf,'PLATFORM':'win32'})
					if type == 'game':
						self.file.write("""\t\t%(GUID)s.%(CONF)s.Build.0 = %(CONF)s|%(PLATFORM)s\n""" % {'GUID':proj,'CONF':conf,'PLATFORM':'win32'})
		self.file.write("""\tEndGlobalSection\nEndGlobal\n""")

