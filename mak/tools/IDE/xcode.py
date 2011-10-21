from waflib.TaskGen import feature
from waflib.Configure import conf
from waflib import Context, Task
import os
import mak
import random
import time

def newid():
	return "%04X%04X%04X%012d" % (random.randint(0, 32767), random.randint(0, 32767), random.randint(0, 32767), int(time.time()))

class XCodeProject:
	def __init__(self, name, path, version, projects, out, bld):
		self.file = open(path, 'w')
		self.version = version
		self.projects = projects
		self.projectID = newid()
		self.mainGroup = newid()
		self.bld = bld
		self.targetId = {}
		self.productId = {}
		self.projectConfIdDebug = newid()
		self.projectConfIdProfile = newid()
		self.projectConfIdFinal = newid()
		self.projectConfListId = newid()
		self.out = out

		for toolchain in self.bld.env.ALL_VARIANTS:
			if toolchain[0:5] == 'debug':
				toolchain = toolchain[6:]
				self.targetId[toolchain] = newid()
				self.productId[toolchain] = newid()

	def writeHeader(self):
		w = self.file.write
		w("// !$*UTF8*$!\n")
		w("{\n")
		w("	archiveVersion = 1;\n")
		w("	classes = {\n")
		w("	};\n")
		w("	objectVersion = %d;\n" % self.version[1])
		w("	objects = {\n\n")

	def pbxDirTree(self, tree, name):
		w = self.file.write
		w("\t%s = {\n" % tree.id)
		w("\t\tisa = PBXGroup;\n")
		w("\t\tchildren = (\n")
		subdirs = []
		for n,d in tree.directories.items():
			d.id = newid()
			subdirs.append((n, d))
		subdirs.sort(key = lambda i: i[0])
		for n,d in subdirs:
			w("\t\t\t%s,\n"%d.id)
		for file in tree.files:
			if isinstance(file, mak.sources.generatedcppsource) or isinstance(file, mak.sources.generatedhsource):
				continue
			w("\t\t\t%s,\n"%file.id)
		w("\t\t);\n")
		w("\t\tname = \"%s\";\n" % name)
		w("\t\tsourceTree = \"<group>\";\n")
		w("\t};\n")
		for n,d in tree.directories.items():
			self.pbxDirTree(d, n)

	def pbxFileRefTree(self, p, tree, path=''):
		w = self.file.write
		for name,dir in tree.directories.items():
			self.pbxFileRefTree(p, dir, os.path.join(path, tree.prefix))
		for file in tree.files:
			filename = os.path.join(path, tree.prefix, file.filename)
			sourceroot = "SOURCE_ROOT"
			file.id = newid()
			if isinstance(file, mak.sources.generatedcppsource) or isinstance(file, mak.sources.generatedhsource):
				continue
			if isinstance(file, mak.sources.hsource):
				if file.filename[-2:] == '.h':
					filetype = "sourcecode.c.h"
				else:
					filetype = "sourcecode.cpp.h"
			elif isinstance(file, mak.sources.cppsource):
				if file.filename[-2:] == '.c':
					filetype = "sourcecode.c.c"
				elif file.filename[-2:] == '.m':
					filetype = "sourcecode.c.objc"
				elif file.filename[-3:] == '.mm':
					filetype = "sourcecode.cpp.objcpp"
				else:
					filetype = "sourcecode.cpp.cpp"
			elif isinstance(file, mak.sources.datasource):
				filetype = "sourcecode."
			elif isinstance(file, mak.sources.lexsource):
				filetype = "sourcecode.lex"
			elif isinstance(file, mak.sources.yaccsource):
				filetype = "sourcecode.yacc"
			elif isinstance(file, mak.sources.deployedsource):
				if file.filename.endswith('.plist'):
					filetype = "text.plist.xml"
				else:
					filetype = "text"
			else:
				filetype = "text"
			w("\t%s = {\n\t\tisa = PBXFileReference;\n\t\tfileEncoding = 4;\n\t\tlastKnownFileType = %s;\n\t\tname = \"%s\";\n\t\tpath = \"%s\";\n\t\tsourceTree = %s;\n\t};\n" % (file.id, filetype, os.path.split(filename)[1], filename, sourceroot))

	def writePBXFileReference(self):
		w = self.file.write
		w("/* Begin PBXFileReference section */\n")
		for d in self.projects:
			self.pbxFileRefTree(d, d.sourceTree)
		w("/* End PBXFileReference section */\n\n")

	def writePBXGroup(self):
		w = self.file.write

		products = newid()
		w("/* Begin PBXGroup section */\n")
		w("\t%s = {\n" % products)
		w("\t\tisa = PBXGroup;\n")
		w("\t\tchildren = (\n")
		for toolchain in self.bld.env.ALL_VARIANTS:
			env = self.bld.all_envs[toolchain]
			if toolchain[0:5] == 'debug':
				toolchain = toolchain[6:]
				if env.ABI == 'mach_o':
					w("\t\t\t%s,\n" % self.productId[toolchain])
		w("\t\t);\n")
		w("\t\tname = out;\n")
		w("\t\tsourceTree = \"<group>\";\n")
		w("\t};\n")

		w("\t%s = {\n" % self.mainGroup)
		w("\t\tisa = PBXGroup;\n")
		w("\t\tchildren = (\n")
		projects = []
		for d in self.projects:
			d.sourceTree.id = newid()
			projects.append((d.projectCategory+'.'+d.projectName, d))
		projects.sort(key = lambda i: i[0])
		for name, d in projects:
			w("\t\t\t%s,\n"%d.sourceTree.id)
		w("\t\t\t%s\n" % products)
		w("\t\t);\n")
		w("\t\tname = BugEngine;\n")
		w("\t\tsourceTree = \"<group>\";\n")
		w("\t};\n")
		for name, d in projects:
			self.pbxDirTree(d.sourceTree, name)
		w("/* End PBXGroup section */\n\n")

	def writeTarget(self, toolchain):
		w = self.file.write
		env = self.bld.all_envs['debug-'+toolchain]

		appname = getattr(Context.g_module, 'APPNAME', 'noname')

		def macarch(arch):
			if arch == 'amd64':
				return 'x86_64'
			elif arch == 'arm':
				return 'armv6'
			elif arch == 'x86':
				return 'i386'
			elif arch == 'ppc':
				return 'powerpc'
			elif arch == 'ppc64':
				return 'powerpc64'
			else:
				return arch


		conflist = newid()
		confDebug = newid()
		confProfile = newid()
		confFinal = newid()
		if env.ABI == 'mach_o':
			productname = "%s/%s" % (env.PREFIX, appname)
		else:
			productname = "%s/%s/%s" % (env.PREFIX, env.DEPLOY['prefix'], env.DEPLOY['bin'], env.program_PATTERN % self.out)

		w("\t%s = {\n" % confDebug)
		w("\t\tisa = XCBuildConfiguration;\n")
		w("\t\tbaseConfigurationReference = \"\";\n")
		w("\t\tbuildSettings = {\n")
		w("\t\t\tPRODUCT_NAME = \"%s\";\n" % appname)
		if env.ABI == 'mach_o':
			w("\t\t\tARCHS = \"%s\";\n" % macarch(env.ARCHITECTURE))
			w("\t\t\tSDKROOT = \"%s\";\n" % env.SDKROOT)
			w("\t\t\tTOOLCHAIN = \"%s\";\n" % toolchain)
			w("\t\t\tEFFECTIVE_PLATFORM_NAME = \"-%s\";\n" % toolchain)
		w("\t\t};\n")
		w("\t\tname = %s;\n" % "debug")
		w("\t};\n")

		w("\t%s = {\n" % confProfile)
		w("\t\tisa = XCBuildConfiguration;\n")
		w("\t\tbaseConfigurationReference = \"\";\n")
		w("\t\tbuildSettings = {\n")
		w("\t\t\tPRODUCT_NAME = \"%s\";\n" % appname)
		if env.ABI == 'mach_o':
			w("\t\t\tARCHS = \"%s\";\n" % macarch(env.ARCHITECTURE))
			w("\t\t\tSDKROOT = \"%s\";\n" % env.SDKROOT)
			w("\t\t\tCONFIGURATION_BUILD_DIR = \"$CONFIG-$TOOLCHAIN\";\n")
			w("\t\t\tTOOLCHAIN = \"%s\";\n" % toolchain)
			w("\t\t\tEFFECTIVE_PLATFORM_NAME = \"-%s\";\n" % toolchain)
		w("\t\t};\n")
		w("\t\tname = %s;\n" % "profile")
		w("\t};\n")

		w("\t%s = {\n" % confFinal)
		w("\t\tisa = XCBuildConfiguration;\n")
		w("\t\tbaseConfigurationReference = \"\";\n")
		w("\t\tbuildSettings = {\n")
		w("\t\t\tPRODUCT_NAME = \"%s\";\n" % appname)
		if env.ABI == 'mach_o':
			w("\t\t\tARCHS = \"%s\";\n" % macarch(env.ARCHITECTURE))
			w("\t\t\tSDKROOT = \"%s\";\n" % env.SDKROOT)
			w("\t\t\tCONFIGURATION_BUILD_DIR = \"$CONFIG-$TOOLCHAIN\";\n")
			w("\t\t\tTOOLCHAIN = \"%s\";\n" % toolchain)
			w("\t\t\tEFFECTIVE_PLATFORM_NAME = \"-%s\";\n" % toolchain)
		w("\t\t};\n")
		w("\t\tname = %s;\n" % "final")
		w("\t};\n")

		w("\t%s = {\n" % conflist)
		w("\t\tisa = XCConfigurationList;\n")
		w("\t\tbuildConfigurations = (\n")
		w("\t\t\t%s,\n" % confDebug)
		w("\t\t\t%s,\n" % confProfile)
		w("\t\t\t%s,\n" % confFinal)
		w("\t\t);\n")
		w("\t\tdefaultConfigurationIsVisible = 0;\n")
		w("\t\tdefaultConfigurationName = \"%s\";\n" % "debug")
		w("\t};\n")

		if env.ABI != 'mach_o':
			w("\t%s = {\n" % self.targetId[toolchain])
			w("\t\tisa = PBXLegacyTarget;\n")
			w("\t\tbuildArgumentsString = \"waf install_$CONFIG-$TOOLCHAIN\";\n")
			w("\t\tbuildConfigurationList = %s;\n" % conflist)
			w("\t\tbuildPhases = (\n")
			w("\t\t);\n")
			w("\t\tbuildToolPath = %s;\n" % "python")
			w("\t\tbuildWorkingDirectory = \"\";\n")
			w("\t\tdependencies = (\n")
			w("\t\t);\n")
			w("\t\tname = \"%s\";\n" % toolchain)
			w("\t\tproductName = \"%s\";\n" % toolchain)
			w("\t\tpassBuildSettingsInEnvironment = 0;\n")
			w("\t};\n")


		else:
			shellscript = newid()
			w("\t%s = {\n" % self.productId[toolchain])
			w("\t\tisa = PBXFileReference;\n")
			w("\t\texplicitFileType = wrapper.application;\n")
			w("\t\tincludeInIndex = 0;\n")
			w("\t\tpath = \"%s.app\";\n" % appname)
			w("\t\tsourceTree = BUILT_PRODUCTS_DIR;\n")
			w("\t};\n")

			w("\t%s = {\n" % shellscript)
			w("\t\tisa = PBXShellScriptBuildPhase;\n")
			w("\t\tbuildActionMask = 2147483647;\n")
			w("\t\tfiles = (\n")
			w("\t\t);\n")
			w("\t\tinputPaths = (\n")
			w("\t\t);\n")
			w("\t\toutputPaths = (\n")
			w("\t\t);\n")
			w("\t\trunOnlyForDeploymentPostProcessing = 0;\n")
			w("\t\tshellPath = /bin/sh;\n")
			w("\t\tshellScript = \"python waf install_$CONFIG-$TOOLCHAIN\";\n")
			w("\t};\n")


			w("\t%s = {\n" % self.targetId[toolchain])
			w("\t\tisa = PBXNativeTarget;\n")
			w("\t\tbuildConfigurationList = %s;\n" % conflist)
			w("\t\tbuildPhases = (\n")
			w("\t\t\t%s,\n" % shellscript)
			w("\t\t);\n")
			w("\t\tbuildRules = (\n")
			w("\t\t);\n")
			w("\t\tdependencies = (\n")
			w("\t\t);\n")
			w("\t\tname = \"%s\";\n" % toolchain)
			w("\t\tproductName = \"%s\";\n" % toolchain)
			w("\t\tproductReference = %s;\n" % self.productId[toolchain])
			w("\t\tproductType = \"com.apple.product-type.application\";\n")
			w("\t};\n")


	def writePBXLegacyTarget(self):
		w = self.file.write
		w("/* Begin PBXLegacyTarget section */\n")
		for toolchain in self.bld.env.ALL_VARIANTS:
			if toolchain[0:5] == 'debug':
				toolchain = toolchain[6:]
				self.writeTarget(toolchain)
		w("/* End PBXLegacyTarget section */\n\n")

	def writePBXProject(self):
		w = self.file.write
		w("/* Begin PBXProject section */\n")
		w("\t%s /* Project object */ = {\n" % self.projectID)
		w("\t\tisa = PBXProject;\n")
		w("\t\tbuildConfigurationList = %s;\n" % self.projectConfListId)
		w("\t\tcompatibilityVersion = \"%s\";\n" % self.version[0])
		w("\t\thasScannedForEncodings = 1;\n")
		w("\t\tmainGroup = %s;\n" % self.mainGroup)
		w("\t\tprojectDirPath=\"\";\n")
		w("\t\tprojectRoot=\"\";\n")
		w("\t\ttargets = (\n")
		for toolchain in self.bld.env.ALL_VARIANTS:
			if toolchain[0:5] == 'debug':
				toolchain = toolchain[6:]
				w("\t\t\t%s,\n" % self.targetId[toolchain])
		w("\t\t);\n")
		w("\t};\n")
		w("/* End PBXProject section */\n\n")

	def writeSources(self, sources, all):
		w = self.file.write
		for name,d in sources.directories.items():
			self.writeSources(d, all)
		for file in sources.files:
			if file.process:
				if isinstance(file, mak.sources.generatedcppsource):
					continue
				elif isinstance(file, mak.sources.cppsource) and all:
					w("\t\t\t%s,\n" % file.buildid)
				else:
					w("\t\t\t%s,\n" % file.buildid)


	def writeXCBuildConfiguration(self):
		w = self.file.write
		w("/* Begin XCBuildConfiguration section */\n")
		w("\t%s = {\n" % self.projectConfIdDebug)
		w("\t\tisa = XCBuildConfiguration;\n")
		w("\t\tbaseConfigurationReference = \"\";\n")
		w("\t\tbuildSettings = {\n")
		w("\t\t\tCONFIG=debug;\n")
		w("\t\t};\n")
		w("\t\tname = %s;\n" % "debug")
		w("\t};\n")

		w("/* Begin XCBuildConfiguration section */\n")
		w("\t%s = {\n" % self.projectConfIdProfile)
		w("\t\tisa = XCBuildConfiguration;\n")
		w("\t\tbaseConfigurationReference = \"\";\n")
		w("\t\tbuildSettings = {\n")
		w("\t\t\tCONFIG=profile;\n")
		w("\t\t};\n")
		w("\t\tname = %s;\n" % "profile")
		w("\t};\n")

		w("/* Begin XCBuildConfiguration section */\n")
		w("\t%s = {\n" % self.projectConfIdFinal)
		w("\t\tisa = XCBuildConfiguration;\n")
		w("\t\tbaseConfigurationReference = \"\";\n")
		w("\t\tbuildSettings = {\n")
		w("\t\t\tCONFIG=final;\n")
		w("\t\t};\n")
		w("\t\tname = %s;\n" % "final")
		w("\t};\n")
		w("/* End XCBuildConfiguration section */\n\n")

	def writeXCConfigurationList(self):
		self.writeXCBuildConfiguration()
		w = self.file.write
		w("/* Begin XCConfigurationList section */\n")
		w("\t%s = {\n" % self.projectConfListId)
		w("\t\tisa = XCConfigurationList;\n")
		w("\t\tbuildConfigurations = (\n")
		w("\t\t\t%s,\n" % self.projectConfIdDebug)
		w("\t\t\t%s,\n" % self.projectConfIdProfile)
		w("\t\t\t%s,\n" % self.projectConfIdFinal)
		w("\t\t);\n")
		w("\t\tdefaultConfigurationIsVisible = 0;\n")
		w("\t\tdefaultConfigurationName = \"%s\";\n" % "debug")
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
	'xcode3': ('Xcode 3.X', 45),
	'xcode4': ('Xcode 4.X', 46),
}

def generateProject(task):
	solution = XCodeProject( task.name,
							 task.outputs[0].abspath(),
							 task.version,
							 task.projects,
							 task.out,
							 task.bld)
	solution.writeHeader()
	#solution.writePBXBuildFile()
	solution.writePBXFileReference()
	solution.writePBXGroup()
	solution.writePBXLegacyTarget()
	solution.writePBXProject()
	solution.writeXCConfigurationList()
	solution.writeFooter()

GenerateProject = Task.task_factory('generateProject', generateProject)

solutions = {}
def create_xcode_project(t):
	toolName = t.features[0]
	appname = getattr(Context.g_module, 'APPNAME', 'noname')
	projectName = appname+'.'+toolName+'.xcodeproj/'
	if not toolName in solutions:
		outname = projectName+'project.pbxproj'
		solution = t.create_task('generateProject')
		solution.env = t.env.derive()
		outnode = t.path.find_or_declare("projects/"+outname)
		solution.set_outputs(outnode)
		t.bld.install_files(projectName, outnode)
		solution.name = appname
		solution.bld = t.bld
		solution.version = xcodeprojects[toolName]
		solution.install_path = t.path.srcpath()+'/'
		solution.projects = []
		solution.dep_vars = ['XCODE_PROJECT_DEPENDS']
		solution.env['XCODE_PROJECT_DEPENDS'] = []
		solutions[toolName] = solution
	solution = solutions[toolName]


	project = Project()
	project.type			= t.type
	project.platforms 		= t.platforms
	project.version 		= toolName
	project.projectCategory = t.category
	project.projectName 	= t.name
	project.sourceTree 		= t.sourcetree
	if project.type == 'game':
		solution.out = project.projectName
	solution.projects.append(project)
	solution.env['XCODE_PROJECT_DEPENDS'].append(t.sourcetree)

for pname in xcodeprojects.keys():
	feature(pname)(create_xcode_project)

