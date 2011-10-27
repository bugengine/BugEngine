from waflib.TaskGen import feature
from waflib.Configure import conf
from waflib import Context, Task
import os, sys
import mak
import random
import time

def newid():
	return "%04X%04X%04X%012d" % (random.randint(0, 32767), random.randint(0, 32767), random.randint(0, 32767), int(time.time()))


class XCodeNode:
	def __init__(self):
		self._id = newid()

	def tostring(self, value):
		if isinstance(value, dict):
			result = "{\n"
			for k,v in value.iteritems():
				result = result + "\t\t\t%s = %s;\n" % (k, self.tostring(v))
			result = result + "\t\t}"
			return result
		elif isinstance(value, str):
			return "\"%s\"" % value
		elif isinstance(value, list):
			result = "(\n"
			for i in value:
				result = result + "\t\t\t%s,\n" % self.tostring(i)
			result = result + "\t\t)"
			return result
		elif isinstance(value, XCodeNode):
			return value._id
		else:
			return str(value)

	def write_recursive(self, value, file):
		if isinstance(value, dict):
			for k,v in value.iteritems():
				self.write_recursive(v, file)
		elif isinstance(value, list):
			for i in value:
				self.write_recursive(i, file)
		elif isinstance(value, XCodeNode):
			value.write(file)
		
	def write(self, file):
		for attribute,value in self.__dict__.iteritems():
			if attribute[0] != '_':
				self.write_recursive(value, file)

		w = file.write
		w("\t%s = {\n" % self._id)
		w("\t\tisa = %s;\n" % self.__class__.__name__)
		for attribute,value in self.__dict__.iteritems():
			if attribute[0] != '_':
				w("\t\t%s = %s;\n" % (attribute, self.tostring(value)))
		w("\t};\n\n")



# Configurations
class XCBuildConfiguration(XCodeNode):
	def __init__(self, name, settings = {}):
		XCodeNode.__init__(self)
		self.baseConfigurationReference = ""
		self.buildSettings = settings
		self.name = name

class XCConfigurationList(XCodeNode):
	def __init__(self, settings):
		XCodeNode.__init__(self)
		self.buildConfigurations = settings
		self.defaultConfigurationIsVisible = 0
		self.defaultConfigurationName = settings and settings[0].name or ""

# Group/Files
class PBXFileReference(XCodeNode):
	def __init__(self, name, path, filetype = '', sourcetree = "SOURCE_ROOT"):
		XCodeNode.__init__(self)
		self.fileEncoding = 4
		if not filetype:
			_, ext = os.path.splitext(name)
			if ext == '.h':
				filetype = "sourcecode.c.h"
			elif ext in ['.hh', '.inl', '.hpp']:
				filetype = "sourcecode.cpp.h"
			elif ext == '.c':
				filetype = "sourcecode.c.c"
			elif ext == '.m':
				filetype = "sourcecode.c.objc"
			elif ext == '.mm':
				filetype = "sourcecode.cpp.objcpp"
			elif ext in ['.cc', '.cpp', '.C', '.cxx', '.c++']:
				filetype = "sourcecode.cpp.cpp"
			elif ext in ['.l', '.ll']:
				filetype = "sourcecode.lex"
			elif ext in ['.y', '.yy']:
				filetype = "sourcecode.yacc"
			elif ext == '.plist':
				filetype = "text.plist.xml"
			elif ext == ".nib":
				filetype = "wrapper.nib"
			elif ext == ".xib":
				filetype = "text.xib"
			else:
				filetype = "text"
		self.lastKnownFileType = filetype
		self.name = name
		self.path = path
		self.sourceTree = sourcetree

class PBXGroup(XCodeNode):
	def __init__(self, name, sourcetree = "<group>"):
		XCodeNode.__init__(self)
		self.children = []
		self.name = name
		self.sourceTree = sourcetree

	def add(self, sourcetree, path):
		for name,dir in sourcetree.directories.iteritems():
			group = PBXGroup(name)
			self.children.append(group)
			group.add(dir, os.path.join(path, dir.prefix))
		for file in sourcetree.files:
			ref = PBXFileReference(file.filename, os.path.join(path, file.filename))


# Targets
class PBXLegacyTarget(XCodeNode):
	def __init__(self, action, target=''):
		XCodeNode.__init__(self)
		self.buildConfigurationList = XCConfigurationList([
			XCBuildConfiguration('debug', {}),
			XCBuildConfiguration('profile', {}),
			XCBuildConfiguration('final', {}),
			])
		if not target:
			self.buildArgumentsString = "%s %s" % (sys.argv[0], action)
		else:
			self.buildArgumentsString = "%s %s --targets=%s" % (sys.argv[0], action, target)
		self.buildPhases = []
		self.buildToolPath = sys.executable
		self.buildWorkingDirectory = ""
		self.dependencies = []
		self.name = target or action
		self.productName = target or action
		self.passBuildSettingsInEnvironment = 0

class PBXShellScriptBuildPhase(XCodeNode):
	def __init__(self, action, toolchain):
		XCodeNode.__init__(self)
		self.buildActionMask = 2147483647
		self.files = []
		self.inputPaths = []
		self.outputPaths = []
		self.runOnlyForDeploymentPostProcessing = 0
		self.shellPath = "/bin/sh"
		self.shellScript = "%s %s %s%s" % (sys.executable, sys.argv[0], action,  toolchain)

class PBXNativeTarget(XCodeNode):
	def __init__(self, action, toolchain, app, output, archs, sdk, platforms):
		XCodeNode.__init__(self)
		debug = XCBuildConfiguration('debug',
				{ 'PRODUCT_NAME':app, 'CONFIGURATION_BUILD_DIR':output%'debug', 'ARCHS':archs, 'SDKROOT': sdk, 'SUPPORTED_PLATFORMS':platforms})
		profile = XCBuildConfiguration('profile',
				{ 'PRODUCT_NAME':app, 'CONFIGURATION_BUILD_DIR':output%'profile', 'ARCHS':archs, 'SDKROOT': sdk, 'SUPPORTED_PLATFORMS':platforms})
		final = XCBuildConfiguration('final',
				{ 'PRODUCT_NAME':app, 'CONFIGURATION_BUILD_DIR':output%'final', 'ARCHS':archs, 'SDKROOT': sdk, 'SUPPORTED_PLATFORMS':platforms})
		self.buildConfigurationList = XCConfigurationList([debug, profile, final])
		self.buildPhases = [PBXShellScriptBuildPhase(action, "${CONFIG}-"+toolchain)]
		self.buildRules = []
		self.dependencies = []
		self.name = toolchain
		self.productName = toolchain
		self.productType = "com.apple.product-type.application"
		self.productReference = PBXFileReference(app, app+'.app', 'wrapper.application', 'BUILT_PRODUCTS_DIR')

# Root project object

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


class PBXProject(XCodeNode):
	def __init__(self, name, version):
		XCodeNode.__init__(self)
		self.buildConfigurationList = XCConfigurationList([
			XCBuildConfiguration('debug', {'CONFIG':'debug'}),
			XCBuildConfiguration('profile', {'CONFIG':'profile'}),
			XCBuildConfiguration('final', {'CONFIG':'final'}),
			])
		self.compatibilityVersion = version[0]
		self.hasScannedForEncodings = 1;
		self.mainGroup = PBXGroup(name)
		self.projectRoot = ""
		self.projectDirPath = ""
		self.targets = []
		self._objectVersion = version[1]
		self._output = PBXGroup('out')
		self.mainGroup.children.append(self._output)

	def write(self, file):
		w = file.write
		w("// !$*UTF8*$!\n")
		w("{\n")
		w("\tarchiveVersion = 1;\n")
		w("\tclasses = {\n")
		w("\t};\n")
		w("\tobjectVersion = %d;\n" % self._objectVersion)
		w("\tobjects = {\n\n")

		XCodeNode.write(self, file)

		w("\t};\n")
		w("\trootObject = %s;\n" % self._id)
		w("}\n")

	def add(self, bld, p):
		group = PBXGroup(p.projectName)
		group.add(p.sourceTree, '')
		self.mainGroup.children.append(group)
		if p.type == 'game':
			appname = getattr(Context.g_module, 'APPNAME', 'noname')
			for toolchain in bld.env.ALL_VARIANTS:
				if toolchain.startswith('debug-'):
					env = bld.all_envs[toolchain]
					toolchain = toolchain[6:]
					if env.ABI == 'mach_o':
						target = PBXNativeTarget('install_', toolchain, appname,
								env.PREFIX.replace('debug-'+toolchain, '%s-'+toolchain),
								macarch(env.ARCHITECTURE), env.SDKROOT, env.SUPPORTEDPLATFORMS)
						self.targets.append(target)
						self._output.children.append(target.productReference)
					else:
						self.targets.append(PBXLegacyTarget('install_', tg.name))
		



class Project:
	pass

xcodeprojects = {
	'xcode3': ('Xcode 3.X', 45),
	'xcode4': ('Xcode 4.X', 46),
}

def generateProject(task):
	project = PBXProject(task.name, task.version)
	for p in task.projects:
		project.add(task.bld, p)
	project.write(open(task.outputs[0].abspath(), 'w'))

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

