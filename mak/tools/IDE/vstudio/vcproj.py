import os
from mak.tools.IDE.vstudio import solution
import mak.sources
import string
from xml.dom import minidom

def xmlify(s):
	s = s.replace("&", "&amp;") # do this first
	s = s.replace("'", "&apos;")
	s = s.replace('"', "&quot;")
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

	def __init__(self, filename, name, category, versionName, versionNumber, type):
		self.versionName = versionName
		self.versionNumber = versionNumber
		self.name = name
		self.category = category
		self.output = open(filename, 'w')
		self.targetName = os.path.join('.build', versionName, category+'.'+name+'.'+versionName+self.extensions[0])
		self.projectType = '10'

	def writeHeader(self, configs):
		self.output.write('<VisualStudioProject\n	ProjectType="Visual C++"\n	Version="%s"\n	Name="%s"\n	ProjectGUID="%s"\n	RootNamespace="%s"\n	Keyword="Win32Proj">\n' % (self.versionNumber, self.category+'.'+self.name, solution.generateGUID(self.targetName, self.name), self.name))
		self.output.write('	<Platforms>\n')
		self.output.write('		<Platform\n			Name="Win32"/>\n')
		self.output.write('	</Platforms>\n')
		self.output.write('	<Configurations>\n')
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
				'VCResourceCompilerTool':
						{
							'PreprocessorDefinitions':defines,
							'AdditionalIncludeDirectories':apidirs,
						},
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
			self.output.write('		<Configuration\n')
			self.output.write('			Name="%s|Win32"\n' % (config))
			self.output.write('			OutputDirectory="$(SolutionDir)build\\$(ConfigurationName)\\"\n')
			self.output.write('			IntermediateDirectory="$(SolutionDir).build\\%s\\$(PlatformName)\\$(ConfigurationName)\\$(ProjectName)\\"\n' % self.versionName)
			self.output.write('			ConfigurationType="%s"\n' % self.projectType)
			self.output.write('			CharacterSet="2"\n')
			self.output.write('			>\n')
		self.output.write('	</Configurations>\n')
		self.output.write('	<References>\n')
		self.output.write('	</References>\n')

	def writeFooter(self):
		self.output.write('	<Globals>\n')
		self.output.write('	</Globals>\n')
		self.output.write('</VisualStudioProject>\n')

	def addDummyFile(self, path, filename, source, tabs):
		self.output.write(tabs+'<File RelativePath="..\\..\\%s" />\n'%filename)

	def addFiles(self, path, directory, tabs):
		for subname,subdir in directory.directories.items():
			self.output.write(tabs+'<Filter Name="%s">\n' % subname)
			self.addFiles(os.path.join(path, subdir.prefix), subdir, tabs+'	')
			self.output.write(tabs+'</Filter>\n')
		for source in directory.files:
			filename = os.path.join(path, source.filename)
			if not isinstance(source, mak.sources.generatedcppsource):
				self.addFile(path, filename, source, tabs)

	def addGeneratedFiles(self, path, directory, tabs):
		for subname,subdir in directory.directories.items():
			self.output.write(tabs+'<Filter Name="%s">\n' % subname)
			self.addGeneratedFiles(os.path.join(path, subdir.prefix), subdir, tabs+'	')
			self.output.write(tabs+'</Filter>\n')
		for source in directory.files:
			filename = os.path.join(path, source.filename)
			if isinstance(source, mak.sources.generatedcppsource):
				self.addGeneratedCppFile(path, filename, source, tabs, source.usepch)

	def addDirectory(self, sources):
		self.output.write('	<Files>\n')
		self.output.write('		<Filter Name="__build__">\n')
		self.addGeneratedFiles(sources.prefix, sources, '			')
		self.output.write('		</Filter>\n')
		self.addFiles(sources.prefix, sources, '		')
		self.output.write('	</Files>\n')
