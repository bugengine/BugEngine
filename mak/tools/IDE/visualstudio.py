from waflib.TaskGen import feature
from waflib.Configure import conf
from waflib import Context, Task
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
	s = solution.Solution( task.name,
						   task.outputs[0].abspath(),
						   task.version,
						   task.versionNumber,
						   task.versionName,
						)
	s.writeHeader()
	# adds games and exes first
	for d in task.depends:
		if d.type == 'game':
			s.addProject(d)
	for d in task.depends:
		if d.type != 'game':
			s.addProject(d)
	s.writeFooter(task.allplatforms, allconfigs)

def generateProject(task):
	project = task.projectClass( task.outputs[0].abspath(),
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

GenerateSolution = Task.task_factory('GenerateSolution', func=generateSolution)
GenerateProject = Task.task_factory('GenerateProject', func=generateProject)

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
	if not toolName in solutions:
		appname = getattr(Context.g_module, 'APPNAME', 'noname')
		outname = appname+'.'+toolName+'.sln'
		solution = t.create_task("GenerateSolution")
		solution.env=t.env
		solution.version = toolName
		solution.versionName, solution.versionNumber = projects[toolName][0]
		solution.allplatforms    = projects[toolName][2]
		outnode = t.path.find_or_declare(outname)
		solution.set_outputs(outnode)
		t.bld.install_files(t.path.srcpath(), outnode)
		solution.depends = []
		solution.dep_vars = ['MSVC_PROJECT_DEPENDS']
		solution.env['MSVC_PROJECT_DEPENDS'] = []
		solutions[toolName] = solution
	solution = solutions[toolName]
	projectClass,versionNumber = projects[toolName][1]

	project = t.create_task("GenerateProject")
	project.env=t.env.derive()
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
	project.depends         = t.depends

	install_path	= os.path.join(t.path.srcpath(), '.build', toolName)
	outname = t.category+'.'+t.name+'.'+toolName+projectClass.extensions[0]
	t.outname = os.path.join('.build', toolName, outname)
	for extension in projectClass.extensions:
		outname = t.category+'.'+t.name+'.'+toolName+extension
		outnode = t.path.find_or_declare(outname)
		project.set_outputs(outnode)
		t.bld.install_files(install_path, outnode)
	project.env['MSVC_PROJECT_SOURCES'] = t.sourcetree.hash()
	project.env['MSVC_PROJECT_FLAGS'] = t.platforms
	project.dep_vars = ['MSVC_PROJECT_SOURCES', 'MSVC_PROJECT_FLAGS']

	solution.depends.append(t)
	solution.env['MSVC_PROJECT_DEPENDS'].append(outname)

for pname in projects.keys():
	feature(pname)(create_project)

def configure(ctx):
	pass

