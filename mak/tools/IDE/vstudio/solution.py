import string
try:
	import cStringIO as StringIO
except ImportError:
	try:
		import StringIO
	except ImportError:
		import io as StringIO

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
	def __init__(self, name, filename, version, versionnumber, versionname, folders):
		self.name = name
		self.filename = filename
		self.guid = "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}" #vcproj/vcxproj
		self.folderGUID = "{2150E333-8FDC-42A3-9474-1A3956D46DE8}" #folder
		self.file = StringIO.StringIO()
		self.projectlist = []
		self.version = version
		self.versionnumber = versionnumber
		self.versionname = versionname
		self.done = set([])
		self.folders = folders
		self.layout = {}

	def writeHeader(self):
		self.file.write("""Microsoft Visual Studio Solution File, Format Version %s\n""" % (self.versionnumber))
		self.file.write("""# %s\n""" % (self.versionname))

	def addProject(self, project, filename):
		if project in self.done:
			return
		self.done.add(project)

		name = project.category+'.'+project.name
		projectGUID = generateGUID(project.targetName, project.name)
		if self.folders:
			lst = name.split('.')
			l = self.layout
			for i in lst[:-1]:
				try:
					l = l[i][1]
				except KeyError:
					l[i] = (generateGUID(i, i), {})
					l = l[i][1]
			l[lst[-1]] = (projectGUID, {})


		self.projectlist.append((projectGUID, project.category))
		self.file.write("Project(\"%s\") = \"%s\", \"%s\", \"%s\"\n" % (self.guid, name, filename, projectGUID))
		self.file.write('EndProject\n')

	def writeLayout(self, folder):
		for name, value in folder.items():
			guid, children = value
			if children:
				self.file.write("Project(\"%s\") = \"%s\", \"%s\", \"%s\"\n" % (self.folderGUID, name, name, guid))
				self.file.write('EndProject\n')
				self.writeLayout(children)

	def writeProjectFolders(self, parent, folder):
		for name, value in folder.items():
			guid, children = value
			if parent:
				self.file.write("%s = %s\n" % (guid, parent))
			self.writeProjectFolders(guid, children)

	def writeFooter(self, configs):
		if self.folders:
			self.writeLayout(self.layout)
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
		if self.folders:
			self.file.write("""\tEndGlobalSection\n\tGlobalSection(NestedProjects) = preSolution\n""")
			self.writeProjectFolders("", self.layout)
		self.file.write("""\tEndGlobalSection\nEndGlobal\n""")


		with open(self.filename,'r') as original:
			content = self.file.getvalue()
			if original.read() != content:
				print('writing %s...' % self.filename)
				with open(self.filename, 'w') as f:
					f.write(content)

