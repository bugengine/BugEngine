#! /usr/bin/env python
# encoding: utf-8

from waflib.TaskGen import feature
from waflib.Configure import conf
from waflib import TaskGen, Context, Build
import os, sys
import mak
from xml.dom.minidom import Document

oe_cdt = 'org.eclipse.cdt'
cdt_mk = oe_cdt + '.make.core'
cdt_core = oe_cdt + '.core'
cdt_bld = oe_cdt + '.build.core'

class eclipse(Build.BuildContext):
	cmd = 'eclipse'
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

		appname = getattr(Context.g_module, 'APPNAME', 'noname')
		self.create_cproject(appname)


		settings = self.srcnode.find_or_declare('mak/eclipse/.settings/')
		out = self.srcnode.make_node('.settings')
		out.mkdir()
		for f in settings.listdir():
			n = settings.find_or_declare(f)
			out.make_node(f).write(n.read())




	def create_cproject(self, appname):
		"""
		Create the Eclipse CDT .project and .cproject files
		@param appname The name that will appear in the Project Explorer
		@param build The BuildContext object to extract includes from
		@param workspace_includes Optional project includes to prevent
			  "Unresolved Inclusion" errors in the Eclipse editor
		"""
		project = self.impl_create_project(sys.executable, sys.argv[0], appname)
		self.srcnode.make_node('.project').write(project.toxml())

		project = self.impl_create_cproject(sys.executable, sys.argv[0], appname)
		self.srcnode.make_node('.cproject').write(project.toxml())

		project = self.impl_create_pydevproject(appname, sys.path, sys.executable)
		self.srcnode.make_node('.pydevproject').write(project.toxml())

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

		resources = self.add(doc, projectDescription, 'linkedResources')
		for category in ['3rdparty', 'engine', 'game', 'plugin']:
			link = self.add(doc, resources, 'link')
			self.add(doc, link, 'name', category)
			self.add(doc, link, 'type', '2')
			self.add(doc, link, 'locationURI', 'virtual:/virtual')
		for g in self.groups:
			for tg in g:
				if not isinstance(tg, TaskGen.task_gen):
					continue
				tg.post()

				name = tg.name
				category = tg.category
				sourcetree = tg.sourcetree
				self.addSourceTree(doc, resources, sourcetree, category+'/'+name, os.path.join('PROJECT_LOC', sourcetree.prefix))

		filters = self.add(doc, projectDescription, 'filteredResources')
		filter = self.add(doc, filters, 'filter')
		self.add(doc, filter, 'id', '1321206201691')
		self.add(doc, filter, 'name')
		self.add(doc, filter, 'type', '30')
		matcher = self.add(doc, filter, 'matcher')
		self.add(doc, matcher, 'id', 'org.eclipse.ui.ide.multiFilter')
		self.add(doc, matcher, 'arguments', '1.0-name-matches-false-false-*')

		doc.appendChild(projectDescription)
		return doc

	def addSourceTree(self, doc, resources, sourcetree, folder, path):
		link = self.add(doc, resources, 'link')
		self.add(doc, link, 'name', folder)
		self.add(doc, link, 'type', '2')
		self.add(doc, link, 'locationURI', 'virtual:/virtual')
		for name, tree in sourcetree.directories.items():
			self.addSourceTree(doc, resources, tree, folder+"/"+name, os.path.join(path, tree.prefix))
		for file in sourcetree.files:
			if file.generated():
				continue
			link = self.add(doc, resources, 'link')
			self.add(doc, link, 'name', folder+"/"+file.filename)
			self.add(doc, link, 'type', '1')
			self.add(doc, link, 'locationURI', os.path.join(path, file.filename).replace('\\', '/'))




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
			count = count+1
			folderInfo = self.add(doc, config, 'folderInfo',
								{'id': cconf_id+'.%d'%count, 'resourcePath': '', 'name': ''})
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
			count = count+1
			tool = self.add(doc, toolChain, 'tool',
							{	'id': cdt_bld + '.settings.holder.libs.%d'%count,
								'name': "holder for library settings",
								'superClass': cdt_bld + '.settings.holder.libs'})


			for g in self.groups:
				for tg in g:
					if not isinstance(tg, TaskGen.task_gen):
						continue
					tg.post()

					name = tg.name
					category = tg.category
					sourcetree = tg.sourcetree

					env = self.all_envs[toolchainName]
					option = tg.options[toolchainName]
					count = count+1
					folderInfo = self.add(doc, config, 'folderInfo',
										{'id': cconf_id+'.%d'%count, 'resourcePath': '/'+category+'/'+name, 'name': ''})
					count = count+1
					toolChain = self.add(doc, folderInfo, 'toolChain',
							{	'id': cdt_bld + '.prefbase.toolchain.%d'%count,
								'name': 'No ToolChain',
								'unusedChidlren': '',
								'superClass': cdt_bld + '.prefbase.toolchain'})
					count = count+1
					tool = self.add(doc, toolChain, 'tool',
									{	'id': cdt_bld + '.settings.holder.libs.%d'%count,
										'name': "holder for library settings",
										'superClass': cdt_bld + '.settings.holder.libs'})
					for tool_name in ("Assembly", "GNU C++", "GNU C"):
						count = count+1
						tool = self.add(doc, toolChain, 'tool',
								{	'id': cdt_bld + '.settings.holder.%d'%count,
									'name': tool_name,
									'superClass': cdt_bld + '.settings.holder'})
						count = count+1
						includes = self.add(doc, tool, 'option',
								{	'id': cdt_bld + '.settings.holder.incpaths.%d'%count,
									'superClass': cdt_bld + '.settings.holder.incpaths',
									'valueType': 'includePath'})
						for i in option.includedir:
							self.add(doc, includes, 'listOptionValue', {'builtin': 'false', 'value': '${ProjDirPath}/'+i})
						count = count+1
						defines = self.add(doc, tool, 'option',
								{	'id': cdt_bld + '.settings.holder.symbols.%d'%count,
									'superClass': cdt_bld + '.settings.holder.symbols',
									'valueType': 'definedSymbols'})
						for i in option.defines.union(env.DEFINES+['__ECLIPSE']):
							self.add(doc, defines, 'listOptionValue', {'builtin': 'false', 'value': i})
						count = count+1
						defines = self.add(doc, tool, 'option',
								{	'id': cdt_bld + '.settings.holder.inType.%d'%count,
									'superClass': cdt_bld + '.settings.holder.inType'})

						input = self.add(doc, tool, 'inputType',
								{	'id': cdt_bld + '.settings.holder.inType.%d'%count,
									'superClass': cdt_bld + '.settings.holder.inType'})
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
		addTargetWrap('eclipse', False)

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

