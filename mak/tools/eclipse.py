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
		for node in getattr(task_gen, 'source_nodes', []):
			if path.is_child_of(node):
				return '/%s/%s/%s' % (appname, task_gen.name.replace('.', '/'), path.path_from(node).replace('\\', '/'))
		else:
			return path.abspath()


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
	optim = 'debug'

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

		self.launch = []
		for g in self.groups:
			for tg in g:
				if 'launcher' in tg.features:
					self.launcher = tg
					self.launch.append(tg)
				if 'game' in tg.features:
					self.launch.append(tg)

		self.settings = self.srcnode.make_node('.settings')
		setting_files = []
		self.settings.mkdir()
		settings = self.srcnode.find_or_declare('mak/tools/eclipse')
		for f in settings.listdir():
			n = settings.find_or_declare(f)
			self.settings.make_node(f).write(n.read())
			setting_files.append(f)

		self.create_cproject(appname, setting_files)

		for s in self.settings.listdir():
			if s not in setting_files:
				file = self.settings.make_node(s)
				file.delete()


	def create_cproject(self, appname, setting_files):
		"""
		Create the Eclipse CDT .project and .cproject files
		@param appname The name that will appear in the Project Explorer
		@param build The BuildContext object to extract includes from
		@param workspace_includes Optional project includes to prevent
			"Unresolved Inclusion" errors in the Eclipse editor
		"""
		self.impl_create_project(self.srcnode.make_node('.project'), sys.executable, sys.argv[0], appname)
		self.impl_create_cproject(self.srcnode.make_node('.cproject'), sys.executable, sys.argv[0], appname, setting_files)
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
						com.android.ide.eclipse.adt.AndroidNature
						core.ccnature
						managedbuilder.core.ScannerConfigNature
						managedbuilder.core.managedBuildNature
						core.cnature
						org.python.pydev.pythonNature
					""".split()
					for n in nature_list:
						XmlNode(natures, 'nature', oe_cdt + '.' + n).close()
				with XmlNode(projectDescription, 'linkedResources') as resources:
					self.addSourceTree(resources, self.settings, '.settings', os.path.join('PROJECT_LOC', self.settings.path_from(self.srcnode)))
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
							if not 'kernel' in tg.features:
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


	def impl_create_cproject(self, node, executable, waf, appname, setting_files):
		source_dirs = []

		setting_node = self.settings.make_node('language.settings.xml')
		setting_files.append(setting_node.name)
		setting = XmlDocument(open(setting_node.abspath(), 'w'), 'UTF-8', [('fileVersion', '4.0.0')])
		cproject_setting = XmlNode(setting, 'project')

		with XmlDocument(open(node.abspath(), 'w'), 'UTF-8', [('fileVersion', '4.0.0')]) as doc:
			with XmlNode(doc, 'cproject') as cproject:
				count = 0
				with XmlNode(cproject, 'storageModule',	{'moduleId': cdt_core + '.settings'}) as rootStorageModule:
					for toolchain in self.env.ALL_TOOLCHAINS:
						env = self.all_envs[toolchain]
						if env.SUB_TOOLCHAINS:
							sub_env = self.all_envs[env.SUB_TOOLCHAINS[0]]
						for variant in self.env.ALL_VARIANTS:
							count = count+1
							cconf_id = cdt_core + '.default.config.%d'%count


							for launch_tg in self.launch:
								node = self.settings.make_node('%s-%s-%s.launch' % (launch_tg.target, toolchain, variant))
								setting_files.append(node.name)
								program = os.path.join('${project_loc}', env.PREFIX, 'debug', env.DEPLOY_BINDIR, sub_env.cxxprogram_PATTERN % self.launcher.target)
								argument = launch_tg.target if self.launcher != launch_tg else ''
								with XmlDocument(open(node.abspath(), 'w'), 'UTF-8') as doc:
									with XmlNode(doc, 'launchConfiguration', {'type':'org.eclipse.cdt.launch.applicationLaunchType'}) as launchConfig:
										XmlNode(launchConfig, 'booleanAttribute', {'key': 'org.eclipse.cdt.dsf.gdb.AUTO_SOLIB', 'value': 'true'}).close()
										with XmlNode(launchConfig, 'listAttribute', {'key': 'org.eclipse.cdt.dsf.gdb.AUTO_SOLIB_LIST'}) as soLibList:
											pass
										XmlNode(launchConfig, 'stringAttribute', {'key':'org.eclipse.cdt.dsf.gdb.DEBUG_NAME', 'value': env.GDB or 'gdb'}).close()
										XmlNode(launchConfig, 'booleanAttribute', {'key':'org.eclipse.cdt.dsf.gdb.DEBUG_ON_FORK', 'value': 'false'}).close()
										XmlNode(launchConfig, 'stringAttribute', {'key':'org.eclipse.cdt.dsf.gdb.GDB_INIT', 'value': '.gdbinit'}).close()
										XmlNode(launchConfig, 'booleanAttribute', {'key':'org.eclipse.cdt.dsf.gdb.NON_STOP', 'value': 'false'}).close()
										XmlNode(launchConfig, 'booleanAttribute', {'key':'org.eclipse.cdt.dsf.gdb.REVERSE', 'value': 'false'}).close()
										XmlNode(launchConfig, 'listAttribute', {'key':'org.eclipse.cdt.dsf.gdb.SOLIB_PATH'}).close()
										XmlNode(launchConfig, 'stringAttribute', {'key':'org.eclipse.cdt.dsf.gdb.TRACEPOINT_MODE', 'value': 'TP_NORMAL_ONLY'}).close()
										XmlNode(launchConfig, 'booleanAttribute', {'key':'org.eclipse.cdt.dsf.gdb.UPDATE_THREADLIST_ON_SUSPEND', 'value': 'false'}).close()
										XmlNode(launchConfig, 'booleanAttribute', {'key':'org.eclipse.cdt.dsf.gdb.internal.ui.launching.LocalApplicationCDebuggerTab.DEFAULTS_SET', 'value': 'true'}).close()
										XmlNode(launchConfig, 'intAttribute', {'key':'org.eclipse.cdt.launch.ATTR_BUILD_BEFORE_LAUNCH_ATTR', 'value': '2'}).close()
										XmlNode(launchConfig, 'stringAttribute', {'key':'org.eclipse.cdt.launch.COREFILE_PATH', 'value': ''}).close()
										XmlNode(launchConfig, 'stringAttribute', {'key':'org.eclipse.cdt.launch.DEBUGGER_ID', 'value': 'gdb'}).close()
										XmlNode(launchConfig, 'stringAttribute', {'key':'org.eclipse.cdt.launch.DEBUGGER_START_MODE', 'value': 'run'}).close()
										XmlNode(launchConfig, 'booleanAttribute', {'key':'org.eclipse.cdt.launch.DEBUGGER_STOP_AT_MAIN', 'value': 'true'}).close()
										XmlNode(launchConfig, 'stringAttribute', {'key':'org.eclipse.cdt.launch.DEBUGGER_STOP_AT_MAIN_SYMBOL', 'value': 'main'}).close()
										XmlNode(launchConfig, 'stringAttribute', {'key':'org.eclipse.cdt.launch.PROGRAM_ARGUMENTS', 'value': argument}).close()
										XmlNode(launchConfig, 'stringAttribute', {'key':'org.eclipse.cdt.launch.PROGRAM_NAME', 'value': program}).close()
										XmlNode(launchConfig, 'stringAttribute', {'key':'org.eclipse.cdt.launch.PROJECT_ATTR', 'value': 'BugEngine'}).close()
										XmlNode(launchConfig, 'booleanAttribute', {'key':'org.eclipse.cdt.launch.PROJECT_BUILD_CONFIG_AUTO_ATTR', 'value': 'false'}).close()
										XmlNode(launchConfig, 'stringAttribute', {'key':'org.eclipse.cdt.launch.PROJECT_BUILD_CONFIG_ID_ATTR', 'value': cconf_id}).close()
										with XmlNode(launchConfig, 'listAttribute', {'key':'org.eclipse.debug.core.MAPPED_RESOURCE_PATHS'}) as resourcePaths:
											XmlNode(resourcePaths, 'listEntry', {'value': '/BugEngine'}).close()
										with XmlNode(launchConfig, 'listAttribute', {'key':'org.eclipse.debug.core.MAPPED_RESOURCE_TYPES'}) as resourceTypes:
											XmlNode(resourceTypes, 'listEntry', {'value': '4'}).close()
										with XmlNode(launchConfig, 'listAttribute', {'key':'org.eclipse.debug.ui.favoriteGroups'}) as resourceTypes:
											XmlNode(resourceTypes, 'listEntry', {'value': 'org.eclipse.debug.ui.launchGroup.profile'}).close()
											XmlNode(resourceTypes, 'listEntry', {'value': 'org.eclipse.debug.ui.launchGroup.debug'}).close()
											XmlNode(resourceTypes, 'listEntry', {'value': 'org.eclipse.debug.ui.launchGroup.run'}).close()


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
														'name': 'BugEngine',
														'resourceTypeBasedDiscovery': 'false',
														'superClass': 'cdt.managedbuild.toolchain.gnu.base',
														'unusedChildren': 'cdt.managedbuild.tool.gnu.c.linker.base;cdt.managedbuild.tool.gnu.archiver.base;cdt.managedbuild.tool.gnu.cpp.linker.base;cdt.managedbuild.tool.gnu.assembler.base'}) as toolChain:
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
												for tool_name, id_name in (('GCC C Compiler', 'c'), ('GCC C++ Compiler', 'cpp')):
													count = count+1
													XmlNode(toolChain, 'tool',
																	{	'id': 'cdt.managedbuild.tool.gnu.%s.compiler.base.%d'%(id_name, count),
																		'name': tool_name,
																		'superClass': 'cdt.managedbuild.tool.gnu.%s.compiler.base'%id_name}).close()
							XmlNode(cconf, 'storageModule', {'moduleId': 'org.eclipse.cdt.core.externalSettings'}).close()
						with XmlNode(cproject_setting, 'configuration', {'id':cconf_id, 'name':toolchain+':'+variant}) as cconf_setting:
							with XmlNode(cconf_setting, 'extension', {'point': 'org.eclipse.cdt.core.LanguageSettingsProvider'}) as extension:
								XmlNode(extension, 'provider-reference', {'id': 'org.eclipse.cdt.managedbuilder.core.MBSLanguageSettingsProviderorg.eclipse.cdt.managedbuilder.core.MBSLanguageSettingsProvider', 'ref': 'shared-provider'}).close()
								with XmlNode(extension, 'provider', {'class': 'org.eclipse.cdt.core.language.settings.providers.LanguageSettingsGenericProvider',
																	'id': 'org.eclipse.cdt.ui.UserLanguageSettingsProvider',
																	'name': 'CDT User Setting Entries',
																	'prefer-non-shared': 'true',
																	'store-entries-with-project': 'true'}) as provider:
									with XmlNode(provider, 'language', {'id': 'org.eclipse.cdt.core.g++'}) as language:
										for g in self.groups:
											for tg in g:
												if not isinstance(tg, TaskGen.task_gen):
													continue
												if 'kernel' in tg.features:
													continue
												task_includes, task_defines = gather_includes_defines(tg, appname)
												with XmlNode(language, 'resource', {'project-relative-path': '/' + tg.name.replace('.', '/')}) as resource:
													for include in env.INCLUDES + ['%s/usr/include'%sysroot for sysroot in env.SYSROOT] + env.SYSTEM_INCLUDES + task_includes:
														with XmlNode(resource, 'entry', {'kind': 'includePath', 'name': include}) as entry:
															XmlNode(entry, 'flag', {'value': 'BUILTIN'}).close()
													for d in task_defines + env.DEFINES + env.SYSTEM_DEFINES:
														try:
															define, value = d.split('=')
														except:
															define = d
															value = ''
														with XmlNode(resource, 'entry', {'kind': 'macro', 'name': define, 'value': value}) as entry:
															XmlNode(entry, 'flag', {'value': 'BUILTIN'}).close()

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

		cproject_setting.close()
		setting.close()


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

