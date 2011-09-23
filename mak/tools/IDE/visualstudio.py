from waflib.TaskGen import feature
from waflib.Configure import conf
from waflib import Context, Task
import os
import mak
from mak.tools.IDE.vstudio import solution,vcproj,vcxproj

projects = {
	'vs2003':	(('Visual Studio .NET 2003', '8.00'),(vcproj.VCproj, '7.10')),
	'vs2005':	(('Visual Studio 2005', '9.00'),(vcproj.VCproj, '8.00')),
	'vs2005e':	(('Visual C++ Express 2005', '9.00'),(vcproj.VCproj, '8.00')),
	'vs2008':	(('Visual Studio 2008', '10.00'),(vcproj.VCproj, '9.00')),
	'vs2008e':	(('Visual C++ Express 2008', '10.00'),(vcproj.VCproj, '9.00')),
	'vs2010':	(('Visual Studio 2010', '11.00'),(vcxproj.VCxproj, '4.0')),
}

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
	s.writeFooter(task.env.ALL_VARIANTS)

def generateProject(task):
	project = task.projectClass( task.outputs[0].abspath(),
								 task.projectName,
								 task.projectCategory,
								 task.version,
								 task.versionNumber,
								 task.type,
								 task.bld.all_envs
								)
	project.writeHeader(task.allplatforms)
	project.addDirectory(task.sourceTree)
	project.writeFooter()

GenerateSolution = Task.task_factory('GenerateSolution', func=generateSolution)
GenerateProject = Task.task_factory('GenerateProject', func=generateProject)

solutions = {}
def create_project(t):
	toolName = t.features[0]
	if not toolName in solutions:
		appname = getattr(Context.g_module, 'APPNAME', 'noname')
		outname = os.path.join('.build', appname+'.'+toolName+'.sln')
		solution = t.create_task("GenerateSolution")
		solution.env=t.env
		solution.version = toolName
		solution.versionName, solution.versionNumber = projects[toolName][0]
		solution.allplatforms    = t.env.ALL_VARIANTS
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
	project.allplatforms    = t.env.ALL_VARIANTS
	project.version 		= toolName
	project.versionNumber 	= versionNumber
	project.projectClass 	= projectClass
	project.projectCategory = t.category
	project.projectName 	= t.name
	project.type 			= t.type
	project.sourceTree 		= t.sourcetree

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
	project.bld = t.bld

	solution.depends.append(t)
	solution.env['MSVC_PROJECT_DEPENDS'].append(outname)

for pname in projects.keys():
	feature(pname)(create_project)

def configure(ctx):
	pass

