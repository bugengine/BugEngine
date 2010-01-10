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
		i = ord(c)
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
	solution = _hexdigest(createMd5(str(slnfile)+str(name)).digest()).upper()
	# convert most of the signature to GUID form (discard the rest)
	solution = "{" + solution[:8] + "-" + solution[8:12] + "-" + solution[12:16] + "-" + solution[16:20] + "-" + solution[20:32] + "}"
	return solution



class Solution:
	def __init__(self, name, filename, version, versionnumber, versionname):
		self.name = name
		self.filename = filename
		self.guid = generateGUID(self.filename,self.name)
		self.file = open(self.filename,'w')
		self.projectlist = []
		self.version = version
		self.versionnumber = versionnumber
		self.versionname = versionname
		self.done = set([])

	def writeHeader(self):
		self.file.write("""Microsoft Visual Studio Solution File, Format Version %s
# %s
""" % (self.versionnumber, self.versionname))

	def addProject(self, project):
		filename = project.outname
		name = project.name
		if project in self.done: return
		self.done.add(project)
		projectGUID = generateGUID(filename,name)
		self.projectlist.append(projectGUID)
		self.file.write("Project(\"%s\") = \"%s\", \"%s\", \"%s\"\n" % (self.guid, name, filename, projectGUID))
		if project.depends:
			self.file.write('\tProjectSection(ProjectDependencies) = postProject\n')
			depends = project.depends[:]
			seen = set()
			while depends:
				dep = depends.pop()
				if dep not in seen:
					seen.add(dep)
					#depends += dep.depends
					if 'win32-x86' in dep.platforms or 'win32-amd64' in dep.platforms:
						self.file.write('\t\t%s = %s\n' % (generateGUID(dep.outname,dep.name),generateGUID(dep.outname,dep.name)))
			self.file.write('\tEndProjectSection\n')
		self.file.write('EndProject\n')
		for dep in project.depends:
			self.addProject(dep)

	def writeFooter(self,platforms,configs):
		self.file.write("""Global
\tGlobalSection(SolutionConfigurationPlatforms) = preSolution
""")
		for conf in configs:
			for platform in platforms:
				self.file.write("""\t\t%s|%s = %s|%s
""" % (conf,platform,conf,platform))
		self.file.write("""\tEndGlobalSection
\tGlobalSection(ProjectConfigurationPlatforms) = postSolution
""")
		for proj in self.projectlist:
			for conf in configs:
				for platform in platforms:
					self.file.write("""\t\t%(GUID)s.%(CONF)s|%(PLATFORM)s.ActiveCfg = %(CONF)s|%(PLATFORM)s
\t\t%(GUID)s.%(CONF)s|%(PLATFORM)s.Build.0 = %(CONF)s|%(PLATFORM)s
""" % {'GUID':proj,'CONF':conf,'PLATFORM':platform})
		self.file.write("""\tEndGlobalSection
EndGlobal
""")

