import os
import mak.tools.msvc_project_generator
import mak.module
import Utils

import Task

allconfigs = ['debug','release','genprofile','final']
allplatforms = ['Win32', 'x64']

def generateProject(task):
	project = mak.tools.msvc_project_generator.Project( task.name,
														task.category,
														task.msvcVersion,
														task.outputs[0].bldpath(task.env),
														os.path.join('.projects',task.category+'.'+task.name+'.' + task.msvcVersion + '.vcproj'),
														task.platforms)
	project.writeHeader()
		
	project.addConfigurations(allconfigs, task.type)
	project.addDirectory(task.sourcetree)
	project.writeFooter()
	return

def generateSolution(task):
	solution = mak.tools.msvc_project_generator.Solution(task.name,task.msvcVersion,task.outputs[0].bldpath(task.env))
	solution.writeHeader()
	# adds games and exes first
	done = set()
	for d in task.depends:
		if d.type == 'game':
			solution.addProject(d, done)
	for d in task.depends:
		if d.type != 'game':
			solution.addProject(d, done)
	solution.writeFooter(allplatforms, allconfigs)
	return

GenerateProject = Task.task_type_from_func('generateProject', generateProject)
GenerateSolution = Task.task_type_from_func('generateSolution', generateSolution)
	
	
def filterplatforms(type,platforms,depends):
	supportedplatforms = {'win32-x86': 'Win32', 'win32-amd64': 'x64'}
	common_options = mak.module.coptions( defines = ['PREFIX=\\"\\"',
										   'DATA_DIR=\\"data/\\"',
										   'PLUGIN_DIR=\\"data/plugins\\"',
										   'CONF_DIR=\\"conf/\\"'])
	if type == 'plugin':
		for d in depends:
			if d.type == 'game':
				common_options.libs.append('game.'+d.name)

	return [(p,supportedplatforms[p], platforms[p].merge(common_options)) for p in platforms.keys() if p in supportedplatforms.keys()] or [('win32-x86', 'Win32', common_options),('win32-amd64', 'x64', common_options)]


from TaskGen import taskgen, extension, feature, before, after

solutions = {}
@taskgen
@feature('vs2005', 'vs2008', 'vs2005e', 'vs2008e')
@after('apply_core')
def create_msvc_solution(self):
	toolName = self.features[0]
	if not solutions.has_key(toolName):
		solution = GenerateSolution(self.env)
		filename = appname = getattr(Utils.g_module, 'APPNAME', 'noname')
		outname = filename+'.'+toolName+'.sln'
		solution.set_outputs(self.path.find_or_declare(outname))
		solution.install_path	= self.path.srcpath(self.env)
		solution.depends = []
		solution.dep_vars = ['MSVC_PROJECT_DEPENDS']
		solution.env['MSVC_PROJECT_DEPENDS'] = []
		solution.msvcVersion = toolName
		solutions[toolName] = solution
	else:
		solution = solutions[toolName]
		
	type = self.type
	category = self.category
	task = GenerateProject(self.env.copy())
	filename = self.target
	outname = self.category+'.'+filename+'.vcproj'
	task.set_outputs(self.path.find_or_declare(outname))
	task.msvcVersion	= toolName
	task.type			= self.type
	task.sourcetree		= self.sourcetree
	task.category		= self.category
	task.name			= self.name
	task.depends		= self.depends
	task.platforms		= filterplatforms(self.type, self.platforms, self.depends)
	task.install_path	= os.path.join(self.path.srcpath(self.env), '.projects')
	task.env['MSVC_PROJECT_SOURCES'] = self.sourcetree
	task.env['MSVC_PROJECT_FLAGS'] = self.platforms
	task.dep_vars = ['MSVC_PROJECT_SOURCES','MSVC_PROJECT_FLAGS']
	
	solution.env['MSVC_PROJECT_DEPENDS'] += [task.name, str([d.name for d in task.depends])]
	solution.set_inputs(task.outputs)
	solution.set_run_after(task)
	solution.depends.append(self)
