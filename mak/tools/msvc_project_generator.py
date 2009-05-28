import mak.sources
import os


import string
import hashlib

		 
def _hexdigest(s):
	"""Return a string as a string of hex characters.
	"""
	# NOTE:  This routine is a method in the Python 2.0 interface
	# of the native md5 module, but we want SCons to operate all
	# the way back to at least Python 1.5.2, which doesn't have it.
	h = string.hexdigits
	r = ''
	for c in s:
		i = ord(c)
		r = r + h[(i >> 4) & 0xF] + h[i & 0xF]
	return r

def xmlify(s):
	s = string.replace(s, "&", "&amp;") # do this first
	s = string.replace(s, "'", "&apos;")
	s = string.replace(s, '"', "&quot;")
	return s

def _generateGUID(slnfile, name):
	"""This generates a dummy GUID for the sln file to use.  It is
	based on the MD5 signatures of the sln filename plus the name of
	the project.  It basically just needs to be unique, and not
	change with each invocation."""
	solution = _hexdigest(hashlib.new(str(slnfile)+str(name)).digest()).upper()
	# convert most of the signature to GUID form (discard the rest)
	solution = "{" + solution[:8] + "-" + solution[8:12] + "-" + solution[12:16] + "-" + solution[16:20] + "-" + solution[20:32] + "}"
	return solution
	
	


class Solution:
	allversions = {
		'vs2005': ('9.00', 'Visual Studio 2005'),
		'vs2005e': ('9.00', 'Visual C++ Express 2005'),
		'vs2008': ('10.00', 'Visual Studio 2008'),
		'vs2008e': ('10.00', 'Visual C++ Express 2008'),
		'vs2010': ('11.00', 'Visual Studio 2010'),
	}
	
	def __init__(self,name,v,filename):
		self.name = name
		self.filename = filename
		self.guid = _generateGUID(self.filename,self.name)
		self.file = open(self.filename,'w')
		self.projectlist = []
		self.v = v
		

	def version(self):
		return self.allversions[self.v]
		
	def getDirectory(self):
		return self.v

	def writeHeader(self):
		self.file.write("""Microsoft Visual Studio Solution File, Format Version %s
# %s
""" % self.version())

	def addProject(self,project,done):
		if project in done: return
		done.add(project)
		filename,name=self.getProjectName(project)
		self.projectlist.append(_generateGUID(filename,name))
		self.file.write("Project(\"%s\") = \"%s\", \"%s\", \"%s\"\n" % (self.guid, name, filename, _generateGUID(filename,name)))
		if not project.depends == []:
			self.file.write('\tProjectSection(ProjectDependencies) = postProject\n')
			depends = project.depends[:]
			seen = set()
			while depends:
				dep = depends.pop()
				if dep not in seen:
					seen.add(dep)
					#depends += dep.depends
					if 'win32-x86' in dep.platforms or 'win32-amd64' in dep.platforms:
						filename,name = self.getProjectName(dep)
						self.file.write('\t\t%s = %s\n' % (_generateGUID(filename,name),_generateGUID(filename,name)))
			self.file.write('\tEndProjectSection\n')
		self.file.write('EndProject\n')
		for dep in project.depends:
			self.addProject(dep,done)


	def getProjectName(self,project):
		if project.type == 'c#lib':
			return (os.path.join('.projects',project.category+'.'+project.name+'.'+self.getDirectory()+'.csproj'),project.category+'.'+project.name)
		else:
			return (os.path.join('.projects',project.category+'.'+project.name+'.'+self.getDirectory()+'.vcproj'),project.category+'.'+project.name)
			
	def writeFooter(self,platforms,configs):
		self.file.write("""Global
\tGlobalSection(SolutionConfigurationPlatforms) = preSolution
""")
		for conf in configs:
			for platform in platforms:
				self.file.write("""\t\t%s|%s = %s|%s
""" % (conf,platform,conf,platform))
		self.file.write("""\tEndGlobalSection
\tGlobalSection(ProjectConfigurationPlatforms) = postSolution
""")
		for proj in self.projectlist:
			for conf in configs:
				for platform in platforms:
					self.file.write("""\t\t%(GUID)s.%(CONF)s|%(PLATFORM)s.ActiveCfg = %(CONF)s|%(PLATFORM)s
\t\t%(GUID)s.%(CONF)s|%(PLATFORM)s.Build.0 = %(CONF)s|%(PLATFORM)s
""" % {'GUID':proj,'CONF':conf,'PLATFORM':platform})
		self.file.write("""\tEndGlobalSection
EndGlobal
""")




		
def getFileDeployPath(type):
	if type == 'data':
		return 'data'
	elif type == 'etc':
		return 'conf'
	elif type == 'plugin':
		return os.path.join('data','plugins')
	else:
		return ''
		
class Project:
	allversions = {
		'vs2005': ('8.00', 'Visual Studio 2005'),
		'vs2005e': ('8.00', 'Visual C++ Express 2005'),
		'vs2008': ('9.00', 'Visual Studio 2008'),
		'vs2008e': ('9.00', 'Visual C++ Express 2008'),
	}
	
	def __init__(self, name, category, v, filename, dstfilename, platforms):
		self.name = name
		self.category = category
		self.filename = dstfilename
		self.guid = _generateGUID(self.filename,self.category+'.'+self.name)
		self.file = open(filename,'w')
		self.platforms = platforms
		self.v = v


	def version(self):
		version,name = self.allversions[self.v]
		return version

	def platformmap(self,plat):
		if plat == 'Win32':
			return 'x86'
		elif plat == 'x64':
			return 'amd64'
			
	def addConfigurations(self, confs, type):
		self.configs = confs
		if 'win32-x86' not in self.platforms and 'win32-amd64' not in self.platforms:
			type = 'util'
		self.file.write("""
	<Configurations>
""")
		for conf in confs:
			if type == 'shared_library':
				self.addDllConfig(conf)
			elif type == 'static_library':
				self.addLibConfig(conf)
			elif type == 'game':
				self.addExeConfig(conf)
			elif type == 'plugin':
				self.addPluginConfig(conf)
			elif type == 'util':
				self.addUtilityConfig(conf)
			elif type == 'library':
				self.addLibConfig(conf)
			else:
				print "don't know how to make a project configuration for %s-%s"%(type, conf)
		self.file.write("""
	</Configurations>
""")
		
	def addDirectory(self,directory):
		self.file.write("""	<References>
	</References>
	<Files>
""")
		self._addDirectory(directory,'')
		self.file.write("""	</Files>
""")



	fileconf = """				<FileConfiguration
						Name=\"%(CONFIG)s|%(PLATFORM)s\"
						>
"""
	fileconfexcluded = """				<FileConfiguration
						Name=\"%(CONFIG)s|%(PLATFORM)s\"
						ExcludedFromBuild=\"true\"
						>
"""
	endfileconf = """				</FileConfiguration>
"""
	toolconfpch = """					<Tool
							Name=\"VCCLCompilerTool\"
							UsePrecompiledHeader=\"%(PCHTYPE)s\"
							PrecompiledHeaderThrough=\"%(PCHSTOP)s\"
							ObjectFile=\"%(OutDir)s\\\"
						/>
"""
	toolconfdeploy = """					<Tool
							Name=\"VCCustomBuildTool\"
							Description=\"Deploying &quot;$(InputPath)&quot; => &quot;%(PATH)s\\$(InputFileName)&quot;...\"
							CommandLine=\"xcopy &quot;$(InputPath)&quot; &quot;%(PATH)s&quot; /q /y\"
							Outputs=\"&quot;%(PATH)s\\$(InputFileName)&quot;\"
						/>
"""
	lexconf = """					<Tool
							Name=\"VCCustomBuildTool\"
							Description=\"flex &quot;$(InputPath)&quot;...\"
							CommandLine=\"set PATH=%%PATH%%;&quot;$(SolutionDir)..\\..\\bin\\&quot;&#x0D;&#x0A;flex.exe -o&quot;%(FILECPP)s&quot; &quot;$(InputPath)&quot;\"
							Outputs=\"&quot;%(FILECPP)s&quot;\"
						/>
"""
	
	bisonconf = """					<Tool
							Name=\"VCCustomBuildTool\"
							Description=\"bison &quot;$(InputPath)&quot;...\"
							CommandLine=\"set PATH=%%PATH%%;&quot;$(SolutionDir)..\\..\\bin\\&quot;&#x0D;&#x0A;bison.exe -o&quot;%(FILECPP)s&quot; -d --no-lines &quot;$(InputPath)&quot; \"
							Outputs=\"&quot;%(FILECPP)s&quot;;&quot;%(FILEH)s&quot;\"
						/>
"""
	def _addDirectory(self,sourcelist,prefix):
		for repname,rep in sourcelist.directories.iteritems():
			self.file.write( """		<Filter Name=\"%s\">
""" % repname )
			self._addDirectory(rep, os.path.join(prefix,repname) )
			self.file.write( """		</Filter>
""" )
		for filename in sourcelist.files:
			if not filename.generated():
				self.file.write( """			<File RelativePath=\"..\\src\\%s\\%s\\%s\">
""" % (self.category,self.name,os.path.join(prefix,filename.filename)))
			else:
				self.file.write( """			<File RelativePath=\"$(IntDir)\\src\\%s\\%s\\%s\">
""" % (self.category,self.name,os.path.join(prefix,filename.filename)))
			if isinstance(filename,mak.sources.lexsource):
				for config in self.configs:
					for key,platform,opts in self.platforms:
						if ('win32' in filename.platforms) and (self.platformmap(platform) in filename.archs):
							self.file.write(Project.fileconf %{'CONFIG':config,'PLATFORM':platform})
							self.file.write(Project.lexconf % {'FILECPP':os.path.join('$(IntDir)',filename.generatedcpp)})
							self.file.write(Project.endfileconf)
						else:
							self.file.write(Project.fileconfexcluded % {'CONFIG':config,'PLATFORM':platform})
							self.file.write(Project.endfileconf)
			elif isinstance(filename,mak.sources.yaccsource):
				for config in self.configs:
					for key,platform,opts in self.platforms:
						if ('win32' in filename.platforms) and (self.platformmap(platform) in filename.archs):
							self.file.write(Project.fileconf %{'CONFIG':config,'PLATFORM':platform})
							self.file.write(Project.bisonconf % {'FILECPP':os.path.join('$(IntDir)',filename.generatedcpp), 'FILEH':os.path.join('$(IntDir)',filename.generatedh)})
							self.file.write(Project.endfileconf)
						else:
							self.file.write(Project.fileconfexcluded % {'CONFIG':config,'PLATFORM':platform})
							self.file.write(Project.endfileconf)
			elif isinstance(filename,mak.sources.cppsource):
				for config in self.configs:
					for key,platform,opts in self.platforms:
						if filename.process and ('win32' in filename.platforms) and (self.platformmap(platform) in filename.archs):
							self.file.write(Project.fileconf %{'CONFIG':config,'PLATFORM':platform})
							if filename.usepch and (opts.pchname != '') and (opts.pchname == os.path.join('src',self.category,self.name,prefix,filename.filename)):
								self.file.write(Project.toolconfpch % {'PCHTYPE':'1' ,'PCHSTOP':opts.pchstop, 'OutDir':os.path.join('$(IntDir)', prefix)})
							elif filename.usepch and opts.pchname != '':
								self.file.write(Project.toolconfpch % {'PCHTYPE':'2' ,'PCHSTOP':opts.pchstop, 'OutDir':os.path.join('$(IntDir)', prefix)})
							self.file.write(Project.endfileconf)
						else:
							self.file.write(Project.fileconfexcluded % {'CONFIG':config,'PLATFORM':platform})
							self.file.write(Project.endfileconf)
								
			elif isinstance(filename,mak.sources.deployedsource):
				for config in self.configs:
					for key,platform,opts in self.platforms:
						if ('win32' in filename.platforms) and (self.platformmap(platform) in filename.archs):
							self.file.write(Project.fileconf %{'CONFIG':config,'PLATFORM':platform})
							self.file.write(Project.toolconfdeploy % {'PATH':os.path.join('$(OutDir)',getFileDeployPath(filename.type), filename.outdir)})
							self.file.write(Project.endfileconf)
						else:
							self.file.write(Project.fileconfexcluded % {'CONFIG':config,'PLATFORM':platform})
							self.file.write(Project.endfileconf)
			self.file.write( """			</File>
""") 
		
	def writeHeader(self):
		self.file.write("""<?xml version="1.0" encoding="Windows-1252"?>
<VisualStudioProject
	ProjectType="Visual C++"
	Version="%(VERSION)s"
	Name="%(NAME)s"
	ProjectGUID="%(GUID)s"
	RootNamespace="%(NAME)s"
	Keyword="Win32Proj"
	TargetFrameworkVersion="196613"
	>
	<Platforms>
		<Platform
			Name="Win32"/>
		<Platform
			Name="x64"/>
	</Platforms>
	<ToolFiles>
	</ToolFiles>
""" % {'VERSION':self.version(),'NAME':self.category+'.'+self.name, 'GUID':self.guid})


	def writeFooter(self):
		self.file.write("""	<Globals>
	</Globals>
</VisualStudioProject>
""")


	def addDllConfig(self, optim):
		for platid,plat,opts in self.platforms:
			defines = ";".join([xmlify(d.replace('"','\\"')) for d in opts.defines])
			apidirs = ";".join([xmlify("\"..\\"+d+"\"") for d in opts.includedir])
			libdirs = ";".join([xmlify("\"..\\"+d+"\"") for d in opts.libdir])
			libs	= " ".join([xmlify("\""+str(d)+".lib\"") for d in opts.libs])
			deffile = opts.deffile
			if(deffile != ''):
				deffile = '$(SolutionDir)' + deffile
			self.file.write("""		<Configuration
			Name=\"%(CONFIG)s|%(PLATFORM)s\"
			OutputDirectory=\"$(SolutionDir)deploy\\%(VERSION)s\\$(PlatformName)\\$(ConfigurationName)\"
			IntermediateDirectory=\"$(SolutionDir).build\\%(VERSION)s\\$(PlatformName)\\$(ConfigurationName)\\$(ProjectName)\"
			ConfigurationType=\"2\"
			InheritedPropertySheets=\"$(SolutionDir)mak\\msvc\\dll.vsprops;$(SolutionDir)mak\\msvc\\%(OPTIM)s.vsprops;$(SolutionDir)mak\\msvc\\%(PLATFORM)s.vsprops\"
			CharacterSet=\"2\"
			>
			<Tool Name=\"VCPreBuildEventTool\" />
			<Tool Name=\"VCCustomBuildTool\" />
			<Tool Name=\"VCXMLDataGeneratorTool\" />
			<Tool Name=\"VCWebServiceProxyGeneratorTool\" />
			<Tool Name=\"VCMIDLTool\" />
			<Tool Name=\"VCCLCompilerTool\"
				AdditionalIncludeDirectories=\"%(APIDIRS)s\"
				PreprocessorDefinitions=\"%(DEFINES)s\" />
			<Tool Name=\"VCManagedResourceCompilerTool\" />
			<Tool Name=\"VCResourceCompilerTool\"
				AdditionalIncludeDirectories=\"%(APIDIRS)s\" />
			<Tool Name=\"VCPreLinkEventTool\" />
			<Tool Name=\"VCLinkerTool\"
				OutputFile=\"$(OutDir)\\%(NAME)s.dll\"
				AdditionalDependencies=\"%(LIBS)s\"
				ModuleDefinitionFile=\"%(DEFFILE)s\"
				AdditionalLibraryDirectories=\"%(LIBDIRS)s\" />
			<Tool Name=\"VCALinkTool\" />
			<Tool Name=\"VCManifestTool\" />
			<Tool Name=\"VCXDCMakeTool\" />
			<Tool Name=\"VCBscMakeTool\" />
			<Tool Name=\"VCFxCopTool\" />
			<Tool Name=\"VCAppVerifierTool\" />
			<Tool Name=\"VCWebDeploymentTool\" />
			<Tool Name=\"VCPostBuildEventTool\" />
		</Configuration>
""" % {'CONFIG':optim, 'OPTIM':optim, 'PLATFORM':plat, 'DEFINES':defines, 'APIDIRS':apidirs, 'LIBDIRS':libdirs, 'LIBS':libs, 'NAME':self.name, 'DEFFILE':deffile, 'VERSION':self.v})


	def addPluginConfig(self,optim):
		for platid,plat,opts in self.platforms:
			defines = ";".join([xmlify(d.replace('"','\\"')) for d in opts.defines])
			apidirs = ";".join([xmlify("\"..\\"+d+"\"") for d in opts.includedir])
			libdirs = ";".join([xmlify("\"..\\"+d+"\"") for d in opts.libdir])
			libs	= " ".join([xmlify("\""+str(d)+".lib\"") for d in opts.libs])
			self.file.write("""		<Configuration
			Name=\"%(CONFIG)s|%(PLATFORM)s\"
			OutputDirectory=\"$(SolutionDir)deploy\\%(VERSION)s\\$(PlatformName)\\$(ConfigurationName)\"
			IntermediateDirectory=\"$(SolutionDir).build\\%(VERSION)s\\$(PlatformName)\\$(ConfigurationName)\\$(ProjectName)\"
			ConfigurationType=\"2\"
			InheritedPropertySheets=\"$(SolutionDir)mak\\msvc\\plugin.vsprops;$(SolutionDir)mak\\msvc\\%(OPTIM)s.vsprops;$(SolutionDir)mak\\msvc\\%(PLATFORM)s.vsprops\"
			CharacterSet=\"2\"
			>
			<Tool Name=\"VCPreBuildEventTool\" />
			<Tool Name=\"VCCustomBuildTool\" />
			<Tool Name=\"VCXMLDataGeneratorTool\" />
			<Tool Name=\"VCWebServiceProxyGeneratorTool\" />
			<Tool Name=\"VCMIDLTool\" />
			<Tool Name=\"VCCLCompilerTool\"
				AdditionalIncludeDirectories=\"%(APIDIRS)s\"
				PreprocessorDefinitions=\"%(DEFINES)s\" />
			<Tool Name=\"VCManagedResourceCompilerTool\" />
			<Tool Name=\"VCResourceCompilerTool\"
				AdditionalIncludeDirectories=\"%(APIDIRS)s\" />
			<Tool Name=\"VCPreLinkEventTool\" />
			<Tool Name=\"VCLinkerTool\"
				OutputFile=\"$(OutDir)\\data\\plugins\\%(NAME)s.dll\"
				AdditionalDependencies=\"%(LIBS)s\"
				AdditionalLibraryDirectories=\"%(LIBDIRS)s\" />
			<Tool Name=\"VCALinkTool\" />
			<Tool Name=\"VCManifestTool\" />
			<Tool Name=\"VCXDCMakeTool\" />
			<Tool Name=\"VCBscMakeTool\" />
			<Tool Name=\"VCFxCopTool\" />
			<Tool Name=\"VCAppVerifierTool\" />
			<Tool Name=\"VCWebDeploymentTool\" />
			<Tool Name=\"VCPostBuildEventTool\" />
		</Configuration>
""" % {'CONFIG':optim, 'OPTIM':optim, 'PLATFORM':plat, 'DEFINES':defines, 'APIDIRS':apidirs, 'LIBDIRS':libdirs, 'LIBS':libs, 'NAME':self.name, 'VERSION':self.v})


	def addExeConfig(self,optim):
		for platid,plat,opts in self.platforms:
			defines = ";".join([xmlify(d.replace('"','\\"')) for d in opts.defines])
			apidirs = ";".join([xmlify("\"..\\"+d+"\"") for d in opts.includedir])
			libdirs = ";".join([xmlify("\"..\\"+d+"\"") for d in opts.libdir])
			libs	= " ".join([xmlify("\""+str(d)+".lib\"") for d in opts.libs])
			self.file.write("""		<Configuration
			Name=\"%(CONFIG)s|%(PLATFORM)s\"
			OutputDirectory=\"$(SolutionDir)deploy\\%(VERSION)s\\$(PlatformName)\\$(ConfigurationName)\"
			IntermediateDirectory=\"$(SolutionDir).build\\%(VERSION)s\\$(PlatformName)\\$(ConfigurationName)\\$(ProjectName)\"
			ConfigurationType=\"1\"
			InheritedPropertySheets=\"$(SolutionDir)mak\\msvc\\exe.vsprops;$(SolutionDir)mak\\msvc\\%(OPTIM)s.vsprops;$(SolutionDir)mak\\msvc\\%(PLATFORM)s.vsprops\"
			CharacterSet=\"2\"
			>
			<Tool Name=\"VCPreBuildEventTool\" />
			<Tool Name=\"VCCustomBuildTool\" />
			<Tool Name=\"VCXMLDataGeneratorTool\" />
			<Tool Name=\"VCWebServiceProxyGeneratorTool\" />
			<Tool Name=\"VCMIDLTool\" />
			<Tool Name=\"VCCLCompilerTool\"
				AdditionalIncludeDirectories=\"%(APIDIRS)s\"
				PreprocessorDefinitions=\"%(DEFINES)s\" />
			<Tool Name=\"VCManagedResourceCompilerTool\" />
			<Tool Name=\"VCResourceCompilerTool\"
				AdditionalIncludeDirectories=\"%(APIDIRS)s\" />
			<Tool Name=\"VCPreLinkEventTool\" />
			<Tool Name=\"VCLinkerTool\"
				OutputFile=\"$(OutDir)\\%(NAME)s.exe\"
				AdditionalDependencies=\"%(LIBS)s\"
				AdditionalLibraryDirectories=\"%(LIBDIRS)s\" />
			<Tool Name=\"VCALinkTool\" />
			<Tool Name=\"VCManifestTool\" />
			<Tool Name=\"VCXDCMakeTool\" />
			<Tool Name=\"VCBscMakeTool\" />
			<Tool Name=\"VCFxCopTool\" />
			<Tool Name=\"VCAppVerifierTool\" />
			<Tool Name=\"VCWebDeploymentTool\" />
			<Tool Name=\"VCPostBuildEventTool\" />
		</Configuration>
""" % {'CONFIG':optim, 'OPTIM':optim, 'PLATFORM':plat, 'DEFINES':defines, 'APIDIRS':apidirs, 'LIBDIRS':libdirs, 'LIBS':libs, 'NAME':self.name, 'VERSION':self.v})

	def addUtilityConfig(self,optim):
		for platid,plat,opts in self.platforms:
			self.file.write("""		<Configuration
			Name=\"%(CONFIG)s|%(PLATFORM)s\"
			OutputDirectory=\"$(SolutionDir)deploy\\%(VERSION)s\\$(PlatformName)\\$(ConfigurationName)\"
			IntermediateDirectory=\"$(SolutionDir).build\\%(VERSION)s\\$(PlatformName)\\$(ConfigurationName)\\$(ProjectName)\"
			InheritedPropertySheets=\"$(SolutionDir)mak\\msvc\\%(PLATFORM)s.vsprops\"
			ConfigurationType=\"10\"
			>
			<Tool Name=\"VCPreBuildEventTool\"/>
			<Tool Name=\"VCCustomBuildTool\"/>
			<Tool Name=\"VCMIDLTool\"/>
			<Tool Name=\"VCPostBuildEventTool\"/>
		</Configuration>
""" % {'CONFIG':optim, 'PLATFORM':plat, 'VERSION':self.v})



	def addLibConfig(self,optim):
		for platid,plat,opts in self.platforms:
			defines = ";".join([xmlify(d.replace('"','\\"'))for d in opts.defines])
			apidirs = ";".join([xmlify("\"..\\"+d+"\"") for d in opts.includedir])
			libdirs = ";".join([xmlify("\"..\\"+d+"\"") for d in opts.libdir])
			libs	= "" #" ".join([xmlify("\""+str(d)+".lib\"") for d in opts.libs])
			self.file.write("""		<Configuration
			Name=\"%(CONFIG)s|%(PLATFORM)s\"
			OutputDirectory=\"$(SolutionDir)deploy\\%(VERSION)s\\$(PlatformName)\\$(ConfigurationName)\"
			IntermediateDirectory=\"$(SolutionDir).build\\%(VERSION)s\\$(PlatformName)\\$(ConfigurationName)\\$(ProjectName)\"
			ConfigurationType=\"4\"
			InheritedPropertySheets=\"$(SolutionDir)mak\\msvc\\lib.vsprops;$(SolutionDir)mak\\msvc\\%(OPTIM)s.vsprops;$(SolutionDir)mak\\msvc\\%(PLATFORM)s.vsprops\"
			CharacterSet=\"2\"
			>
			<Tool Name=\"VCPreBuildEventTool\"/>
			<Tool Name=\"VCCustomBuildTool\"/>
			<Tool Name=\"VCXMLDataGeneratorTool\"/>
			<Tool Name=\"VCWebServiceProxyGeneratorTool\"/>
			<Tool Name=\"VCMIDLTool\"/>
			<Tool Name=\"VCCLCompilerTool\"
				AdditionalIncludeDirectories=\"%(APIDIRS)s\"
				PreprocessorDefinitions=\"%(DEFINES)s\" />
			<Tool Name=\"VCManagedResourceCompilerTool\"/>
			<Tool Name=\"VCResourceCompilerTool\"
				AdditionalIncludeDirectories=\"%(APIDIRS)s\" />
			<Tool Name=\"VCPreLinkEventTool\"/>
			<Tool Name=\"VCLibrarianTool\"
				OutputFile=\"$(OutDir)\\lib\\%(NAME)s.lib\"
				AdditionalDependencies=\"%(LIBS)s\"
				AdditionalLibraryDirectories=\"%(LIBDIRS)s\" />
			<Tool Name=\"VCALinkTool\"/>
			<Tool Name=\"VCXDCMakeTool\"/>
			<Tool Name=\"VCBscMakeTool\"/>
			<Tool Name=\"VCFxCopTool\"/>
			<Tool Name=\"VCPostBuildEventTool\"/>
		</Configuration>
""" % {'CONFIG':optim, 'OPTIM':optim, 'PLATFORM':plat, 'DEFINES':defines, 'APIDIRS':apidirs, 'LIBDIRS':libdirs, 'LIBS':libs, 'NAME':self.name, 'VERSION':self.v})





class CsLibProject(Project):
	def __init__(self,name,category,filename,libs):
		Project.__init__(self,name,category,filename)
		self.CILlibs = libs
		
	def version(self):
		return '9.0.21022'
	def writeHeader(self):
		self.file.write("""<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="3.5" DefaultTargets="Build" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <PropertyGroup>
	<Configuration Condition=" '$(Configuration)' == '' ">debug</Configuration>
	<Platform Condition=" '$(Platform)' == '' ">Win32</Platform>
	<ProductVersion>%(VERSION)s</ProductVersion>
	<SchemaVersion>2.0</SchemaVersion>
	<ProjectGuid>%(GUID)s</ProjectGuid>
	<OutputType>Library</OutputType>
	<AppDesignerFolder>Properties</AppDesignerFolder>
	<RootNamespace>%(NAME)s</RootNamespace>
	<AssemblyName>%(NAME)s</AssemblyName>
	<TargetFrameworkVersion>v2.0</TargetFrameworkVersion>
	<FileAlignment>512</FileAlignment>
  </PropertyGroup>
  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'debug|Win32' ">
	<DebugSymbols>true</DebugSymbols>
	<DebugType>full</DebugType>
	<Optimize>false</Optimize>
	<OutputPath>$(SolutionDir)deploy\\$(Platform)\\$(Configuration)\\data\\plugins\\</OutputPath>
	<DefineConstants>DEBUG;TRACE</DefineConstants>
	<ErrorReport>prompt</ErrorReport>
	<WarningLevel>4</WarningLevel>
  </PropertyGroup>
  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'release|Win32' ">
	<DebugType>pdbonly</DebugType>
	<Optimize>true</Optimize>
	<OutputPath>$(SolutionDir)deploy\\$(Platform)\\$(Configuration)\\data\\plugins\\</OutputPath>
	<DefineConstants>TRACE</DefineConstants>
	<ErrorReport>prompt</ErrorReport>
	<WarningLevel>4</WarningLevel>
  </PropertyGroup>
  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'final|Win32' ">
	<DebugType>pdbonly</DebugType>
	<Optimize>true</Optimize>
	<OutputPath>$(SolutionDir)deploy\\$(Platform)\\$(Configuration)\\data\\plugins\\</OutputPath>
	<DefineConstants>TRACE</DefineConstants>
	<ErrorReport>prompt</ErrorReport>
	<WarningLevel>4</WarningLevel>
  </PropertyGroup>
  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'debug|x64' ">
	<DebugSymbols>true</DebugSymbols>
	<DebugType>full</DebugType>
	<Optimize>false</Optimize>
	<OutputPath>$(SolutionDir)deploy\\$(Platform)\\$(Configuration)\\data\\plugins\\</OutputPath>
	<DefineConstants>DEBUG;TRACE</DefineConstants>
	<ErrorReport>prompt</ErrorReport>
	<WarningLevel>4</WarningLevel>
  </PropertyGroup>
  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'release|x64' ">
	<DebugType>pdbonly</DebugType>
	<Optimize>true</Optimize>
	<OutputPath>$(SolutionDir)deploy\\$(Platform)\\$(Configuration)\\data\\plugins\\</OutputPath>
	<DefineConstants>TRACE</DefineConstants>
	<ErrorReport>prompt</ErrorReport>
	<WarningLevel>4</WarningLevel>
  </PropertyGroup>
  <PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'final|x64' ">
	<DebugType>pdbonly</DebugType>
	<Optimize>true</Optimize>
	<OutputPath>$(SolutionDir)deploy\\$(Platform)\\$(Configuration)\\data\\plugins\\</OutputPath>
	<DefineConstants>TRACE</DefineConstants>
	<ErrorReport>prompt</ErrorReport>
	<WarningLevel>4</WarningLevel>
  </PropertyGroup>
""" % {'VERSION':self.version(),'NAME':self.name, 'GUID':self.guid})

	def addConfigurations(self,confs, opts):
		self.file.write("""    <ItemGroup>
""")
		for lib in self.CILlibs:
			self.file.write("""    <Reference Include="%s" />
""" % lib)
		self.file.write("""    </ItemGroup>
""")

	def writeFooter(self):
		self.file.write("""  <Import Project="$(MSBuildBinPath)\Microsoft.CSharp.targets" />
  <!-- To modify your build process, add your task inside one of the targets below and uncomment it. 
	   Other similar extension points exist, see Microsoft.Common.targets.
  <Target Name="BeforeBuild">
  </Target>
  <Target Name="AfterBuild">
  </Target>
  -->
</Project>
""")

	csbuildfile = """	 <Compile Include="%(FileName)s">
			<Link>%(FileAlias)s</Link>
		</Compile>
	"""
	def addDirectory(self,directory,opts):
		self.file.write("""    <ItemGroup>
""")
		self._addDirectory(directory, os.path.join('$(SolutionDir)', 'src', self.category, self.name), '')
		self.file.write("""    </ItemGroup>
""")
		pass
		
	def _addDirectory(self,sources,prefix,linkprefix):
		for repname,rep in sources.directories.iteritems():
			self._addDirectory(rep, prefix, os.path.join(linkprefix,repname) )
		for filename in sources.files:
			self.file.write( self.csbuildfile % { 'FileName':os.path.join(prefix,linkprefix,filename.filename), 'FileAlias':os.path.join(linkprefix,filename.filename)} )
