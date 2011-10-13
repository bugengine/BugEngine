#! /usr/bin/env python
# encoding: utf-8

from waflib.TaskGen import feature
from waflib.Configure import conf
from waflib import Task, Context
import os, sys
import mak
import xml
from xml.dom.minidom import Document


def create_project(self, appname):
	project = self.create_creator(appname)
	self.outputs[0].write(project)
	project = self.create_creator_user(appname)
	self.outputs[1].write(project.toxml())
	project = self.create_files(appname)
	self.outputs[2].write(project)
	project = self.create_includes(appname)
	self.outputs[3].write(project)
	project = self.create_config(appname)
	self.outputs[4].write(project)

def create_creator(self, appname):
	return "[general]"

try:
	w64 = os.environ['PROGRAMW6432']
	cdb = "mak/win32/bin/cdb64.exe"
except KeyError:
	cdb = "mak/win32/bin/cdb.exe"

def create_creator_user(self, appname):
	imp= xml.dom.minidom.getDOMImplementation('')
	dt= imp.createDocumentType('QtCreatorProject', '', '')
	doc = imp.createDocument('qtcreator', 'qtcreator', dt)
	qtproject = doc.documentElement

	data = self.add(doc, qtproject, 'data')
	self.add(doc, data, 'variable', 'ProjectExplorer.Project.ActiveTarget')
	self.add(doc, data, 'value', '0', {'type':'int'})

	data = self.add(doc, qtproject, 'data')
	self.add(doc, data, 'variable', 'ProjectExplorer.Project.EditorSetting')
	map = self.add(doc, data, 'valuemap', '', {'type':'QVariantMap'})

	targetcount = 0
	for toolchainName in self.env['BUILD_VARIANTS']+[i for i in self.env['ALL_VARIANTS'] if i not in self.env['BUILD_VARIANTS']]:
		env = self.bld.all_envs[toolchainName]
		data = self.add(doc, qtproject, 'data')
		self.add(doc, data, 'variable', 'ProjectExplorer.Project.Target.%d' % targetcount)
		target = self.add(doc, data, 'valuemap', '', {'type':'QVariantMap'})
		self.add(doc, target, 'value', '%s'%toolchainName, { 'key':'ProjectExplorer.ProjectConfiguration.DefaultDisplayName', 'type':'QString' })
		self.add(doc, target, 'value', '', { 'key':'ProjectExplorer.ProjectConfiguration.DisplayName', 'type':'QString' })
		self.add(doc, target, 'value', 'GenericProjectManager.GenericTarget', { 'key':'ProjectExplorer.ProjectConfiguration.Id', 'type':'QString' })
		self.add(doc, target, 'value', '0', { 'key':'ProjectExplorer.Target.ActiveBuildConfiguration', 'type':'int' })
		self.add(doc, target, 'value', '0', { 'key':'ProjectExplorer.Target.ActiveDeployConfiguration', 'type':'int' })
		self.add(doc, target, 'value', '0', { 'key':'ProjectExplorer.Target.ActiveRunConfiguration', 'type':'int' })

		map = self.add(doc, target, 'valuemap', '', {'type':'QVariantMap', 'key':'ProjectExplorer.Target.BuildConfiguration.0' })
		self.add(doc, map, 'value', env['PREFIX'], {'type':'QString', 'key':"GenericProjectManager.GenericBuildConfiguration.BuildDirectory"})
		if env.CC_NAME == 'msvc':
			self.add(doc, map, 'value', 'ProjectExplorer.ToolChain.Msvc:%s:%s:%s' % (env.MSVC_ENVIRONMENT[0],env.MSVC_ENVIRONMENT[1], cdb), {'type':'QString', 'key':'ProjectExplorer.BuildConfiguration.ToolChain'})
		else:
			abi = '%s-%s-%s' % (env.ARCHITECTURE, env.PLATFORM[0], env.ABI or 'generic')
			if env.LP64:
				abi += '-64bit'
			else:
				abi += '-32bit'
			self.add(doc, map, 'value', 'ProjectExplorer.ToolChain.GCC:%s.%s.%s'% (env.CXX[0], abi, env.GDB), {'type':'QString', 'key':'ProjectExplorer.BuildConfiguration.ToolChain'})

		build = self.add(doc, map, 'valuemap', '', { 'key':'ProjectExplorer.BuildConfiguration.BuildStepList.0', 'type':'QVariantMap' })
		step = self.add(doc, build, 'valuemap', '', { 'key':'ProjectExplorer.BuildStepList.Step.0', 'type':'QVariantMap' })

		self.add(doc, step, 'value', 'waf install_%s'%toolchainName, { 'key':'ProjectExplorer.ProcessStep.Arguments', 'type':'QString' })
		self.add(doc, step, 'value', sys.executable, { 'key':'ProjectExplorer.ProcessStep.Command', 'type':'QString' })
		self.add(doc, step, 'value', 'true', { 'key':'ProjectExplorer.ProcessStep.Enabled', 'type':'bool' })
		self.add(doc, step, 'value', '%{CurrentProject:Path}', { 'key':'ProjectExplorer.ProcessStep.WorkingDirectory', 'type':'QString' })
		self.add(doc, step, 'value', 'waf %s' % toolchainName, { 'key':'ProjectExplorer.ProjectConfiguration.DefaultDisplayName', 'type':'QString' })
		self.add(doc, step, 'value', '', { 'key':'ProjectExplorer.ProjectConfiguration.DisplayName', 'type':'QString' })
		self.add(doc, step, 'value', 'ProjectExplorer.ProcessStep', { 'key':'ProjectExplorer.ProjectConfiguration.Id', 'type':'QString' })

		self.add(doc, build, 'value', '1', { 'key':'ProjectExplorer.BuildStepList.StepsCount', 'type':'int' })
		self.add(doc, build, 'value', 'Build', { 'key':'ProjectExplorer.ProjectConfiguration.DefaultDisplayName', 'type':'QString' })
		self.add(doc, build, 'value', 'Build', { 'key':'ProjectExplorer.ProjectConfiguration.DisplayName', 'type':'QString' })
		self.add(doc, build, 'value', 'ProjectExplorer.BuildSteps.Build', { 'key':'ProjectExplorer.ProjectConfiguration.Id', 'type':'QString' })

		build = self.add(doc, map, 'valuemap', '', { 'key':'ProjectExplorer.BuildConfiguration.BuildStepList.1', 'type':'QVariantMap' })
		step = self.add(doc, build, 'valuemap', '', { 'key':'ProjectExplorer.BuildStepList.Step.0', 'type':'QVariantMap' })
		self.add(doc, step, 'value', sys.executable, { 'key':'ProjectExplorer.ProcessStep.Command', 'type':'QString' })
		self.add(doc, step, 'value', 'waf clean_%s'%toolchainName, { 'key':'ProjectExplorer.ProcessStep.Arguments', 'type':'QString' })
		self.add(doc, step, 'value', 'true', { 'key':'ProjectExplorer.ProcessStep.Enabled', 'type':'bool' })
		self.add(doc, step, 'value', 'Clean %s' % toolchainName, { 'key':'ProjectExplorer.ProjectConfiguration.DefaultDisplayName', 'type':'QString' })
		self.add(doc, step, 'value', '%{CurrentProject:Path}', { 'key':'ProjectExplorer.ProcessStep.WorkingDirectory', 'type':'QString' })
		self.add(doc, step, 'value',  'Clean %s' % toolchainName, { 'key':'ProjectExplorer.ProjectConfiguration.DisplayName', 'type':'QString' })
		self.add(doc, step, 'value', 'ProjectExplorer.ProcessStep', { 'key':'ProjectExplorer.ProjectConfiguration.Id', 'type':'QString' })

		self.add(doc, build, 'value', '1', { 'key':'ProjectExplorer.BuildStepList.StepsCount', 'type':'int' })
		self.add(doc, build, 'value', 'Clean', { 'key':'ProjectExplorer.ProjectConfiguration.DefaultDisplayName', 'type':'QString' })
		self.add(doc, build, 'value', 'Clean', { 'key':'ProjectExplorer.ProjectConfiguration.DisplayName', 'type':'QString' })
		self.add(doc, build, 'value', 'ProjectExplorer.BuildSteps.Clean', { 'key':'ProjectExplorer.ProjectConfiguration.Id', 'type':'QString' })

		

		self.add(doc, map, 'value', '2', { 'key':'ProjectExplorer.BuildConfiguration.BuildStepListCount', 'type':'int' })
		self.add(doc, map, 'value', 'false', { 'key':'ProjectExplorer.BuildConfiguration.ClearSystemEnvironment', 'type':'bool' })
		self.add(doc, map, 'valuelist', '', { 'key':'ProjectExplorer.BuildConfiguration.UserEnvironmentChanges', 'type':'QVariantList' })
		self.add(doc, map, 'value', '', { 'key':'ProjectExplorer.ProjectConfiguration.DefaultDisplayName', 'type':'QString' })
		self.add(doc, map, 'value', toolchainName, { 'key':'ProjectExplorer.ProjectConfiguration.DisplayName', 'type':'QString' })
		self.add(doc, map, 'value', 'GenericProjectManager.GenericBuildConfiguration', { 'key':'ProjectExplorer.ProjectConfiguration.Id', 'type':'QString' })

		self.add(doc, target, 'value', '1', { 'key':'ProjectExplorer.Target.BuildConfigurationCount', 'type':'int' })
		self.add(doc, target, 'value', '0', { 'key':'ProjectExplorer.Target.DeployConfigurationCount', 'type':'int' })
		self.add(doc, target, 'value', '0', { 'key':'ProjectExplorer.Target.RunConfigurationCount', 'type':'int' })
		targetcount = targetcount + 1

	self.add(doc, data, 'variable', 'ProjectExplorer.Project.TargetCount')
	self.add(doc, data, 'value', '%d'%targetcount, { 'type':'int' })
	self.add(doc, data, 'variable', 'ProjectExplorer.Project.Updater.EnvironmentId')
	self.add(doc, data, 'value', '{81f17ba5-6dc5-418d-8074-a27a07355d8a}', { 'type':'QString' })
	self.add(doc, data, 'variable', 'ProjectExplorer.Project.Updater.FileVersion')
	self.add(doc, data, 'value', '10', { 'type':'int' })
	return doc

def append_directory(self, tree, path):
	source = ""
	for name,subdir in tree.directories.iteritems():
		source += self.append_directory(subdir, os.path.join(path, subdir.prefix))
	for file in tree.files:
		if not file.generated():
			source += "%s\n" % os.path.join(path, file.filename)
	return source

def create_files(self, appname):
	source = ""
	for name, category, sourcetree, platforms in self.depends:
		source += self.append_directory(sourcetree, sourcetree.prefix)
	return source

def create_includes(self, appname):
	includes = set([])
	for toolchainName in self.env['BUILD_VARIANTS']+[i for i in self.env['ALL_VARIANTS'] if i not in self.env['BUILD_VARIANTS']]:
		for name, category, sourcetree, platforms in self.depends:
			env = self.bld.all_envs[toolchainName]
			try:
				options = platforms['%s-%s' % (env['PLATFORM'][0], env['ARCHITECTURE'])]
			except Exception as e:
				options = None
			if options:
				includes = includes.union(options.includedir)
	return "\n".join(includes)

def create_config(self, appname):
	defines = set(['__ECLIPSE'])
	for toolchainName in self.env['BUILD_VARIANTS']+[i for i in self.env['ALL_VARIANTS'] if i not in self.env['BUILD_VARIANTS']]:
		for name, category, sourcetree, platforms in self.depends:
			env = self.bld.all_envs[toolchainName]
			defines = defines.union(env['DEFINES'])
			try:
				options = platforms['%s-%s' % (env['PLATFORM'][0], env['ARCHITECTURE'])]
			except Exception as e:
				options = None
			if options:
				defines = defines.union(options.defines)
	return "\n".join(["#define %s" % d for d in defines])

def add(self, doc, parent, tag, value = '', attrs={}):
	el = doc.createElement(tag)
	if value:
		el.appendChild(doc.createTextNode(value))
	if attrs:
		self.setAttributes(el, attrs)
	parent.appendChild(el)
	return el

def setAttributes(self, node, attrs):
	for k, v in attrs.items():
		node.setAttribute(k, v)



def generateProject(task):
	task.create_project(task.appname)

QtCreatorGenerateProject = Task.task_factory('QtCreatorGenerateProject', generateProject)
QtCreatorGenerateProject.append_directory = append_directory
QtCreatorGenerateProject.create_project = create_project
QtCreatorGenerateProject.create_creator = create_creator
QtCreatorGenerateProject.create_creator_user = create_creator_user
QtCreatorGenerateProject.create_files = create_files
QtCreatorGenerateProject.create_includes = create_includes
QtCreatorGenerateProject.create_config = create_config
QtCreatorGenerateProject.add = add
QtCreatorGenerateProject.setAttributes = setAttributes


solutions={}
@feature('qtcreator')
def create_qtcreator_project(t):
	toolName = t.features[0]
	if not toolName in solutions:
		appname = getattr(Context.g_module, 'APPNAME', 'noname')
		outname = [appname+i for i in ['.creator', '.creator.user', '.files', '.includes', '.config']]
		solution = t.create_task("QtCreatorGenerateProject")
		solution.appname = appname
		solution.bld = t.bld
		solution.env=t.env
		solution.version = toolName
		solution.allplatforms    = t.env['ALL_VARIANTS']
		outnode = [t.path.find_or_declare(n) for n in outname]
		solution.set_outputs(outnode)
		t.bld.install_files(t.path.srcpath(), outnode)
		solution.depends = []
		solution.dep_vars = ['QTCREATOR_PROJECT_DEPENDS']
		solution.env['QTCREATOR_PROJECT_DEPENDS'] = []
		solutions[toolName] = solution
	solution = solutions[toolName]
	solution.depends.append((t.name, t.category, t.sourcetree, t.platforms))

