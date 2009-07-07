from TaskGen import feature
from Configure import conf
import Utils
import Task
import os
import mak

allconfigs = ['debug','release','profile','final']

def addFolder(file, tree, name, path, tabs):
	file.write(tabs+'<logicalFolder name="%s" displayName="%s" projectFiles="true">\n' % (name, name))
	for dname, d in tree.directories.iteritems():
		addFolder(file, d, dname, path + '/' + dname, tabs+'  ')
	for f in tree.files:
		file.write(tabs+'  <itemPath>%s</itemPath>\n' % (path + '/' + f.filename))
	file.write(tabs+'</logicalFolder>\n')
	
def generateProject(task):
	file = open(task.outputs[0].bldpath(task.env), 'w')
	file.write('<?xml version="1.0" encoding="UTF-8"?>\n')
	file.write('<project xmlns="http://www.netbeans.org/ns/project/1">\n')
	file.write('    <type>org.netbeans.modules.cnd.makeproject</type>\n')
	file.write('    <configuration>\n')
	file.write('        <data xmlns="http://www.netbeans.org/ns/make-project/1">\n')
	file.write('            <name>%s</name>\n' % (task.projectCategory+'.'+task.projectName))
	file.write('            <make-project-type>0</make-project-type>\n')
	file.write('            <c-extensions>c</c-extensions>\n')
	file.write('            <cpp-extensions>cpp,cc,cxx</cpp-extensions>\n')
	file.write('            <header-extensions>h,hh</header-extensions>\n')
	file.write('            <sourceEncoding>UTF-8</sourceEncoding>\n')
	file.write('            <make-dep-projects></make-dep-projects>\n')
	file.write('        </data>\n')
	file.write('    </configuration>\n')
	file.write('</project>\n')
	
	file = open(task.outputs[1].bldpath(task.env), 'w')
	file.write('<?xml version="1.0" encoding="UTF-8"?>')
	file.write('<configurationDescriptor version="62">')
	addFolder(file, task.sourceTree, 'root', '../../../src/%s/%s' %(task.projectCategory, task.projectName), '  ')
	file.write('  <projectmakefile>Makefile</projectmakefile>')
	file.write('  <confs>')
	file.write('    <conf name="Debug" type="1">')
	file.write('      <toolsSet>')
	file.write('        <developmentServer>localhost</developmentServer>')
	file.write('        <compilerSet>MinGW|MinGW</compilerSet>')
	file.write('        <cRequired>true</cRequired>')
	file.write('        <platform>3</platform>')
	file.write('      </toolsSet>')
	file.write('      <compileType>')
	file.write('        <ccTool>')
	file.write('          <stripSymbols>true</stripSymbols>')
	file.write('          <warningLevel>3</warningLevel>')
	file.write('        </ccTool>')
	file.write('        <linkerTool>')
	file.write('          <linkerLibItems>')
	file.write('          </linkerLibItems>')
	file.write('        </linkerTool>')
	file.write('      </compileType>')
	file.write('    </conf>')
	file.write('    <conf name="Release" type="1">')
	file.write('      <toolsSet>')
	file.write('        <developmentServer>localhost</developmentServer>')
	file.write('        <compilerSet>None|Unknown</compilerSet>')
	file.write('        <platform>3</platform>')
	file.write('      </toolsSet>')
	file.write('      <compileType>')
	file.write('        <cTool>')
	file.write('          <developmentMode>5</developmentMode>')
	file.write('        </cTool>')
	file.write('        <ccTool>')
	file.write('          <developmentMode>5</developmentMode>')
	file.write('        </ccTool>')
	file.write('        <fortranCompilerTool>')
	file.write('          <developmentMode>5</developmentMode>')
	file.write('        </fortranCompilerTool>')
	file.write('        <linkerTool>')
	file.write('          <linkerLibItems>')
	file.write('          </linkerLibItems>')
	file.write('        </linkerTool>')
	file.write('      </compileType>')
	file.write('    </conf>')
	file.write('  </confs>')
	file.write('</configurationDescriptor>')


	return

GenerateProject = Task.task_type_from_func('generateProject', generateProject)

@feature('netbeans')
def create_netbeans_project(t):
	toolName = t.features[0]

	project = GenerateProject(env=t.env)
	project.type			= t.type
	project.version 		= toolName
	project.projectCategory = t.category
	project.projectName 	= t.name
	project.type 			= t.type
	project.sourceTree 		= t.sourcetree
	project.install_path	= os.path.join(t.path.srcpath(t.env), '.build', toolName, t.category+'.'+t.name, 'nbproject')

	project.set_outputs([t.path.find_or_declare(os.path.join('src', t.category, t.name, 'project.xml'))])
	project.set_outputs([t.path.find_or_declare(os.path.join('src', t.category, t.name, 'configurations.xml'))])

	project.env['NETBEANS_PROJECT_SOURCES'] = t.sourcetree
	project.dep_vars = ['NETBEANS_PROJECT_SOURCES']
