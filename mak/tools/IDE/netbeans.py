from waflib.TaskGen import feature
from waflib.Configure import conf
from waflib import TaskGen, Context, Build
import os, sys
import mak
from xml.dom.minidom import Document

def setAttributes(node, attrs):
	for k, v in attrs.items():
		node.setAttribute(k, v)

def add(doc, parent, tag, value = None):
	el = doc.createElement(tag)
	if (value):
		if type(value) == type(str()):
			el.appendChild(doc.createTextNode(value))
		elif type(value) == type(dict()):
			setAttributes(el, value)
	parent.appendChild(el)
	return el

class netbeans(Build.BuildContext):
	cmd		= 'netbeans'
	fun		= 'build'
	version	= 79

	def generateProjectXml(self, appname, confs):
		doc = Document()
		doc.encoding = "UTF-8"
		project = add(doc, doc, 'project', {'xmlns': "http://www.netbeans.org/ns/project/1"})
		add(doc, project, 'type', "org.netbeans.modules.cnd.makeproject")
		configuration = add(doc, project, 'configuration')
		data = add(doc, configuration, 'data', {'xmlns': "http://www.netbeans.org/ns/make-project/1"})
		add(doc, data, 'name', appname)
		add(doc, data, 'make-project-type', '1')
		add(doc, data, 'c-extensions', 'c,m')
		add(doc, data, 'cpp-extensions', 'cpp,cc,cxx,mm')
		add(doc, data, 'header-extensions', 'h,hh,hxx,inl')
		add(doc, data, 'sourceEncoding', 'UTF-8')
		add(doc, data, 'make-dep-projects')
		if self.__class__.version >= 70:
			sourceRootList = add(doc, data, 'sourceRootList')
			sourceRoot = add(doc, sourceRootList, 'sourceRootElem', '.')
			confList = add(doc, data, 'confList')
			for i in confs:
				elem = add(doc, confList, 'confElem')
				add(doc, elem, 'name', i)
				add(doc, elem, 'type', '0')
		return doc

	def addSourceTree(self, doc, xml, name, folder, prefix):
		f = add(doc, xml, 'logicalFolder', {'name': name, 'displayName': name, 'projectFiles': 'true'})
		for subname, subdir in folder.directories.items():
			self.addSourceTree(doc, f, subname, subdir, os.path.join(prefix, subdir.prefix))
		for source in folder.files:
			if not source.generated():
				add(doc, f, 'itemPath', os.path.join(prefix, source.filename))

	def generateConfigurationsXml(self, sources, configurations, bld, out):
		doc = Document()
		doc.encoding = "UTF-8"
		cd = add(doc, doc, 'configurationDescriptor', {'version':'%i'%self.__class__.version})
		lf = add(doc, cd, 'logicalFolder', {'name': 'root', 'displayName': 'root', 'projectFiles': 'true', 'kind': "ROOT"})
		categories = {}
		for i in ['3rdparty', 'engine', 'game', 'plugin']:
			categories[i] = (add(doc, lf, 'logicalFolder', {'name': i, 'displayName': i, 'projectFiles': 'true'}), {})
		for project, category, source, options in sources:
			f, subs = categories[category]
			project = project.split('.')
			for subname in project[:-1]:
				try:
					f, subs = subs[subname]
				except KeyError:
					f = add(doc, f, 'logicalFolder', {'name': subname, 'displayName': subname, 'projectFiles': 'true'})
					subs[subname] = (f, {})
					f, subs = subs[subname]
			self.addSourceTree(doc, f, project[-1], source, source.prefix)
		impfiles = add(doc, lf, 'logicalFolder', {'name': 'ExternalFiles', 'displayName': 'waf', 'projectFiles': 'false', 'kind':'IMPORTANT_FILES_FOLDER'})
		add(doc, impfiles, 'itemPath', sys.argv[0])
		#add(doc, cd, 'sourceFolderFilter')
		srl = add(doc, cd, 'sourceRootList')
		add(doc, srl, 'Elem', '.')
		add(doc, srl, 'Elem', './build/')
		add(doc, cd, 'projectmakefile', sys.argv[0])
		confs = add(doc, cd, 'confs')
		for c in configurations:
			env = bld.all_envs[c]
			conf = add(doc, confs, 'conf', { 'name': c, 'type': '0' })
			toolsSet = add(doc, conf, 'toolsSet')
			if self.__class__.version >= 70:
				add(doc, toolsSet, 'remote-sources-mode', 'LOCAL_SOURCES')
			else:
				add(doc, toolsSet, 'developmentServer', 'localhost')
			add(doc, toolsSet, 'compilerSet', 'default')
			if self.__class__.version < 70:
				add(doc, toolsSet, 'platform', '4')
			mtype = add(doc, conf, 'makefileType')
			mtool = add(doc, mtype, 'makeTool')
			add(doc, mtool, 'buildCommandWorkingDir', '.')
			add(doc, mtool, 'buildCommand', 'python waf install_%s'%c)
			add(doc, mtool, 'cleanCommand', 'python waf clean_%s'%c)
			if env.ABI == 'mach_o' and False:
				add(doc, mtool, 'executablePath', os.path.join(env.PREFIX, getattr(Context.g_module, 'APPNAME', 'noname')+'.app'))
			else:
				add(doc, mtool, 'executablePath', os.path.join(env.PREFIX, env.DEPLOY['prefix'], env.DEPLOY['bin'], env.program_PATTERN%out))
			if self.__class__.version >= 70:
				ctool = add(doc, mtool, 'cTool')
				cincdir = add(doc, ctool, 'incDir')
				cdefines = add(doc, ctool, 'preprocessorList')
				cctool = add(doc, mtool, 'ccTool')
				ccincdir = add(doc, cctool, 'incDir')
				ccdefines = add(doc, cctool, 'preprocessorList')
			else:
				ctool = add(doc, mtool, 'cCompilerTool')
				cincdir = add(doc, ctool, 'includeDirectories')
				cdefines = add(doc, ctool, 'preprocessorList')
				cctool = add(doc, mtool, 'ccCompilerTool')
				ccincdir = add(doc, cctool, 'includeDirectories')
				ccdefines = add(doc, cctool, 'preprocessorList')
			add(doc, mtype, 'requiredProjects')
			includes=set([])
			defines = set([])
			for d in ['be_api(x)=', 'BE_EXPORT=']:
				add(doc, cdefines, 'Elem', d)
				add(doc, ccdefines, 'Elem', d)
			for project, category, source, options in sources:
				options = options[c]
				for i in options.includedir:
					if i not in includes:
						includes.add(i)
						add(doc, cincdir, 'directoryPath', i)
						add(doc, ccincdir, 'directoryPath', i)
				for d in env.DEFINES:
					if d not in defines:
						defines.add(d)
						add(doc, cdefines, 'Elem', d)
						add(doc, ccdefines, 'Elem', d)

		return doc

	def execute(self):
		"""
		Entry point
		"""
		self.restore()
		if not self.all_envs:
			self.load_envs()
		self.env.PROJECTS=[self.__class__.cmd]
		self.recurse([self.run_dir])


		appname = getattr(Context.g_module, Context.APPNAME, os.path.basename(self.srcnode.abspath()))
		path = self.srcnode.make_node('nbproject')
		path.mkdir()
		project = path.make_node('project.xml')
		confs = path.make_node('configurations.xml')
		deps = []

		for g in self.groups:
			for tg in g:
				if not isinstance(tg, TaskGen.task_gen):
					continue
				tg.post()
				deps.append((tg.name, tg.category, tg.sourcetree, tg.options))

		p = self.generateProjectXml(appname, self.env['ALL_VARIANTS'])
		project.write(p.toxml())
		c = self.generateConfigurationsXml(deps, self.env['ALL_VARIANTS'], self, self.game.name)
		confs.write(c.toxml())


class netbeans7(netbeans):
	cmd = 'netbeans7'
	fun = 'build'

class netbeans6(netbeans):
	cmd		= 'netbeans6'
	fun		= 'build'
	version	= 51

class sunstudio(netbeans6):
	cmd = 'sunstudio'
	fun = 'build'
