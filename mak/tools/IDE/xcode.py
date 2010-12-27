from TaskGen import feature
from Configure import conf
import Utils
import Task
import os
import mak
import random
import time

def newid():
	return "%04X%04X%04X%012d" % (random.randint(0, 32767), random.randint(0, 32767), random.randint(0, 32767), int(time.time()))

class XCodeProject:
	def __init__(self, name, path, version, projects):
		self.file = open(path, 'w')
		self.version = version
		self.projects = projects
		self.projectID = newid()
		self.buildSettingsId = (newid(),
					[('iphone-debug', newid(), newid(), newid()),
					 ('iphone-profile', newid(), newid(), newid()),
					 ('iphone-final', newid(), newid(), newid()),
					 ('osx-debug', newid(), newid(), newid()),
					 ('osx-profile', newid(), newid(), newid()),
					 ('osx-final', newid(), newid(), newid())])
		self.mainGroup = newid()

	def writeHeader(self):
		w = self.file.write
		w("// !$*UTF8*$!\n")
		w("{\n")
		w("	archiveVersion = 1;\n")
		w("	classes = {\n")
		w("	};\n")
		w("	objectVersion = %d;\n" % self.version[1])
		w("	objects = {\n\n")

	def pbxBuildTree(self, tree, process):
		w = self.file.write
		tree.id = newid()
		for name,dir in tree.directories.iteritems():
			self.pbxBuildTree(dir, process)
		for file in tree.files:
			file.id = newid()
			file.buildid = newid()
			if process and not isinstance(file, mak.sources.hsource):
				w("\t%s = { isa = PBXBuildFile; fileRef = %s; };\n" % (file.buildid, file.id))

	def pbxDirTree(self, tree, name, children = []):
		w = self.file.write
		w("\t%s = {\n" % tree.id)
		w("\t\tisa = PBXGroup;\n")
		w("\t\tname = \"%s\";\n" % name)
		w("\t\tsourceTree = \"<group>\";\n")
		w("\t\tchildren = (\n")
		subdirs = []
		for n,d in tree.directories.iteritems():
			subdirs.append((n, d))
		subdirs.sort(key = lambda i: i[0])
		for n,d in subdirs:
			w("\t\t\t%s,\n"%d.id)
		for file in tree.files:
			w("\t\t\t%s,\n"%file.id)
		for id in children:
			w("\t\t\t%s,\n"%id)
		w("\t\t);\n")
		w("\t};\n")
		for n,d in tree.directories.iteritems():
			self.pbxDirTree(d, n)

	def pbxFileRefTree(self, tree, path=''):
		w = self.file.write
		for name,dir in tree.directories.iteritems():
			self.pbxFileRefTree(dir, os.path.join(path, tree.prefix))
		for file in tree.files:
			filename = os.path.join(path, tree.prefix, file.filename)
			if isinstance(file, mak.sources.hsource):
				filetype = "sourcecode.c.h"
			elif isinstance(file, mak.sources.cppsource):
				filetype = "sourcecode.c.cpp"
			else:
				filetype = "sourcecode.c.h"
			w("\t%s = {\n\t\tisa = PBXFileReference;\n\t\tfileEncoding = 4;\n\t\tlastKnownFileType = %s;\n\t\tname = \"%s\";\n\t\tpath = \"%s\";\n\t\tsourceTree = \"<group>\";\n\t};\n" % (file.id, filetype, os.path.split(filename)[1], filename))

	def writePBXBuildFile(self):
		w = self.file.write
		w("/* Begin PBXBuildFile section */\n")
		for d in self.projects:
			if d.usemaster:
				d.masterid = newid()
				d.masterbuildid = newid()
				w("\t%s = { isa = PBXBuildFile; fileRef = %s; };\n" % (d.masterbuildid, d.masterid))
			self.pbxBuildTree(d.sourceTree, not d.usemaster)
		w("/* End PBXBuildFile section */\n\n")

	def writePBXFileReference(self):
		w = self.file.write
		w("/* Begin PBXFileReference section */\n")
		for name, setting, buildfile, fileref in self.buildSettingsId[1]:
			w("\t%s = {\n\t\tisa = PBXFileReference;\n\t\tfileEncoding = 4;\n\t\tlastKnownFileType = text.xcconfig;\n\t\tname = \"%s\";\n\t\tpath = \"%s\";\n\t\tsourceTree = \"SOURCE_ROOT\";\n\t};\n" % (fileref, name+'.xcconfig', os.path.join('mak', 'xcode', name+'.xcconfig')))
		for d in self.projects:
			d.targetId = newid()
			d.phaseId = [newid(), newid(), newid()]
			d.applicationId = newid()
			if d.usemaster:
				w("\t%s = {\n\t\tisa = PBXFileReference;\n\t\tfileEncoding = 4;\n\t\tlastKnownFileType = sourcecode.c.cpp;\n\t\tname = \"%s\";\n\t\tpath = \"%s\";\n\t\tsourceTree = \"SOURCE_ROOT\";\n\t};\n" % (d.masterid, os.path.split(d.masterfilename)[1], d.masterfilename))
			self.pbxFileRefTree(d.sourceTree)
			if d.type in ['game', 'tool']:
				w("\t%s = {\n\t\tisa = PBXFileReference;\n\t\texplicitFileType = wrapper.application;\n\t\tincludeInIndex = 0;\n\t\tpath = \"%s.app\";\n\t\tsourceTree = BUILT_PRODUCTS_DIR;\n\t};\n" % (d.applicationId, d.projectName))
			elif d.type in ['library', 'static_library', 'plugin']:
				w("\t%s = {\n\t\tisa = PBXFileReference;\n\t\texplicitFileType = archive.ar;\n\t\tincludeInIndex = 0;\n\t\tpath = \"lib%s.a\";\n\t\tsourceTree = BUILT_PRODUCTS_DIR;\n\t};\n" % (d.applicationId, d.projectName))
			d.buildSettingsId = (newid(),
						[('iphone-debug', newid()), ('iphone-profile', newid()), ('iphone-final', newid()),
						 ('osx-debug', newid()), ('osx-profile', newid()), ('osx-final', newid())])
		w("/* End PBXFileReference section */\n\n")

	def writePBXGroup(self):
		w = self.file.write
		w("/* Begin PBXGroup section */\n")
		makid = newid()
		w("\t%s = {\n" % makid)
		w("\t\tisa = PBXGroup;\n")
		w("\t\tname = config;\n")
		w("\t\tsourceTree = \"<group>\";\n")
		w("\t\tchildren = (\n")
		for name, setting, buildfile, fileref in self.buildSettingsId[1]:
			w("\t\t\t%s,\n" % fileref)
		w("\t\t);\n")
		w("\t};\n")

		w("\t%s = {\n" % self.mainGroup)
		w("\t\tisa = PBXGroup;\n")
		w("\t\tname = BugEngine;\n")
		w("\t\tsourceTree = \"<group>\";\n")
		w("\t\tchildren = (\n")
		w("\t\t\t%s,\n" % makid)
		projects = []
		for d in self.projects:
			projects.append((d.projectCategory+'.'+d.projectName, d))
		projects.sort(key = lambda i: i[0])
		for name, d in projects:
			w("\t\t\t%s,\n"%d.sourceTree.id)
		w("\t\t);\n")
		w("\t};\n")
		for name, d in projects:
			children = []
			if d.usemaster:
				children.append(d.masterid)
			if d.type in ['game', 'tool', 'library', 'static_library', 'plugin']:
				children.append(d.applicationId)
			self.pbxDirTree(d.sourceTree, name, children)
		w("/* End PBXGroup section */\n\n")

	def writeTarget(self, d):
		w = self.file.write
		w("\t%s = {\n" % d.targetId)
		w("\t\tisa = PBXNativeTarget;\n")
		w("\t\tbuildConfigurationList = %s;\n" % d.buildSettingsId[0])
		w("\t\tbuildPhases = (\n")
		w("\t\t\t%s,\n" % d.phaseId[0])
		w("\t\t);\n")
		w("\t\tbuildRules = (\n")
		w("\t\t);\n")
		w("\t\tdependencies = (\n")
		w("\t\t);\n")
		w("\t\tname = \"%s\";\n" % (d.projectName))
		w("\t\tproductName = %s;\n" % d.projectName)
		w("\t\tproductReference = %s;\n" % d.applicationId)
		if d.type in ['game', 'tool']:
			w("\t\tproductType = \"com.apple.product-type.application\";\n")
		elif d.type in ['library', 'static_library', 'plugin']:
			w("\t\tproductType = \"com.apple.product-type.library.static\";\n")
		w("\t};\n")

	def writePBXNativeTarget(self):
		w = self.file.write
		w("/* Begin PBXNativeTarget section */\n")
		for d in self.projects:
			if d.type in ['game', 'tool']:
				self.writeTarget(d)
		for d in self.projects:
			if d.type in ['library', 'static_library', 'plugin']:
				self.writeTarget(d)
		w("/* End PBXNativeTarget section */\n\n")

	def writePBXProject(self):
		w = self.file.write
		w("/* Begin PBXProject section */\n")
		w("\t%s /* Project object */ = {\n" % self.projectID)
		w("\t\tisa = PBXProject;\n")
		w("\t\tcompatibilityVersion = \"%s\";\n" % self.version[0])
		w("\t\tbuildConfigurationList = %s;\n" % self.buildSettingsId[0])
		w("\t\thasScannedForEncodings = 1;\n")
		w("\t\tprojectDirPath=\"\";\n")
		w("\t\tmainGroup = %s;\n" % self.mainGroup)
		w("\t\ttargets = (\n")
		for d in self.projects:
			if d.type in ['game', 'tool']:
				w("\t\t\t%s,\n" % d.targetId)
		for d in self.projects:
			if d.type in ['library', 'static_library', 'plugin']:
				w("\t\t\t%s,\n" % d.targetId)
		w("\t\t);\n")
		w("\t};\n")
		w("/* End PBXProject section */\n\n")

	def writeSources(self, sources):
		w = self.file.write
		for name,d in sources.directories.iteritems():
			self.writeSources(d)
		for file in sources.files:
			if not isinstance(file, mak.sources.hsource):
				w("\t\t\t%s,\n" % file.buildid)

	def writePBXSourcesBuildPhase(self):
		w = self.file.write
		w("/* Begin PBXSourcesBuildPhase section */\n")
		for d in self.projects:
			w("\t%s = {\n" % d.phaseId[0])
			w("\t\tisa = PBXSourcesBuildPhase;\n")
			w("\t\tbuildActionMask = 2147483647;\n")
			w("\t\tfiles = (\n")
			if d.usemaster:
				w("\t\t\t%s,\n" % d.masterbuildid)
			else:
				self.writeSources(d.sourceTree)
			w("\t\t);\n")
			w("\t\trunOnlyForDeploymentPostprocessing = 0;\n")
			w("\t};\n")
		w("/* End PBXSourcesBuildPhase section */\n\n")


	def writeXCBuildConfiguration(self):
		def toXCodeArch(arch):
			if arch == 'x86':
				return 'i386'
			elif arch == 'amd64':
				return 'x86_64'
			elif arch == 'arm':
				return 'arm*'
			elif arch == 'ppc':
				return 'ppc'
			elif arch == 'ppc64':
				return 'ppc64'
			else:
				return ''
		def toSDK(platform):
			if platform == 'darwin':
				return 'osx', 'macosx*'
			elif platform == 'iphone':
				return 'iphone', 'iphoneos*'
			else:
				return '', ''

		w = self.file.write
		w("/* Begin XCBuildConfiguration section */\n")
		for name, setting, buildfile, fileref in self.buildSettingsId[1]:
			w("\t%s = {\n" % setting)
			w("\t\tisa = XCBuildConfiguration;\n")
			w("\t\tbaseConfigurationReference = %s;\n" % fileref)
			w("\t\tbuildSettings = {\n")
			w("\t\t};\n")
			w("\t\tname = %s;\n" % name)
			w("\t};\n")
		for d in self.projects:
			for name, setting in d.buildSettingsId[1]:
				w("\t%s = {\n" % setting)
				w("\t\tisa = XCBuildConfiguration;\n")
				w("\t\tbuildSettings = {\n")
				w("\t\t\tPRODUCT_NAME = %s;\n" % d.projectName)
				for platform, options in d.platforms.iteritems():
					p, arch = platform.split('-')
					p, sdk = toSDK(p)
					arch = toXCodeArch(arch)
					if p and arch and name.startswith(p):
						w("\t\t\t\"GCC_PREPROCESSOR_DEFINITIONS[arch=%s]\" = \"$(GCC_PREPROCESSOR_DEFINITIONS_PLATFORM) %s\";\n" % (arch, " ".join(options.defines).replace('"', '\\"')))
						w("\t\t\t\"HEADER_SEARCH_PATHS[arch=%s]\" = \"%s\";\n" % (arch, " ".join("\"%s\"" % i for i in options.includedir).replace('"', '\\"')))
				w("\t\t};\n")
				w("\t\tname = %s;\n" % name)
				w("\t};\n")
		w("/* End XCBuildConfiguration section */\n\n")

	def writeXCConfigurationList(self):
		self.writeXCBuildConfiguration()
		w = self.file.write
		w("/* Begin XCConfigurationList section */\n")
		w("\t%s = {\n" % self.buildSettingsId[0])
		w("\t\tisa = XCConfigurationList;\n")
		w("\t\tbuildConfigurations = (\n")
		for name, setting, buildfile, fileref in self.buildSettingsId[1]:
			w("\t\t\t%s,\n" % setting)
		w("\t\t);\n")
		w("\t\tdefaultConfigurationIsVisible = 0;\n")
		w("\t\tdefaultConfigurationName = %s;\n" % self.buildSettingsId[1][0][0])
		w("\t};\n")
		for d in self.projects:
			w("\t%s = {\n" % d.buildSettingsId[0])
			w("\t\tisa = XCConfigurationList;\n")
			w("\t\tbuildConfigurations = (\n")
			for name, setting in d.buildSettingsId[1]:
				w("\t\t\t%s,\n" % setting)
			w("\t\t);\n")
			w("\t\tdefaultConfigurationIsVisible = 0;\n")
			w("\t\tdefaultConfigurationName = %s;\n" % d.buildSettingsId[1][0][0])
			w("\t};\n")
		w("/* End XCConfigurationList section */\n\n")

	def writeFooter(self):
		self.file.write("\t};\n")
		self.file.write("\trootObject = %s;\n" % self.projectID)
		self.file.write("}\n")

class Project:
	def __init__(self):
		pass

xcodeprojects = {
	'xcode2': ('Xcode 2.5', 42),
	'xcode3': ('Xcode 3.1', 45),
}

def writemaster(sourcetree, f, path = ''):
	for source in sourcetree.files:
		if isinstance(source, mak.sources.cppsource):
			f.write("#if %s\n" % " || ".join(["defined(_%s)" % i.upper() for i in source.archs]))
			f.write("# if %s\n" % " || ".join(["defined(_%s)" % i.upper() for i in source.platforms]))
			f.write("#  include \"../%s\"\n" % os.path.join(path, sourcetree.prefix, source.filename))
			f.write("# endif\n")
			f.write("#endif\n")
	for name,directory in sourcetree.directories.iteritems():
		writemaster(directory, f, os.path.join(path, sourcetree.prefix))

def generateProject(task):
	solution = XCodeProject( task.name,
							 task.outputs[0].bldpath(task.env),
							 task.version,
							 task.projects)
	solution.writeHeader()
	solution.writePBXBuildFile()
	solution.writePBXFileReference()
	#solution.writePBXFrameworksBuildPhase()
	solution.writePBXGroup()
	#solution.writePBXHeadersBuildPhase()
	solution.writePBXNativeTarget()
	solution.writePBXProject()
	solution.writePBXSourcesBuildPhase()
	solution.writeXCConfigurationList()
	solution.writeFooter()
	for p in task.projects:
		if p.usemaster:
			writemaster(p.sourceTree, open(p.masterfile.bldpath(task.env), 'w'))

GenerateProject = Task.task_type_from_func('generateProject', generateProject)

solutions = {}
def create_xcode_project(t):
	toolName = t.features[0]
	appname = getattr(Utils.g_module, 'APPNAME', 'noname')
	if not solutions.has_key(toolName):
		outname = 'project.pbxproj'
		solution = GenerateProject(env=t.env)
		solution.set_outputs(t.path.find_or_declare(outname))
		solution.name = appname
		solution.version = xcodeprojects[toolName]
		solution.install_path = t.path.srcpath(t.env)+'/'+appname+'.'+toolName+'.xcodeproj/'
		solution.chmod = 0444
		solution.projects = []
		solution.dep_vars = ['XCODE_PROJECT_DEPENDS']
		solution.env['XCODE_PROJECT_DEPENDS'] = []
		solutions[toolName] = solution
	solution = solutions[toolName]


	project = Project()
	project.type			= t.type
	#project.allplatforms    = projects[toolName][2]
	project.platforms 		= t.platforms
	project.version 		= toolName
	project.projectCategory = t.category
	project.projectName 	= t.name
	project.type 			= t.type
	project.sourceTree 		= t.sourcetree
	project.usemaster		= t.usemaster
	project.depends			= t.depends
	if t.usemaster:
		filename = "master-%s.cpp" % t.name
		node = t.path.find_or_declare(filename)
		solution.set_outputs(node)
		project.masterfile = node
		project.masterfilename = os.path.join(appname+'.'+toolName+'.xcodeproj/', filename)
	solution.projects.append(project)
	solution.env['XCODE_PROJECT_DEPENDS'].append(t.sourcetree)

for pname in xcodeprojects.keys():
	feature(pname)(create_xcode_project)

