from waflib import Context, Build, TaskGen, Logs
import os
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
	return includes, defines

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
			Logs.pprint('NORMAL', 'writing %s' % node.name)
			node.write(newxml)

class Solution:
	def __init__(self, appname, version, version_number, version_name, use_folders):
		pass

	def add(self, task_gen, project):
		pass

	def write(self, node):
		pass

class VCproj:
	extensions = ['vcproj']
	def __init__(self, task_gen, version, version_project):
		self.vcproj = XmlFile()

	def write(self, node):
		self.vcproj.write(node)

class VCxproj:
	extensions = ['vcxproj', 'vcxproj.filters']
	def __init__(self, task_gen, version, version_project):
		self.vcxproj = XmlFile()
		self.vcxfilters = XmlFile()
		self.guid = generateGUID(task_gen.target)
		project = self.vcxproj._add(self.vcxproj.document, 'Project', {'DefaultTargets':'Build', 'ToolsVersion':'4.0', 'xmlns':'http://schemas.microsoft.com/developer/msbuild/2003'})
		configs = self.vcxproj._add(project, 'ItemGroup', {'Label': 'ProjectConfigurations'})
		for toolchain in task_gen.bld.env.ALL_TOOLCHAINS:
			for variant in task_gen.bld.env.ALL_VARIANTS:
				config = self.vcxproj._add(configs, 'ProjectConfiguration', {'Include': '%s|%s' % (variant, toolchain)})
				self.vcxproj._add(config, 'Configuration', '%s/%s' % (toolchain, variant))
				self.vcxproj._add(config, 'Platform', 'Win32')
		globals = self.vcxproj._add(project, 'PropertyGroup', {'Label': 'Globals'})
		self.vcxproj._add(globals, 'ProjectGUID', self.guid)
		self.vcxproj._add(globals, 'TargetFrameworkVersion', 'v'+version_project[0])
		self.vcxproj._add(globals, 'RootNamespace', task_gen.target)
		self.vcxproj._add(project, 'Import', {'Project': '$(VCTargetsPath)\\Microsoft.Cpp.Default.props'})
		self.vcxproj._add(project, 'Import', {'Project': '$(VCTargetsPath)\\Microsoft.Cpp.props'})
		configuration = self.vcxproj._add(project, 'PropertyGroup', {'Label': 'Configuration'})
		self.vcxproj._add(configuration, 'ConfigurationType', 'Makefile')
		self.vcxproj._add(configuration, 'OutDir', '$(SolutionDir)build\\$(Configuration)\\')
		self.vcxproj._add(configuration, 'IntDir', '$(SolutionDir)build\\$(Configuration)\\')
		self.vcxproj._add(configuration, 'PlatformToolset', 'v%d'% (float(version_project[1])*10))

		includes, defines = gather_includes_defines(task_gen)
		for toolchain in task_gen.bld.env.ALL_TOOLCHAINS:
			for variant in task_gen.bld.env.ALL_VARIANTS:
				env = task_gen.bld.all_envs['%s-%s'%(toolchain, variant)]
				properties = self.vcxproj._add(project, 'PropertyGroup', {'Condition': "'$(Configuration)'=='%s/%s'" % (toolchain, variant)})
				self.vcxproj._add(properties, 'NMakeBuildCommandLine', 'cd $(SolutionDir) && mak\\win32\\bin\\python.exe waf install:%s:%s --targets=%s' % (toolchain, variant, task_gen.target))
				self.vcxproj._add(properties, 'NMakeReBuildCommandLine', 'cd $(SolutionDir) && mak\\win32\\bin\\python.exe waf clean:%s:%s install:%s:%s --targets=%s' % (toolchain, variant, toolchain, variant, task_gen.target))
				self.vcxproj._add(properties, 'NMakeCleanCommandLine', 'cd $(SolutionDir) && mak\\win32\\bin\\python.exe waf clean:%s:%s --targets=%s' % (toolchain, variant, task_gen.target))
				if 'cxxprogram' in task_gen.features:
					self.vcxproj._add(properties, 'NMakeOutput', '%s' % os.path.join('$(OutDir)', env.DEPLOY_BINDIR, env.cxxprogram_PATTERN%task_gen.target))
				self.vcxproj._add(properties, 'NMakePreprocessorDefinitions', ';'.join(defines))
				self.vcxproj._add(properties, 'NMakeIncludeSearchPath', ';'.join([path_from(i, task_gen.bld) for i in includes]))
		files = self.vcxproj._add(project, 'ItemGroup')
		for file in getattr(task_gen, 'all_sources', []) or getattr(task_gen, 'source', []):
			self.vcxproj._add(files, 'None', {'Include':  '$(SolutionDir)%s' % file.path_from(task_gen.bld.srcnode)})
		self.vcxproj._add(project, 'Import', {'Project': '$(VCTargetsPath)\\Microsoft.Cpp.targets'})
		project = self.vcxfilters._add(self.vcxfilters.document, 'Project', {'DefaultTargets':'Build', 'ToolsVersion':version_project[0], 'xmlns':'http://schemas.microsoft.com/developer/msbuild/2003'})

	def write(self, node):
		self.vcxproj.write(node)
		self.vcxfilters.write(node.change_ext('.%s'%self.__class__.extensions[1]))



class vs2003(Build.BuildContext):
	cmd = 'vs2003'
	fun = 'build'
	version = (('Visual Studio .NET 2003', '8.00', False), (VCproj, '7.10'))

	def execute(self):
		"""
		Entry point
		"""
		self.restore()
		if not self.all_envs:
			self.load_envs()
		self.env.PROJECTS=[self.__class__.cmd]
		self.env.TOOLCHAIN = "$(Platform)"
		self.env.VARIANT = "$(Configuration)"
		self.recurse([self.run_dir])

		version = self.__class__.cmd
		version_name, version_number, folders = self.__class__.version[0]
		klass, version_project = self.__class__.version[1]

		appname = getattr(Context.g_module, Context.APPNAME, os.path.basename(self.srcnode.abspath()))

		solution_node = self.srcnode.make_node(appname+'.'+version+'.sln')
		projects = self.srcnode.make_node('.build').make_node(version)
		projects.mkdir()


		solution = Solution(appname, version, version_number, version_name, folders)

		for g in self.groups:
			for tg in g:
				if not isinstance(tg, TaskGen.task_gen):
					continue
				tg.post()

				node = projects.make_node("%s.%s" % (tg.target, klass.extensions[0]))
				project = klass(tg, version, version_project)
				project.write(node)
				solution.add(tg, project)

		solution.write(solution_node)

class vs2005(vs2003):
	cmd = 'vs2005'
	fun = 'build'
	version =	(('Visual Studio 2005', '9.00', True),(VCproj, '8.00'))

class vs2005e(vs2003):
	cmd = 'vs2005e'
	fun = 'build'
	version =	(('Visual C++ Express 2005', '9.00', False),(VCproj, '8.00'))


class vs2008(vs2003):
	cmd = 'vs2008'
	fun = 'build'
	version =	(('Visual Studio 2008', '10.00', True),(VCproj, '9.00'))

class vs2008e(vs2003):
	cmd = 'vs2008e'
	fun = 'build'
	version =	(('Visual C++ Express 2008', '10.00', False),(VCproj, '9.00'))

class vs2010(vs2003):
	cmd = 'vs2010'
	fun = 'build'
	version =	(('Visual Studio 2010', '11.00', True),(VCxproj, ('4.0','10.0')))

class vs2010e(vs2003):
	cmd = 'vs2010e'
	fun = 'build'
	version =	(('Visual C++ Express 2010', '11.00', False),(VCxproj, ('4.0','10.0')))

class vs11(vs2003):
	cmd = 'vs11'
	fun = 'build'
	version =	(('Visual Studio 11', '12.00', True),(VCxproj, ('4.5','11.0')))

class vs2012(vs11):
	cmd = 'vs2012'
	fun = 'build'

class vs11e(vs2003):
	cmd = 'vs11e'
	fun = 'build'
	version =	(('Visual C++ Express 11', '12.00', False),(VCxproj, ('4.5','11.0')))

class vs2012e(vs11e):
	cmd = 'vs2012e'
	fun = 'build'
