import os
import solution
import mak.sources
import string

def xmlify(s):
	s = string.replace(s, "&", "&amp;") # do this first
	s = string.replace(s, "'", "&apos;")
	s = string.replace(s, '"', "&quot;")
	return s

def getFileDeployPath(type):
	if type == 'data':
		return 'data'
	elif type == 'etc':
		return 'conf'
	elif type == 'plugin':
		return os.path.join('data','plugins')
	else:
		return ''

class VCproj:
	extensions = ['.vcproj']
	archs = { 'Win32':'x86', 'x64':'amd64' }

	def __init__(self, filename, name, category, versionName, versionNumber, type):
		self.versionName = versionName
		self.versionNumber = versionNumber
		self.name = name
		self.category = category
		self.output = open(filename, 'w')
		self.targetName = os.path.join('.build', versionName, category+'.'+name+'.'+versionName+self.extensions[0])
		self.type = type
		if type == 'game':
			self.projectType = '1'
		elif type == 'plugin':
			self.projectType = '2'
		elif type == 'static_library':
			self.projectType = '4'
		elif type == 'shared_library':
			self.projectType = '2'
		elif type == 'library':
			self.projectType = '4'
		elif type == 'util':
			self.projectType = '10'
		elif type == 'test':
			self.projectType = '1'
		else:
			print 'dunno project type : '+type

	def writeHeader(self, configs, platforms, options):
		self.configs = configs
		self.platforms = platforms
		self.output.write('<VisualStudioProject\n	ProjectType="Visual C++"\n	Version="%s"\n	Name="%s"\n	ProjectGUID="%s"\n	RootNamespace="%s"\n	Keyword="Win32Proj">\n' % (self.versionNumber, self.category+'.'+self.name, solution.generateGUID(self.targetName, self.name), self.name))
		self.output.write('	<Platforms>\n')
		for platform in platforms:
			self.output.write('		<Platform\n			Name="%s"/>\n' % platform)
		self.output.write('	</Platforms>\n')
		self.output.write('	<Configurations>\n')
		for platform in platforms:
			for (platform_, pname_, opts_) in options:
				if pname_ == platform:
					opts = opts_
			self.pchname = opts.pchname
			defines = ";".join([xmlify(d.replace('"','\\"')) for d in opts.defines])
			apidirs = ";".join([xmlify("\"..\\..\\"+d+"\"") for d in opts.includedir])
			libdirs = ";".join([xmlify("\"..\\..\\"+d+"\"") for d in opts.libdir])
			libs	= " ".join([xmlify("\""+str(d)+".lib\"") for d in opts.libs])
			for config in configs:
				self.output.write('		<Configuration\n')
				self.output.write('			Name="%s|%s"\n' % (config,platform))
				self.output.write('			OutputDirectory="$(SolutionDir)deploy\\vs2010\\$(PlatformName)\\$(ConfigurationName)\\"\n')
				self.output.write('			IntermediateDirectory="$(SolutionDir).build\\vs2010\\$(PlatformName)\\$(ConfigurationName)\\$(ProjectName)\\"\n')
				self.output.write('			ConfigurationType="%s"\n' % self.projectType)
				self.output.write('			InheritedPropertySheets="$(SolutionDir)mak\\msvc\\vs200x\\%s.vsprops;$(SolutionDir)mak\\msvc\\vs200x\\%s.vsprops;$(SolutionDir)mak\\msvc\\vs200x\\%s.vsprops"\n' % (config,platform,self.type))
				self.output.write('			CharacterSet="2"\n')
				self.output.write('			>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCPreBuildEventTool"\n')
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCCustomBuildTool"\n')
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCXMLDataGeneratorTool"\n')
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCWebServiceProxyGeneratorTool"\n')
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCMIDLTool"\n')
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCCLCompilerTool"\n')
				self.output.write('				PreprocessorDefinitions="%s"\n' % defines)
				self.output.write('				AdditionalIncludeDirectories="%s"\n' % apidirs)
				self.output.write('				UsePrecompiledHeader="0"\n')
				if self.category == '3rdparty':
					self.output.write('				WarningLevel="1"\n')
				self.output.write('				PrecompiledHeaderThrough="%s"\n' % opts.pchstop)
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCManagedResourceCompilerTool"\n')
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCResourceCompilerTool"\n')
				self.output.write('				AdditionalIncludeDirectories="%s"\n' % apidirs)
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCPreLinkEventTool"\n')
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCLinkerTool"\n')
				self.output.write('				AdditionalDependencies="%s"\n' % libs)
				self.output.write('				AdditionalLibraryDirectories="%s"\n' % libdirs)
				if self.type == 'plugin':
					self.output.write('				OutputFile="$(OutDir)data\\plugins\\%s.dll"\n' % self.name)
				elif self.type == 'dynamic_library':
					self.output.write('				OutputFile="$(OutDir)%s.dll"\n' % self.name)
				else:
					self.output.write('				OutputFile="$(OutDir)%s.exe"\n' % self.name)
				self.output.write('				ImportLibrary="$(OutDir)lib\\%s.lib"\n' % self.name)
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCLibrarianTool"\n')
				self.output.write('				OutputFile="$(OutDir)lib\\$(ProjectName).lib"\n')
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCALinkTool"\n')
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCManifestTool"\n')
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCXDCMakeTool"\n')
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCBscMakeTool"\n')
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCFxCopTool"\n')
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCAppVerifierTool"\n')
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCWebDeploymentTool"\n')
				self.output.write('			/>\n')
				self.output.write('			<Tool\n')
				self.output.write('				Name="VCPostBuildEventTool"\n')
				self.output.write('			/>\n')
				self.output.write('		</Configuration>\n')
		self.output.write('	</Configurations>\n')
		self.output.write('	<References>\n')
		self.output.write('	</References>\n')

	def writeFooter(self):
		self.output.write('	<Globals>\n')
		self.output.write('	</Globals>\n')
		self.output.write('</VisualStudioProject>\n')

	def addCppFile(self, path, filename, source, tabs):
		self.output.write(tabs+'<File RelativePath="%s">\n' % filename)
		for platform in self.platforms:
			for config in self.configs:
				self.output.write(tabs+'	<FileConfiguration\n')
				if 'win32' not in source.platforms or not source.process:
					self.output.write(tabs+'		ExcludedFromBuild="true"\n')
				else:
					if self.archs[platform] not in source.archs:
						self.output.write(tabs+'		ExcludedFromBuild="true"\n')
				self.output.write(tabs+'		Name="%s|%s">\n' % (config,platform))
				self.output.write(tabs+'	<Tool\n')
				self.output.write(tabs+'		Name="VCCLCompilerTool"\n')
				if self.pchname and source.usepch:
					if os.path.join('src', self.category, self.name, path, source.filename) == self.pchname:
						self.output.write(tabs+'		UsePrecompiledHeader="1"\n')
					else:
						self.output.write(tabs+'		UsePrecompiledHeader="2"\n')
				self.output.write(tabs+'		ObjectFile="$(IntDir)%s\\"\n' % path)
				self.output.write(tabs+'	/>\n')
				self.output.write(tabs+'	</FileConfiguration>\n')
		self.output.write(tabs+'</File>\n')

	def addRcFile(self, path, filename, source, tabs):
		self.output.write(tabs+'<File RelativePath="%s" />\n' % filename)

	def addHFile(self, path, filename, source, tabs):
		self.output.write(tabs+'<File RelativePath="%s" />\n' % filename)

	def addBisonFile(self, path, filename, source, tabs):
		self.output.write(tabs+'<File RelativePath="%s">\n' % filename)
		for platform in self.platforms:
			for config in self.configs:
				self.output.write(tabs+'	<FileConfiguration\n')
				if 'win32' not in source.platforms or not source.process:
					self.output.write(tabs+'		ExcludedFromBuild="true"\n')
				else:
					if self.archs[platform] not in source.archs:
						self.output.write(tabs+'		ExcludedFromBuild="true"\n')
				self.output.write(tabs+'		Name="%s|%s">\n' % (config,platform))
				self.output.write(tabs+'	<Tool\n')
				self.output.write(tabs+'		Name="VCCustomBuildTool"\n')
				self.output.write(tabs+'		Description="bison &quot;$(InputPath)&quot;"\n')
				self.output.write(tabs+'		CommandLine="set PATH=&quot;$(SolutionDir)../../bin&quot;;%%PATH%% &amp;&amp; (if not exist &quot;%s&quot; mkdir &quot;%s&quot;) &amp;&amp; bison.exe -o&quot;%s&quot; -d --no-lines &quot;%s&quot;"\n' % (os.path.split('$(IntDir)'+source.generatedcpp)[0], os.path.split('$(IntDir)'+source.generatedcpp)[0], '$(IntDir)'+source.generatedcpp, filename))
				self.output.write(tabs+'		Outputs="&quot;%s&quot;;&quot;%s&quot;"\n' % ('$(IntDir)'+source.generatedcpp, '$(IntDir)'+source.generatedh))
				self.output.write(tabs+'	/>\n')
				self.output.write(tabs+'	</FileConfiguration>\n')
		self.output.write(tabs+'</File>\n')


	def addFlexFile(self, path, filename, source, tabs):
		self.output.write(tabs+'<File RelativePath="%s">\n' % filename)
		for platform in self.platforms:
			for config in self.configs:
				self.output.write(tabs+'	<FileConfiguration\n')
				if 'win32' not in source.platforms or not source.process:
					self.output.write(tabs+'		ExcludedFromBuild="true"\n')
				else:
					if self.archs[platform] not in source.archs:
						self.output.write(tabs+'		ExcludedFromBuild="true"\n')
				self.output.write(tabs+'		Name="%s|%s">\n' % (config,platform))
				self.output.write(tabs+'	<Tool\n')
				self.output.write(tabs+'		Name="VCCustomBuildTool"\n')
				self.output.write(tabs+'		Description="flex &quot;$(InputPath)&quot;"\n')
				self.output.write(tabs+'		CommandLine="set PATH=&quot;$(SolutionDir)../../bin&quot;;%%PATH%% &amp;&amp; (if not exist &quot;%s&quot; mkdir &quot;%s&quot;) &amp;&amp; flex.exe -o&quot;%s&quot; &quot;%s&quot;"\n' % (os.path.split('$(IntDir)'+source.generatedcpp)[0], os.path.split('$(IntDir)'+source.generatedcpp)[0], '$(IntDir)'+source.generatedcpp, filename))
				self.output.write(tabs+'		Outputs="&quot;%s&quot;"\n' % ('$(IntDir)'+source.generatedcpp))
				self.output.write(tabs+'	/>\n')
				self.output.write(tabs+'	</FileConfiguration>\n')
		self.output.write(tabs+'</File>\n')


	def addDeployedFile(self, path, filename, source, tabs):
		self.output.write(tabs+'<File RelativePath="%s">\n' % filename)
		outputpath = os.path.join('$(OutDir)'+getFileDeployPath(source.type), source.outdir)
		for platform in self.platforms:
			for config in self.configs:
				self.output.write(tabs+'	<FileConfiguration\n')
				if 'win32' not in source.platforms or not source.process:
					self.output.write(tabs+'		ExcludedFromBuild="true"\n')
				else:
					if self.archs[platform] not in source.archs:
						self.output.write(tabs+'		ExcludedFromBuild="true"\n')
				self.output.write(tabs+'		Name="%s|%s">\n' % (config,platform))
				self.output.write(tabs+'	<Tool\n')
				self.output.write(tabs+'		Name="VCCustomBuildTool"\n')
				self.output.write(tabs+'		Description="Deploying &quot;$(InputPath)&quot; => &quot;%s\\$(InputFileName)&quot;..."\n' % outputpath)
				self.output.write(tabs+'		CommandLine="(if not exist &quot;%s&quot; mkdir &quot;%s&quot;) &amp;&amp; copy &quot;%s&quot; &quot;%s&quot; /y"\n' % (outputpath, outputpath, filename, os.path.join(outputpath,source.filename)))
				self.output.write(tabs+'		Outputs="&quot;%s&quot;"\n' % os.path.join(outputpath,source.filename))
				self.output.write(tabs+'	/>\n')
				self.output.write(tabs+'	</FileConfiguration>\n')
		self.output.write(tabs+'</File>\n')


	def addFiles(self, path, directory, tabs):
		for subname,subdir in directory.directories.iteritems():
			self.output.write(tabs+'<Filter Name="%s">\n' % subname)
			self.addFiles(os.path.join(path, subname), subdir, tabs+'	')
			self.output.write(tabs+'</Filter>\n')
		for source in directory.files:
			if not source.generated():
				filename = os.path.join('..', '..', 'src', self.category, self.name, path, source.filename)
			else:
				filename = os.path.join('$(IntDir)', 'src', self.category, self.name, path, source.filename)
			if isinstance(source, mak.sources.hsource):
				self.addHFile(path, filename, source, tabs)
			elif isinstance(source, mak.sources.cppsource):
				self.addCppFile(path, filename, source, tabs)
			elif isinstance(source, mak.sources.lexsource):
				self.addFlexFile(path, filename, source, tabs)
			elif isinstance(source, mak.sources.yaccsource):
				self.addBisonFile(path, filename, source, tabs)
			elif isinstance(source, mak.sources.deployedsource):
				self.addDeployedFile(path, filename, source, tabs)

	def addDirectory(self, sources):
		self.output.write('	<Files>\n')
		self.addFiles('', sources, '		')
		self.output.write('	</Files>\n')
