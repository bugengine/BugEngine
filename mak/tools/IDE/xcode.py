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

	def writeHeader(self):
		w = self.file.write
		w("// !$*UTF8*$!\n")
		w("{\n")
		w("	archiveVersion = 1;\n")
		w("	classes = {\n")
		w("	};\n")
		w("	objectVersion = %d;\n" % self.version)
		w("	objects = {\n\n")

	def writePBXBuildFile(self):
		w = self.file.write
		w("/* Begin PBXBuildFile section */\n")
		for d in self.projects:
			pass
		w("/* End PBXBuildFile section */\n\n")

	def writePBXFileReference(self):
		w = self.file.write
		w("/* Begin PBXFileReference section */\n")
		for d in self.projects:
			pass
		w("/* End PBXFileReference section */\n\n")

	def writePBXFrameworksBuildPhase(self):
		w = self.file.write
		w("/* Begin PBXFrameworksBuildPhase section */\n")
		for d in self.projects:
			pass
		w("/* End PBXFrameworksBuildPhase section */\n\n")

	def writePBXBuildGroup(self):
		w = self.file.write
		w("/* Begin PBXBuildGroup section */\n")
		for d in self.projects:
			pass
		w("/* End PBXBuildGroup section */\n\n")

	def writePBXHeadersBuildPhase(self):
		w = self.file.write
		w("/* Begin PBXHeadersBuildPhase section */\n")
		for d in self.projects:
			pass
		w("/* End PBXHeadersBuildPhase section */\n\n")

	def writePBXNativeTarget(self):
		w = self.file.write
		w("/* Begin PBXNativeTarget section */\n")
		for d in self.projects:
			pass
		w("/* End PBXNativeTarget section */\n\n")

	def writePBXProject(self):
		w = self.file.write
		w("/* Begin PBXProject section */\n")
		w("\t%s /* Project object */ = {\n" % self.projectID)
		w("\t\tisa = PBXProject;\n")
		w("\t\thasScannedForEncodings = 1;\n")
		w("\t\tprojectDirPath=\"\";\n")
		w("\t\ttargets = (\n")
		w("\t\t);\n")
		w("\t};\n")
		for d in self.projects:
			pass
		w("/* End PBXProject section */\n\n")

	def writePBXSourcesBuildPhase(self):
		w = self.file.write
		w("/* Begin PBXSourcesBuildPhase section */\n")
		for d in self.projects:
			pass
		w("/* End PBXSourcesBuildPhase section */\n\n")

	def writeXCBuildConfiguration(self):
		w = self.file.write
		w("/* Begin XCBuildConfiguration section */\n")
		for d in self.projects:
			pass
		w("/* End XCBuildConfiguration section */\n\n")

	def writeXCConfigurationList(self):
		w = self.file.write
		w("/* Begin XCConfigurationList section */\n")
		for d in self.projects:
			pass
		w("/* End XCConfigurationList section */\n\n")

	def writeFooter(self):
		self.file.write("\t};\n")
		self.file.write("\trootObject = %s;\n" % self.projectID)
		self.file.write("}\n")

class Project:
	def __init__(self):
		pass

xcodeprojects = {
	'xcode2': ('XCode 2.5', 42),
}

allconfigs = ['debug','release','profile','final']

def generateProject(task):
	solution = XCodeProject( task.name,
							 task.outputs[0].bldpath(task.env),
							 task.version,
							 task.projects)
	solution.writeHeader()
	solution.writePBXBuildFile()
	solution.writePBXFileReference()
	solution.writePBXFrameworksBuildPhase()
	solution.writePBXBuildGroup()
	solution.writePBXHeadersBuildPhase()
	solution.writePBXNativeTarget()
	solution.writePBXProject()
	solution.writePBXSourcesBuildPhase()
	solution.writeXCBuildConfiguration()
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
		solution.version = xcodeprojects[toolName][1]
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
	#solution.projects.append(t)
	solution.env['XCODE_PROJECT_DEPENDS'].append(t.sourcetree)

for pname in xcodeprojects.keys():
	feature(pname)(create_xcode_project)

