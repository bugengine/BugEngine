import os
import solution

class VCxproj:
	extension = '.vcxproj'

	def __init__(self, filename, name, category, versionName, versionNumber, type):
		self.versionName = versionName
		self.versionNumber = versionNumber
		self.name = name
		self.output = open(filename, 'w')
		self.targetName = os.path.join('.projects', versionName, category+'.'+name+self.extension)
		self.type = type
		if type == 'game':
			self.projectType = 'Win32proj'
		elif type == 'plugin':
			self.projectType = 'DynamicLibrary'
		elif type == 'static_library':
			self.projectType = 'StaticLibrary'
		elif type == 'shared_library':
			self.projectType = 'DynamicLibrary'
		elif type == 'library':
			self.projectType = 'StaticLibrary'
		elif type == 'util':
			self.projectType = 'Application'
		elif type == 'test':
			self.projectType = 'Application'
		else:
			print 'dunno project type : '+type
		pass

	def writeHeader(self, configs, platforms):
		self.output.write('<Project DefaultTargets="Build" ToolsVersion="%s" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">\n' % self.versionNumber)
		self.output.write('  <ItemGroup Label="ProjectConfigurations">\n')
		for platform in platforms:
			for config in configs:
				self.output.write('    <ProjectConfiguration Include="%s|%s">\n' % (config, platform))
				self.output.write('      <Configuration>%s</Configuration>\n' % config)
				self.output.write('      <Platform>%s</Platform>\n' % platform)
				self.output.write('    </ProjectConfiguration>\n')
		self.output.write('  </ItemGroup>\n')
		self.output.write('  <PropertyGroup Label="Globals">\n')
		self.output.write('    <ProjectGUID>%s</ProjectGUID>\n' % solution.generateGUID(self.targetName, self.name))
		self.output.write('    <TargetFrameworkVersion>v%s</TargetFrameworkVersion>\n' % self.versionNumber)
		self.output.write('    <RootNamespace>%s</RootNamespace>\n' % self.name)
		self.output.write('  </PropertyGroup>\n')
		self.output.write('  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.Default.props" />\n')
		self.output.write('  <ImportGroup Label="PropertySheets">\n')
		self.output.write('    <Import Project="$(SolutionDir)\\mak\\msvc\\%s\\%s.props" />\n' % (self.versionName, self.type))
		self.output.write('  </ImportGroup>\n')
		for platform in platforms:
			self.output.write('  <ImportGroup Label="PropertySheets" Condition="\'$(Platform)\'==\'%s\'">\n' % platform)
			self.output.write('    <Import Project="$(SolutionDir)\\mak\\msvc\\%s\\%s.props" />\n' % (self.versionName, platform))
			self.output.write('  </ImportGroup>\n')
		for config in configs:
			self.output.write('  <ImportGroup Label="PropertySheets" Condition="\'$(Configuration)\'==\'%s\'">\n' % config)
			self.output.write('    <Import Project="$(SolutionDir)\\mak\\msvc\\%s\\%s.props" />\n' % (self.versionName, config))
			self.output.write('  </ImportGroup>\n')
		for platform in platforms:
			for config in configs:
				self.output.write('  <PropertyGroup Condition="\'$(Configuration)|$(Platform)\'==\'%s|%s\'" Label="Configuration">\n' % (config,platform))
				self.output.write('    <ConfigurationType>%s</ConfigurationType>\n' % self.projectType)
				self.output.write('  </PropertyGroup>\n')
		self.output.write('  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.props" />\n')

	def writeFooter(self):
		self.output.write('  <ItemGroup>\n  </ItemGroup>\n  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.targets" />\n  <ImportGroup Label="ExtensionTargets">\n  </ImportGroup>\n</Project>')