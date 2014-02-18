from waflib import TaskGen, Context, Build
import os
import sys
from xml.dom.minidom import Document

def relpath(i, node):
	if isinstance(i, str):
		return i
	else:
		return i.path_from(node)

def gather_includes_defines(task_gen):
	defines = getattr(task_gen, 'defines', [])
	includes = getattr(task_gen, 'includes', [])
	seen = []
	use = getattr(task_gen, 'use', [])[:]
	while use:
		name = use.pop()
		if name not in seen:
			try:
				t = task_gen.bld.get_tgen_by_name(name)
			except:
				pass
			else:
				use = use + getattr(t, 'use', [])
				includes = includes + getattr(t, 'includes ', [])
				defines = defines + getattr(t, 'defines ', [])
	return includes, defines

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

class NbFolder:
	def __init__(self, name, document, xml):
		self.xml = add(document, xml, 'logicalFolder', {'name': name, 'displayName': name, 'projectFiles': 'true'})
		self.subfolders = {}

class Netbeans(Build.BuildContext):
	cmd		= 'netbeans'
	fun		= 'build'
	optim	= 'debug'
	version	= 79

	def generateProjectXml(self, appname, bld):
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
			for t in bld.env.ALL_TOOLCHAINS:
				for v in bld.env.ALL_VARIANTS:
					elem = add(doc, confList, 'confElem')
					add(doc, elem, 'name', '%s:%s' % (t, v))
					add(doc, elem, 'type', '0')
		return doc

	def add_folder(self, name, doc, folder):
		if name not in folder.subfolders:
			folder.subfolders[name] = NbFolder(name, doc, folder.xml)
		return folder.subfolders[name]

	def add(self, doc, folder, node):
		if os.path.isdir(node.abspath()):
			if node.name not in folder.subfolders:
				folder.subfolders[node.name] = NbFolder(node.name, doc, folder.xml)
			f = folder.subfolders[node.name]
			for child in node.listdir():
				self.add(doc, f, node.make_node(child))
		else:
			add(doc, folder.xml, 'itemPath', node.abspath())

	def generateConfigurationsXml(self, task_gens, bld, out):
		doc = Document()
		doc.encoding = "UTF-8"
		cd = add(doc, doc, 'configurationDescriptor', {'version':'%i'%self.__class__.version})
		lf = add(doc, cd, 'logicalFolder', {'name': 'root', 'displayName': 'root', 'projectFiles': 'true', 'kind': "ROOT"})
		self.subfolders = {}
		options = []
		for task_gen in task_gens:
			options.append(gather_includes_defines(task_gen))
			path = task_gen.target.split('.')
			root_folder = self
			root_folder.xml = lf
			for p in path[:-1]:
				root_folder = self.add_folder(p, doc, root_folder)
			for node in getattr(task_gen, 'source_nodes', []):
				self.add(doc, root_folder, node)

			#f, subs = categories[category]
			#project = project.split('.')
			#for subname in project:
			#	try:
			#		f, subs = subs[subname]
			#	except KeyError:
			#		f = add(doc, f, 'logicalFolder', {'name': subname, 'displayName': subname, 'projectFiles': 'true'})
			#		subs[subname] = (f, {})
			#		f, subs = subs[subname]
			#f.setAttribute('displayName', '['+project[-1]+']')
			#self.addSourceTree(doc, f, source, source.prefix)
		impfiles = add(doc, lf, 'logicalFolder', {'name': 'ExternalFiles', 'displayName': 'waf', 'projectFiles': 'false', 'kind':'IMPORTANT_FILES_FOLDER'})
		add(doc, impfiles, 'itemPath', sys.argv[0])
		#add(doc, cd, 'sourceFolderFilter')
		srl = add(doc, cd, 'sourceRootList')
		#add(doc, srl, 'Elem', '.')
		#add(doc, srl, 'Elem', './build/')
		add(doc, cd, 'projectmakefile', sys.argv[0])
		add(doc, cd, 'sourceFolderFilter', '^*$')
		confs = add(doc, cd, 'confs')
		for toolchain in bld.env.ALL_TOOLCHAINS:
			bld_env = bld.all_envs[toolchain]
			if bld_env.SUB_TOOLCHAINS:
				env = bld.all_envs[bld_env.SUB_TOOLCHAINS[0]]
			else:
				env = bld_env
			platform_defines = env.SYSTEM_DEFINES
			platform_includes = env.SYSTEM_INCLUDES
			platform_includes += [os.path.join(i, 'usr', 'include') for i in env.SYSROOT]
			options.append((platform_includes, platform_defines))

			for variant in bld.env.ALL_VARIANTS:
				conf = add(doc, confs, 'conf', { 'name': '%s:%s'%(toolchain, variant), 'type': '0' })
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
				add(doc, mtool, 'buildCommand', 'python waf install:%s:%s'%(toolchain, variant))
				add(doc, mtool, 'cleanCommand', 'python waf clean:%s:%s'%(toolchain, variant))
				if env.ABI == 'mach_o':
					add(doc, mtool, 'executablePath', os.path.join(bld_env.PREFIX, variant, getattr(Context.g_module, 'APPNAME', 'noname')+'.app'))
				else:
					add(doc, mtool, 'executablePath', os.path.join(bld_env.PREFIX, variant, env.DEPLOY_BINDIR, env.cxxprogram_PATTERN%out.target))
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
				for d in ['be_api(x)=', 'BE_EXPORT='] + env.DEFINES:
					add(doc, cdefines, 'Elem', d)
					add(doc, ccdefines, 'Elem', d)
				for tg_includes, tg_defines in options:
					for i in tg_includes:
						i = relpath(i, bld.srcnode)
						if i not in includes:
							includes.add(i)
							add(doc, cincdir, 'directoryPath', i)
							add(doc, ccincdir, 'directoryPath', i)
					for d in tg_defines:
						if d not in tg_defines:
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
		self.env.TOOLCHAIN = '$(TOOLCHAIN)'
		self.env.VARIANT = '$(CONFIG)'
		self.env.PREFIX = '$(PREFIX)'
		self.env.DEPLOY_ROOTDIR = '$(DEPLOY_ROOTDIR)'
		self.env.DEPLOY_BINDIR = '$(DEPLOY_BINDIR)'
		self.env.DEPLOY_RUNBINDIR = '$(DEPLOY_RUNBINDIR)'
		self.env.DEPLOY_LIBDIR = '$(DEPLOY_LIBDIR)'
		self.env.DEPLOY_INCLUDEDIR = '$(DEPLOY_INCLUDEDIR)'
		self.env.DEPLOY_DATADIR = '$(DEPLOY_DATADIR)'
		self.env.DEPLOY_PLUGINDIR = '$(DEPLOY_PLUGINDIR)'
		self.env.DEPLOY_KERNELDIR = '$(DEPLOY_KERNELDIR)'
		self.features = ['GUI']
		self.recurse([self.run_dir])


		appname = getattr(Context.g_module, Context.APPNAME, os.path.basename(self.srcnode.abspath()))
		path = self.srcnode.make_node('nbproject')
		path.mkdir()
		project = path.make_node('project.xml')
		confs = path.make_node('configurations.xml')
		deps = []
		out = None

		for g in self.groups:
			for tg in g:
				if not isinstance(tg, TaskGen.task_gen):
					continue
				tg.post()
				if not 'kernel' in tg.features:
					deps.append(tg)
				if 'launcher' in tg.features:
					out = tg


		p = self.generateProjectXml(appname, self)
		project.write(p.toxml())
		c = self.generateConfigurationsXml(deps, self, out)
		confs.write(c.toxml())


class Netbeans7(Netbeans):
	cmd = 'netbeans7'
	fun = 'build'

class Netbeans6(Netbeans):
	cmd		= 'netbeans6'
	fun		= 'build'
	version	= 51

class SunStudio(Netbeans6):
	cmd = 'sunstudio'
	fun = 'build'
