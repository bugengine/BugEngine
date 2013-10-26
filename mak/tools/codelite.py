from waflib import TaskGen, Context, Build
import os, sys
import mak
from xml.dom.minidom import Document


class XmlFile:
	def __init__(self):
		self.doc = Document()
		self.doc.encoding = "UTF-8"

	def setAttributes(self, node, attrs):
		for k, v in attrs.items():
			node.setAttribute(k, v)

	def add(self, parent, tag, value = None):
		el = self.doc.createElement(tag)
		if (value):
			if type(value) == type(str()):
				el.appendChild(self.doc.createTextNode(value))
			elif type(value) == type(dict()):
				self.setAttributes(el, value)
		parent.appendChild(el)
		return el

	def write(self, node):
		node.write(self.doc.toxml())


class Project (XmlFile):
	def __init__(self, task_gen, builder):
		XmlFile.__init__(self)
		self.project = self.add(self.doc, 'Codelite_Project', {'Name': '%s.%s'%(task_gen.category, task_gen.name)})
		self.add_files(self.project, task_gen.sourcetree, os.path.join('..','..'))
		self.add(self.project, 'Description')
		self.add(self.project, 'Dependencies')
		settings = self.add(self.project, 'Settings', {'Type':'Dynamic Library'})
		for env_name in builder.env.ALL_VARIANTS:
			env = builder.all_envs[env_name]
			conf = self.add(settings, 'Configuration', {
					'Name': env_name,
					'CompilerType': 'gnu g++',
					'DebuggerType': 'GNU gdb debugger',
					'Type': 'Dynamic Library',
					'BuildCmpWithGlobalSettings': 'append',
					'BuildLnkWithGlobalSettings': 'append',
					'BuildResWithGlobalSettings': 'append' })
			if task_gen.type == 'game':
				general = self.add(conf, 'General', {
						'OutputFile': '%s/%s/%s'%(env.PREFIX, env.DEPLOY['bin'], env.program_PATTERN%builder.game.name),
						'Command': '%s/%s/%s'%(env.PREFIX, env.DEPLOY['bin'], env.program_PATTERN%builder.game.name),
						'CommandArguments': task_gen.name
					})
			completion = self.add(conf, 'Completion')
			search_paths = self.add(completion, 'SearchPaths')
			custom_build = self.add(conf, 'CustomBuild', {'Enabled':'Yes'})
			if task_gen.type == 'waf':
				self.add(custom_build, 'BuildCommand', '%s %s install_%s' % (sys.executable, sys.argv[0], env_name))
				self.add(custom_build, 'CleanCommand', '%s %s clean_%s' % (sys.executable, sys.argv[0], env_name))
				self.add(custom_build, 'RebuildCommand', '%s %s clean_%s install_%s' % (sys.executable, sys.argv[0], env_name, env_name))


	def add_files(self, container, source_tree, local_path):
		local_path = os.path.join(local_path, source_tree.prefix)
		for dir_name, dir in source_tree.directories.iteritems():
			c = self.add(container, 'VirtualDirectory', {'Name':dir_name})
			self.add_files(c, dir, local_path)
		for file in source_tree.files:
			self.add(container, 'File', {'Name': os.path.join(local_path, file.filename)})


class Workspace (XmlFile):
	def __init__(self, appname, env_list):
		XmlFile.__init__(self)
		self.project = self.add(self.doc, 'CodeLite_Workspace', {'Name':appname, 'Database': 'mak/codelite/%s.tags'%appname})
		self.build_matrix = self.add(self.project, 'BuildMatrix')
		self.configs = {}
		for env in env_list:
			self.configs[env] = self.add(self.build_matrix, 'WorkspaceConfiguration', {'Name': env, 'Selected': 'yes'})

	def addProject(self, task_gen):
		project_name = task_gen.category + '.' + task_gen.name
		self.add(self.project, 'Project', {'Name': project_name, 'Path':'.build/codelite/%s.project' % project_name, 'Active':'No'})
		for env, conf in self.configs.iteritems():
			self.add(conf, 'Project', {'Name': project_name, 'ConfigName': env})


class codelite(Build.BuildContext):
	cmd = 'codelite'
	fun = 'build'
	optim = 'debug'

	def execute(self):
		"""
		Entry point
		"""
		self.restore()
		if not self.all_envs:
			self.load_envs()
		self.env.PROJECTS=[self.__class__.cmd]
		self.recurse([self.run_dir])

		folder = self.srcnode.make_node('.build')
		folder = folder.make_node('codelite')
		folder.mkdir()

		appname = getattr(Context.g_module, Context.APPNAME, os.path.basename(self.srcnode.abspath()))
		w = Workspace(appname, self.env.ALL_VARIANTS)

		for g in self.groups:
			for tg in g:
				if not isinstance(tg, TaskGen.task_gen):
					continue
				tg.post()
				node = folder.make_node('%s.%s.project' % (tg.category,tg.name))
				Project(tg, self).write(node)
				w.addProject(tg)

		node = self.srcnode.make_node('%s.workspace' % (appname))
		w.write(node)


