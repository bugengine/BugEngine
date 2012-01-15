from waflib import Context, Build, TaskGen
import os
import mak
from mak.tools.IDE.vstudio import solution,vcproj,vcxproj

class vs2003(Build.BuildContext):
	cmd = 'vs2003'
	fun = 'build'
	version = (('Visual Studio .NET 2003', '8.00', False), (vcproj.VCproj, '7.10'))

	def execute(self):
		"""
		Entry point
		"""
		self.restore()
		if not self.all_envs:
			self.load_envs()
		self.env.PROJECTS=[self.__class__.cmd]
		self.recurse([self.run_dir])

		version = self.__class__.cmd
		version_name, version_number, folders = self.__class__.version[0]
		klass, version_project = self.__class__.version[1]
		appname = getattr(Context.g_module, Context.APPNAME, os.path.basename(self.srcnode.abspath()))

		node = self.srcnode.make_node(appname+'.'+self.__class__.cmd+'.sln')
		projects = self.srcnode.make_node('.build').make_node(version)
		projects.mkdir()


		s = solution.Solution(appname, node.abspath(), version, version_number, version_name, folders)
		s.writeHeader()

		project_list = []
		for g in self.groups:
			for tg in g:
				if not isinstance(tg, TaskGen.task_gen):
					continue
				tg.post()

				allplatforms = []
				for i in self.env.ALL_VARIANTS:
					env = self.all_envs[i]
					options = tg.options[i]
					allplatforms.append((i, options))

				node = projects.make_node("%s.%s%s" % (tg.category, tg.name, klass.extensions[0]))
				project = klass(node.path_from(self.srcnode), tg.name, tg.category, version, version_project, tg.category, self.all_envs)
				project.writeHeader(allplatforms, self.game.name)
				project.addDirectory(tg.sourcetree)
				project.writeFooter()
				project_list.append((project, node))
		for project, node in project_list:
			if project.category == 'game':
				s.addProject(project, node.path_from(self.srcnode))
		for project, node in project_list:
			if project.category != 'game':
				s.addProject(project, node.path_from(self.srcnode))
		s.writeFooter(self.env.ALL_VARIANTS)

class vs2005(vs2003):
	cmd = 'vs2005'
	fun = 'build'
	version =	(('Visual Studio 2005', '9.00', True),(vcproj.VCproj, '8.00'))

class vs2005e(vs2003):
	cmd = 'vs2005e'
	fun = 'build'
	version =	(('Visual C++ Express 2005', '9.00', False),(vcproj.VCproj, '8.00'))


class vs2008(vs2003):
	cmd = 'vs2008'
	fun = 'build'
	version =	(('Visual Studio 2008', '10.00', True),(vcproj.VCproj, '9.00'))

class vs2008e(vs2003):
	cmd = 'vs2008e'
	fun = 'build'
	version =	(('Visual C++ Express 2008', '10.00', False),(vcproj.VCproj, '9.00'))

class vs2010(vs2003):
	cmd = 'vs2010'
	fun = 'build'
	version =	(('Visual Studio 2010', '11.00', True),(vcxproj.VCxproj, '4.0'))

class vs2010e(vs2003):
	cmd = 'vs2010e'
	fun = 'build'
	version =	(('Visual C++ Express 2010', '11.00', False),(vcxproj.VCxproj, '4.0'))

class vs11(vs2003):
	cmd = 'vs11'
	fun = 'build'
	version =	(('Visual Studio 11', '12.00', True),(vcxproj.VCxproj, '4.0'))

class vs11e(vs2003):
	cmd = 'vs11e'
	fun = 'build'
	version =	(('Visual C++ Express 11', '12.00', False),(vcxproj.VCxproj, '4.0'))

