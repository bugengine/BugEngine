from waflib.TaskGen import feature
from waflib.Configure import conf
from waflib import Context, TaskGen, Build
import os, sys
import mak
import random
import time

x = 2000
y = 10500999
def newid():
	global x, y
	y = y + 1
	return "%04X%04X%04X%012d" % (0, x, 0, y)


class XCodeNode:
	def __init__(self):
		self._id = newid()

	def tostring(self, value):
		if isinstance(value, dict):
			result = "{\n"
			for k,v in value.items():
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
			for k,v in value.items():
				self.write_recursive(v, file)
		elif isinstance(value, list):
			for i in value:
				self.write_recursive(i, file)
		elif isinstance(value, XCodeNode):
			value.write(file)

	def write(self, file):
		for attribute,value in self.__dict__.items():
			if attribute[0] != '_':
				self.write_recursive(value, file)

		w = file.write
		w("\t%s = {\n" % self._id)
		w("\t\tisa = %s;\n" % self.__class__.__name__)
		for attribute,value in self.__dict__.items():
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
	sort_prefix = 'b'
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
	sort_prefix = 'a'
	def __init__(self, name, sourcetree = "<group>"):
		XCodeNode.__init__(self)
		self.children = []
		self.name = name
		self.sourceTree = sourcetree

	def add(self, sourcetree, path):
		for name,dir in sourcetree.directories.items():
			group = PBXGroup(name)
			self.children.append(group)
			group.add(dir, os.path.join(path, dir.prefix))
		for file in sourcetree.files:
			ref = PBXFileReference(file.filename, os.path.join(path, file.filename))
			self.children.append(ref)
		self.sort()

	def sort(self):
		self.children.sort(key = lambda x: x.__class__.sort_prefix+x.name)


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

def macarch(arch, is64):
	if arch == 'amd64':
		return 'x86_64'
	elif arch == 'arm':
		return 'armv6'
	elif arch == 'x86':
		return 'i386'
	elif arch == 'ppc':
		if is64:
			return 'ppc64'
		else:
			return 'ppc'
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
		self._output = PBXGroup('Products')
		self.mainGroup.children.append(self._output)
		self._groups = {}
		for i in ['3rdparty', 'engine', 'game', 'plugin']:
			g = PBXGroup(i)
			self._groups[i] = g
			self.mainGroup.children.append(g)

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
		group = PBXGroup(p.name)
		group.add(p.sourcetree, p.sourcetree.prefix)
		g = self._groups[p.category]
		g.children.append(group)
		g.sort()
		if p.category == 'game':
			appname = getattr(Context.g_module, 'APPNAME', 'noname')
			for toolchain in bld.env.ALL_VARIANTS:
				if toolchain.startswith('debug-'):
					env = bld.all_envs[toolchain]
					toolchain = toolchain[6:]
					if env.ABI == 'mach_o':
						target = PBXNativeTarget('install_', toolchain, appname,
								env.PREFIX.replace('debug-'+toolchain, '%s-'+toolchain),
								macarch(env.ARCHITECTURE, env.LP64), env.SDKROOT, env.SUPPORTEDPLATFORMS)
						self.targets.append(target)
						self._output.children.append(target.productReference)
					else:
						self.targets.append(PBXLegacyTarget('install_', p.name))


class xcode3(Build.BuildContext):
	cmd = 'xcode3'
	fun = 'build'
	version = ('Xcode 3.2', 45)

	def execute(self):
		"""
		Entry point
		"""
		self.restore()
		if not self.all_envs:
			self.load_envs()
		self.env.PROJECTS=[self.__class__.cmd]
		self.recurse([self.run_dir])


		appname = getattr(Context.g_module, Context.APPNAME, os.path.basename(self.srcnode.abspath()))
		p = PBXProject(appname, self.__class__.version)

		for g in self.groups:
			for tg in g:
				if not isinstance(tg, TaskGen.task_gen):
					continue
				tg.post()
				p.add(self, tg)

		node = self.srcnode.make_node('%s.%s.xcodeproj' % (appname, self.__class__.cmd))
		node.mkdir()
		node = node.make_node('project.pbxproj')
		p.write(open(node.abspath(), 'w'))


