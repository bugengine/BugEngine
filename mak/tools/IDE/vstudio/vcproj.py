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


class VCproj:
	extensions = ['.vcproj']

	def __init__(self, filename, name, category, versionName, versionNumber, type, envs):
		self.versionName = versionName
		self.versionNumber = versionNumber
		self.name = name
		self.category = category
		self.output = open(filename, 'w')
		self.targetName = filename
		self.envs = envs

	def writeHeader(self, configs):
		self.output.write('<VisualStudioProject\n	ProjectType="Visual C++"\n	Version="%s"\n	Name="%s"\n	ProjectGUID="%s"\n	RootNamespace="%s"\n	Keyword="Win32Proj">\n' % (self.versionNumber, self.category+'.'+self.name, solution.generateGUID(self.targetName, self.name), self.name))
		self.output.write('	<Platforms>\n')
		self.output.write('		<Platform\n			Name="Win32"/>\n')
		self.output.write('	</Platforms>\n')
		self.output.write('	<Configurations>\n')
		for (config, options) in configs:
			env = self.envs[config]
			if options:
				includes = options.includedir.union(env.INCPATHS)
				defines = options.defines.union(env.DEFINES)
			else:
				includes = env.INCPATHS
				defines = env.DEFINES
			includes = [os.path.abspath(i) for i in includes]
			if self.category == 'game':
				target = ""
			else:
				target = "--targets=%s" % self.name
			self.output.write('		<Configuration\n')
			self.output.write('			Name="%s|Win32"\n' % (config))
			self.output.write('			OutputDirectory="$(SolutionDir)build\\$(ConfigurationName)\\"\n')
			self.output.write('			IntermediateDirectory="$(SolutionDir).build\\waf\\$(ConfigurationName)\\)\\"\n')
			self.output.write('			ConfigurationType="0"\n')
			self.output.write('			CharacterSet="2"\n')
			self.output.write('			>\n')
			self.output.write('			<Tool\n')
			self.output.write('				Name="VCNMakeTool"\n')
			self.output.write('				BuildCommandLine="cd $(SolutionDir) &amp;&amp; mak\\win32\\bin\\python.exe waf install_%s %s"\n' % (config, target))
			self.output.write('				ReBuildCommandLine="cd $(SolutionDir) &amp;&amp; mak\\win32\\bin\\python.exe waf clean_%s install_%s %s"\n' %(config, config, target))
			self.output.write('				CleanCommandLine="cd $(SolutionDir) &amp;&amp; mak\\win32\\bin\\python.exe waf clean_%s %s"\n' %(config, target))
			self.output.write('				Output="$(SolutionDir)%s\\%s\\%s"\n' % (env.PREFIX, env.DEPLOY['bin'], env.program_PATTERN%self.name))
			if float(self.versionNumber) >= 8:
				self.output.write('				PreprocessorDefinitions="%s"\n' % xmlify(";".join(defines)))
				self.output.write('				IncludeSearchPath="%s"\n' % xmlify(";".join(includes)))
				self.output.write('				ForcedIncludes=""\n')
				self.output.write('				AssemblySearchPath=""\n')
				self.output.write('				ForcedUsingAssemblies=""\n')
				self.output.write('				CompileAsManaged=""\n')
			self.output.write('			/>\n')
			self.output.write('		</Configuration>\n')
		self.output.write('	</Configurations>\n')
		self.output.write('	<References>\n')
		self.output.write('	</References>\n')

	def writeFooter(self):
		self.output.write('	<Globals>\n')
		self.output.write('	</Globals>\n')
		self.output.write('</VisualStudioProject>\n')

	def addFile(self, path, filename, source, tabs):
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


	def addDirectory(self, sources):
		self.output.write('	<Files>\n')
		self.addFiles(sources.prefix, sources, '		')
		self.output.write('	</Files>\n')
