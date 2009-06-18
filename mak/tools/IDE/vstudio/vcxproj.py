import os
import solution
import mak.sources

class VCxproj:
	extension = '.vcxproj'

	def __init__(self, filename, name, category, versionName, versionNumber, type):
		self.versionName = versionName
		self.versionNumber = versionNumber
		self.name = name
		self.category = category
		self.output = open(filename, 'w')
		self.filters = open(filename+'.filters', 'w')
		self.targetName = os.path.join('.projects', versionName, category+'.'+name+'.'+versionName+self.extension)
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

	def writeHeader(self, configs, platforms, options):
		self.filters.write('<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">\n')
		self.output.write('<Project DefaultTargets="Build" ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">\n')
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
		for (platform, pname, options) in options:
			includedirs = ';'.join([os.path.join('$(SolutionDir)', i) for i in options.includedir])
			libdirs = ';'.join([os.path.join('$(SolutionDir)', i) for i in options.libdir])
			self.output.write('  <ItemDefinitionGroup Condition="\'$(Platform)\'==\'%s\'">\n' % (pname))
			self.output.write('    <ClCompile>\n')
			self.output.write('      <PreprocessorDefinitions>%s</PreprocessorDefinitions>\n' % ';'.join(options.defines))
			self.output.write('      <AdditionalIncludeDirectories>%s</AdditionalIncludeDirectories>\n' % includedirs)
			self.output.write('    </ClCompile>\n')
			self.output.write('    <Link>\n')
			self.output.write('      <AdditionalDependencies>%s</AdditionalDependencies>\n' % libdirs)
			self.output.write('      <AdditionalLibraryDirectories>%s</AdditionalLibraryDirectories>\n' % ';'.join(options.libdir))
			self.output.write('    </Link>\n')
			self.output.write('  </ItemDefinitionGroup>\n')

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
		for subname,subdir in directory.directories.iteritems():
			self.addFilter(os.path.join(name,subname), subdir)

	def addCppFile(self, path, filename, source):
		self.output.write('    <ClCompile Include="%s" />\n' % filename)
		self.filters.write('    <ClCompile Include="%s">\n' % filename)
		self.filters.write('      <Filter>%s</Filter>\n' % path)
		self.filters.write('    </ClCompile>\n')

	def addFiles(self, path, directory):
		for subname,subdir in directory.directories.iteritems():
			self.addFiles(os.path.join(path, subname), subdir)
		for source in directory.files:
			if not source.generated():
				filename = os.path.join('..', '..', 'src', self.category, self.name, path, source.filename)
			else:
				filename = os.path.join('$(IntDir)', 'src', self.category, self.name, path, source.filename)
			if isinstance(source, mak.sources.cppsource):
				self.addCppFile(path, filename, source)

	def addDirectory(self, sources):
		self.output.write('  <ItemGroup>\n')
		self.filters.write('  <ItemGroup>\n')
		for name,subdir in sources.directories.iteritems():
			self.addFilter(name, subdir)
		self.output.write('  </ItemGroup>\n')
		self.output.write('  <ItemGroup>\n')
		self.filters.write('  </ItemGroup>\n')
		self.filters.write('  <ItemGroup>\n')
		self.addFiles('', sources)
		self.output.write('  </ItemGroup>\n')
		self.filters.write('  </ItemGroup>\n')
