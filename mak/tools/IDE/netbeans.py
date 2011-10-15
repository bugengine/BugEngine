from waflib.TaskGen import feature
from waflib.Configure import conf
from waflib import Task, Context
import os
import mak
from xml.dom.minidom import Document

allconfigs = ['debug','profile','final']

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

def addSourceTree(doc, xml, name, folder):
	if folder.prefix != name:
		f = add(doc, xml, "df", {'name': name, 'root': folder.prefix})
	else:
		f = add(doc, xml, "df", {'name': name})
	for subname, subdir in folder.directories.items():
		addSourceTree(doc, f, subname, subdir)
	for source in folder.files:
		add(doc, f, 'in', source.filename)

def generateConfigurationsXml(sources, configurations, bld, out):
	doc = Document()
	doc.encoding = "UTF-8"
	cd = add(doc, doc, 'configurationDescriptor', {'version':'79'})
	lf = add(doc, cd, 'logicalFolder', {'name': 'root', 'displayName': 'root', 'projectFiles': 'true', 'kind': "ROOT"})
	for project,category,source in sources:
		addSourceTree(doc, lf, category+'.'+project, source)
	#add(doc, cd, 'sourceFolderFilter')
	srl = add(doc, cd, 'sourceRootList')
	add(doc, srl, 'Elem', '.')
	add(doc, srl, 'Elem', './build/')
	add(doc, cd, 'projectmakefile', 'wscript')
	confs = add(doc, cd, 'confs')
	for c in configurations:
		env = bld.all_envs[c]
		conf = add(doc, confs, 'conf', { 'name': c, 'type': '0' })
		toolsSet = add(doc, conf, 'toolsSet')
		add(doc, toolsSet, 'remote-source-mode', 'LOCAL_SOURCES')
		mtype = add(doc, conf, 'makefileType')
		mtool = add(doc, mtype, 'makeTool')
		add(doc, mtool, 'buildCommandWorkingDir', '.')
		add(doc, mtool, 'buildCommand', 'python waf install_%s'%c)
		add(doc, mtool, 'cleanCommand', 'python waf clean_%s'%c)
		if env.ABI == 'mach_o' and False:
			add(doc, mtool, 'executablePath', os.path.join(env.PREFIX, getattr(Context.g_module, 'APPNAME', 'noname')+'.app'))
		else:
			add(doc, mtool, 'executablePath', os.path.join(env.PREFIX, env.DEPLOY['prefix'], env.DEPLOY['bin'], env.program_PATTERN%out))
	return doc

def generateProject(task):
	project = generateProjectXml(task.appname, task.allplatforms)
	task.outputs[0].write(project.toxml())
	configurations = generateConfigurationsXml(task.depends, task.allplatforms, task.bld, task.out)
	task.outputs[1].write(configurations.toxml())

NetbeansGenerateProject = Task.task_factory('NetbeansGenerateProject', generateProject)

solutions={}
@feature('netbeans')
def create_netbeans_project(t):
	toolName = t.features[0]
	if not toolName in solutions:
		outname = ['projects/project.xml', 'projects/configurations.xml']
		solution = t.create_task("NetbeansGenerateProject")
		solution.bld = t.bld
		solution.appname = getattr(Context.g_module, 'APPNAME', 'noname')
		solution.env=t.env
		solution.version = toolName
		solution.allplatforms    = t.env['ALL_VARIANTS']
		outnode = [t.path.find_or_declare(n) for n in outname]
		solution.set_outputs(outnode)
		t.bld.install_files(os.path.join(t.path.srcpath(), 'nbproject'), outnode)
		solution.depends = []
		solution.dep_vars = ['NB_PROJECT_DEPENDS']
		solution.env['NB_PROJECT_DEPENDS'] = []
		solutions[toolName] = solution
	solution = solutions[toolName]
	solution.depends.append((t.name, t.category, t.sourcetree))
	if t.type == 'game':
		solution.out = t.name

