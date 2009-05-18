import os
import sources
import Options
import misc

alloptims	 = [ 'debug', 'release', 'genprofile', 'final']
allplatforms = [ 'win32', 'sunos', 'darwin', 'psp', 'gp2x', 'nds', 'wii', 'ogc', 'gba' ]
allarchs	 = [ 'amd64', 'x86', 'alpha', 'arm', 'arm7', 'arm9', 'hppa', 'ia64', 'mips', 'ppc', 's390', 's390x', 'sparc' ]


class coptions:
	def __init__( self, 
				  includedir = [],
				  libdir = [],
				  defines = [],
				  libs = [] ):
		self.includedir = set(includedir)
		self.libdir		= set(libdir)
		self.defines	= set(defines)
		self.libs		= libs[:]
		self.deffile	= ''
		self.pchname	= ''
		self.pchstop	= ''
		
	def merge(self, other):
		self.includedir |= other.includedir
		self.libdir		|= other.libdir
		self.defines	|= other.defines
		self.libs		+= other.libs
		if not self.pchname: self.pchname = other.pchname
		if not self.pchstop: self.pchstop = other.pchstop
		if not self.deffile: self.deffile = other.deffile
		return self
		
	def __repr__(self):
		return (str(self.includedir)+','+
				str(self.libdir)+','+
				str(self.defines)+','+
				str(self.libs)+','+
				self.deffile+','+
				self.pchname+','+
				self.pchstop)
		
class module:
	def __init__( self,
				  name,
				  depends,
				  category,
				  localoptions,
				  globaloptions,
				  localarchoptions,
				  globalarchoptions,
				  platforms,
				  archs,
				):
				
		while None in depends:
			depends.remove(None)
		self.name	  = name
		self.category = category
		self.depends  = depends
		self.tasks = {}
		self.root = os.path.join('src', category, name)
		self.platforms = platforms
		self.archs = archs
		self.projects = {}

		self.localoptions = coptions()
		self.localoptions.merge(localoptions)
		self.localoptions.defines |= set([ 'building_'+name,
											'BE_PROJECTCATEGORY="'+category+'"',
											'BE_PROJECTNAME="'+name+'"',
											'BE_SOURCEDIR="'+os.path.abspath('src').replace('\\','\\\\')+'"'])
		self.globaloptions = coptions()
		self.globaloptions.merge(globaloptions)
		self.localarchoptions = localarchoptions
		self.globalarchoptions = {}
		self.usemaster = True
		
		for k in globalarchoptions.keys():
			self.globalarchoptions[k] = coptions()
			self.globalarchoptions[k].merge(globalarchoptions[k])
		for d in depends:
			self.globaloptions.merge(d.globaloptions)
			for arch in d.globalarchoptions.keys():
				if not self.globalarchoptions.has_key(arch):
					self.globalarchoptions[arch] = coptions()
				self.globalarchoptions[arch].merge(d.globalarchoptions[arch])

		self.usepch = 0
		if os.path.isfile(os.path.join('src', category, self.name, 'src', 'stdafx.cpp')):
			self.pchsource = os.path.join('src', category, self.name, 'src', 'stdafx.cpp')
			if os.path.isfile(os.path.join('src', category, self.name, 'api', name, 'stdafx.h')):
				self.pchheader = os.path.join(self.name, 'stdafx.h')
				self.pchfullheader = os.path.join('src', category, self.name, 'api', name, 'stdafx.h')
			elif os.path.isfile(os.path.join('src', category, self.name, 'include', 'stdafx.h')):
				self.pchheader = 'stdafx.h'
				self.pchfullheader = os.path.join('src', category, self.name, 'include', 'stdafx.h')
			self.localoptions.pchname = self.pchsource
			self.localoptions.pchstop = self.pchheader
			self.usepch = 1

		if os.path.isdir(os.path.join('src', category, name, 'include')):
			self.localoptions.includedir.add(os.path.join('src', category, name, 'include'))
		if os.path.isdir(os.path.join('src', category, name, 'api')):
			self.globaloptions.includedir.add(os.path.join('src', category, name, 'api'))

		self.sourcetree		   = sources.directory()
		if os.path.isdir(os.path.join('src', category, name, 'include')):
			self.sourcetree.addDirectory(self.scandir(os.path.join('src', category, name, 'include'), '', 0, allplatforms, allarchs), 'include')
		if os.path.isdir(os.path.join('src', category, name, 'api')):
			self.sourcetree.addDirectory(self.scandir(os.path.join('src', category, name, 'api'), '', 0, allplatforms, allarchs), 'api')
		if os.path.isdir(os.path.join('src', category, name, 'data')):
			self.sourcetree.addDirectory(self.scandir(os.path.join('src', category, name, 'data'), '', 1, allplatforms, allarchs), 'data')
		if os.path.isdir(os.path.join('src', category, name, 'src')):
			self.sourcetree.addDirectory(self.scandir(os.path.join('src', category, name, 'src'), '', 1, allplatforms, allarchs), 'src')
		for arch in allarchs:
			if os.path.isdir(os.path.join('src', category, name, 'lib.'+arch)):
				self.sourcetree.addDirectory(self.scandir(os.path.join('src', category, name, 'lib.'+arch), '', 0, allplatforms, [arch]), 'lib.'+arch)
			if os.path.isdir(os.path.join('src', category, name, 'bin.'+arch)):
				self.sourcetree.addDirectory(self.scandir(os.path.join('src', category, name, 'bin.'+arch), '', 0, allplatforms, [arch]), 'bin.'+arch)

	def getoptions(self, platform, arch):
		options = coptions()
		options.merge(self.localoptions)
		options.merge(self.globaloptions)
		try: options.merge(self.localarchoptions[arch])
		except KeyError: pass
		try: options.merge(self.globalarchoptions[arch])
		except KeyError: pass
		try: options.merge(self.localarchoptions[platform])
		except KeyError: pass
		try: options.merge(self.globalarchoptions[platform])
		except KeyError: pass
		try: options.merge(self.localarchoptions[platform+'-'+arch])
		except KeyError: pass
		try: options.merge(self.globalarchoptions[platform+'-'+arch])
		except KeyError: pass
		return options
		
	
	def gentask(self, bld, env, variant, type, options = coptions(), inheritedoptions = coptions()):
		if not self.tasks.has_key(variant):
			if type=='dummy' or not env['PLATFORM'] in self.platforms or not env['ARCHITECTURE'] in self.archs:
				task = None
				# will deploy files that were scheduled to be deployed
				self.sourcetree.make_sources(bld, env, self.root)
			else:
				task					= bld.new_task_gen()
				task.target				= self.name
				task.env				= env.copy()
				task.type				= type
				task.features			= ['cc', 'cxx', type]
				task.usemaster			= self.usemaster
				
				task.inheritedoptions	= coptions()
				task.inheritedoptions.merge(inheritedoptions)
				task.uselib_local = []
				task.add_objects = []
				task.install_path = os.path.abspath(os.path.join(env['PREFIX'],env['DEPLOY']['prefix'],env['DEPLOY'][self.install_path]))
				dps = self.depends[:]
				seen = set()
				while dps:
					d = dps.pop(0)
					t = d.tasks[variant]
					if t:
						seen.add(d)
						if t.type == 'cobjects' and d in self.depends:
							task.add_objects.append(d.name)
						else:
							task.uselib_local.append(d.name)
						task.inheritedoptions.merge(t.inheritedoptions)
						dps += [dep for dep in d.depends if dep not in seen]
				task.options			= self.getoptions(env['PLATFORM'], env['ARCHITECTURE'])
				task.options.merge(options)
				task.options.merge(task.inheritedoptions)
				try:
					task.pchsource = self.pchsource
					task.pchheader = self.pchheader
					task.pchfullheader = self.pchfullheader
				except AttributeError:
					pass

				task.defines			= [d for d in task.options.defines]
				task.includes			= [i for i in task.options.includedir]
				task.extralibdirs		= [l for l in task.options.libdir]
				task.extralibs			= [l for l in task.options.libs]
				
				task.source				= self.sourcetree.make_sources(bld, env, self.root)
				task.do_install			= 1
			self.tasks[variant]		= task
		return self.tasks[variant]
		
	def scandir(self, path, local, process, platforms, archs):
		result = sources.directory()
		
		for file in os.listdir(path):
			if file == '.svn':
				continue
			elif file == '.cvs':
				continue
			elif os.path.isdir(os.path.join(path,file)):
				if file in platforms:
					result.addDirectory( self.scandir(os.path.join(path,file), os.path.join(local,file), process, [file], archs), file )
				elif file in archs:
					result.addDirectory( self.scandir(os.path.join(path,file), os.path.join(local,file),process, platforms, [file]), file )
				else:
					result.addDirectory( self.scandir(os.path.join(path,file), os.path.join(local,file),process, platforms, archs), file )
			else:
				filename, ext = os.path.splitext(file)
				fullname = os.path.join(path, file)
				

				newexts = { '.ll':('.cc','.hh'), '.yy': ('.cc','.hh'), '.l':('.c', '.h'), '.y':('.c', '.h') }
				if ext in set(['.cc', '.cpp', '.cxx', '.c', '.C', '.s', '.bin', '.grit']):
					result.addFile(sources.cppsource(file, platforms, archs, process, self.usepch))
				elif ext in set(['.rc']):
					result.addFile(sources.source(file, platforms, archs, process))
				elif ext in set(['.y', '.yy']):
					cext, hext = newexts[ext]
					generatedcfile = os.path.join(path, filename+cext)
					generatedhfile = os.path.join(path, filename+hext)
					result.addFile(sources.yaccsource(file, generatedcfile, generatedhfile, platforms, archs, process))
					result.addFile(sources.generatedcppsource(filename+cext, platforms, archs, process))
					result.addFile(sources.generatedhsource(filename+hext, platforms, archs, process))
				elif ext in set(['.l', '.ll']):
					cext, hext = newexts[ext]
					generatedcfile = os.path.join(path, filename+cext)
					result.addFile(sources.lexsource(file, generatedcfile, platforms, archs, process))
					result.addFile(sources.generatedcppsource(filename+cext, platforms, archs, process))
				elif ext in set(['.def']):
					self.localoptions.deffile = fullname
					result.addFile(sources.source(file,platforms,archs, process))
				elif ext in set(['.h', '.hpp', '.inl','.hh', '.hxx', '']):
					result.addFile(sources.hsource(file,platforms,archs, process))
				elif ext in set(['.ogg', '.lua', '.nut', '.dd', '.cg', '.pcf', '.ttf', '.fon', '.tga']):
					result.addFile( sources.deployedsource(file, local, 'data', platforms, archs, process ) )

		return result
		
	def deploy( self, filename, outputdir, deploytype, platforms = None, archs = None ):
		if not platforms: platforms=allplatforms
		if not archs: archs=allarchs
		self.sourcetree.addFullFile( filename, sources.deployedsource(os.path.split(filename)[1], outputdir, deploytype, platforms, archs, 1) )

	def deployDirectory( self, dirname, outputdir, deploytype, platforms = None, archs = None ):
		for file in os.listdir(os.path.join(self.root,dirname)):
			if file == '.svn':
				continue
			elif file == '.cvs':
				continue
			if os.path.isdir(os.path.join(self.root,dirname,file)):
				if file in allarchs:
					self.deployDirectory(os.path.join(dirname,file),os.path.join(outputdir,file),deploytype, platforms,[file])
				elif file in allplatforms:
					self.deployDirectory(os.path.join(dirname,file),os.path.join(outputdir,file),deploytype, [file],archs)
				else:
					self.deployDirectory(os.path.join(dirname,file),os.path.join(outputdir,file),deploytype, platforms,archs)
			else:
				self.deploy(os.path.join(dirname,file),outputdir,deploytype,platforms, archs)

	def makeproject(self, bld):
		for p,pname in bld.env['PROJECTS']:
			if getattr(Options.options,p):
				if not self.projects.has_key(p):
					task = bld.new_task_gen()
					task.features		= [p]
					task.depends		= [d.projects[p] for d in self.depends]
					task.target			= self.name+'.'+p
					task.name			= self.name
					task.env			= bld.env
					task.sourcetree		= self.sourcetree
					task.type			= self.__class__.__name__
					task.category		= self.category
					platforms = {}
					for platform in self.platforms or allplatforms:
						for arch in self.archs or allarchs:
							options = self.getoptions(platform, arch)
							platforms[platform+'-'+arch] = options
					task.platforms		= platforms
					self.projects[p] = task

""" simple objects """
class library(module):
	def __init__( self,
				  name,
				  depends = [],
				  category = 'engine',
				  localoptions = coptions(),
				  globaloptions = coptions(),
				  localarchoptions = {},
				  globalarchoptions = {},
				  platforms = allplatforms,
				  archs = allarchs,
				):
		self.install_path = 'lib'
		module.__init__(self,
						name,
						depends,
						category,
						localoptions,
						globaloptions,
						localarchoptions,
						globalarchoptions,
						platforms,
						archs)

	def post(self, builder):
		for d in self.depends:
			d.post(builder)
		self.makeproject(builder)
		for envname in builder.env['BUILD_VARIANTS']:
			env = builder.all_envs[envname]
			options = coptions()
			ioptions = coptions()
			task = self.gentask(builder, env, envname, 'cobjects', options, ioptions)
			
""" shared lib """
class shared_library(module):
	def __init__( self,
				  name,
				  depends = [],
				  category = 'engine',
				  localoptions = coptions(),
				  globaloptions = coptions(),
				  localarchoptions = {},
				  globalarchoptions = {},
				  platforms = allplatforms,
				  archs = allarchs,
				):
		self.install_path = 'bin'
		module.__init__(self,
						name,
						depends,
						category,
						localoptions,
						globaloptions,
						localarchoptions,
						globalarchoptions,
						platforms,
						archs)

	def post(self, builder):
		for d in self.depends:
			d.post(builder)
		self.makeproject(builder)
		for envname in builder.env['BUILD_VARIANTS']:
			env = builder.all_envs[envname]
			options = coptions()
			options.defines.add('BUILDING_DLL')
			options.defines.add('_USRDLL')
			ioptions = coptions( defines = [self.name+'_dll'] )
			task = self.gentask(builder, env, envname, 'cshlib', options, ioptions)

""" static lib """
class static_library(module):
	def __init__( self,
				  name,
				  depends = [],
				  category = 'engine',
				  localoptions = coptions(),
				  globaloptions = coptions(),
				  localarchoptions = {},
				  globalarchoptions = {},
				  platforms = allplatforms,
				  archs = allarchs,
				):
		self.install_path = 'lib'
		module.__init__(self,
						name,
						depends,
						category,
						localoptions,
						globaloptions,
						localarchoptions,
						globalarchoptions,
						platforms,
						archs)

	def post(self, builder):
		for d in self.depends:
			d.post(builder)
		self.makeproject(builder)
		for envname in builder.env['BUILD_VARIANTS']:
			env = builder.all_envs[envname]
			options = coptions()
			task = self.gentask(builder, env, envname, 'cstaticlib', options, coptions())
			
""" plugin """
class plugin(module):
	def __init__( self,
				  name,
				  depends = [],
				  category = 'plugin',
				  localoptions = coptions(),
				  globaloptions = coptions(),
				  localarchoptions = {},
				  globalarchoptions = {},
				  platforms = allplatforms,
				  archs = allarchs,
				):
		self.install_path = 'plugin'
		module.__init__(self,
						name,
						depends,
						category,
						localoptions,
						globaloptions,
						localarchoptions,
						globalarchoptions,
						platforms,
						archs)

	def post(self, builder):
		for d in self.depends:
			d.post(builder)
		self.makeproject(builder)
		for envname in builder.env['BUILD_VARIANTS']:
			env = builder.all_envs[envname]
			options = coptions()
			options.defines.add('BUILDING_DLL')
			options.defines.add('_USRDLL')
			task = self.gentask(builder, env, envname, 'cshlib', options, coptions())
			if task:
				task.install_bindir = os.path.join(env['DEPLOY']['plugin'])

""" game """
class game(module):
	def __init__( self,
				  name,
				  depends = [],
				  category = 'game',
				  localoptions = coptions(),
				  globaloptions = coptions(),
				  localarchoptions = {},
				  globalarchoptions = {},
				  platforms = allplatforms,
				  archs = allarchs,
				):
		self.install_path = 'bin'
		module.__init__(self,
						name,
						depends,
						category,
						localoptions,
						globaloptions,
						localarchoptions,
						globalarchoptions,
						platforms,
						archs)

	def post(self, builder):
		for d in self.depends:
			d.post(builder)
		self.makeproject(builder)
		for envname in builder.env['BUILD_VARIANTS']:
			env = builder.all_envs[envname]
			options = coptions()
			ioptions = coptions( )
			for d in self.depends:
				if env['STATIC'] and d.__class__.__name__ == 'library':
					ioptions.defines.add(d.name + '_dll')
			task = self.gentask(builder, env, envname, 'cprogram', options, ioptions)

""" unit test """
class test(module):
	def __init__( self,
				  name,
				  depends = [],
				  category = 'test',
				  localoptions = coptions(),
				  globaloptions = coptions(),
				  localarchoptions = {},
				  globalarchoptions = {},
				  platforms = allplatforms,
				  archs = allarchs,
				):
		module.__init__(self,
						name,
						depends,
						category,
						localoptions,
						globaloptions,
						localarchoptions,
						globalarchoptions,
						platforms,
						archs)

	def post(self, builder):
		for d in self.depends:
			d.post(builder)
		self.makeproject(builder)
		for envname in builder.env['BUILD_VARIANTS']:
			env = builder.all_envs[envname]
			options = coptions()
			task = self.gentask(builder, env, envname, 'cprogram', options, coptions())
			task.subsystem = 'console'
			task.do_install = 0
			task.features.append("unittest")

""" barely a C option holder """
class util(module):
	def __init__( self,
				  name,
				  depends = [],
				  category = '3rdparty',
				  localoptions = coptions(),
				  globaloptions = coptions(),
				  localarchoptions = {},
				  globalarchoptions = {},
				  platforms = allplatforms,
				  archs = allarchs,
				):
		module.__init__(self,
						name,
						depends,
						category,
						localoptions,
						globaloptions,
						localarchoptions,
						globalarchoptions,
						platforms,
						archs)

	def post(self, builder):
		for d in self.depends:
			d.post(builder)
		self.makeproject(builder)
		for envname in builder.env['BUILD_VARIANTS']:
			env = builder.all_envs[envname]
			task = self.gentask(builder, env, envname, 'dummy')
	
m={}
def external( name,
			  depends = [],
			  localoptions = coptions(),
			  globaloptions = coptions()):
	file = open(os.path.join('src', '3rdparty', name, 'wscript_build'), 'r')
	exec file
	if file: file.close()
	return m[name]
