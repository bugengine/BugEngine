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

def generateProjectXml(appname, confs):
	doc = Document()
	doc.encoding = "UTF-8"
	project = add(doc, doc, 'project', {'xmlns': "http://www.netbeans.org/ns/project/1"})
	add(doc, project, 'type', "org.netbeans.modules.cnd.makeproject")
	configuration = add(doc, project, 'configuration')
	data = add(doc, configuration, 'data', {'xmlns': "http://www.netbeans.org/ns/make-project/1"})
	add(doc, data, 'name', appname)
	add(doc, data, 'c-extensions', 'c,m')
	add(doc, data, 'cpp-extensions', 'cpp,cc,cxx,mm')
	add(doc, data, 'header-extensions', 'h,hh,hxx,inl')
	add(doc, data, 'sourceEncoding', 'UTF-8')
	add(doc, data, 'make-dep-projects')
	sourceRootList = add(doc, data, 'sourceRootList')
	sourceRoot = add(doc, sourceRootList, 'sourceRootElem', '.')
	confList = add(doc, data, 'confList')
	for i in confs:
		elem = add(doc, confList, 'confElem')
		add(doc, elem, 'name', i)
		add(doc, elem, 'type', '0')
	return doc

def addSourceTree(doc, xml, name, folder, prefix):
	f = add(doc, xml, 'logicalFolder', {'name': name, 'displayName': name, 'projectFiles': 'true'})
	for subname, subdir in folder.directories.items():
		addSourceTree(doc, f, subname, subdir, os.path.join(prefix, subdir.prefix))
	for source in folder.files:
		if not source.generated():
			add(doc, f, 'itemPath', os.path.join(prefix, source.filename))

def generateConfigurationsXml(sources, configurations, bld, out):
	doc = Document()
	doc.encoding = "UTF-8"
	cd = add(doc, doc, 'configurationDescriptor', {'version':'79'})
	lf = add(doc, cd, 'logicalFolder', {'name': 'root', 'displayName': 'root', 'projectFiles': 'true', 'kind': "ROOT"})
	categories = {}
	for i in ['3rdparty', 'engine', 'game', 'plugin']:
		categories[i] = add(doc, lf, 'logicalFolder', {'name': i, 'displayName': i, 'projectFiles': 'true'})
	for project, category, source, options in sources:
		addSourceTree(doc, categories[category], project, source, source.prefix)
	impfiles = add(doc, lf, 'logicalFolder', {'name': 'ExternalFiles', 'displayName': 'Makefile', 'projectFiles': 'false', 'kind':'IMPORTANT_FILES_FOLDER'})
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
		add(doc, toolsSet, 'remote-sources-mode', 'LOCAL_SOURCES')
		add(doc, toolsSet, 'compilerSet', 'default')
		mtype = add(doc, conf, 'makefileType')
		mtool = add(doc, mtype, 'makeTool')
		add(doc, mtool, 'buildCommandWorkingDir', '.')
		add(doc, mtool, 'buildCommand', 'python waf install_%s'%c)
		add(doc, mtool, 'cleanCommand', 'python waf clean_%s'%c)
		if env.ABI == 'mach_o' and False:
			add(doc, mtool, 'executablePath', os.path.join(env.PREFIX, getattr(Context.g_module, 'APPNAME', 'noname')+'.app'))
		else:
			add(doc, mtool, 'executablePath', os.path.join(env.PREFIX, env.DEPLOY['prefix'], env.DEPLOY['bin'], env.program_PATTERN%out))
		for project, category, source, options in sources:
			options = options[c]
			name = category+'/'+project
			f = add(doc, conf, 'folder', {'path': name})
			ctool = add(doc, f, 'cTool')
			incdir = add(doc, ctool, 'incDir')
			for i in options.includedir:
				add(doc, incdir, 'pElem', i)
			defines = add(doc, ctool, 'preprocessorList')
			for d in ['be_api(x)=', 'BE_EXPORT=']:
				add(doc, defines, 'Elem', d)
			for d in env.DEFINES:
				add(doc, defines, 'Elem', d)
			for d in options.defines:
				add(doc, defines, 'Elem', d)
			ctool = add(doc, f, 'ccTool')
			incdir = add(doc, ctool, 'incDir')
			for i in options.includedir:
				add(doc, incdir, 'pElem', i)
			defines = add(doc, ctool, 'preprocessorList')
			for d in ['be_api(x)=', 'BE_EXPORT=']:
				add(doc, defines, 'Elem', d)
			for d in env.DEFINES:
				add(doc, defines, 'Elem', d)
			for d in options.defines:
				add(doc, defines, 'Elem', d)

	return doc

class netbeans(Build.BuildContext):
	cmd = 'netbeans'
	fun = 'build'

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

		p = generateProjectXml(appname, self.env['ALL_VARIANTS'])
		project.write(p.toxml())
		c = generateConfigurationsXml(deps, self.env['ALL_VARIANTS'], self, self.game.name)
		confs.write(c.toxml())


