#! /usr/bin/env python
# encoding: utf-8

from waflib.TaskGen import feature
from waflib.Configure import conf
from waflib import TaskGen, Context, Build
import os, sys
from minixml import XmlNode, XmlDocument

oe_cdt = 'org.eclipse.cdt'
cdt_mk = oe_cdt + '.make.core'
cdt_core = oe_cdt + '.core'
cdt_bld = oe_cdt + '.build.core'

def unique(seq):
	seen = set()
	seen_add = seen.add
	return [x for x in seq if x not in seen and not seen_add(x)]

def path_from(path, task_gen, appname):
	if isinstance(path, str):
		return path
	else:
		for node in task_gen.source_nodes:
			if path.is_child_of(node):
				return '${workspace_loc:/%s/%s/%s}' % (appname, task_gen.name.replace('.', '/'), path.path_from(node).replace('\\', '/'))
		else:
			return node.abspath()


def gather_includes_defines(task_gen, appname):
	def gather_includes_defines_recursive(task_gen):
		try:
			return task_gen.bug_eclipse_cache
		except AttributeError:
			includes = getattr(task_gen, 'export_includes', [])
			defines = getattr(task_gen, 'export_defines', [])
			includes = [path_from(i, task_gen, appname) for i in includes]
			use = getattr(task_gen, 'use', [])
			for task_name in use:
				try:
					t = task_gen.bld.get_tgen_by_name(task_name)
				except:
					pass
				else:
					use_includes, use_defines = gather_includes_defines_recursive(t)
					includes = includes+use_includes
					defines = defines+use_defines
			task_gen.bug_eclipse_cache = (includes, defines)
			return task_gen.bug_eclipse_cache
	includes, defines = gather_includes_defines_recursive(task_gen)
	includes = includes + [path_from(i, task_gen, appname) for i in getattr(task_gen, 'includes', [])]
	defines = defines + getattr(task_gen, 'defines', [])
	return unique(includes), unique(defines)

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
		self.env.VARIANT = '$(Variant)'
		self.env.TOOLCHAIN = '$(Toolchain)'
		self.env.PREFIX = '$(Prefix)'
		self.env.DEPLOY_ROOTDIR = '$(Deploy_RootDir)'
		self.env.DEPLOY_BINDIR = '$(Deploy_BinDir)'
		self.env.DEPLOY_RUNBINDIR = '$(Deploy_RunBinDir)'
		self.env.DEPLOY_LIBDIR = '$(Deploy_LibDir)'
		self.env.DEPLOY_INCLUDEDIR = '$(Deploy_IncludeDir)'
		self.env.DEPLOY_DATADIR = '$(Deploy_DataDir)'
		self.env.DEPLOY_PLUGINDIR = '$(Deploy_PluginDir)'
		self.env.DEPLOY_KERNELDIR = '$(Deploy_KernelDir)'
		self.features = ['GUI']
		self.recurse([self.run_dir])

		appname = getattr(Context.g_module, Context.APPNAME, self.srcnode.name)
		self.create_cproject(appname)


		#settings = self.srcnode.find_or_declare('mak/eclipse/.settings/')
		#out = self.srcnode.make_node('.settings')
		#out.mkdir()
		#for f in settings.listdir():
		#	n = settings.find_or_declare(f)
		#	out.make_node(f).write(n.read())


	def create_cproject(self, appname):
		"""
		Create the Eclipse CDT .project and .cproject files
		@param appname The name that will appear in the Project Explorer
		@param build The BuildContext object to extract includes from
		@param workspace_includes Optional project includes to prevent
			"Unresolved Inclusion" errors in the Eclipse editor
		"""
		self.impl_create_project(self.srcnode.make_node('.project'), sys.executable, sys.argv[0], appname)
		self.impl_create_cproject(self.srcnode.make_node('.cproject'), sys.executable, sys.argv[0], appname)
		self.impl_create_pydevproject(self.srcnode.make_node('.pydevproject'), appname, sys.path, [])

	def impl_create_project(self, node, executable, waf, appname):
		with XmlDocument(open(node.abspath(), 'w'), 'UTF-8') as doc:
			with XmlNode(doc, 'projectDescription') as projectDescription:
				XmlNode(projectDescription, 'name', appname).close()
				XmlNode(projectDescription, 'comment').close()
				XmlNode(projectDescription, 'projects').close()
				with XmlNode(projectDescription, 'buildSpec') as buildSpec :
					with XmlNode(buildSpec, 'buildCommand') as buildCommand:
						XmlNode(buildCommand, 'name', oe_cdt + '.managedbuilder.core.genmakebuilder').close()
						XmlNode(buildCommand, 'triggers', 'clean,full,incremental,').close()
						XmlNode(buildCommand, 'arguments').close()
				with XmlNode(projectDescription, 'natures') as natures:
					nature_list = """
						core.ccnature
						managedbuilder.core.ScannerConfigNature
						managedbuilder.core.managedBuildNature
						core.cnature
						org.python.pydev.pythonNature
					""".split()
					for n in nature_list:
						XmlNode(natures, 'nature', oe_cdt + '.' + n).close()
				with XmlNode(projectDescription, 'linkedResources') as resources:
					def createProjectFolder(name, element, seen):
						name = name.split('.')
						path = ''
						for n in name:
							path = path + '/' + n
							if path not in seen:
								seen.add(path)
								with XmlNode(element, 'link') as link:
									XmlNode(link, 'name', path).close()
									XmlNode(link, 'type', '2')
									XmlNode(link, 'locationURI', 'virtual:/virtual')
						return path[1:]
					seen = set([])
					for g in self.groups:
						for tg in g:
							if not isinstance(tg, TaskGen.task_gen):
								continue
							name = createProjectFolder(tg.name, resources, seen)
							for node in getattr(tg, 'source_nodes', []):
								self.addSourceTree(resources, node, name, os.path.join('PROJECT_LOC', node.path_from(self.srcnode)))

				with XmlNode(projectDescription, 'filteredResources') as filters:
					with XmlNode(filters, 'filter') as filter:
						XmlNode(filter, 'id', '1321206201691').close()
						XmlNode(filter, 'name').close()
						XmlNode(filter, 'type', '30').close()
						with XmlNode(filter, 'matcher') as matcher:
							XmlNode(matcher, 'id', 'org.eclipse.ui.ide.multiFilter').close()
							XmlNode(matcher, 'arguments', '1.0-name-matches-false-false-*').close()

	def addSourceTree(self, element, node, folder, path):
		abspath = node.abspath()
		if os.path.isdir(abspath):
			with XmlNode(element, 'link') as link:
				XmlNode(link, 'name', folder).close()
				XmlNode(link, 'type', '2').close()
				XmlNode(link, 'locationURI', 'virtual:/virtual').close()
			for n in node.listdir():
				self.addSourceTree(element, node.make_node(n), folder+'/'+n, os.path.join(path, n))
		else:
			with XmlNode(element, 'link') as link:
				XmlNode(link, 'name', folder).close()
				XmlNode(link, 'type', '1').close()
				XmlNode(link, 'locationURI', path.replace('\\', '/')).close()




	def impl_create_cproject(self, node, executable, waf, appname):
		source_dirs = []
		with XmlDocument(open(node.abspath(), 'w'), 'UTF-8', [('fileVersion', '4.0.0')]) as doc:
			with XmlNode(doc, 'cproject') as cproject:
				count = 0
				with XmlNode(cproject, 'storageModule',	{'moduleId': cdt_core + '.settings'}) as rootStorageModule:
					for toolchain in self.env.ALL_TOOLCHAINS:
						for variant in self.env.ALL_VARIANTS:
							env = self.all_envs['%s-%s'%(toolchain, variant)]
							count = count+1
							cconf_id = cdt_core + '.default.config.%d'%count
							with XmlNode(rootStorageModule, 'cconfiguration', {'id':cconf_id}) as cconf:
								with XmlNode(cconf, 'storageModule',
										{'buildSystemId': oe_cdt + '.managedbuilder.core.configurationDataProvider',
										'id': cconf_id,
										'moduleId': cdt_core + '.settings',
										'name': toolchain+':'+variant}) as storageModule:
									XmlNode(storageModule, 'externalSettings').close()
									with XmlNode(storageModule, 'extensions') as extensions:
										extension_list = """
											VCErrorParser
											GmakeErrorParser
											GCCErrorParser
											GASErrorParser
											GLDErrorParser
											CWDLocator
										""".split()
										XmlNode(extensions, 'extension',
													{'id': cdt_core + '.ELF', 'point':cdt_core + '.BinaryParser'}).close()
										XmlNode(extensions, 'extension',
													{'id': cdt_core + '.PE', 'point':cdt_core + '.BinaryParser'}).close()
										XmlNode(extensions, 'extension',
													{'id': cdt_core + '.MachO64', 'point':cdt_core + '.BinaryParser'}).close()
										for e in extension_list:
											XmlNode(extensions, 'extension',
													{'id': cdt_core + '.' + e, 'point':cdt_core + '.ErrorParser'}).close()

								with XmlNode(cconf, 'storageModule', {'moduleId': 'cdtBuildSystem', 'version': '4.0.0'}) as storageModule:
									with XmlNode(storageModule, 'configuration',
												{	'artifactName': appname,
													'buildProperties': '',
													'description': '',
													'id': cconf_id,
													'name': toolchain+':'+variant,
													'parent': cdt_bld + '.prefbase.cfg'}) as config:
										count = count+1
										with XmlNode(config, 'folderInfo',
															{'id': cconf_id+'.%d'%count, 'resourcePath': '/', 'name': ''}) as folderInfo:
											count = count+1
											with XmlNode(folderInfo, 'toolChain',
													{	'id': cdt_bld + '.prefbase.toolchain.%d'%count,
														'name': 'No ToolChain',
														'resourceTypeBasedDiscovery': 'false',
														'superClass': cdt_bld + '.prefbase.toolchain'}) as toolChain:
												count = count+1
												XmlNode(toolChain, 'targetPlatform',
														{	'binaryParser': 'org.eclipse.cdt.core.ELF;org.eclipse.cdt.core.MachO64;org.eclipse.cdt.core.PE',
															'id': cdt_bld + '.prefbase.toolchain.%d'%count, 'name': ''}).close()
												waf_build = '"%s" install:%s:%s'%(waf, toolchain, variant)
												waf_clean = '"%s" clean:%s:%s'%(waf, toolchain, variant)
												count = count+1
												XmlNode(toolChain, 'builder',
																{	'autoBuildTarget': waf_build,
																	'command': executable,
																	'enableAutoBuild': 'false',
																	'cleanBuildTarget': waf_clean,
																	'id': cdt_bld + '.settings.default.builder.%d'%count,
																	'incrementalBuildTarget': waf_build,
																	'keepEnvironmentInBuildfile': 'false',
																	'managedBuildOn': 'false',
																	'name': 'Gnu Make Builder',
																	'superClass': cdt_bld + '.settings.default.builder'}).close()
												count = count+1
												XmlNode(toolChain, 'tool',
																{	'id': cdt_bld + '.settings.holder.libs.%d'%count,
																	'name': "holder for library settings",
																	'superClass': cdt_bld + '.settings.holder.libs'}).close()
												for tool_name in ("Assembly", "GNU C++", "GNU C"):
													count = count+1
													with XmlNode(toolChain, 'tool',
																				{	'id': cdt_bld + '.settings.holder.%d'%count,
																					'name': tool_name,
																					'superClass': cdt_bld + '.settings.holder'}) as tool:
														count = count+1
														with XmlNode(tool, 'option',
																	{	'id': cdt_bld + '.settings.holder.incpaths.%d'%count,
																		'superClass': cdt_bld + '.settings.holder.incpaths',
																		'valueType': 'includePath'}) as includes:
															for i in env.INCLUDES + ['%s/usr/include'%sysroot for sysroot in env.SYSROOT] + env.SYSTEM_INCLUDES:
																XmlNode(includes, 'listOptionValue', {'builtin': 'true', 'value': i}).close()
														count = count+1
														with XmlNode(tool, 'option',
																	{	'id': cdt_bld + '.settings.holder.symbols.%d'%count,
																		'superClass': cdt_bld + '.settings.holder.symbols',
																		'valueType': 'definedSymbols'}) as defines:
															for i in env.DEFINES:
																XmlNode(defines, 'listOptionValue', {'builtin': 'false', 'value': i}).close()
														count = count+1
														XmlNode(tool, 'inputType',
																{	'id': cdt_bld + '.settings.holder.inType.%d'%count,
																	'superClass': cdt_bld + '.settings.holder.inType'}).close()
											count = count+1
										for g in self.groups:
											for tg in g:
												if not isinstance(tg, TaskGen.task_gen):
													continue
												task_includes, task_defines = gather_includes_defines(tg, appname)
												with XmlNode(config, 'folderInfo',
																	{'id': cconf_id+'.%d'%count, 'resourcePath': '/' + tg.name.replace('.', '/'), 'name': ''}) as folderInfo:
													count = count+1
													with XmlNode(folderInfo, 'toolChain',
																	{	'id': cdt_bld + '.prefbase.toolchain.%d'%count,
																		'name': 'No ToolChain',
																		'unusedChidlren': '',
																		'superClass': cdt_bld + '.prefbase.toolchain'}) as toolChain:
														count = count+1
														XmlNode(toolChain, 'tool',
																			{	'id': cdt_bld + '.settings.holder.libs.%d'%count,
																				'name': "holder for library settings",
																				'superClass': cdt_bld + '.settings.holder.libs'}).close()
														for tool_name in ("Assembly", "GNU C++", "GNU C"):
															count = count+1
															with XmlNode(toolChain, 'tool',
																						{	'id': cdt_bld + '.settings.holder.%d'%count,
																							'name': tool_name,
																							'superClass': cdt_bld + '.settings.holder'}) as tool:
																count = count+1
																with XmlNode(tool, 'option',
																			{	'id': cdt_bld + '.settings.holder.incpaths.%d'%count,
																				'superClass': cdt_bld + '.settings.holder.incpaths',
																				'valueType': 'includePath'}) as includes:
																	pass
																	#for i in task_includes:
																	#	XmlNode(includes, 'listOptionValue', {'builtin': 'false', 'value': i}).close()
																count = count+1
																with XmlNode(tool, 'option',
																			{	'id': cdt_bld + '.settings.holder.symbols.%d'%count,
																				'superClass': cdt_bld + '.settings.holder.symbols',
																				'valueType': 'definedSymbols'}) as defines:
																	for i in task_defines:
																		XmlNode(defines, 'listOptionValue', {'builtin': 'false', 'value': i}).close()
																count = count+1
																XmlNode(tool, 'inputType',
																		{	'id': cdt_bld + '.settings.holder.inType.%d'%count,
																			'superClass': cdt_bld + '.settings.holder.inType'}).close()
								XmlNode(cconf, 'storageModule', {'moduleId': 'org.eclipse.cdt.core.externalSettings'}).close()

				with XmlNode(cproject, 'storageModule',
									{	'moduleId': 'cdtBuildSystem',
										'version': '4.0.0'}) as storageModule:
					XmlNode(storageModule, 'project', {'id': '%s.null.0'%appname, 'name': appname}).close()

				with XmlNode(cproject, 'storageModule', {'moduleId': cdt_mk + '.buildtargets'}) as storageModule:
					with XmlNode(storageModule, 'buildTargets') as buildTargets:
						def addTargetWrap(name, runAll):
							return self.addTarget(buildTargets, executable, name, '"%s" %s'%(waf, name), runAll)
						addTargetWrap('reconfigure', True)
						addTargetWrap('eclipse', False)

	def impl_create_pydevproject(self, node, appname, system_path, user_path):
		# create a pydevproject file
		with XmlDocument(open(node.abspath(), 'w'), 'UTF-8', [('eclipse-pydev', 'version="1.0"')]) as doc:
			with XmlNode(doc, 'pydev_project') as pydevproject:
				XmlNode(pydevproject, 'pydev_property',
						'python %d.%d'%(sys.version_info[0], sys.version_info[1]), {'name': 'org.python.pydev.PYTHON_PROJECT_VERSION'}).close()
				XmlNode(pydevproject, 'pydev_property', 'Default', {'name': 'org.python.pydev.PYTHON_PROJECT_INTERPRETER'}).close()
				# add waf's paths
				wafadmin = [p for p in system_path if p.find('wafadmin') != -1]
				if wafadmin:
					with XmlNode(pydevproject, 'pydev_pathproperty',
							{'name':'org.python.pydev.PROJECT_EXTERNAL_SOURCE_PATH'}) as prop:
						for i in wafadmin:
							XmlNode(prop, 'path', i).close()
				if user_path:
					with XmlNode(pydevproject, 'pydev_pathproperty',
							{'name':'org.python.pydev.PROJECT_SOURCE_PATH'}) as prop:
						for i in user_path:
							XmlNode(prop, 'path', '/'+appname+'/'+i).close()


	def addTarget(self, buildTargets, executable, name, buildTarget, runAllBuilders=True):
		with XmlNode(buildTargets, 'target',
						{	'name': name,
							'path': '',
							'targetID': oe_cdt + '.build.MakeTargetBuilder'}) as target:
			XmlNode(target, 'buildCommand', executable).close()
			XmlNode(target, 'buildArguments', None).close()
			XmlNode(target, 'buildTarget', buildTarget).close()
			XmlNode(target, 'stopOnError', 'true').close()
			XmlNode(target, 'useDefaultCommand', 'false').close()
			XmlNode(target, 'runAllBuilders', str(runAllBuilders).lower()).close()

