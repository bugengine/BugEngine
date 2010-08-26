import os
import solution
import mak.sources
import string
from xml.dom import minidom

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
	vcplatforms = { 'Win32':'win32', 'x64':'win32', 'Xbox 360':'xbox360' }
	archs = { 'Win32':'x86', 'x64':'amd64', 'Xbox 360':'ppc' }

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
		elif type == 'tool':
			self.projectType = '1'
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
				props =	{
					'VCPreBuildEventTool': { },
					'VCCustomBuildTool': { },
					'VCXMLDataGeneratorTool': { },
					'VCWebServiceProxyGeneratorTool': { },
					'VCCLCompilerTool':
							{
								'PreprocessorDefinitions':defines,
								'AdditionalIncludeDirectories':apidirs,
								'UsePrecompiledHeader':'0',
								'PrecompiledHeaderThrough':opts.pchstop,
							},
					'VCManagedResourceCompilerTool': { },
					'VCResourceCompilerTool': { 'AdditionalIncludeDirectories':apidirs, },
					'VCVCPrelinkEventTool': { },
					'VCLinkerTool':
							{
								'AdditionalDependencies':libs,
								'AdditionalLibraryDirectories':libdirs,
								'ImportLibrary':"$(OutDir)lib\\%s.lib"%self.name,
							},
					'VCLibrarianTool': { 'OutputFile':"$(OutDir)lib\\$(ProjectName).lib", },
					'VCALinkTool': { },
					'VCManifestTool': { },
					'VCXDCMakeTool': { },
					'VCBscMakeTool': { },
					'VCFxCopTool': { },
					'VCAppVerifierTool': { },
					'VCWebDeploymentTool': { },
					'VCPostBuildEventTool': { },
				}
				if self.type == 'plugin':
					props['VCLinkerTool']['OutputFile'] = "$(OutDir)data\\plugins\\%s.dll" % self.name
				elif self.type == 'shared_library':
					props['VCLinkerTool']['OutputFile'] = "$(OutDir)%s.dll" % self.name
				else:
					props['VCLinkerTool']['OutputFile'] = "$(OutDir)%s.exe" % self.name

				for sheet in set([config, platform, self.type, self.category]):
					document = minidom.parse("../../mak/msvc/vs200x/%s.vsprops"%sheet)
					for tool in document.getElementsByTagName("Tool"):
						name = tool.attributes['Name'].value
						if not props.has_key(name):
							props[name] = { }
						for attribute in tool.attributes.values():
							if attribute.name == 'Name': continue
							try: props[name][attribute.name] += ';'+attribute.value
							except KeyError: props[name][attribute.name] = attribute.value
					document.unlink()

				self.output.write('		<Configuration\n')
				self.output.write('			Name="%s|%s"\n' % (config,platform))
				self.output.write('			OutputDirectory="$(SolutionDir)deploy\\%s\\$(PlatformName)\\$(ConfigurationName)\\"\n' % self.versionName)
				self.output.write('			IntermediateDirectory="$(SolutionDir).build\\%s\\$(PlatformName)\\$(ConfigurationName)\\$(ProjectName)\\"\n' % self.versionName)
				self.output.write('			ConfigurationType="%s"\n' % self.projectType)
				self.output.write('			CharacterSet="2"\n')
				self.output.write('			>\n')
				for key,values in props.iteritems():
					self.output.write('			<Tool\n')
					self.output.write('				Name="%s"\n' % key)
					for attribute,value in values.iteritems():
						self.output.write('				%s="%s"\n' % (attribute,value))

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
				if VCproj.vcplatforms[platform] not in source.platforms or not source.process:
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
		self.output.write(tabs+'<File RelativePath="%s">\n' % filename)
		for platform in self.platforms:
			for config in self.configs:
				self.output.write(tabs+'	<FileConfiguration\n')
				if VCproj.vcplatforms[platform] not in source.platforms or not source.process:
					self.output.write(tabs+'		ExcludedFromBuild="true"\n')
				else:
					if self.archs[platform] not in source.archs:
						self.output.write(tabs+'		ExcludedFromBuild="true"\n')
				self.output.write(tabs+'		Name="%s|%s" />\n' % (config,platform))
		self.output.write(tabs+'</File>\n')


	def addHFile(self, path, filename, source, tabs):
		self.output.write(tabs+'<File RelativePath="%s" />\n' % filename)

	def addBisonFile(self, path, filename, source, tabs):
		self.output.write(tabs+'<File RelativePath="%s">\n' % filename)
		for platform in self.platforms:
			for config in self.configs:
				self.output.write(tabs+'	<FileConfiguration\n')
				if VCproj.vcplatforms[platform] not in source.platforms or not source.process:
					self.output.write(tabs+'		ExcludedFromBuild="true"\n')
				else:
					if self.archs[platform] not in source.archs:
						self.output.write(tabs+'		ExcludedFromBuild="true"\n')
				self.output.write(tabs+'		Name="%s|%s">\n' % (config,platform))
				self.output.write(tabs+'	<Tool\n')
				self.output.write(tabs+'		Name="VCCustomBuildTool"\n')
				self.output.write(tabs+'		Description="bison &quot;$(InputPath)&quot;"\n')
				self.output.write(tabs+'		CommandLine="set PATH=&quot;$(SolutionDir)mak/win32/bin&quot;;%%PATH%% &amp;&amp; (if not exist &quot;%s&quot; mkdir &quot;%s&quot;) &amp;&amp; bison.exe -o&quot;%s&quot; -d --no-lines &quot;$(ProjectDir)%s&quot;"\n' % (os.path.split('$(IntDir)'+source.generatedcpp)[0], os.path.split('$(IntDir)'+source.generatedcpp)[0], '$(IntDir)'+source.generatedcpp, filename))
				self.output.write(tabs+'		Outputs="&quot;%s&quot;;&quot;%s&quot;"\n' % ('$(IntDir)'+source.generatedcpp, '$(IntDir)'+source.generatedh))
				self.output.write(tabs+'	/>\n')
				self.output.write(tabs+'	</FileConfiguration>\n')
		self.output.write(tabs+'</File>\n')


	def addFlexFile(self, path, filename, source, tabs):
		self.output.write(tabs+'<File RelativePath="%s">\n' % filename)
		for platform in self.platforms:
			for config in self.configs:
				self.output.write(tabs+'	<FileConfiguration\n')
				if VCproj.vcplatforms[platform] not in source.platforms or not source.process:
					self.output.write(tabs+'		ExcludedFromBuild="true"\n')
				else:
					if self.archs[platform] not in source.archs:
						self.output.write(tabs+'		ExcludedFromBuild="true"\n')
				self.output.write(tabs+'		Name="%s|%s">\n' % (config,platform))
				self.output.write(tabs+'	<Tool\n')
				self.output.write(tabs+'		Name="VCCustomBuildTool"\n')
				self.output.write(tabs+'		Description="flex &quot;$(InputPath)&quot;"\n')
				self.output.write(tabs+'		CommandLine="set PATH=&quot;$(SolutionDir)mak/win32/bin&quot;;%%PATH%% &amp;&amp; (if not exist &quot;%s&quot; mkdir &quot;%s&quot;) &amp;&amp; flex.exe -o&quot;%s&quot; &quot;$(ProjectDir)%s&quot;"\n' % (os.path.split('$(IntDir)'+source.generatedcpp)[0], os.path.split('$(IntDir)'+source.generatedcpp)[0], '$(IntDir)'+source.generatedcpp, filename))
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
				if VCproj.vcplatforms[platform] not in source.platforms or not source.process:
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
			self.addFiles(os.path.join(path, subdir.prefix), subdir, tabs+'	')
			self.output.write(tabs+'</Filter>\n')
		for source in directory.files:
			if not source.generated():
				filename = os.path.join('..', '..', path, source.filename)
			else:
				filename = os.path.join('$(IntDir)', path, source.filename)
			if isinstance(source, mak.sources.hsource):
				self.addHFile(path, filename, source, tabs)
			elif isinstance(source, mak.sources.cppsource):
				self.addCppFile(path, filename, source, tabs)
			elif isinstance(source, mak.sources.rcsource):
				self.addRcFile(path, filename, source, tabs)
			elif isinstance(source, mak.sources.lexsource):
				self.addFlexFile(path, filename, source, tabs)
			elif isinstance(source, mak.sources.yaccsource):
				self.addBisonFile(path, filename, source, tabs)
			elif isinstance(source, mak.sources.deployedsource):
				self.addDeployedFile(path, filename, source, tabs)

	def addDirectory(self, sources):
		self.output.write('	<Files>\n')
		self.addFiles(sources.prefix, sources, '		')
		self.output.write('	</Files>\n')
