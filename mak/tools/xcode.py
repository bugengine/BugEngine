from waflib.TaskGen import feature
from waflib.Configure import conf
from waflib import Context, TaskGen, Build
import os, sys, random, time
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
		self._parse = False
		if not filetype:
			_, ext = os.path.splitext(name)
			if ext == '.h':
				filetype = "sourcecode.c.h"
			elif ext in ['.hh', '.inl', '.hpp']:
				filetype = "sourcecode.cpp.h"
				self.xcLanguageSpecificationIdentifier = "xcode.lang.cpp"
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
			elif name == 'wscript' or ext == '.py':
				filetype = "sourcecode.script.python"
				self.xcLanguageSpecificationIdentifier = "xcode.lang.python"
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
		self._groups = {}

	def add_group(self, group):
		self._groups[group.name] = group
		self.children.append(group)
		self.sort()
		return group

	def add(self, root_node, source_node):
		result = []
		for f in source_node.listdir():
			fullname = os.path.join(source_node.abspath(), f)
			if os.path.isdir(fullname):
				try:
					g = self[f]
				except KeyError:
					g = self.add_group(PBXGroup(f))
				result += g.add(root_node, source_node.make_node(f))
			else:
				f = PBXFileReference(f, os.path.join(source_node.path_from(root_node), f))
				self.children.append(f)
				result.append(f)
		self.sort()
		return result

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
		self.fileRef = file

# Targets
class PBXLegacyTarget(XCodeNode):
	def __init__(self, action, target=''):
		XCodeNode.__init__(self)
		self.buildConfigurationList = XCConfigurationList([
			XCBuildConfiguration('debug', {}),
			XCBuildConfiguration('profile', {}),
			XCBuildConfiguration('final', {}),
			])
		self.buildArgumentsString = "%s %s" % (sys.argv[0], action)
		self.buildPhases = []
		self.buildToolPath = sys.executable
		self.buildWorkingDirectory = ""
		self.dependencies = []
		self.name = action
		self.productName = target or action
		self.passBuildSettingsInEnvironment = 0

class PBXShellScriptBuildPhase(XCodeNode):
	def __init__(self, action):
		XCodeNode.__init__(self)
		self.buildActionMask = 2147483647
		self.files = []
		self.inputPaths = []
		self.outputPaths = []
		self.runOnlyForDeploymentPostProcessing = 0
		self.shellPath = "/bin/sh"
		self.shellScript = "%s %s %s" % (sys.executable, sys.argv[0], action)

class PBXSourcesBuildPhase(XCodeNode):
	def __init__(self, files):
		XCodeNode.__init__(self)
		self.buildActionMask = 2147483647
		self.runOnlyForDeploymentPostProcessing = 0
		self.files = [PBXBuildFile(file) for file in files]

class PBXFrameworksBuildPhase(PBXSourcesBuildPhase):
	def __init__(self, frameworks):
		PBXSourcesBuildPhase.__init__(self, frameworks)

class PBXNativeTarget(XCodeNode):
	def __init__(self, name, product, productType, build, configs):
		XCodeNode.__init__(self)
		self.buildConfigurationList = XCConfigurationList(configs)
		self.buildPhases = build
		self.buildRules = []
		self.dependencies = []
		self.name = name
		self.productName = name
		self.productType = productType
		self.productReference =product


# Root project object

def macarch(arch):
	if arch == 'amd64':
		return 'x86_64'
	elif arch == 'arm':
		return 'armv6'
	elif arch == 'x86':
		return 'i386'
	elif arch == 'powerpc64':
		return 'ppc64'
	elif arch == 'powerpc':
		return 'ppc'
	else:
		return arch


class PBXProject(XCodeNode):
	def __init__(self, name, version, builder):
		XCodeNode.__init__(self)
		self.buildConfigurationList = XCConfigurationList([
			XCBuildConfiguration('debug', {'BUILT_OUTPUTS_DIR':'build', 'CONFIG':'debug'}),
			XCBuildConfiguration('profile', {'BUILT_OUTPUTS_DIR':'build', 'CONFIG':'profile'}),
			XCBuildConfiguration('final', {'BUILT_OUTPUTS_DIR':'build', 'CONFIG':'final'}),
			])
		self.compatibilityVersion = version[0]
		self.hasScannedForEncodings = 1;
		self.mainGroup = PBXGroup(name)
		self._mainGroup = PBXGroup(name)
		self.mainGroup.children.append(self._mainGroup)
		self.projectRoot = ""
		self.projectDirPath = ""
		self.targets = []
		self._objectVersion = version[1]
		self._output = PBXGroup('Products')
		self._output2 = PBXGroup('Dummies')
		self.mainGroup.children.append(self._output)
		self._output.children.append(self._output2)
		self._frameworks = PBXGroup('Frameworks')
		self.mainGroup.children.append(self._frameworks)
		self._frameworks_seen = {}
		self._frameworks_files = []
		for env_name in builder.env.ALL_TOOLCHAINS:
			env = builder.all_envs[env_name]
			if env.XCODE_FRAMEWORKS:
				try:
					seen,group = self._frameworks_seen[env.MACOSX_SDK]
				except:
					seen = []
					group = PBXGroup(env.MACOSX_SDK)
					self._frameworks.children.append(group)
					self._frameworks_seen[env.MACOSX_SDK] = (seen, group)
				for f in env.XCODE_FRAMEWORKS:
					if f not in seen:
						seen.append(f)
						file = PBXFileReference('%s.framework'%f, os.path.join(env.XCODE_SDK_PATH, 'System', 'Library', 'Frameworks', '%s.framework'%f), 'wrapper.framework', '<absolute>')
						self._frameworks_files.append(file)
						group.children.append(file)

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
		def get_include_path(i):
			if isinstance(i, str):
				return i
			else:
				return i.path_from(bld.srcnode)
		names = p.target.split('.')
		sources = []
		if 'kernel' not in p.features:
			group = self._mainGroup
			for name in names:
				try:
					group = group[name]
				except KeyError:
					group = group.add_group(PBXGroup(name))

			source_nodes = getattr(p, 'source_nodes', [])
			for source_node in source_nodes:
				sources += group.add(bld.path, source_node)

		includes = set(getattr(p, 'includes', []))
		defines = set(getattr(p, 'defines', []))
		task_gens = getattr(p, 'use', [])[:]
		seen = set([])
		while task_gens:
			name = task_gens.pop()
			if name not in seen:
				try:
					task_gen = bld.get_tgen_by_name(name)
				except:
					continue
				seen.add(name)
				task_gens += getattr(task_gen, 'use', [])
				includes.union(getattr(task_gen, 'export_includes', []))
				defines.union(getattr(task_gen, 'export_defines', []))


		for toolchain in bld.env.ALL_TOOLCHAINS:
			env = bld.all_envs[toolchain]
			includes.union(env.INCLUDES)
			defines.union(env.DEFINES)
		variants = []
		for variant in bld.env.ALL_VARIANTS:
			variants.append(XCBuildConfiguration(variant, {
				'PRODUCT_NAME': p.target,
				'ARCHS':['i386'],
				'SDKROOT': 'macosx',
				'SUPPORTED_PLATFORMS': 'macosx',
				'HEADER_SEARCH_PATHS': [get_include_path(i) for i in includes],
				'GCC_PREPROCESSOR_DEFINITIONS': [d for d in defines]}))
		buildPhase = PBXSourcesBuildPhase(sources)
		frameworkPhase = PBXFrameworksBuildPhase(self._frameworks_files)
		target = PBXNativeTarget(
				'index.'+p.target,
				PBXFileReference(p.target, 'lib%s.a'%p.target, 'archive.ar', 'BUILT_PRODUCTS_DIR'),
				"com.apple.product-type.library.static",
				[buildPhase, frameworkPhase],
				variants)
		self._output2.children.append(target.productReference)
		self.targets.append(target)
		scheme = XCodeScheme(target._id, 'index.'+p.target, 'lib%s.a'%p.target, schemes.project_name, True)
		schememanagement.add(scheme)



class xcode(Build.BuildContext):
	cmd = 'xcode'
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
		self.env.TOOLCHAIN = '$(TOOLCHAIN)'
		self.env.VARIANT = '$(CONFIG)'
		self.env.DEPLOY_ROOTDIR = '$(DEPLOY_BINDIR)'
		self.env.DEPLOY_BINDIR = '$(DEPLOY_BINDIR)'
		self.env.DEPLOY_RUNBINDIR = '$(DEPLOY_RUNBINDIR)'
		self.env.DEPLOY_LIBDIR = '$(DEPLOY_LIBDIR)'
		self.env.DEPLOY_INCLUDEDIR = '$(DEPLOY_INCLUDEDIR)'
		self.env.DEPLOY_DATADIR = '$(DEPLOY_DATADIR)'
		self.env.DEPLOY_PLUGINDIR = '$(DEPLOY_PLUGINDIR)'
		self.env.DEPLOY_KERNELDIR = '$(DEPLOY_KERNELDIR)'
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
		p = PBXProject(appname, self.__class__.version, self)

		schememanagement = XCodeSchemeList()

		for g in self.groups:
			for tg in g:
				if not isinstance(tg, TaskGen.task_gen):
					continue
				tg.post()
				p.add(self, tg, schemes, schememanagement)


		for toolchain in self.env.ALL_TOOLCHAINS:
			env = self.all_envs['%s'%(toolchain)]
			if env.XCODE_ABI == 'mach_o':
				variants = []
				for variant in self.env.ALL_VARIANTS:
					variants.append(XCBuildConfiguration(variant, {
								'PRODUCT_NAME': appname,
								'BUILT_PRODUCTS_DIR': 'build/%s/%s'%(toolchain, variant),
								'CONFIGURATION_BUILD_DIR':  'build/%s/%s'%(toolchain, variant),
								'ARCHS': macarch(env.VALID_ARCHITECTURES[0]),
								'SDKROOT': env.XCODE_SDKROOT,
								'SUPPORTED_PLATFORMS': env.XCODE_SUPPORTEDPLATFORMS}))
				build = PBXShellScriptBuildPhase('install:'+toolchain+':${CONFIG}')
				target = PBXNativeTarget(
								toolchain,
								PBXFileReference(appname, appname+'.app', 'wrapper.application', 'BUILT_PRODUCTS_DIR'),
								"com.apple.product-type.application",
								[build],
								variants)
				p.targets.append(target)
				p._output.children.append(target.productReference)
				scheme = XCodeScheme(target._id, toolchain, appname+'.app', schemes.project_name, False)
			else:
				target = PBXLegacyTarget(toolchain, 'install:%s:$(CONFIG)'%toolchain)
				p.targets.append(target)
				scheme = XCodeScheme(target._id, toolchain, appname, schemes.project_name, False)
				scheme.write(schemes.make_node('%s.xcscheme'%toolchain))
				schememanagement.add(scheme)

		schememanagement.write(schemes.make_node('xcschememanagement.plist'))
		node = project.make_node('project.pbxproj')
		p.write(open(node.abspath(), 'w'))


