from TaskGen import feature
from Configure import conf
import Utils
import Task
import os
import mak
import random
import time

def newid():
	return "{0:04X}{1:04X}{2:04X}{3:012d}".format(random.randint(0, 32767), random.randint(0, 32767), random.randint(0, 32767), int(time.time()))

class XCodeProject:
	def __init__(self, name, path, version, projects):
		self.file = open(path, 'w')
		self.version = version
		self.projects = projects
		self.projectID = newid()
		self.configurationsID = newid()
		self.buildSettingsId = (newid(), [newid()])
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

	def pbxBuildTree(self, tree):
		w = self.file.write
		tree.id = newid()
		for name,dir in tree.directories.iteritems():
			self.pbxBuildTree(dir)
		for file in tree.files:
			file.id = newid()
			file.buildid = newid()
			if not isinstance(file, mak.sources.hsource):
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
			w("\t%s = { isa = PBXFileReference; fileEncoding = 4; lastKnownFileType = %s; name = \"%s\"; path = \"%s\" ; sourceTree = \"<group>\"; };\n" % (file.id, filetype, os.path.split(filename)[1], filename))

	def writePBXBuildFile(self):
		w = self.file.write
		w("/* Begin PBXBuildFile section */\n")
		for d in self.projects:
			self.pbxBuildTree(d.sourceTree)
		w("/* End PBXBuildFile section */\n\n")

	def writePBXFileReference(self):
		w = self.file.write
		w("/* Begin PBXFileReference section */\n")
		for d in self.projects:
			self.pbxFileRefTree(d.sourceTree)
			if d.type in ['game', 'tool']:
				d.applicationId = newid()
				d.targetId = newid()
				d.phaseId = [newid(), newid(), newid()]
				w("\t%s = { isa = PBXFileReference; explicitFileType = wrapper.application; includeInIndex = 0; path = \"%s.app\" ; sourceTree = BUILT_PRODUCTS_DIR; };\n" % (d.applicationId, d.projectName))
		w("/* End PBXFileReference section */\n\n")

	def writePBXGroup(self):
		w = self.file.write
		w("/* Begin PBXGroup section */\n")
		w("\t%s = {\n" % self.mainGroup)
		w("\t\tisa = PBXGroup;\n")
		w("\t\tname = BugEngine;\n")
		w("\t\tsourceTree = \"<group>\";\n")
		w("\t\tchildren = (\n")
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
			if d.type in ['game', 'tool']:
				children.append(d.applicationId)
			self.pbxDirTree(d.sourceTree, name, children)
		w("/* End PBXGroup section */\n\n")

	def writePBXNativeTarget(self):
		w = self.file.write
		w("/* Begin PBXNativeTarget section */\n")
		for d in self.projects:
			if d.type in ['game', 'tool']:
				w("\t%s = {\n" % d.targetId)
				w("\t\tisa = PBXNativeTarget;\n")
				w("\t\tbuildConfigurationList = %s;\n" % self.buildSettingsId[0])
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
				w("\t\tproductType = \"com.apple.product-type.application\";\n")
				w("\t};\n")
		w("/* End PBXNativeTarget section */\n\n")

	def writePBXProject(self):
		w = self.file.write
		w("/* Begin PBXProject section */\n")
		w("\t%s /* Project object */ = {\n" % self.projectID)
		w("\t\tisa = PBXProject;\n")
		w("\t\tcompatibilityVersion = \"%s\";\n" % self.version[0])
		w("\t\tbuildConfigurationList = %s;\n" % self.configurationsID)
		w("\t\thasScannedForEncodings = 1;\n")
		w("\t\tprojectDirPath=\"\";\n")
		w("\t\tmainGroup = %s;\n" % self.mainGroup)
		w("\t\ttargets = (\n")
		for d in self.projects:
			if d.type in ['game', 'tool']:
				w("\t\t\t%s,\n" % d.targetId)
		w("\t\t);\n")
		w("\t};\n")
		w("/* End PBXProject section */\n\n")

	def writePBXSourcesBuildPhase(self):
		w = self.file.write
		w("/* Begin PBXSourcesBuildPhase section */\n")
		for d in self.projects:
			if d.type in ['game', 'tool']:
				w("\t%s = {\n" % d.phaseId[0])
				w("\t\tisa = PBXSourcesBuildPhase;\n")
				w("\t\tbuildActiobMask = 2147483647;\n")
				w("\t\tfiles = (\n")
				w("\t\t);\n")
				w("\t\trunOnlyForDeploymentPostprocessing = 0;\n")
				w("\t};\n")
		w("/* End PBXSourcesBuildPhase section */\n\n")

	def writeXCBuildConfiguration(self):
		w = self.file.write
		w("/* Begin XCBuildConfiguration section */\n")
		w("\t%s = {\n" % self.buildSettingsId[1][0])
		w("\t\tisa = XCBuildConfiguration;\n")
		w("\t\tbuildSettings = {\n")
		w("\t\t};\n")
		w("\t\tname = Default;\n")
		w("\t};\n")
		w("/* End XCBuildConfiguration section */\n\n")

	def writeXCConfigurationList(self):
		w = self.file.write
		w("/* Begin XCConfigurationList section */\n")
		w("\t%s = {\n" % self.buildSettingsId[0])
		w("\t\tisa = XCConfigurationList;\n")
		w("\t\tbuildConfigurations = (\n")
		w("\t\t\t%s,\n" % self.buildSettingsId[1][0])
		w("\t\t);\n")
		w("\t\tdefaultConfigurationIsVisible = 0;\n")
		w("\t\tdefaultConfigurationName = Default;\n")
		w("\t};\n")
		w("/* End XCConfigurationList section */\n\n")
		self.writeXCBuildConfiguration()

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

allconfigs = ['debug','profile','final']

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

GenerateProject = Task.task_type_from_func('generateProject', generateProject)

solutions = {}
def create_xcode_project(t):
	toolName = t.features[0]
	if not solutions.has_key(toolName):
		appname = getattr(Utils.g_module, 'APPNAME', 'noname')
		outname = 'project.pbxproj'
		solution = GenerateProject(env=t.env)
		solution.set_outputs(t.path.find_or_declare(outname))
		solution.name = appname
		solution.version = xcodeprojects[toolName]
		solution.install_path = t.path.srcpath(t.env)+'/'+appname+'.'+toolName+'.xcodeproj/'
		solution.projects = []
		solution.dep_vars = ['XCODE_PROJECT_DEPENDS']
		solution.env['XCODE_PROJECT_DEPENDS'] = []
		solutions[toolName] = solution
	solution = solutions[toolName]

	project = Project()
	project.type			= t.type
	#project.allplatforms    = projects[toolName][2]
	#project.platforms 		= filterplatforms(t.type, t.platforms, t.depends)
	project.version 		= toolName
	project.projectCategory = t.category
	project.projectName 	= t.name
	project.type 			= t.type
	project.sourceTree 		= t.sourcetree
	solution.projects.append(project)
	solution.env['XCODE_PROJECT_DEPENDS'].append(t.sourcetree)

for pname in xcodeprojects.keys():
	feature(pname)(create_xcode_project)

