import os
from mak.tools.IDE.vstudio import solution
import mak
from mak import sources

try:
	import cStringIO as StringIO
except ImportError:
	try:
		import StringIO
	except ImportError:
		import io as StringIO


class VCxproj:
	extensions = ['.vcxproj', '.vcxproj.filters']

	def __init__(self,  filename, name, category, versionName, versionNumber, type, envs):
		self.versionName = versionName
		self.versionNumber = versionNumber
		self.name = name
		self.category = category
		self.output = StringIO.StringIO()
		self.filters = StringIO.StringIO()
		self.targetName = filename
		self.type = type
		self.envs = envs
		self.projectType = 'Makefile'

	def writeHeader(self, configs, engine):
		self.filters.write('<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">\n')
		self.output.write('<Project DefaultTargets="Build" ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">\n')
		self.output.write('  <ItemGroup Label="ProjectConfigurations">\n')
		for (config, options) in configs:
			platform,arch,compiler,version,flavor = config.split('-')
			self.output.write('    <ProjectConfiguration Include="%s|%s-%s-%s-%s">\n' % (flavor, platform, arch, compiler, version))
			self.output.write('      <Configuration>%s</Configuration>\n' % config)
			self.output.write('      <Platform>Win32</Platform>\n')
			self.output.write('    </ProjectConfiguration>\n')
		self.output.write('  </ItemGroup>\n')
		self.output.write('  <PropertyGroup Label="Globals">\n')
		self.output.write('    <ProjectGUID>%s</ProjectGUID>\n' % solution.generateGUID(self.targetName, self.name))
		self.output.write('    <TargetFrameworkVersion>v%s</TargetFrameworkVersion>\n' % self.versionNumber[0])
		self.output.write('    <RootNamespace>%s</RootNamespace>\n' % self.name)
		self.output.write('  </PropertyGroup>\n')

		self.output.write('  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.Default.props" />\n')
		self.output.write('  <ImportGroup Label="PropertySheets">\n')
		self.output.write('  </ImportGroup>\n')
		self.output.write('  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.props" />\n')
		self.output.write('  <PropertyGroup Label="Configuration">\n')
		self.output.write('    <ConfigurationType>%s</ConfigurationType>\n' % self.projectType)
		self.output.write('    <TargetName>%s</TargetName>\n' % self.name)
		self.output.write('    <OutDir>$(SolutionDir)build\\$(Configuration)\\</OutDir>\n')
		self.output.write('    <IntDir>$(SolutionDir)build\\$(Configuration)\\</IntDir>\n')
		self.output.write('    <PlatformToolset>v%d</PlatformToolset>\n' % (float(self.versionNumber[1])*10))
		self.output.write('  </PropertyGroup>\n')

		for (config, options) in configs:
			platform,arch,compiler,version,flavor = config.split('-')
			platformname = '%s-%s-%s-%s'%(platform, arch, compiler, version)
			self.output.write('    <PropertyGroup>\n')
			self.output.write('      <PlatformShortName>%s</PlatformShortName>\n' % platformname)
			self.output.write('      <PlatformArchitecture>%s</PlatformArchitecture>\n' % arch)
			self.output.write('      <PlatformTarget>%s</PlatformTarget>\n' % platformname)
			self.output.write('    </PropertyGroup>\n')

		for (config, options) in configs:
			env = self.envs[config]
			if options:
				includes = options.includedir.union(env.INCPATHS)
				defines = options.defines.union(env.DEFINES)
			else:
				includes = env.INCPATHS
				defines = env.DEFINES
			includes = [os.path.abspath(i) for i in includes]
			self.output.write('  <PropertyGroup Condition="\'$(Configuration)\'==\'%s\'">\n' % (config))
			if self.type == 'game':
				self.output.write('    <NMakeBuildCommandLine></NMakeBuildCommandLine>\n')
				self.output.write('    <NMakeOutput>$(SolutionDir)%s\\%s\\%s</NMakeOutput>\n' % (env.PREFIX, env.DEPLOY['bin'], env.program_PATTERN%engine))
				self.output.write('    <NMakeCleanCommandLine>cd $(SolutionDir) &amp;&amp; mak\\win32\\bin\\python.exe waf clean_%s</NMakeCleanCommandLine>\n' % config)
				self.output.write('    <NMakeReBuildCommandLine>cd $(SolutionDir) &amp;&amp; mak\\win32\\bin\\python.exe waf clean_%s install_%s</NMakeReBuildCommandLine>\n' % (config, config))
				self.output.write('    <LocalDebuggerCommand>$(NMakeOutput)</LocalDebuggerCommand>')
				self.output.write('    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>')
				self.output.write('    <LocalDebuggerCommandArguments>%s</LocalDebuggerCommandArguments>' % self.name)
			else:
				if self.type == 'waf':
					self.output.write('    <NMakeBuildCommandLine>cd $(SolutionDir) &amp;&amp; mak\\win32\\bin\\python.exe waf install_%s</NMakeBuildCommandLine>\n' % (config))
				else:
					self.output.write('    <NMakeBuildCommandLine>cd $(SolutionDir) &amp;&amp; mak\\win32\\bin\\python.exe waf install_%s --targets=%s</NMakeBuildCommandLine>\n' % (config, self.name))
				self.output.write('    <NMakeOutput></NMakeOutput>\n')
				self.output.write('    <NMakeCleanCommandLine>cd $(SolutionDir) &amp;&amp; mak\\win32\\bin\\python.exe waf clean_%s --targets=%s</NMakeCleanCommandLine>\n' % (config, self.name))
				self.output.write('    <NMakeReBuildCommandLine>cd $(SolutionDir) &amp;&amp; mak\\win32\\bin\\python.exe waf clean_%s install_%s --targets=%s</NMakeReBuildCommandLine>\n' % (config, config, self.name))
			self.output.write('    <NMakePreprocessorDefinitions>%s</NMakePreprocessorDefinitions>\n' % ';'.join(defines))
			self.output.write('    <NMakeIncludeSearchPath>%s</NMakeIncludeSearchPath>\n' % ';'.join(includes))
			self.output.write('  </PropertyGroup>\n')

	def writeFooter(self):
		self.output.write('  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.targets" />\n')
		self.output.write('  <ImportGroup Label="ExtensionTargets">\n')
		self.output.write('  </ImportGroup>\n')
		self.output.write('</Project>\n')
		self.filters.write('</Project>\n')

		try:
			with open(self.targetName,'r') as original:
				content = self.output.getvalue()
				if original.read() != content:
					print('writing %s...' % self.targetName)
					with open(self.targetName, 'w') as f:
						f.write(content)
		except IOError:
			content = self.output.getvalue()
			with open(self.targetName, 'w') as f:
				f.write(content)
		try:
			with open(self.targetName+'.filters','r') as original:
				content = self.filters.getvalue()
				if original.read() != content:
					print('writing %s...' % (self.targetName+'.filters'))
					with open(self.targetName+'.filters', 'w') as f:
						f.write(content)
		except IOError:
			content = self.filters.getvalue()
			with open(self.targetName+'.filters', 'w') as f:
				f.write(content)


	def addFilter(self, name, directory):
		self.filters.write('    <Filter Include="%s">\n' % name)
		self.filters.write('      <UniqueIdentifier>%s</UniqueIdentifier>\n' % solution.generateGUID(name,name))
		self.filters.write('    </Filter>\n')
		for subname,subdir in directory.directories.items():
			self.addFilter(os.path.join(name,subname), subdir)

	def addFile(self, path, filter, filename, source):
		self.output.write('    <None Include="%s" />\n' % filename)
		self.filters.write('    <None Include="%s">\n' % filename)
		self.filters.write('      <Filter>%s</Filter>\n' % filter)
		self.filters.write('    </None>\n')

	def addFiles(self, path, filter, directory):
		for subname,subdir in directory.directories.items():
			self.addFiles(os.path.join(path, subdir.prefix), os.path.join(filter, subname), subdir)
		for source in directory.files:
			if not source.generated():
				filename = os.path.join('..', '..', path, source.filename)
				self.addFile(path, filter, filename, source)

	def addDirectory(self, sourceDir):
		self.output.write('  <ItemGroup>\n')
		self.filters.write('  <ItemGroup>\n')
		for name,subdir in sourceDir.directories.items():
			self.addFilter(name, subdir)
		self.output.write('  </ItemGroup>\n')
		self.filters.write('  </ItemGroup>\n')

		self.output.write('  <ItemGroup>\n')
		self.filters.write('  <ItemGroup>\n')
		self.addFiles(sourceDir.prefix, '', sourceDir)
		self.output.write('  </ItemGroup>\n')
		self.filters.write('  </ItemGroup>\n')
