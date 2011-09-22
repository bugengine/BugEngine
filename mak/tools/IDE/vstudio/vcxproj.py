import os
from mak.tools.IDE.vstudio import solution
import mak
from mak import sources


def getFileDeployPath(type):
	if type == 'data':
		return 'data'
	elif type == 'etc':
		return 'conf'
	elif type == 'plugin':
		return os.path.join('data','plugins')
	else:
		return ''

class VCxproj:
	extensions = ['.vcxproj', '.vcxproj.filters']

	def __init__(self, filename, name, category, versionName, versionNumber, type):
		self.versionName = versionName
		self.versionNumber = versionNumber
		self.name = name
		self.category = category
		self.output = open(filename, 'w')
		self.filters = open(filename+'.filters', 'w')
		self.targetName = os.path.join('.build', versionName, category+'.'+name+'.'+versionName+self.extensions[0])
		self.type = type
		if type == 'game':
			self.projectType = 'Makefile'
		else:
			self.projectType = 'Utility'

	def writeHeader(self, configs):
		self.filters.write('<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">\n')
		self.output.write('<Project DefaultTargets="Build" ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">\n')
		self.output.write('  <ItemGroup Label="ProjectConfigurations">\n')
		for config in configs:
			self.output.write('    <ProjectConfiguration Include="%s|Win32">\n' % (config))
			self.output.write('      <Configuration>%s</Configuration>\n' % config)
			self.output.write('      <Platform>Win32</Platform>\n')
			self.output.write('    </ProjectConfiguration>\n')
		self.output.write('  </ItemGroup>\n')
		self.output.write('  <PropertyGroup Label="Globals">\n')
		self.output.write('    <ProjectGUID>%s</ProjectGUID>\n' % solution.generateGUID(self.targetName, self.name))
		self.output.write('    <TargetFrameworkVersion>v%s</TargetFrameworkVersion>\n' % self.versionNumber)
		self.output.write('    <RootNamespace>%s</RootNamespace>\n' % self.name)
		self.output.write('  </PropertyGroup>\n')
		self.output.write('  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.Default.props" />\n')
		self.output.write('  <ImportGroup Label="PropertySheets">\n')
		self.output.write('  </ImportGroup>\n')
		self.output.write('  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.props" />\n')
		self.output.write('  <PropertyGroup Label="Configuration">\n')
		self.output.write('    <ConfigurationType>%s</ConfigurationType>\n' % self.projectType)
		self.output.write('    <TargetName>%s</TargetName>\n' % self.name)
		self.output.write('  </PropertyGroup>\n')
		for config in configs:
			self.output.write('  <PropertyGroup Condition="\'$(Configuration)\'==\'%s\'">\n' % (config))
			self.output.write('    <NMakeBuildCommandLine>cd $(SolutionDir) &amp;&amp; mak\\win32\\bin\\python.exe waf install_%s</NMakeBuildCommandLine>\n' % config)
			self.output.write('    <NMakeOutput></NMakeOutput>\n')
			self.output.write('    <NMakeCleanCommandLine>cd $(SolutionDir) &amp;&amp; mak\\win32\\bin\\python.exe waf clean_%s</NMakeCleanCommandLine>\n' % config)
			self.output.write('    <NMakeReBuildCommandLine>cd $(SolutionDir) &amp;&amp; mak\\win32\\bin\\python.exe waf clean_%s install_%s</NMakeReBuildCommandLine>\n' % (config, config))
			self.output.write('    <NMakePreprocessorDefinitions></NMakePreprocessorDefinitions>\n')
			self.output.write('    <NMakeIncludeSearchPath></NMakeIncludeSearchPath>\n')
			self.output.write('  </PropertyGroup>\n')

	def writeFooter(self):
		self.output.write('  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.targets" />\n')
		self.output.write('  <ImportGroup Label="ExtensionTargets">\n')
		self.output.write('  </ImportGroup>\n')
		self.output.write('</Project>\n')
		self.filters.write('</Project>\n')

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
			else:
				filename = os.path.join('$(IntDir)', path, source.filename)
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
