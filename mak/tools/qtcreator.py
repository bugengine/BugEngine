#! /usr/bin/env python
# encoding: utf-8

import os
import sys
import string
from waflib import Context, Build
from minixml import XmlDocument, XmlNode

try:
	import hashlib
	createMd5=hashlib.md5
except:
	import md5
	createMd5=md5.new

def _hexdigest(s):
	"""Return a string as a string of hex characters.
	"""
	# NOTE:  This routine is a method in the Python 2.0 interface
	# of the native md5 module, but we want SCons to operate all
	# the way back to at least Python 1.5.2, which doesn't have it.
	h = string.hexdigits
	r = ''
	for c in s:
		try:
			i = ord(c)
		except:
			i = c
		r = r + h[(i >> 4) & 0xF] + h[i & 0xF]
	return r

def generateGUID(name):
	"""This generates a dummy GUID for the sln file to use.  It is
	based on the MD5 signatures of the sln filename plus the name of
	the project.  It basically just needs to be unique, and not
	change with each invocation."""
	d = _hexdigest(createMd5(str(name).encode()).digest()).upper()
	# convert most of the signature to GUID form (discard the rest)
	d = "{" + d[:8] + "-" + d[8:12] + "-" + d[12:16] + "-" + d[16:20] + "-" + d[20:32] + "}"
	return d


def unique(seq):
	seen = set()
	seen_add = seen.add
	return [x for x in seq if x not in seen and not seen_add(x)]

def path_from(path, base_node):
	if isinstance(path, str):
		return path
	else:
		return path.path_from(base_node)

class QtCreator(Build.BuildContext):
	cmd = 'qtcreator'
	fun = 'build'
	optim = 'debug'

	def execute(self):
		self.restore()
		if not self.all_envs:
			self.load_envs()
		self.env.PROJECTS=[self.__class__.cmd]
		self.env.PROJECTS=[self.__class__.cmd]
		self.env.VARIANT = '${Variant}'
		self.env.TOOLCHAIN = '${Toolchain}'
		self.env.PREFIX = '${Prefix}'
		self.env.DEPLOY_ROOTDIR = '${Deploy_RootDir}'
		self.env.DEPLOY_BINDIR = '${Deploy_BinDir}'
		self.env.DEPLOY_RUNBINDIR = '${Deploy_RunBinDir}'
		self.env.DEPLOY_LIBDIR = '${Deploy_LibDir}'
		self.env.DEPLOY_INCLUDEDIR = '${Deploy_IncludeDir}'
		self.env.DEPLOY_DATADIR = '${Deploy_DataDir}'
		self.env.DEPLOY_PLUGINDIR = '${Deploy_PluginDir}'
		self.env.DEPLOY_KERNELDIR = '${Deploy_KernelDir}'
		self.features = ['GUI']
		self.recurse([self.run_dir])

		appname = getattr(Context.g_module, Context.APPNAME, self.srcnode.name)
		self.base_node = self.srcnode.make_node('%s.qtcreator'%appname)
		self.base_node.mkdir()

		self.launcher = None
		for group in self.groups:
			for task_gen in group:
				if 'launcher' in task_gen.features:
					self.launcher = task_gen
		for group in self.groups:
			for task_gen in group:
				self.write_project(task_gen)
				self.write_files(task_gen)
				includes, defines = self.gather_includes_defines(task_gen)
				self.write_includes(task_gen, includes)
				self.write_defines(task_gen, defines)
				self.write_user(task_gen)

	def gather_includes_defines(self, task_gen):
		def gather_includes_defines_recursive(task_gen):
			try:
				return task_gen.bug_qtcreator_cache
			except AttributeError:
				includes = getattr(task_gen, 'export_includes', [])
				defines = getattr(task_gen, 'export_defines', [])
				includes = [path_from(i, self.base_node) for i in includes]
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
				task_gen.bug_qtcreator_cache = (includes, defines)
				return task_gen.bug_qtcreator_cache
		includes, defines = gather_includes_defines_recursive(task_gen)
		includes = includes + [path_from(i, self.base_node) for i in getattr(task_gen, 'includes', [])]
		defines = defines + getattr(task_gen, 'defines', [])
		return unique(includes), unique(defines)

	def write_project(self, task_gen):
		self.base_node.make_node('%s.creator'%task_gen.target).write('[General]')

	def write_files(self, task_gen):
		file_list = []
		for source_node in getattr(task_gen, 'source_nodes', []):
			try:
				file_list += [node.path_from(self.base_node) for node in source_node.ant_glob('**')]
			except Exception:
				pass
		self.base_node.make_node('%s.files'%task_gen.target).write('\n'.join(file_list))

	def write_includes(self, task_gen, includes):
		for toolchain in self.env.ALL_TOOLCHAINS:
			env = self.all_envs[toolchain]
			includes += env.INCLUDES + env.SYSTEM_INCLUDES
		self.base_node.make_node('%s.includes'%task_gen.target).write('\n'.join(includes))

	def write_defines(self, task_gen, defines):
		for toolchain in self.env.ALL_TOOLCHAINS:
			env = self.all_envs[toolchain]
			defines += env.DEFINES + env.SYSTEM_DEFINES
		self.base_node.make_node('%s.config'%task_gen.target).write('\n'.join(defines))

	def write_user(self, task_gen):
		def write_value(node, value, key=''):
			def convert_value(v):
				if isinstance(v, bool):
					return 'bool', v and 'true' or 'false'
				elif isinstance(v, float):
					return 'double', str(v)
				elif isinstance(v, int):
					return 'int', str(v)
				elif isinstance(v, bytes):
					return 'QByteArray', v.decode('utf-8')
				elif isinstance(v, str):
					return 'QString', v
				elif isinstance(v, tuple):
					return 'QVariantList', ''
				elif isinstance(v, list):
					return 'QVariantMap', ''
				else:
					raise Exception('unknown type in a map: %s'%type(v))
			type, strvalue = convert_value(value)
			attrs = [('type', type)]
			if key:
				attrs.append(('key', key))
			if isinstance(value, tuple):
				with XmlNode(node, 'valuelist', strvalue, attrs) as value_list:
					for v in value:
						write_value(value_list, v)
			elif isinstance(value, list):
				with XmlNode(node, 'valuemap', strvalue, attrs) as value_map:
					for key, v in value:
						write_value(value_map, v, key)
			else:
				XmlNode(node, 'value', strvalue, attrs).close()

		node = self.base_node.make_node('%s.creator.user' % task_gen.target)
		file = open(node.abspath(), 'w')
		with XmlDocument(file, 'UTF-8', [('DOCTYPE', 'QtCreatorProject')]) as project:
			with XmlNode(project, 'qtcreator') as qtcreator:
				with XmlNode(qtcreator, 'data') as data_node:
					XmlNode(data_node, 'variable', 'ProjectExplorer.Project.ActiveTarget').close()
					write_value(data_node, 0)
				with XmlNode(qtcreator, 'data') as data:
					XmlNode(data, 'variable', 'ProjectExplorer.Project.EditorSettings').close()
					write_value(data, [
						('EditorConfiguration.AutoIndent', True),
						('EditorConfiguration.AutoSpacesForTabs', False),
						('EditorConfiguration.CamelCaseNavigation', True),
						('EditorConfiguration.CodeStyle.0', [
							('language', 'Cpp'),
							('value', [('CurrentPreferences', b'CppGlobal')]),
						]),
						('EditorConfiguration.CodeStyle.1', [
							('language', 'QmlJS'),
							('value', [('CurrentPreferences', b'QmlJSGlobal')]),
						]),
						('EditorConfiguration.CodeStyle.Count', 2),
						('EditorConfiguration.Codec', b'UTF-8'),
						('EditorConfiguration.ConstrainToolTips', False),
						('EditorConfiguration.IndentSize',  4),
						('EditorConfiguration.KeyboardTooltips', False),
						('EditorConfiguration.MouseNavigation', True),
						('EditorConfiguration.PaddingMode', 1),
						('EditorConfiguration.ScrollWheelZooming', True),
						('EditorConfiguration.SmartBackspaceBehavior', 0),
						('EditorConfiguration.SpacesForTabs', True),
						('EditorConfiguration.TabKeyBehavior', 0),
						('EditorConfiguration.TabSize', 4),
						('EditorConfiguration.UseGlobal', True),
						('EditorConfiguration.Utf8BomBehavior', 1),
						('EditorConfiguration.addFinalNewLine', True),
						('EditorConfiguration.cleanIndentation', False),
						('EditorConfiguration.cleanWhitespace', True),
						('EditorConfiguration.inEntireDocument', False),
					])
				with XmlNode(qtcreator, 'data') as data:
					XmlNode(data, 'variable', 'ProjectExplorer.Project.PluginSettings').close()
					write_value(data, [])
				with XmlNode(qtcreator, 'data') as data:
					XmlNode(data, 'variable', 'ProjectExplorer.Project.Target.0').close()
					build_configurations = []
					build_configuration_index = 0
					for env_name in self.env.ALL_TOOLCHAINS:
						for variant in self.env.ALL_VARIANTS:
							bld_env = self.all_envs[env_name]
							if bld_env.SUB_TOOLCHAINS:
								env = self.all_envs[bld_env.SUB_TOOLCHAINS[0]]
							else:
								env = bld_env
							build_configurations.append((
								'ProjectExplorer.Target.BuildConfiguration.%d'%build_configuration_index,
								[
									('ProjectExplorer.BuildConfiguration.BuildDirectory', self.srcnode.abspath()),
									('ProjectExplorer.BuildConfiguration.BuildStepList.0', [
										('ProjectExplorer.BuildStepList.Step.0', [
											('ProjectExplorer.BuildStep.Enabled', True),
											('ProjectExplorer.ProcessStep.Arguments', 'waf install:%s:%s'%(env_name, variant)),
											('ProjectExplorer.ProcessStep.Command', sys.executable),
											('ProjectExplorer.ProcessStep.WorkingDirectory', '%{buildDir}'),
											('ProjectExplorer.ProjectConfiguration.DefaultDisplayName', 'Waf configuration'),
											('ProjectExplorer.ProjectConfiguration.DisplayName', ''),
											('ProjectExplorer.ProjectConfiguration.Id', 'ProjectExplorer.ProcessStep')
										]),
										('ProjectExplorer.BuildStepList.StepsCount', 1),
										('ProjectExplorer.ProjectConfiguration.DefaultDisplayName', 'Build'),
										('ProjectExplorer.ProjectConfiguration.DisplayName', ''),
										('ProjectExplorer.ProjectConfiguration.Id', 'ProjectExplorer.BuildSteps.Build')
									]),
									('ProjectExplorer.BuildConfiguration.BuildStepList.1', [
										('ProjectExplorer.BuildStepList.Step.0', [
											('ProjectExplorer.BuildStep.Enabled', True),
											('ProjectExplorer.ProcessStep.Arguments', 'waf clean:%s:%s'%(env_name, variant)),
											('ProjectExplorer.ProcessStep.Command', sys.executable),
											('ProjectExplorer.ProcessStep.WorkingDirectory', '%{buildDir}'),
											('ProjectExplorer.ProjectConfiguration.DefaultDisplayName', 'Waf configuration'),
											('ProjectExplorer.ProjectConfiguration.DisplayName', ''),
											('ProjectExplorer.ProjectConfiguration.Id', 'ProjectExplorer.ProcessStep')
										]),
										('ProjectExplorer.BuildStepList.StepsCount', 1),
										('ProjectExplorer.ProjectConfiguration.DefaultDisplayName', 'Clean'),
										('ProjectExplorer.ProjectConfiguration.DisplayName', ''),
										('ProjectExplorer.ProjectConfiguration.Id', 'ProjectExplorer.BuildSteps.Clean')
									]),
									('ProjectExplorer.BuildConfiguration.BuildStepListCount', 2),
									('ProjectExplorer.BuildConfiguration.ClearSystemEnvironment', False),
									('ProjectExplorer.BuildConfiguration.UserEnvironmentChanges', tuple(
										'%s=%s'%(var_name, env[var_name.upper()])
										for var_name in ('Toolchain', 'Prefix', 'Deploy_RootDir',
														 'Deploy_BinDir', 'Deploy_RunBinDir',
														 'Deploy_LibDir', 'Deploy_IncludeDir',
														 'Deploy_DataDir')
									) + ('OUT_NAME=%s' % os.path.join(
											self.srcnode.abspath(),
											bld_env.PREFIX, variant, env.DEPLOY_BINDIR,
											env.cxxprogram_PATTERN%self.launcher.target),)),
									('ProjectExplorer.ProjectConfiguration.DefaultDisplayName', 'Default'),
									('ProjectExplorer.ProjectConfiguration.DisplayName', '%s:%s'%(env_name, variant)),
									('ProjectExplorer.ProjectConfiguration.Id', 'GenericProjectManager.GenericBuildConfiguration')
								]))
							build_configuration_index += 1
					run_configurations = []
					if 'game' in task_gen.features:
						run_configurations.append((
							'ProjectExplorer.Target.RunConfiguration.0', [
							('Analyzer.Valgrind.AddedSuppressionFiles', ()),
							('Analyzer.Valgrind.Callgrind.CollectBusEvents', False),
							('Analyzer.Valgrind.Callgrind.CollectSystime', False),
							('Analyzer.Valgrind.Callgrind.EnableBranchSim', False),
							('Analyzer.Valgrind.Callgrind.EnableCacheSim', False),
							('Analyzer.Valgrind.Callgrind.EnableEventTooltips', True),
							('Analyzer.Valgrind.Callgrind.MinimumCastRatio', 0.01),
							('Analyzer.Valgrind.Callgrind.VisualisationMinimumCostRatio', 10),
							('Analyzer.Valgrind.FilterExternalIssues', True),
							('Analyzer.Valgrind.LeakCheckOnFinish', 1),
							('Analyzer.Valgrind.NumCallers', 32),
							('Analyzer.Valgrind.RemovedSuppressionFiles', ()),
							('Analyzer.Valgrind.SelfModifyingCodeDetection', 1),
							('Analyzer.Valgrind.Settings.UseGlobalSettings', True),
							('Analyzer.Valgrind.ShowReachable', False),
							('Analyzer.Valgrind.TrackOrigins', True),
							('Analyzer.Valgrind.ValgrindExecutable', 'valgrind'),
							('Analyzer.Valgrind.VisibleErrorKinds', (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14)),
							('PE.EnvironmentAspect.Base', 2),
							('PE.EnvironmentAspect.Changes', ()),
							('ProjectExplorer.CustomExecutableRunConfiguration.Arguments', task_gen.target),
							('ProjectExplorer.CustomExecutableRunConfiguration.Executable', '${OUT_NAME}'),
							('ProjectExplorer.CustomExecutableRunConfiguration.UseTerminal', False),
							('ProjectExplorer.CustomExecutableRunConfiguration.WorkingDirectory', '${buildDir}'),
							('ProjectExplorer.ProjectConfiguration.DefaultDisplayName', 'Run %s' % task_gen.target),
							('ProjectExplorer.ProjectConfiguration.DisplayName', ''),
							('ProjectExplorer.ProjectConfiguration.Id', 'ProjectExplorer.CustomExecutableRunConfiguration'),
							('RunConfiguration.QmlDebugServerPort', 3768),
							('RunConfiguration.UseCppDebugger', True),
							('RunConfiguration.UseCppDebuggerAuto', False),
							('RunConfiguration.UseMultiProcess', False),
							('RunConfiguration.UseQmlDebugger', False),
							('RunConfiguration.UseQmlDebuggerAuto', True),
						]))


					write_value(data, [
						('ProjectExplorer.ProjectConfiguration.DefaultDisplayName', 'Desktop'),
						('ProjectExplorer.ProjectConfiguration.DisplayName', 'Desktop'),
						('ProjectExplorer.ProjectConfiguration.Id', '{fd719de2-edaa-4f39-af33-6b89a122b21a}'),
						('ProjectExplorer.Target.ActiveBuildConfiguration', 0),
						('ProjectExplorer.Target.ActiveDeployConfiguration', 0),
						('ProjectExplorer.Target.ActiveRunConfiguration', 0),
					] + build_configurations + [
						('ProjectExplorer.Target.BuildConfigurationCount', len(build_configurations)),
						('ProjectExplorer.Target.DeployConfiguration', []),
						('ProjectExplorer.Target.DeployConfigurationCount', 0),
						('ProjectExplorer.Target.PluginSettings', [])
					] + run_configurations + [
						('ProjectExplorer.Target.RunConfigurationCount', len(run_configurations)),
					])
				with XmlNode(qtcreator, 'data') as data:
					XmlNode(data, 'variable', 'ProjectExplorer.Project.TargetCount').close()
					write_value(data, 1)
				with XmlNode(qtcreator, 'data') as data:
					XmlNode(data, 'variable', 'ProjectExplorer.Project.Updater.EnvironmentId').close()
					write_value(data, b'{9807fb0e-3785-4641-a197-bb1a10ccc985}')
				with XmlNode(qtcreator, 'data') as data:
					XmlNode(data, 'variable', 'ProjectExplorer.Project.Updater.FileVersion').close()
					write_value(data, 15)

