from TaskGen import feature
from Configure import conf
import Utils
import Task
import os
import mak
import vstudio.solution
import vstudio.vcproj
import vstudio.vcxproj

projects = {
	'vs2005':	(('Visual Studio 2005', '9.00'),(vstudio.vcproj.VCproj, '8.00')),
	'vs2005e':	(('Visual C++ Express 2005', '9.00'),(vstudio.vcproj.VCproj, '8.00')),
	'vs2008':	(('Visual Studio 2008', '10.00'),(vstudio.vcproj.VCproj, '9.00')),
	'vs2008e':	(('Visual C++ Express 2008', '10.00'),(vstudio.vcproj.VCproj, '9.00')),
	'vs2010':	(('Visual Studio 2010', '11.00'),(vstudio.vcxproj.VCxproj, '4.0')),
}

allconfigs = ['debug','release','profile','final']
allplatforms = ['Win32', 'x64', 'Xbox 360']

def generateSolution(task):
	solution = vstudio.solution.Solution( task.name,
										  task.outputs[0].bldpath(task.env),
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
	solution.writeFooter(allplatforms, allconfigs)

def generateProject(task):
	project = task.projectClass( task.outputs[0].bldpath(task.env),
								 task.projectName,
								 task.projectCategory,
								 task.version,
								 task.versionNumber,
								 task.type,
								)
	project.writeHeader(allconfigs, allplatforms, task.platforms)
	project.addDirectory(task.sourceTree)
	project.writeFooter()

GenerateSolution = Task.task_type_from_func('generateSolution', generateSolution)
GenerateProject = Task.task_type_from_func('generateProject', generateProject)

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
	if not solutions.has_key(toolName):
		appname = getattr(Utils.g_module, 'APPNAME', 'noname')
		outname = appname+'.'+toolName+'.sln'
		solution = GenerateSolution(env=t.env)
		solution.version = toolName
		solution.versionName, solution.versionNumber = projects[toolName][0]
		solution.set_outputs(t.path.find_or_declare(outname))
		solution.install_path = t.path.srcpath(t.env)
		solution.depends = []
		solution.dep_vars = ['MSVC_PROJECT_DEPENDS']
		solution.env['MSVC_PROJECT_DEPENDS'] = []
		solutions[toolName] = solution
	solution = solutions[toolName]
	projectClass,versionNumber = projects[toolName][1]

	project = GenerateProject(env=t.env.copy())
	project.type			= t.type
	project.platforms 		= filterplatforms(t.type, t.platforms, t.depends)
	project.version 		= toolName
	project.versionNumber 	= versionNumber
	project.projectClass 	= projectClass
	project.projectCategory = t.category
	project.projectName 	= t.name
	project.type 			= t.type
	project.sourceTree 		= t.sourcetree
	project.install_path	= os.path.join(t.path.srcpath(t.env), '.build', toolName)

	outname = t.category+'.'+t.name+'.'+toolName+projectClass.extensions[0]
	t.outname = os.path.join('.build', toolName, outname)
	for extension in projectClass.extensions:
		outname = t.category+'.'+t.name+'.'+toolName+extension
		project.set_outputs([t.path.find_or_declare(outname)])
	project.env['MSVC_PROJECT_SOURCES'] = t.sourcetree
	project.env['MSVC_PROJECT_FLAGS'] = t.platforms
	project.dep_vars = ['MSVC_PROJECT_SOURCES', 'MSVC_PROJECT_FLAGS']

	solution.depends.append(t)
	solution.env['MSVC_PROJECT_DEPENDS'].append(outname)

for pname in projects.keys():
	feature(pname)(create_project)

