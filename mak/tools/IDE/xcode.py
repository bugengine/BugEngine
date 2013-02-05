from waflib.TaskGen import feature
from waflib.Configure import conf
from waflib import Context, TaskGen, Build
import os, sys, random, time
import mak
from xml.dom.minidom import Document

x = 2000
y = 10500999
def new_xcode_id():
	global x, y
	y = y + 1
	return "%04X%04X%04X%012d" % (0, x, 0, y)


class XmlNode:
	def __init__(self):
		self.document = Document()
		self.document.encoding = 'UTF-8'

	def _add(self, node, child_node, value = None):
		def setAttributes(node, attrs):
			for k, v in attrs.items():
				node.setAttribute(k, v)
		el = self.document.createElement(child_node)
		if (value):
			if type(value) == type(str()):
				el.appendChild(self.document.createTextNode(value))
			elif type(value) == type(dict()):
				setAttributes(el, value)
		node.appendChild(el)
		return el

	def write(self, node):
		node.write(self.document.toxml())

class XCodeScheme(XmlNode):
	def __init__(self, id, name, product, project, dummy):
		XmlNode.__init__(self)
		self.id = id
		self.name = name
		self.dummy = dummy
		scheme = self._add(self.document, 'Scheme', {'LastUpgradeVersion': '0450', 'version': '1.3'})
		buildAction = self._add(scheme, 'BuildAction', {'parallelizeBuildables': 'YES', 'buildImplicitDependencies': 'YES'})
		buildActionEntries = self._add(buildAction, 'BuildActionEntries')
		if not dummy:
			buildActionEntry = self._add(buildActionEntries, 'BuildActionEntry', {
					'buildForTesting': 'YES',
					'buildForRunning': 'YES',
					'buildForProfiling': 'YES',
					'buildForArchiving': 'YES',
					'buildForAnalyzing': 'YES'})
			testAction = self._add(scheme, 'TestAction', {
					'selectedDebuggerIdentifier': 'Xcode.DebuggerFoundation.Debugger.LLDB',
					'selectedLaunchIdentifier': 'Xcode.DebuggerFoundation.Debugger.LLDB',
					'shouldUseLaunchSchemeArgsEnv': 'YES',
					'buildConfiguration': 'debug'
					})
			self._add(testAction, 'Testables')
			macroExpansion = self._add(testAction, 'MacroExpansion')
			launchAction = self._add(scheme, 'LaunchAction', {
					'selectedDebuggerIdentifier': 'Xcode.DebuggerFoundation.Debugger.LLDB',
					'selectedLaunchIdentifier': 'Xcode.DebuggerFoundation.Debugger.LLDB',
					'launchStyle': '0',
					'useCustomWorkingDirectory': 'NO',
					'buildConfiguration': 'debug',
					'ignoresPersistentStateOnLaunch': 'NO',
					'debugDocumentVersioning': 'NO',
					'allowLocationSimulation': 'YES'})
			buildableProductRunnable = self._add(launchAction, 'BuildableProductRunnable')
			self._add(launchAction, 'AdditionalOptions')
			profileAction = self._add(scheme, 'ProfileAction', {
					'buildConfiguration': 'profile',
					'shouldUseLaunchSchemeArgsEnv': 'YES',
					'savedToolIdentifier': '',
					'useCustomWorkingDirectory': 'NO',
					'debugDocumentVersioning': 'NO'})
			buildableProductRunnable2 = self._add(profileAction, 'BuildableProductRunnable')
			analyzeAction = self._add(scheme, 'AnalyzeAction', {'buildConfiguration': 'debug'})
			archiveAction = self._add(scheme, 'ArchiveAction', {'buildConfiguration': 'final', 'revealArchiveInOrganizer': 'YES'})
			for node in [buildActionEntry, macroExpansion, buildableProductRunnable, buildableProductRunnable2]:
				self._add(node, 'BuildableReference', {
						'BuildableIdentifier': 'primary',
						'BlueprintIdentifier': self.id,
						'BuildableName': product,
						'BlueprintName': name,
						'ReferencedContainer': 'container:%s' % project})
		else:
			buildActionEntry = self._add(buildActionEntries, 'BuildActionEntry', {
					'buildForTesting': 'NO',
					'buildForRunning': 'NO',
					'buildForProfiling': 'NO',
					'buildForArchiving': 'NO',
					'buildForAnalyzinf': 'NO'})
			self._add(buildActionEntry, 'BuildableReference', {
					'BuildableIdentifier': 'primary',
					'BlueprintIdentifier': self.id,
					'BuildableName': product,
					'BlueprintName': name,
					'ReferencedContainer': 'container:%s' % project})


class XCodeSchemeList(XmlNode):
	def __init__(self):
		XmlNode.__init__(self)
		plist = self._add(self.document, 'plist', {'version': '1.0'})
		dict = self._add(plist, 'dict')
		self._add(dict, 'key', 'SchemeUserState')
		self.userstates = self._add(dict, 'dict')
		self._add(dict, 'key', 'SuppressBuildableAutocreation')
		self.autocreation = self._add(dict, 'dict')
		self.order = 0

	def add(self, scheme):
		self._add(self.userstates, 'key', '%s.xcscheme'%scheme.name)
		dict = self._add(self.userstates, 'dict')
		self._add(dict, 'key', 'isShown')
		if scheme.dummy:
			self._add(dict, 'false')
		else:
			self._add(dict, 'true')
		self._add(dict, 'key', 'orderHint')
		self._add(dict, 'integer', self.order.__str__())
		self._add(self.autocreation, 'key', scheme.id)
		dict = self._add(self.autocreation, 'dict')
		self._add(dict, 'key', 'primary')
		self._add(dict, 'true')
		self.order = self.order + 1

class XCodeNode:
	def __init__(self):
		self._id = new_xcode_id()

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
			if not file.generated():
				ref = PBXFileReference(file.filename, os.path.join(path, file.filename))
				self.children.append(ref)
				file.ref = ref
		self.sort()

	def sort(self):
		self.children.sort(key = lambda x: x.__class__.sort_prefix+x.name)

	def __getitem__(self, name):
		for g in self.children:
			if g.name == name:
				return g
		raise KeyError()


class PBXBuildFile(XCodeNode):
	def __init__(self, file):
		XCodeNode.__init__(self)
		self.fileRef = file.ref

# Targets
class PBXLegacyTarget(XCodeNode):
	def __init__(self, toolchain, action, target=''):
		XCodeNode.__init__(self)
		self.buildConfigurationList = XCConfigurationList([
			XCBuildConfiguration('debug', {}),
			XCBuildConfiguration('profile', {}),
			XCBuildConfiguration('final', {}),
			])
		if not toolchain:
			self.buildArgumentsString = "%s %s%s-${CONFIG}" % (sys.argv[0], action)
		else:
			self.buildArgumentsString = "%s %s" % (sys.argv[0], action)
		self.buildPhases = []
		self.buildToolPath = sys.executable
		self.buildWorkingDirectory = ""
		self.dependencies = []
		self.name = toolchain or action
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

class PBXSourcesBuildPhase(XCodeNode):
	def __init__(self, target):
		XCodeNode.__init__(self)
		self.buildActionMask = 2147483647
		self.runOnlyForDeploymentPostProcessing = 0
		self.files = []
		self.addFiles(target.sourcetree)

	def addFiles(self, sources):
		for name, tree in sources.directories.items():
			self.addFiles(tree)
		for file in sources.files:
			if not file.generated():
				self.files.append(PBXBuildFile(file))

class PBXNativeTarget(XCodeNode):
	def __init__(self, name, product, productType, build, configs):
		XCodeNode.__init__(self)
		self.buildConfigurationList = XCConfigurationList(configs)
		self.buildPhases = [build]
		self.buildRules = []
		self.dependencies = []
		self.name = name
		self.productName = name
		self.productType = productType
		self.productReference =product


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
			XCBuildConfiguration('debug', {'BUILT_OUTPUTS_DIR':'build', 'CONFIG':'debug'}),
			XCBuildConfiguration('profile', {'BUILT_OUTPUTS_DIR':'build', 'CONFIG':'profile'}),
			XCBuildConfiguration('final', {'BUILT_OUTPUTS_DIR':'build', 'CONFIG':'final'}),
			])
		self.compatibilityVersion = version[0]
		self.hasScannedForEncodings = 1;
		self.mainGroup = PBXGroup(name)
		self.projectRoot = ""
		self.projectDirPath = ""
		self.targets = []
		self._objectVersion = version[1]
		self._output = PBXGroup('Products')
		self._output2 = PBXGroup('Dummies')
		self.mainGroup.children.append(self._output)
		self._output.children.append(self._output2)
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

	def add(self, bld, p, schemes, schememanagement):
		names = p.name.split('.')
		group = PBXGroup(names[-1])
		group.add(p.sourcetree, p.sourcetree.prefix)
		g = self._groups[p.category]
		for subname in names[:-1]:
			try:
				g = g[subname]
			except KeyError:
				newgroup = PBXGroup(subname)
				g.children.append(newgroup)
				g.sort()
				g = newgroup
		g.children.append(group)
		g.sort()
		if p.type == 'waf':
			appname = getattr(Context.g_module, 'APPNAME', 'noname')
			for toolchain in bld.env.ALL_VARIANTS:
				if toolchain.endswith('-debug'):
					env = bld.all_envs[toolchain]
					toolchain = toolchain[:-6]
					if env.ABI == 'mach_o':
						debug = XCBuildConfiguration('debug', {
								'PRODUCT_NAME':appname,
								'BUILT_PRODUCTS_DIR': env.PREFIX,
								'CONFIGURATION_BUILD_DIR': env.PREFIX,
								'ARCHS':macarch(env.ARCHITECTURE, env.LP64),
								'SDKROOT': env.SDKROOT,
								'SUPPORTED_PLATFORMS': env.SUPPORTEDPLATFORMS})
						profile = XCBuildConfiguration('profile', {
								'PRODUCT_NAME':appname,
								'BUILT_PRODUCTS_DIR' :env.PREFIX.replace(toolchain+'-debug', toolchain+'-profile'),
								'CONFIGURATION_BUILD_DIR': env.PREFIX.replace(toolchain+'-debug', toolchain+'-profile'),
								'ARCHS':macarch(env.ARCHITECTURE, env.LP64),
								'SDKROOT': env.SDKROOT,
								'SUPPORTED_PLATFORMS': env.SUPPORTEDPLATFORMS})
						final = XCBuildConfiguration('final', {
								'PRODUCT_NAME':appname,
								'BUILT_PRODUCTS_DIR': env.PREFIX.replace(toolchain+'-debug', toolchain+'-final'),
								'CONFIGURATION_BUILD_DIR': env.PREFIX.replace(toolchain+'-debug', toolchain+'-final'),
								'ARCHS':macarch(env.ARCHITECTURE, env.LP64),
								'SDKROOT': env.SDKROOT,
								'SUPPORTED_PLATFORMS': env.SUPPORTEDPLATFORMS})
						build = PBXShellScriptBuildPhase('install_', toolchain+'-${CONFIG}')
						target = PBXNativeTarget(
								toolchain,
								PBXFileReference(appname, appname+'.app', 'wrapper.application', 'BUILT_PRODUCTS_DIR'),
								"com.apple.product-type.application",
								build,
								[debug, profile, final])
						self.targets.append(target)
						self._output.children.append(target.productReference)
						scheme = XCodeScheme(target._id, toolchain, appname+'.app', schemes.project_name, False)
					else:
						target = PBXLegacyTarget(toolchain, 'install_', p.name)
						self.targets.append(target)
						scheme = XCodeScheme(target._id, toolchain, appname, schemes.project_name, False)
					scheme.write(schemes.make_node('%s.xcscheme'%toolchain))
					schememanagement.add(scheme)
		else:
			includes=set([])
			defines = set([])
			for config in bld.env.ALL_VARIANTS:
				env = bld.all_envs[config]
				options = p.options[config]
				for i in options.includedir:
					if i not in includes:
						includes.add(i)
				for d in env.DEFINES:
					if d not in defines:
						defines.add(d)
			debug = XCBuildConfiguration('debug', {
					'PRODUCT_NAME': p.target,
					'ARCHS':['i386'],
					'SDKROOT': 'macosx',
					'SUPPORTED_PLATFORMS': 'macosx',
					'HEADER_SEARCH_PATHS': [i for i in includes],
					'GCC_PREPROCESSOR_DEFINITIONS': [d for d in defines]})
			profile = XCBuildConfiguration('profile', {
					'PRODUCT_NAME': p.target,
					'ARCHS':['i386'],
					'SDKROOT': 'macosx',
					'SUPPORTED_PLATFORMS': 'macosx',
					'HEADER_SEARCH_PATHS': [i for i in includes],
					'GCC_PREPROCESSOR_DEFINITIONS': [d for d in defines]})
			final = XCBuildConfiguration('final', {
					'PRODUCT_NAME': p.target,
					'ARCHS':['i386'],
					'SDKROOT': 'macosx',
					'SUPPORTED_PLATFORMS': 'macosx',
					'HEADER_SEARCH_PATHS': [i for i in includes],
					'GCC_PREPROCESSOR_DEFINITIONS': [d for d in defines]})
			buildPhase = PBXSourcesBuildPhase(p)
			target = PBXNativeTarget(
					p.target,
					PBXFileReference(p.target, 'lib%s.a'%p.target, 'archive.ar', 'BUILT_PRODUCTS_DIR'),
					"com.apple.product-type.library.static",
					buildPhase,
					[debug, profile, final])
			self._output2.children.append(target.productReference)
			self.targets.append(target)
			scheme = XCodeScheme(target._id, p.target, 'lib%s.a'%p.target, schemes.project_name, True)
			schememanagement.add(scheme)



class xcode3(Build.BuildContext):
	cmd = 'xcode3'
	fun = 'build'
	version = ('Xcode 3.1', 45)

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
		import getpass


		project = self.srcnode.make_node('%s.%s.xcodeproj' % (appname, self.__class__.cmd))
		project.mkdir()
		userdata = project.make_node('xcuserdata')
		userdata.mkdir()
		userdata = userdata.make_node('%s.xcuserdatad' % getpass.getuser())
		userdata.mkdir()
		schemes = userdata.make_node('xcschemes')
		schemes.mkdir()
		schemes.project_name = project.name
		p = PBXProject(appname, self.__class__.version)

		schememanagement = XCodeSchemeList()

		for g in self.groups:
			for tg in g:
				if not isinstance(tg, TaskGen.task_gen):
					continue
				tg.post()
				p.add(self, tg, schemes, schememanagement)

		schememanagement.write(schemes.make_node('xcschememanagement.plist'))
		node = project.make_node('project.pbxproj')
		p.write(open(node.abspath(), 'w'))


