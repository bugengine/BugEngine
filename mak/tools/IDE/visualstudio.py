from waflib.TaskGen import feature
from waflib.Configure import conf
from waflib import Utils, Task
import os
import mak
from mak.tools.IDE.vstudio import solution,vcproj,vcxproj

projects = {
	'vs2003':	(('Visual Studio .NET 2003', '8.00'),(vcproj.VCproj, '7.10'), ['Win32']),
	'vs2005':	(('Visual Studio 2005', '9.00'),(vcproj.VCproj, '8.00'), ['Win32', 'x64', 'Xbox 360']),
	'vs2005e':	(('Visual C++ Express 2005', '9.00'),(vcproj.VCproj, '8.00'), ['Win32']),
	'vs2008':	(('Visual Studio 2008', '10.00'),(vcproj.VCproj, '9.00'), ['Win32', 'x64', 'Xbox 360']),
	'vs2008e':	(('Visual C++ Express 2008', '10.00'),(vcproj.VCproj, '9.00'), ['Win32']),
	'vs2010':	(('Visual Studio 2010', '11.00'),(vcxproj.VCxproj, '4.0'), ['Win32', 'x64', 'Xbox 360']),
}

allconfigs = ['debug','profile','final']

def generateSolution(task):
	solution = solution.Solution( task.name,
								  task.outputs[0].bldpath(),
								  task.version,
								  task.versionNumber,
								  task.versionName,
								 )
	solution.writeHeader()
	# adds games and exes first
	for d in task.depends:
		if d.type == 'game':
			solution.addProject(d)
	for d in task.depends:
		if d.type != 'game':
			solution.addProject(d)
	solution.writeFooter(task.allplatforms, allconfigs)

def generateProject(task):
	project = task.projectClass( task.outputs[0].bldpath(),
								 task.projectName,
								 task.projectCategory,
								 task.version,
								 task.versionNumber,
								 task.type,
								 task.depends
								)
	project.writeHeader(allconfigs, task.allplatforms, task.platforms)
	project.addDirectory(task.sourceTree)
	project.writeFooter()

GenerateSolution = Task.task_factory('generateSolution', generateSolution)
GenerateProject = Task.task_factory('generateProject', generateProject)

def filterplatforms(type,platforms,depends):
	supportedplatforms = {'win32-x86': 'Win32', 'win32-amd64': 'x64', 'xbox360-ppc':'Xbox 360'}
	common_options = mak.module.coptions( defines = ['PREFIX=\\"\\"',
										   'DATA_DIR=\\"data/\\"',
										   'PLUGIN_DIR=\\"data/plugins\\"',
										   'CONF_DIR=\\"conf/\\"'])
	if type == 'plugin':
		for d in depends:
			if d.type == 'game':
				common_options.libs.append(d.name)

	return [(p,supportedplatforms[p], platforms[p].merge(common_options)) for p in platforms.keys() if p in supportedplatforms.keys()] or [('win32-x86', 'Win32', common_options),('win32-amd64', 'x64', common_options),('xbox360-ppc', 'Xbox 360', common_options)]

solutions = {}
def create_project(t):
	toolName = t.features[0]
	if not toolname in solutions:
		appname = getattr(Utils.g_module, 'APPNAME', 'noname')
		outname = appname+'.'+toolName+'.sln'
		solution = GenerateSolution(env=t.env)
		solution.version = toolName
		solution.versionName, solution.versionNumber = projects[toolName][0]
		solution.allplatforms    = projects[toolName][2]
		solution.set_outputs(t.path.find_or_declare(outname))
		solution.install_path = t.path.srcpath()
		solution.depends = []
		solution.dep_vars = ['MSVC_PROJECT_DEPENDS']
		solution.env['MSVC_PROJECT_DEPENDS'] = []
		solutions[toolName] = solution
	solution = solutions[toolName]
	projectClass,versionNumber = projects[toolName][1]

	project = GenerateProject(env=t.env.copy())
	project.type			= t.type
	project.allplatforms    = projects[toolName][2]
	project.platforms 		= filterplatforms(t.type, t.platforms, t.depends)
	project.version 		= toolName
	project.versionNumber 	= versionNumber
	project.projectClass 	= projectClass
	project.projectCategory = t.category
	project.projectName 	= t.name
	project.type 			= t.type
	project.sourceTree 		= t.sourcetree
	project.install_path	= os.path.join(t.path.srcpath(t.env), '.build', toolName)
	project.depends         = t.depends

	outname = t.category+'.'+t.name+'.'+toolName+projectClass.extensions[0]
	t.outname = os.path.join('.build', toolName, outname)
	for extension in projectClass.extensions:
		outname = t.category+'.'+t.name+'.'+toolName+extension
		project.set_outputs([t.path.find_or_declare(outname)])
	project.env['MSVC_PROJECT_SOURCES'] = t.sourcetree.hash()
	project.env['MSVC_PROJECT_FLAGS'] = t.platforms
	project.dep_vars = ['MSVC_PROJECT_SOURCES', 'MSVC_PROJECT_FLAGS']

	solution.depends.append(t)
	solution.env['MSVC_PROJECT_DEPENDS'].append(outname)

for pname in projects.keys():
	feature(pname)(create_project)

