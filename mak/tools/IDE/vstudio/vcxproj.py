import os
import solution
import mak.sources


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

	def __init__(self, filename, name, category, versionName, versionNumber, type, depends):
		self.versionName = versionName
		self.versionNumber = versionNumber
		self.name = name
		self.category = category
		self.output = open(filename, 'w')
		self.filters = open(filename+'.filters', 'w')
		self.targetName = os.path.join('.build', versionName, category+'.'+name+'.'+versionName+self.extensions[0])
		self.type = type
		self.depends = depends
		if type == 'game':
			self.projectType = 'Application'
		elif type == 'plugin':
			self.projectType = 'DynamicLibrary'
		elif type == 'static_library':
			self.projectType = 'StaticLibrary'
		elif type == 'shared_library':
			self.projectType = 'DynamicLibrary'
		elif type == 'library':
			self.projectType = 'StaticLibrary'
		elif type == 'util':
			self.projectType = 'Utility'
		elif type == 'tool':
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
				self.output.write('    <TargetName>%s</TargetName>\n' % self.name)
				self.output.write('  </PropertyGroup>\n')
		self.output.write('  <Import Project="$(VCTargetsPath)\\Microsoft.Cpp.props" />\n')
		for (platform, pname, opts) in options:
			self.pchstop = opts.pchstop
			self.pchname = opts.pchname
			includedirs = ';'.join([os.path.join('$(SolutionDir)', i) for i in opts.includedir])
			libdirs = ';'.join([os.path.join('$(SolutionDir)', i) for i in opts.libdir])
			libs = ';'.join([i+'.lib' for i in opts.libs])
			self.output.write('  <ItemDefinitionGroup Condition="\'$(Platform)\'==\'%s\'">\n' % (pname))
			self.output.write('    <ClCompile>\n')
			self.output.write('      <PrecompiledHeaderFile>%s</PrecompiledHeaderFile>\n' % opts.pchstop)
			self.output.write('      <PrecompiledHeader>Use</PrecompiledHeader>\n')
			self.output.write('      <PreprocessorDefinitions>%s;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\n' % ';'.join(opts.defines))
			self.output.write('      <AdditionalIncludeDirectories>%s;%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\n' % includedirs)
			if self.category == '3rdparty':
				self.output.write('      <WarningLevel>Level1</WarningLevel>\n')
			self.output.write('    </ClCompile>\n')
			self.output.write('    <ResourceCompile>\n')
			self.output.write('      <PreprocessorDefinitions>%s;%%(PreprocessorDefinitions)</PreprocessorDefinitions>\n' % ';'.join(opts.defines))
			self.output.write('      <AdditionalIncludeDirectories>%s;%%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\n' % includedirs)
			self.output.write('    </ResourceCompile>\n')
			self.output.write('    <Link>\n')
			self.output.write('      <AdditionalDependencies>%s;%%(AdditionalDependencies)</AdditionalDependencies>\n' % libs)
			self.output.write('      <AdditionalLibraryDirectories>%s;%%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>\n' % libdirs)
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

	def addCppFile(self, path, filter, filename, source):
		self.output.write('    <ClCompile Include="%s">\n' % filename)
		self.output.write('      <ObjectFileName>$(IntDir)%s\\</ObjectFileName>\n' % path)
		if ('win32' not in source.platforms) or not source.process:
			self.output.write('      <ExcludedFromBuild>true</ExcludedFromBuild>\n')
		else:
			if 'x86' not in source.archs:
				self.output.write('      <ExcludedFromBuild Condition="\'$(Platform)\'==\'Win32\'">true</ExcludedFromBuild>\n')
			if 'amd64' not in source.archs:
				self.output.write('      <ExcludedFromBuild Condition="\'$(Platform)\'==\'x64\'">true</ExcludedFromBuild>\n')
		if source.usepch:
			if os.path.join(path, source.filename) == self.pchname:
				self.output.write('      <PrecompiledHeader>Create</PrecompiledHeader>\n')
		else:
			self.output.write('      <PrecompiledHeader>NotUsing</PrecompiledHeader>\n')
		self.output.write('    </ClCompile>\n')
		self.filters.write('    <ClCompile Include="%s">\n' % filename)
		self.filters.write('      <Filter>%s</Filter>\n' % filter)
		self.filters.write('    </ClCompile>\n')

	def addRcFile(self, path, filter, filename, source):
		self.output.write('    <ResourceCompile Include="%s">\n' % filename)
		if 'win32' not in source.platforms or not source.process:
			self.output.write('      <ExcludedFromBuild>true</ExcludedFromBuild>\n')
		else:
			if 'x86' not in source.archs:
				self.output.write('      <ExcludedFromBuild Condition="\'$(Platform)\'==\'Win32\'">true</ExcludedFromBuild>\n')
			if 'amd64' not in source.archs:
				self.output.write('      <ExcludedFromBuild Condition="\'$(Platform)\'==\'x64\'">true</ExcludedFromBuild>\n')
		self.output.write('    </ResourceCompile>\n')
		self.filters.write('    <ResourceCompile Include="%s">\n' % filename)
		self.filters.write('      <Filter>%s</Filter>\n' % filter)
		self.filters.write('    </ResourceCompile>\n')

	def addHFile(self, path, filter, filename, source):
		self.output.write('    <ClInclude Include="%s" />\n' % filename)
		self.filters.write('    <ClInclude Include="%s">\n' % filename)
		self.filters.write('      <Filter>%s</Filter>\n' % filter)
		self.filters.write('    </ClInclude>\n')

	def addBisonFile(self, path, filter, filename, source):
		self.output.write('    <CustomBuild Include="%s">\n' % filename)
		if 'win32' not in source.platforms or not source.process:
			self.output.write('      <ExcludedFromBuild>true</ExcludedFromBuild>\n')
		else:
			if 'x86' not in source.archs:
				self.output.write('      <ExcludedFromBuild Condition="\'$(Platform)\'==\'Win32\'">true</ExcludedFromBuild>\n')
			if 'amd64' not in source.archs:
				self.output.write('      <ExcludedFromBuild Condition="\'$(Platform)\'==\'x64\'">true</ExcludedFromBuild>\n')
		self.output.write('      <Command>set PATH=&quot;$(SolutionDir)mak/win32/bin&quot;;%%PATH%% &amp;&amp; (if not exist &quot;%s&quot; mkdir &quot;%s&quot;) &amp;&amp; bison.exe -o&quot;%s&quot; -d --no-lines &quot;$(ProjectDir)%s&quot;</Command>\n' % (os.path.split('$(IntDir)'+source.generatedcpp)[0], os.path.split('$(IntDir)'+source.generatedcpp)[0], '$(IntDir)'+source.generatedcpp, filename))
		self.output.write('      <Outputs>%s;%s</Outputs>\n' % ('$(IntDir)'+source.generatedcpp, '$(IntDir)'+source.generatedh))
		self.output.write('    </CustomBuild>\n')
		self.filters.write('    <CustomBuild Include="%s">\n' % filename)
		self.filters.write('      <Filter>%s</Filter>\n' % filter)
		self.filters.write('    </CustomBuild>\n')

	def addDataFile(self, path, filter, filename, source):
		self.output.write('    <CustomBuild Include="%s">\n' % filename)
		if 'win32' not in source.platforms or not source.process:
			self.output.write('      <ExcludedFromBuild>true</ExcludedFromBuild>\n')
		else:
			if 'x86' not in source.archs:
				self.output.write('      <ExcludedFromBuild Condition="\'$(Platform)\'==\'Win32\'">true</ExcludedFromBuild>\n')
			if 'amd64' not in source.archs:
				self.output.write('      <ExcludedFromBuild Condition="\'$(Platform)\'==\'x64\'">true</ExcludedFromBuild>\n')
		self.output.write('      <Command>set PATH=&quot;$(SolutionDir)mak/win32/bin&quot;;%%PATH%% &amp;&amp; (if not exist &quot;%s&quot; mkdir &quot;%s&quot;) &amp;&amp; python.exe $(SolutionDir)mak/ddf.py -D $(SolutionDir)mak/macros_ignore -p %s -o &quot;%s&quot; &quot;$(ProjectDir)%s&quot;</Command>\n' % (os.path.split('$(IntDir)'+source.generatedcpp)[0], os.path.split('$(IntDir)'+source.generatedcpp)[0], self.pchstop, os.path.split('$(IntDir)'+source.generatedcpp)[0], filename))
		self.output.write('      <Outputs>%s</Outputs>\n' % ('$(IntDir)'+source.generatedcpp))
		self.output.write('    </CustomBuild>\n')
		self.filters.write('    <CustomBuild Include="%s">\n' % filename)
		self.filters.write('      <Filter>%s</Filter>\n' % filter)
		self.filters.write('    </CustomBuild>\n')


	def addFlexFile(self, path, filter, filename, source):
		self.output.write('    <CustomBuild Include="%s">\n' % filename)
		if 'win32' not in source.platforms or not source.process:
			self.output.write('      <ExcludedFromBuild>true</ExcludedFromBuild>\n')
		else:
			if 'x86' not in source.archs:
				self.output.write('      <ExcludedFromBuild Condition="\'$(Platform)\'==\'Win32\'">true</ExcludedFromBuild>\n')
			if 'amd64' not in source.archs:
				self.output.write('      <ExcludedFromBuild Condition="\'$(Platform)\'==\'x64\'">true</ExcludedFromBuild>\n')
		self.output.write('      <Command>set PATH=&quot;$(SolutionDir)mak/win32/bin&quot;;%%PATH%% &amp;&amp; (if not exist &quot;%s&quot; mkdir &quot;%s&quot;) &amp;&amp; flex.exe -o&quot;%s&quot; &quot;$(ProjectDir)%s&quot;</Command>\n' % (os.path.split('$(IntDir)'+source.generatedcpp)[0], os.path.split('$(IntDir)'+source.generatedcpp)[0], '$(IntDir)'+source.generatedcpp, filename))
		self.output.write('      <Outputs>%s</Outputs>\n' % ('$(IntDir)'+source.generatedcpp))
		self.output.write('    </CustomBuild>\n')
		self.filters.write('    <CustomBuild Include="%s">\n' % filename)
		self.filters.write('      <Filter>%s</Filter>\n' % filter)
		self.filters.write('    </CustomBuild>\n')

	def addDeployedFile(self, path, filter, filename, source):
		self.output.write('    <CustomBuild Include="%s">\n' % filename)
		if 'win32' not in source.platforms or not source.process:
			self.output.write('      <ExcludedFromBuild>true</ExcludedFromBuild>\n')
		else:
			if 'x86' not in source.archs:
				self.output.write('      <ExcludedFromBuild Condition="\'$(Platform)\'==\'Win32\'">true</ExcludedFromBuild>\n')
			if 'amd64' not in source.archs:
				self.output.write('      <ExcludedFromBuild Condition="\'$(Platform)\'==\'x64\'">true</ExcludedFromBuild>\n')
		outputpath = os.path.join('$(OutDir)'+getFileDeployPath(source.type), source.outdir)
		self.output.write('      <Command>(if not exist "%s" mkdir "%s") &amp;&amp; copy "%s" "%s" /y</Command>\n' % (outputpath, outputpath, filename, os.path.join(outputpath,source.filename)))
		self.output.write('      <Outputs>%s</Outputs>\n' % os.path.join(outputpath,source.filename))
		self.output.write('    </CustomBuild>\n')
		self.filters.write('    <CustomBuild Include="%s">\n' % filename)
		self.filters.write('      <Filter>%s</Filter>\n' % filter)
		self.filters.write('    </CustomBuild>\n')

	def addFiles(self, path, filter, directory, runtypes):
		for subname,subdir in directory.directories.iteritems():
			self.addFiles(os.path.join(path, subdir.prefix), os.path.join(filter, subname), subdir, runtypes)
		for source in directory.files:
			if not source.generated():
				filename = os.path.join('..', '..', path, source.filename)
			else:
				filename = os.path.join('$(IntDir)', path, source.filename)
			for type,function in runtypes:
				if isinstance(source, type):
					function(path, filter, filename, source)

	def addDirectory(self, sources):
		self.output.write('  <ItemGroup>\n')
		self.filters.write('  <ItemGroup>\n')
		for name,subdir in sources.directories.iteritems():
			self.addFilter(name, subdir)
		self.output.write('  </ItemGroup>\n')
		self.filters.write('  </ItemGroup>\n')

		self.output.write('  <ItemGroup>\n')
		self.filters.write('  <ItemGroup>\n')
		self.addFiles(sources.prefix, '', sources, [(mak.sources.lexsource, self.addFlexFile), (mak.sources.yaccsource, self.addBisonFile), (mak.sources.deployedsource, self.addDeployedFile)])
		self.output.write('  </ItemGroup>\n')
		self.filters.write('  </ItemGroup>\n')

		self.output.write('  <ItemGroup>\n')
		self.filters.write('  <ItemGroup>\n')
		self.addFiles(sources.prefix, '', sources, [(mak.sources.cppsource, self.addCppFile)])
		self.output.write('  </ItemGroup>\n')
		self.filters.write('  </ItemGroup>\n')

		self.output.write('  <ItemGroup>\n')
		self.filters.write('  <ItemGroup>\n')
		self.addFiles(sources.prefix, '', sources, [(mak.sources.rcsource, self.addRcFile)])
		self.output.write('  </ItemGroup>\n')
		self.filters.write('  </ItemGroup>\n')

		self.output.write('  <ItemGroup>\n')
		self.filters.write('  <ItemGroup>\n')
		self.addFiles(sources.prefix, '', sources, [(mak.sources.datasource, self.addDataFile)])
		self.output.write('  </ItemGroup>\n')
		self.filters.write('  </ItemGroup>\n')


		self.output.write('  <ItemGroup>\n')
		self.filters.write('  <ItemGroup>\n')
		self.addFiles(sources.prefix, '', sources, [(mak.sources.hsource, self.addHFile)])
		self.output.write('  </ItemGroup>\n')
		self.filters.write('  </ItemGroup>\n')

		self.output.write('  <ItemGroup>\n')
		self.filters.write('  <ItemGroup>\n')
		for d in self.depends:
			if 'win32-x86' in d.platforms or 'win32-amd64' in d.platforms:
				self.output.write('    <ProjectReference Include="%s">\n' % os.path.split(d.outname)[1])
				self.output.write('      <Project>%s</Project>\n' % solution.generateGUID(d.outname, d.name))
				self.output.write('      <Private>true</Private>\n')
				self.output.write('      <ReferenceOutputAssembly>true</ReferenceOutputAssembly>\n')
				self.output.write('      <CopyLocalSatelliteAssemblies>false</CopyLocalSatelliteAssemblies>\n')
				if self.type in ['game', 'tool', 'plugin', 'shared_library']:
					self.output.write('      <LinkLibraryDependencies>true</LinkLibraryDependencies>\n')
				else:
					self.output.write('      <LinkLibraryDependencies>false</LinkLibraryDependencies>\n')
				if self.type in ['game', 'tool']:
					self.output.write('      <UseLibraryDependencyInputs>true</UseLibraryDependencyInputs>\n')
				else:
					self.output.write('      <UseLibraryDependencyInputs>false</UseLibraryDependencyInputs>\n')
				self.output.write('    </ProjectReference>\n')
		self.output.write('  </ItemGroup>\n')
		self.filters.write('  </ItemGroup>\n')
