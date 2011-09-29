#! /usr/bin/env python
# encoding: utf-8

from waflib.TaskGen import feature
from waflib.Configure import conf
from waflib import Task, Context
import os, sys
import mak
from xml.dom.minidom import Document

oe_cdt = 'org.eclipse.cdt'
cdt_mk = oe_cdt + '.make.core'
cdt_core = oe_cdt + '.core'
cdt_bld = oe_cdt + '.build.core'


def create_cproject(self, appname, workspace_includes=[], pythonpath=[]):
	"""
	Create the Eclipse CDT .project and .cproject files
	@param appname The name that will appear in the Project Explorer
	@param build The BuildContext object to extract includes from
	@param workspace_includes Optional project includes to prevent
		  "Unresolved Inclusion" errors in the Eclipse editor
	@param pythonpath Optional project specific python paths
	"""
	project = self.impl_create_project(sys.executable, 'waf', appname)
	self.outputs[0].write(project.toxml())

	project = self.impl_create_cproject(sys.executable, 'waf', appname)
	self.outputs[1].write(project.toxml())

	project = self.impl_create_pydevproject(appname, sys.path, pythonpath)
	self.outputs[2].write(project.toxml())

def impl_create_project(self, executable, waf, appname):
	doc = Document()
	projectDescription = doc.createElement('projectDescription')
	self.add(doc, projectDescription, 'name', appname)
	self.add(doc, projectDescription, 'comment')
	self.add(doc, projectDescription, 'projects')
	buildSpec = self.add(doc, projectDescription, 'buildSpec')
	buildCommand = self.add(doc, buildSpec, 'buildCommand')
	self.add(doc, buildCommand, 'name', oe_cdt + '.managedbuilder.core.genmakebuilder')
	self.add(doc, buildCommand, 'triggers', 'clean,full,incremental,')
	arguments = self.add(doc, buildCommand, 'arguments')
	# the default make-style targets are overwritten by the .cproject values
	dictionaries = {
			cdt_mk + '.append_environment': 'true',
			cdt_mk + '.contents': cdt_mk + '.activeConfigSettings',
			cdt_mk + '.enableAutoBuild': 'false',
			cdt_mk + '.autoBuildTarget': '"%s" install_%s' % (waf, self.env['SELECTED_TOOLCHAINS'][0]),
			cdt_mk + '.enableCleanBuild': 'true',
			cdt_mk + '.cleanBuildTarget': '"%s" clean_%s' % (waf, self.env['SELECTED_TOOLCHAINS'][0]),
			cdt_mk + '.enableFullBuild': 'true',
			cdt_mk + '.fullBuildTarget': '"%s" install_%s' % (waf, self.env['SELECTED_TOOLCHAINS'][0]),
			cdt_mk + '.useDefaultBuildCmd': 'false',
			cdt_mk + '.buildCommand': executable
			}
	for k, v in dictionaries.items():
		self.addDictionary(doc, arguments, k, v)

	natures = self.add(doc, projectDescription, 'natures')
	nature_list = """
		core.ccnature
		managedbuilder.core.ScannerConfigNature
		managedbuilder.core.managedBuildNature
		core.cnature
	""".split()
	for n in nature_list:
		self.add(doc, natures, 'nature', oe_cdt + '.' + n)

	self.add(doc, natures, 'nature', 'org.python.pydev.pythonNature')

	resourceFilters = self.add(doc, projectDescription, 'filteredResources')
	id = 1291476911
	for name,recursively,type in [
		('src', 'true', '9'),
		('mak', 'true', '9'),
		('waf', 'false', '5'),
		('wscript', 'false', '5'),
		('.svn', 'true', '26'),
		('.cvs', 'true', '26'),
		]:
		filter = self.add(doc, resourceFilters, 'filter')
		self.add(doc, filter, 'id', str(id))
		self.add(doc, filter, 'name')
		self.add(doc, filter, 'type', type)
		matcher = self.add(doc, filter, 'matcher')
		self.add(doc, matcher, 'id', 'org.eclipse.ui.ide.multiFilter')
		self.add(doc, matcher, 'arguments', '1.0-name-matches-%s-false-%s' % (recursively, name))
		id = id + 2

	doc.appendChild(projectDescription)
	return doc

def impl_create_cproject(self, executable, waf, appname):
	source_dirs = []
	doc = Document()
	doc.encoding = "UTF-8"
	doc.appendChild(doc.createProcessingInstruction('fileVersion', '4.0.0'))

	cproject = doc.createElement('cproject')
	count = 0
	rootStorageModule = self.add(doc, cproject, 'storageModule',
			{'moduleId': cdt_core + '.settings'})

	for toolchainName in self.env['BUILD_VARIANTS']+[i for i in self.env['ALL_VARIANTS'] if i not in self.env['BUILD_VARIANTS']]:
		count = count+1
		cconf_id = cdt_core + '.default.config.%d'%count
		cconf = self.add(doc, rootStorageModule, 'cconfiguration', {'id':cconf_id})

		storageModule = self.add(doc, cconf, 'storageModule',
				{'buildSystemId': oe_cdt + '.managedbuilder.core.configurationDataProvider',
				'id': cconf_id,
				'moduleId': cdt_core + '.settings',
				'name': toolchainName})
		self.add(doc, storageModule, 'externalSettings')

		extensions = self.add(doc, storageModule, 'extensions')
		extension_list = """
			VCErrorParser
			GmakeErrorParser
			GCCErrorParser
			GASErrorParser
			GLDErrorParser
			CWDLocator
		""".split()
		ext = self.add(doc, extensions, 'extension',
					{'id': cdt_core + '.ELF', 'point':cdt_core + '.BinaryParser'})
		ext = self.add(doc, extensions, 'extension',
					{'id': cdt_core + '.PE', 'point':cdt_core + '.BinaryParser'})
		ext = self.add(doc, extensions, 'extension',
					{'id': cdt_core + '.MachO64', 'point':cdt_core + '.BinaryParser'})
		for e in extension_list:
			ext = self.add(doc, extensions, 'extension',
					{'id': cdt_core + '.' + e, 'point':cdt_core + '.ErrorParser'})

		storageModule = self.add(doc, cconf, 'storageModule',
				{'moduleId': 'cdtBuildSystem', 'version': '4.0.0'})
		config = self.add(doc, storageModule, 'configuration',
					{	'artifactName': appname,
						'buildProperties': '',
						'description': '',
						'id': cconf_id,
						'name': toolchainName,
						'parent': cdt_bld + '.prefbase.cfg'})
		for name, category, sourcetree, platforms in self.depends:
			env = self.bld.all_envs[toolchainName]
			try:
				options = platforms['%s-%s' % (env['PLATFORM'][0], env['ARCHITECTURE'])]
			except Exception as e:
				options = None
			count = count+1
			folderInfo = self.add(doc, config, 'folderInfo',
								{'id': cconf_id+'.%d'%count, 'resourcePath': '/'+sourcetree.prefix.replace('\\', '/'), 'name': ''})
			count = count+1
			toolChain = self.add(doc, folderInfo, 'toolChain',
					{	'id': cdt_bld + '.prefbase.toolchain.%d'%count,
						'name': 'No ToolChain',
						'resourceTypeBasedDiscovery': 'false',
						'superClass': cdt_bld + '.prefbase.toolchain'})

			count = count+1
			targetPlatform = self.add(doc, toolChain, 'targetPlatform',
					{	'binaryParser': 'org.eclipse.cdt.core.ELF;org.eclipse.cdt.core.MachO64;org.eclipse.cdt.core.PE',
						'id': cdt_bld + '.prefbase.toolchain.%d'%count, 'name': ''})

			waf_build = '"%s" install_%s'%(waf,toolchainName)
			waf_clean = '"%s" clean_%s'%(waf,toolchainName)
			count = count+1
			builder = self.add(doc, toolChain, 'builder',
							{	'autoBuildTarget': waf_build,
								'command': executable,
								'enableAutoBuild': 'false',
								'cleanBuildTarget': waf_clean,
								'id': cdt_bld + '.settings.default.builder.%d'%count,
								'incrementalBuildTarget': waf_build,
								'keepEnvironmentInBuildfile': 'false',
								'managedBuildOn': 'false',
								'name': 'Gnu Make Builder',
								'superClass': cdt_bld + '.settings.default.builder'})

			for tool_name in ("Assembly", "GNU C++", "GNU C"):
				count = count+1
				tool = self.add(doc, toolChain, 'tool',
						{	'id': cdt_bld + '.settings.holder.%d'%count,
							'name': tool_name,
							'superClass': cdt_bld + '.settings.holder'})
				count = count+1
				input = self.add(doc, tool, 'inputType',
						{	'id': cdt_bld + '.settings.holder.inType.%d'%count,
							'superClass': cdt_bld + '.settings.holder.inType'})
				if options:
					cpppath = options.includedir
					incpaths = cdt_bld + '.settings.holder.incpaths'
					count = count+1
					option = self.add(doc, tool, 'option',
							{	'id': incpaths+'.%d'%count,
								'name': 'Include Paths',
								'superClass': incpaths,
								'valueType': 'includePath'})
					for i in cpppath:
						self.add(doc, option, 'listOptionValue',
									{'builtIn': 'false',
									'value': '"${ProjDirPath}/%s"'%(i)})
					incpaths = cdt_bld + '.settings.holder.symbols'
					count = count+1
					option = self.add(doc, tool, 'option',
							{	'id': incpaths+'.%d'%count,
								'name': 'Include Paths',
								'superClass': incpaths,
								'valueType': 'definedSymbols'})
					for d in options.defines|set(env['DEFINES']+['__ECLIPSE']):
						self.add(doc, option, 'listOptionValue',
									{'builtIn': 'false',
									'value': '"%s"'%(d)})

		count = count+1
		tool = self.add(doc, toolChain, 'tool',
						{	'id': cdt_bld + '.settings.holder.libs.%d'%count,
							'name': "holder for library settings",
							'superClass': cdt_bld + '.settings.holder.libs'})
		if source_dirs:
			sourceEntries = self.add(doc, config, 'sourceEntries')
			for i in source_dirs:
				self.add(doc, sourceEntries, 'entry',
							{'excluding': i,
							'flags': 'VALUE_WORKSPACE_PATH|RESOLVED',
							'kind': 'sourcePath',
							'name': ''})
				self.add(doc, sourceEntries, 'entry',
							{
							'flags': 'VALUE_WORKSPACE_PATH|RESOLVED',
							'kind': 'sourcePath',
							'name': i})

		storageModule = self.add(doc, cconf, 'storageModule',
							{'moduleId': 'org.eclipse.cdt.core.externalSettings'})

	storageModule = self.add(doc, cproject, 'storageModule',
						{	'moduleId': 'cdtBuildSystem',
							'version': '4.0.0'})

	project = self.add(doc, storageModule, 'project',
				{'id': '%s.null.0'%appname, 'name': appname})

	storageModule = self.add(doc, cproject, 'storageModule',
						{'moduleId': cdt_mk + '.buildtargets'})
	buildTargets = self.add(doc, storageModule, 'buildTargets')
	def addTargetWrap(name, runAll):
		return self.addTarget(doc, buildTargets, executable, name,
						'"%s" %s'%(waf, name), runAll)
	addTargetWrap('configure', True)
	addTargetWrap('dist', False)
	addTargetWrap('install', False)
	addTargetWrap('check', False)

	doc.appendChild(cproject)
	return doc

def impl_create_pydevproject(self, appname, system_path, user_path):
	# create a pydevproject file
	doc = Document()
	doc.appendChild(doc.createProcessingInstruction('eclipse-pydev', 'version="1.0"'))
	pydevproject = doc.createElement('pydev_project')
	prop = self.add(doc, pydevproject,
					'pydev_property',
					'python %d.%d'%(sys.version_info[0], sys.version_info[1]))
	prop.setAttribute('name', 'org.python.pydev.PYTHON_PROJECT_VERSION')
	prop = self.add(doc, pydevproject, 'pydev_property', 'Default')
	prop.setAttribute('name', 'org.python.pydev.PYTHON_PROJECT_INTERPRETER')
	# add waf's paths
	wafadmin = [p for p in system_path if p.find('wafadmin') != -1]
	if wafadmin:
		prop = self.add(doc, pydevproject, 'pydev_pathproperty',
				{'name':'org.python.pydev.PROJECT_EXTERNAL_SOURCE_PATH'})
		for i in wafadmin:
			self.add(doc, prop, 'path', i)
	if user_path:
		prop = self.add(doc, pydevproject, 'pydev_pathproperty',
				{'name':'org.python.pydev.PROJECT_SOURCE_PATH'})
		for i in user_path:
			self.add(doc, prop, 'path', '/'+appname+'/'+i)

	doc.appendChild(pydevproject)
	return doc

def addDictionary(self, doc, parent, k, v):
	dictionary = self.add(doc, parent, 'dictionary')
	self.add(doc, dictionary, 'key', k)
	self.add(doc, dictionary, 'value', v)
	return dictionary

def addTarget(self, doc, buildTargets, executable, name, buildTarget, runAllBuilders=True):
	target = self.add(doc, buildTargets, 'target',
					{	'name': name,
						'path': '',
						'targetID': oe_cdt + '.build.MakeTargetBuilder'})
	self.add(doc, target, 'buildCommand', executable)
	self.add(doc, target, 'buildArguments', None)
	self.add(doc, target, 'buildTarget', buildTarget)
	self.add(doc, target, 'stopOnError', 'true')
	self.add(doc, target, 'useDefaultCommand', 'false')
	self.add(doc, target, 'runAllBuilders', str(runAllBuilders).lower())

def add(self, doc, parent, tag, value = None):
	el = doc.createElement(tag)
	if (value):
		if type(value) == type(str()):
			el.appendChild(doc.createTextNode(value))
		elif type(value) == type(dict()):
			self.setAttributes(el, value)
	parent.appendChild(el)
	return el

def setAttributes(self, node, attrs):
	for k, v in attrs.items():
		node.setAttribute(k, v)



def generateProject(task):
	appname = getattr(Context.g_module, Context.APPNAME, os.path.basename(task.bld.srcnode.abspath()))
	task.create_cproject(appname, pythonpath=task.env['ECLIPSE_PYTHON_PATH'])

EclipseGenerateProject = Task.task_factory('EclipseGenerateProject', generateProject)
EclipseGenerateProject.create_cproject = create_cproject
EclipseGenerateProject.impl_create_project = impl_create_project
EclipseGenerateProject.impl_create_cproject = impl_create_cproject
EclipseGenerateProject.impl_create_pydevproject = impl_create_pydevproject
EclipseGenerateProject.add = add
EclipseGenerateProject.setAttributes = setAttributes
EclipseGenerateProject.addTarget = addTarget
EclipseGenerateProject.addDictionary = addDictionary


solutions={}
@feature('eclipse')
def create_eclipse_project(t):
	toolName = t.features[0]
	if not toolName in solutions:
		outname = ['.project', '.cproject', '.pydevproject']
		solution = t.create_task("EclipseGenerateProject")
		solution.bld = t.bld
		solution.appname = getattr(Context.g_module, 'APPNAME', 'noname')
		solution.env=t.env
		solution.version = toolName
		solution.allplatforms    = t.env['ALL_VARIANTS']
		outnode = [t.path.find_or_declare(n) for n in outname]
		solution.set_outputs(outnode)
		t.bld.install_files(t.path.srcpath(), outnode)
		t.bld.install_files(os.path.join(t.path.srcpath(), '.settings'), t.path.find_or_declare('mak/eclipse/.settings/org.eclipse.cdt.codan.core.prefs'))
		t.bld.install_files(os.path.join(t.path.srcpath(), '.settings'), t.path.find_or_declare('mak/eclipse/.settings/org.eclipse.cdt.core.prefs'))
		t.bld.install_files(os.path.join(t.path.srcpath(), '.settings'), t.path.find_or_declare('mak/eclipse/.settings/org.eclipse.cdt.ui.prefs'))
		solution.depends = []
		solution.dep_vars = ['ECLIPSE_PROJECT_DEPENDS']
		solution.env['ECLIPSE_PROJECT_DEPENDS'] = []
		solutions[toolName] = solution
	solution = solutions[toolName]
	solution.depends.append((t.name, t.category, t.sourcetree, t.platforms))

