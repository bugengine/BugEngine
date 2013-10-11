from waflib import Context, Build, TaskGen, Logs
import os, sys
from xml.dom.minidom import Document
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
		try:
			i = ord(c)
		except:
			i = c
		r = r + h[(i >> 4) & 0xF] + h[i & 0xF]
	return r

def generateGUID(name):
	"""This generates a dummy GUID for the sln file to use.  It is
	based on the MD5 signatures of the sln filename plus the name of
	the project.  It basically just needs to be unique, and not
	change with each invocation."""
	d = _hexdigest(createMd5(str(name).encode()).digest()).upper()
	# convert most of the signature to GUID form (discard the rest)
	d = "{" + d[:8] + "-" + d[8:12] + "-" + d[12:16] + "-" + d[16:20] + "-" + d[20:32] + "}"
	return d

def unique(seq):
	seen = set()
	seen_add = seen.add
	return [ x for x in seq if x not in seen and not seen_add(x)]

def gather_includes_defines(task_gen):
	defines = getattr(task_gen, 'defines', [])
	includes = getattr(task_gen, 'includes', [])
	seen = []
	use = getattr(task_gen, 'use', [])[:]
	while use:
		name = use.pop()
		if name not in seen:
			try:
				t = task_gen.bld.get_tgen_by_name(name)
			except:
				pass
			else:
				use = use + getattr(t, 'use', [])
				includes = includes + getattr(t, 'includes ', [])
				defines = defines + getattr(t, 'defines ', [])
	return unique(includes), unique(defines)

def path_from(path, bld):
	if isinstance(path, str):
		return path
	else:
		return '$(SolutionDir)%s' % path.path_from(bld.srcnode)

class XmlFile:
	def __init__(self):
		self.document = Document()

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
		try:
			xml = node.read()
		except IOError:
			xml = ''
		newxml = self.document.toxml()
		if xml != newxml:
			Logs.pprint('NORMAL', 'writing %s' % node.abspath())
			node.write(newxml)

class Solution:
	def __init__(self, bld, appname, version_number, version_name, use_folders):
		self.header = 'Microsoft Visual Studio Solution File, Format Version %s\n# %s' % (version_number, version_name)
		self.projects = []
		self.project_configs = []
		self.configs = ['\t\t%s|%s = %s|%s'%(v, t, v, t) for v in bld.env.ALL_VARIANTS for t in bld.env.ALL_TOOLCHAINS]
		self.folders = []
		self.folders_made = {}
		self.use_folders = use_folders
		self.master = ''

	def addFolder(self, name):
		names = name.split('.')[:-1]
		if names:
			folder_name = '.'.join(names)
			try:
				folder = self.folders_made[folder_name]
			except KeyError:
				folder = generateGUID(folder_name)
				self.folders_made[folder_name] = folder
				self.projects.append('Project("{2150E333-8FDC-42A3-9474-1A3956D46DE8}") = "%s", "%s", "%s"\nEndProject' % (names[-1], names[-1], folder))
				parent = self.addFolder(folder_name)
				if parent:
					self.folders.append((folder, parent))
			return folder
		else:
			return None

	def get_dependency(self):
		if self.master:
			return "	ProjectSection(ProjectDependencies) = postProject\n		%s = %s\n	EndProjectSection\n" % (self.master, self.master)
		else:
			return ''


	def add(self, task_gen, project, project_path, build = False):
		self.projects.append('Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "%s", "%s", "%s"\n%sEndProject' % (project.name, project_path, project.guid, self.get_dependency()))
		project_config = []
		for t in task_gen.bld.env.ALL_TOOLCHAINS:
			env = task_gen.bld.all_envs[t]
			platform = task_gen.bld.get_platform(env.MS_PROJECT_PLATFORM)
			project_config += ['\t\t%s.%s|%s.ActiveCfg = %s-%s|%s'%(project.guid, v, t, t, v, platform) for v in task_gen.bld.env.ALL_VARIANTS]
			if build:
				self.master = project.guid
				project_config += ['\t\t%s.%s|%s.Build.0 = %s-%s|%s'%(project.guid, v, t, t, v, platform) for v in task_gen.bld.env.ALL_VARIANTS]
		self.project_configs += project_config
		if self.use_folders:
			parent = self.addFolder(task_gen.target)
			if parent:
				self.folders.append((project.guid, parent))


	def write(self, node):
		nested_projects = ''
		if self.use_folders:
			nested_projects = '\tGlobalSection(NestedProjects) = preSolution\n%s\n\tEndGlobalSection\n' % '\n'.join(
					['\t\t%s = %s' % (project, parent) for project, parent in self.folders]
				)
		newsolution = '%s\n%s\nGlobal\n\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\n%s\n\tEndGlobalSection\n\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\n%s\n\tEndGlobalSection\n%sEndGlobal\n' % (self.header, '\n'.join(self.projects), '\n'.join(self.configs), '\n'.join(self.project_configs), nested_projects)
		try:
			solution = node.read()
		except IOError:
			solution = ''
		if solution != newsolution:
			Logs.pprint('NORMAL', 'writing %s' % node.name)
			node.write(newsolution)

class VCproj:
	extensions = ['vcproj']
	def __init__(self, task_gen, version, version_project):
		self.vcproj = XmlFile()

	def write(self, nodes):
		self.vcproj.write(nodes[0])

class VCxproj:
	extensions = ['vcxproj', 'vcxproj.filters']
	def __init__(self, task_gen, version, version_project, use_folders):
		self.vcxproj = XmlFile()
		self.vcxfilters = XmlFile()
		if use_folders:
			self.name = task_gen.target.split('.')[-1]
		else:
			self.name = task_gen.target
		project = self.vcxfilters._add(self.vcxfilters.document, 'Project', {'DefaultTargets':'Build', 'ToolsVersion':'4.0', 'xmlns':'http://schemas.microsoft.com/developer/msbuild/2003'})
		self.filter_nodes = self.vcxfilters._add(project, 'ItemGroup')
		self.file_nodes = self.vcxfilters._add(project, 'ItemGroup')

		self.guid = generateGUID(task_gen.target)
		project = self.vcxproj._add(self.vcxproj.document, 'Project', {'DefaultTargets':'Build', 'ToolsVersion':'4.0', 'xmlns':'http://schemas.microsoft.com/developer/msbuild/2003'})
		configs = self.vcxproj._add(project, 'ItemGroup', {'Label': 'ProjectConfigurations'})
		for toolchain in task_gen.bld.env.ALL_TOOLCHAINS:
			env = task_gen.bld.all_envs[toolchain]
			for variant in task_gen.bld.env.ALL_VARIANTS:
				config = self.vcxproj._add(configs, 'ProjectConfiguration', {'Include': '%s|%s' % (variant, toolchain)})
				self.vcxproj._add(config, 'Configuration', '%s-%s' % (toolchain, variant))
				self.vcxproj._add(config, 'Platform', task_gen.bld.get_platform(env.MS_PROJECT_PLATFORM))
		for toolchain in task_gen.bld.env.ALL_TOOLCHAINS:
			env = task_gen.bld.all_envs[toolchain]
			pgroup = self.vcxproj._add(project, 'PropertyGroup')
			self.vcxproj._add(pgroup, 'PlatformShortName', toolchain)
			self.vcxproj._add(pgroup, 'PlatformArchitecture', env.VALID_ARCHITECTURES[0])
			self.vcxproj._add(pgroup, 'PlatformTarget', toolchain)

		globals = self.vcxproj._add(project, 'PropertyGroup', {'Label': 'Globals'})
		self.vcxproj._add(globals, 'ProjectGUID', self.guid)
		self.vcxproj._add(globals, 'TargetFrameworkVersion', 'v'+version_project[0])
		self.vcxproj._add(globals, 'RootNamespace', task_gen.target)
		self.vcxproj._add(globals, 'ProjectName', self.name)
		self.vcxproj._add(project, 'Import', {'Project': '$(VCTargetsPath)\\Microsoft.Cpp.Default.props'})
		self.vcxproj._add(project, 'Import', {'Project': '$(VCTargetsPath)\\Microsoft.Cpp.props'})
		configuration = self.vcxproj._add(project, 'PropertyGroup', {'Label': 'Configuration'})
		self.vcxproj._add(configuration, 'ConfigurationType', 'Makefile')
		self.vcxproj._add(configuration, 'PlatformToolset', 'v%d'% (float(version_project[1])*10))
		self.vcxproj._add(configuration, 'OutDir', '$(SolutionDir)\\')
		self.vcxproj._add(configuration, 'IntDir', '$(SolutionDir)\\.build\\')

		includes, defines = gather_includes_defines(task_gen)
		for toolchain in task_gen.bld.env.ALL_TOOLCHAINS:
			env = task_gen.bld.all_envs[toolchain]
			if env.SUB_TOOLCHAINS:
				env = task_gen.bld.all_envs[env.SUB_TOOLCHAINS[0]]
			for variant in task_gen.bld.env.ALL_VARIANTS:
				properties = self.vcxproj._add(project, 'PropertyGroup', {'Condition': "'$(Configuration)'=='%s-%s'" % (toolchain, variant)})
				for var in ['Prefix', 'Variant', 'Toolchain', 'Deploy_BinDir', 'Deploy_RunBinDir', 'Deploy_LibDir',
							'Deploy_IncludeDir', 'Deploy_DataDir', 'Deploy_PluginDir', 'Deploy_KernelDir', 'Deploy_RootDir']:
					self.vcxproj._add(properties, var, env[var.upper()])
				command = getattr(task_gen, 'command', '')
				if command:
					command = command % {'toolchain':toolchain, 'variant':variant}
					self.vcxproj._add(properties, 'NMakeBuildCommandLine', 'cd $(SolutionDir) && %s waf %s' % (sys.executable, command))
				else:
					self.vcxproj._add(properties, 'NMakeBuildCommandLine', 'cd $(SolutionDir) && %s waf install:%s:%s --targets=%s' % (sys.executable, toolchain, variant, task_gen.target))
					self.vcxproj._add(properties, 'NMakeReBuildCommandLine', 'cd $(SolutionDir) && %s waf clean:%s:%s install:%s:%s --targets=%s' % (sys.executable, toolchain, variant, toolchain, variant, task_gen.target))
					self.vcxproj._add(properties, 'NMakeCleanCommandLine', 'cd $(SolutionDir) && %s waf clean:%s:%s --targets=%s' % (sys.executable, toolchain, variant, task_gen.target))
					if 'cxxprogram' in task_gen.features:
						self.vcxproj._add(properties, 'NMakeOutput', '%s' % os.path.join('$(OutDir)', env.PREFIX, variant, env.DEPLOY_BINDIR, env.cxxprogram_PATTERN%task_gen.target))
					elif 'game' in task_gen.features:
						deps = task_gen.use[:]
						seen = set([])
						program = None
						while (deps):
							dep = deps.pop()
							if dep not in seen:
									seen.add(dep)
									try:
										task_dep = task_gen.bld.get_tgen_by_name(dep)
										deps += getattr(task_dep, 'use', [])
										if 'cxxprogram' in task_dep.features:
											program = task_dep
									except:
										pass
						if program:
							self.vcxproj._add(properties, 'NMakeOutput', os.path.join('$(OutDir)', env.PREFIX, variant, env.DEPLOY_BINDIR, env.cxxprogram_PATTERN%program.target))
							self.vcxproj._add(properties, 'LocalDebuggerCommand', '$(NMakeOutput)')
							self.vcxproj._add(properties, 'LocalDebuggerCommandArguments', task_gen.target)
						else:
							self.vcxproj._add(properties, 'NMakeOutput', '%s' % os.path.join('$(OutDir)', env.PREFIX, variant, env.DEPLOY_BINDIR, env.cxxprogram_PATTERN%task_gen.target))
					self.vcxproj._add(properties, 'NMakePreprocessorDefinitions', ';'.join(defines + env.DEFINES))
					includes += ['%s/usr/include'%sysroot for sysroot in env.SYSROOT]
					self.vcxproj._add(properties, 'NMakeIncludeSearchPath', ';'.join([path_from(i, task_gen.bld) for i in includes] + env.INCLUDES))
		files = self.vcxproj._add(project, 'ItemGroup')

		self.filters = {}
		for node in getattr(task_gen, 'source_nodes', []):
			self.add_node(task_gen.bld.srcnode, node, node, files)
		self.vcxproj._add(project, 'Import', {'Project': '$(VCTargetsPath)\\Microsoft.Cpp.targets'})

	def write(self, nodes):
		self.vcxproj.write(nodes[0])
		self.vcxfilters.write(nodes[1])

	def add_node(self, root_node, project_node, node, files):
		path = node.abspath()
		if os.path.isdir(path):
			if project_node != node:
				path = node.path_from(root_node)
				try:
					filter = self.filters[path]
				except KeyError:
					filter = generateGUID(path)
					n = self.vcxfilters._add(self.filter_nodes, 'Filter', {'Include':node.path_from(project_node)})
					self.vcxfilters._add(n, 'UniqueIdentifier', filter)
			for subdir in node.listdir():
				self.add_node(root_node, project_node, node.make_node(subdir), files)
		elif os.path.isfile(path):
			self.vcxproj._add(files, 'None', {'Include':  '$(SolutionDir)%s' % node.path_from(root_node)})
			n = self.vcxfilters._add(self.file_nodes, 'None', {'Include':  '$(SolutionDir)%s' % node.path_from(root_node)})
			self.vcxfilters._add(n, 'Filter', node.parent.path_from(project_node))


class vs2003(Build.BuildContext):
	cmd = 'vs2003'
	fun = 'build'
	optim = 'debug'
	version = (('Visual Studio .NET 2003', '8.00', False), (VCproj, '7.10'))
	platforms = ['Win32', 'x64']

	def get_platform(self, platform_name):
		return platform_name if platform_name in self.__class__.platforms else self.__class__.platforms[0]

	def execute(self):
		"""
		Entry point
		"""
		self.restore()
		if not self.all_envs:
			self.load_envs()
		self.env.PROJECTS=[self.__class__.cmd]

		self.env.VARIANT = '$(Variant)'
		self.env.TOOLCHAIN = '$(Toolchain)'
		self.env.PREFIX = '$(Prefix)'
		self.env.DEPLOY_ROOTDIR = '$(Deploy_RootDir)'
		self.env.DEPLOY_BINDIR = '$(Deploy_BinDir)'
		self.env.DEPLOY_RUNBINDIR = '$(Deploy_RunBinDir)'
		self.env.DEPLOY_LIBDIR = '$(Deploy_LibDir)'
		self.env.DEPLOY_INCLUDEDIR = '$(Deploy_IncludeDir)'
		self.env.DEPLOY_DATADIR = '$(Deploy_DataDir)'
		self.env.DEPLOY_PLUGINDIR = '$(Deploy_PluginDir)'
		self.env.DEPLOY_KERNELDIR = '$(Deploy_KernelDir)'
		self.features = ['GUI']

		self.recurse([self.run_dir])

		version = self.__class__.cmd
		version_name, version_number, folders = self.__class__.version[0]
		klass, version_project = self.__class__.version[1]

		appname = getattr(Context.g_module, Context.APPNAME, self.srcnode.name)

		solution_node = self.srcnode.make_node(appname+'.'+version+'.sln')
		projects = self.srcnode.make_node('.build').make_node(version)
		projects.mkdir()


		solution = Solution(self, appname, version_number, version_name, folders)

		for target, command, do_build in [('build.reconfigure', 'reconfigure', False), ('build.%s'%version, version, False), ('build.all', 'install:%(toolchain)s:%(variant)s', True)]:
			task_gen = lambda: None
			task_gen.target = target
			task_gen.command = command
			task_gen.bld = self
			task_gen.all_sources = []
			task_gen.features = []
			nodes = [projects.make_node("%s.%s" % (target, ext)) for ext in klass.extensions]
			project = klass(task_gen, version, version_project, folders)
			project.write(nodes)
			solution.add(task_gen, project, nodes[0].path_from(self.srcnode), do_build)

		for g in self.groups:
			for tg in g:
				if not isinstance(tg, TaskGen.task_gen):
					continue
				if not 'kernel' in tg.features:
					tg.post()

					nodes = [projects.make_node("%s.%s" % (tg.target, ext)) for ext in klass.extensions]
					project = klass(tg, version, version_project, folders)
					project.write(nodes)
					solution.add(tg, project, nodes[0].path_from(self.srcnode))

		solution.write(solution_node)

class vs2005(vs2003):
	cmd = 'vs2005'
	fun = 'build'
	version =	(('Visual Studio 2005', '9.00', True),(VCproj, '8.00'))
	platforms = ['Win32', 'x64']

class vs2005e(vs2003):
	cmd = 'vs2005e'
	fun = 'build'
	version =	(('Visual C++ Express 2005', '9.00', False),(VCproj, '8.00'))
	platforms = ['Win32', 'x64']


class vs2008(vs2003):
	cmd = 'vs2008'
	fun = 'build'
	version =	(('Visual Studio 2008', '10.00', True),(VCproj, '9.00'))
	platforms = ['Win32', 'x64']

class vs2008e(vs2003):
	cmd = 'vs2008e'
	fun = 'build'
	version =	(('Visual C++ Express 2008', '10.00', False),(VCproj, '9.00'))
	platforms = ['Win32', 'x64']

class vs2010(vs2003):
	cmd = 'vs2010'
	fun = 'build'
	version =	(('Visual Studio 2010', '11.00', True),(VCxproj, ('4.0','10.0')))
	platforms = ['Win32', 'x64']

class vs2010e(vs2003):
	cmd = 'vs2010e'
	fun = 'build'
	version =	(('Visual C++ Express 2010', '11.00', False),(VCxproj, ('4.0','10.0')))
	platforms = ['Win32', 'x64']

class vs11(vs2003):
	cmd = 'vs11'
	fun = 'build'
	version =	(('Visual Studio 11', '12.00', True),(VCxproj, ('4.5','11.0')))
	platforms = ['Win32', 'x64']

class vs2012(vs11):
	cmd = 'vs2012'
	fun = 'build'
	platforms = ['Win32', 'x64']

class vs11e(vs2003):
	cmd = 'vs11e'
	fun = 'build'
	version =	(('Visual C++ Express 11', '12.00', False),(VCxproj, ('4.5','11.0')))
	platforms = ['Win32', 'x64']

class vs2012e(vs11e):
	cmd = 'vs2012e'
	fun = 'build'
	platforms = ['Win32', 'x64']

class vs2013e(vs2003):
	cmd = 'vs2013e'
	fun = 'build'
	version =	(('Visual C++ Express 12', '13.00', False),(VCxproj, ('12.0','12.0')))
	platforms = ['Win32', 'x64']

class vs2013(vs2003):
	cmd = 'vs2013'
	fun = 'build'
	version =	(('Visual Studio 12', '13.00', True),(VCxproj, ('12.0','12.0')))
	platforms = ['Win32', 'x64']

