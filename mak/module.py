import os
import mak
from mak import sources
from waflib import Logs

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
		for lib in other.libs:
			if lib not in self.libs:
				self.libs.append(lib)
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
				  dstname,
				  depends,
				  category,
				  localoptions,
				  globaloptions,
				  localarchoptions,
				  globalarchoptions,
				  platforms,
				  archs,
				  sourcelist,
				):
		while None in depends:
			depends.remove(None)
		self.name	  = name
		self.sname = self.name.split('.')[-1]
		self.dstname = dstname
		self.category = category
		self.depends  = depends
		self.tasks = {}
		self.root = os.path.join('src', category, name.replace('.', '/'))
		self.platforms = set([])
		self.kernels = {}
		for p in platforms or mak.allplatforms.keys():
			for pname, pgroup in mak.allplatforms.items():
				if p in pgroup:
					self.platforms.add(pname)
		self.archs = archs or mak.allarchs[:]
		self.projects = {}
		sourcelist = [os.path.normpath(i) for i in sourcelist]

		self.localoptions = coptions()
		self.localoptions.merge(localoptions)
		self.localoptions.defines |= set([ 'building_'+self.sname,
										   'BE_PROJECTSHORTNAME='+self.sname,
										   'BE_PROJECTNAME='+self.name])
		self.globaloptions = coptions()
		self.globaloptions.merge(globaloptions)
		self.localarchoptions = localarchoptions
		self.globalarchoptions = {}
		self.gobaloptioncache = { }
		self.usemaster = True

		for k in globalarchoptions.keys():
			self.globalarchoptions[k] = coptions()
			self.globalarchoptions[k].merge(globalarchoptions[k])


		self.usepch = 0
		if os.path.isfile(os.path.join(self.root, 'src', 'stdafx.cpp')):
			self.pchsource = os.path.join(self.root, 'src', 'stdafx.cpp')
			if os.path.isfile(os.path.join(self.root, 'api', self.sname, 'stdafx.h')):
				self.pchheader = os.path.join(self.sname, 'stdafx.h')
				self.pchfullheader = os.path.join(self.root, 'api', self.sname, 'stdafx.h')
			elif os.path.isfile(os.path.join(self.root, 'include', 'stdafx.h')):
				self.pchheader = 'stdafx.h'
				self.pchfullheader = os.path.join(self.root, 'include', 'stdafx.h')
			self.localoptions.pchname = self.pchsource
			self.localoptions.pchstop = self.pchheader
			self.usepch = 1

		if os.path.isdir(os.path.join(self.root, 'include')):
			self.localoptions.includedir.add(os.path.join(self.root, 'include'))
		if os.path.isdir(os.path.join(self.root, 'api')):
			self.globaloptions.includedir.add(os.path.join(self.root, 'api'))

		self.sourcetree = sources.directory()
		self.sourcetree.prefix = self.root
		if os.path.isdir(os.path.join(self.root, 'include')):
			self.sourcetree.addDirectory(self.scandir(os.path.join(self.root, 'include'), '', False, self.platforms, self.archs), 'include')
		if os.path.isdir(os.path.join(self.root, 'api')):
			self.sourcetree.addDirectory(self.scandir(os.path.join(self.root, 'api'), '', False, self.platforms, self.archs), 'api')
		if os.path.isdir(os.path.join(self.root, 'data')):
			self.sourcetree.addDirectory(self.scandir(os.path.join(self.root, 'data'), '', True, self.platforms, self.archs, sourcelist, True), 'data')
		if os.path.isdir(os.path.join(self.root, 'src')):
			self.sourcetree.addDirectory(self.scandir(os.path.join(self.root, 'src'), '', True, self.platforms, self.archs, sourcelist), 'src')
		if os.path.isdir(os.path.join(self.root, 'kernels')):
			kernelDirectory = sources.directory()
			self.sourcetree.addDirectory(kernelDirectory, 'kernels')
			for kernel in os.listdir(os.path.join(self.root, 'kernels')):
				kernelDirectory.addDirectory(self.scandir(os.path.join(self.root, 'kernels', kernel), '', False, self.platforms, self.archs, sourcelist), kernel)
				self.kernels[kernel] = sources.directory()
				kernelsdir = sources.directory()
				kernelsdir.addDirectory(self.scandir(os.path.join(self.root, 'kernels', kernel), '', True, self.platforms, self.archs, sourcelist), kernel)
				self.kernels[kernel].addDirectory(kernelsdir, 'kernels')

		platformsdirectory = sources.directory()
		if os.path.isdir('extra'):
			for platform in os.listdir(os.path.join('extra')):
				extraroot = os.path.join('extra', platform, category, name.replace('.', '/'))
				pdir = sources.directory()
				if os.path.isdir(os.path.join(extraroot, 'api')):
					pdir.addDirectory(self.scandir(os.path.join(extraroot, 'api'), '', 0, [platform], self.archs), 'api')
					try:
						self.globalarchoptions[platform].includedir.add(os.path.join(extraroot, 'api'))
					except KeyError:
						self.globalarchoptions[platform] = coptions([os.path.join(extraroot, 'api')])
				if os.path.isdir(os.path.join(extraroot, 'include')):
					pdir.addDirectory(self.scandir(os.path.join(extraroot, 'include'), '', 0, [platform], self.archs), 'include')
					try:
						self.localarchoptions[platform].includedir.add(os.path.join(extraroot, 'include'))
					except KeyError:
						self.localarchoptions[platform] = coptions([os.path.join(extraroot, 'include')])
				if os.path.isdir(os.path.join(extraroot, 'src')):
					pdir.addDirectory(self.scandir(os.path.join(extraroot, 'src'), '', True, [platform], self.archs, sourcelist), 'src')
				if os.path.isdir(os.path.join(extraroot, 'data')):
					pdir.addDirectory(self.scandir(os.path.join(extraroot, 'data'), '', True, [platform], self.archs, sourcelist, True), 'src')
				if pdir.directories or pdir.files:
					platformsdirectory.addDirectory(pdir, platform)
					pdir.prefix = os.path.join(platform, category, name.replace('.', '/'))
					self.sourcetree.addDirectory(platformsdirectory, 'platforms')
			goback = '/'.join(['..' for i in ['src', category]+name.split('.')])
			platformsdirectory.prefix = os.path.join(goback, 'extra')

		for arch in mak.allarchs:
			if os.path.isdir(os.path.join(self.root, 'lib.'+arch)):
				self.sourcetree.addDirectory(self.scandir(os.path.join(self.root, 'lib.'+arch), '', 0, self.platforms, [arch]), 'lib.'+arch)
			if os.path.isdir(os.path.join(self.root, 'bin.'+arch)):
				self.sourcetree.addDirectory(self.scandir(os.path.join(self.root, 'bin.'+arch), '', 0, self.platforms, [arch]), 'bin.'+arch)

	def getoptions(self, platforms, arch):
		options = coptions()
		options.merge(self.localoptions)
		options.merge(self.getglobaloptions(platforms, arch))

		for key,aoptions in self.localarchoptions.items():
			try:
				p,a = key.split('-')
				if a == arch and p in platforms:
					options.merge(aoptions)
			except:
				if key == arch or key in platforms:
					options.merge(aoptions)
		return options

	def getglobaloptions(self, platforms, arch):
		options = coptions()
		try:
			options.merge(self.gobaloptioncache[str(platforms)+"-"+arch])
		except KeyError:
			if not set(platforms) & self.platforms or not arch in self.archs:
				return options
			options.merge(self.globaloptions)
			for d in self.depends:
				options.merge(d.getglobaloptions(platforms, arch))
			for key,aoptions in self.globalarchoptions.items():
				try:
					p,a = key.split('-')
					if a == arch and p in platforms:
						options.merge(aoptions)
				except:
					if key == arch or key in platforms:
						options.merge(aoptions)
			self.gobaloptioncache[str(platforms)+"-"+arch] = options
		return options

	def gentask(self, bld, type, options = coptions(), inheritedoptions = coptions(), extradepends = [], blacklist=[]):
		env = bld.env
		variant = bld.variant
		if not variant in self.tasks:
			if env.PROJECTS:
				task					= bld()
				task.sname				= self.sname
				task.category			= self.category
				task.target				= self.dstname
				task.env				= env.derive()
				task.env.detach()
				task.type				= self.__class__.__name__
				task.features			= ['c', 'cxx', type]
				task.sourcetree			= self.sourcetree
				task.options = {}
				for envname in bld.env.ALL_VARIANTS:
					env = bld.all_envs[envname]
					task.options[envname]	= self.getoptions(env['PLATFORM'], env['ARCHITECTURE'])
			elif type=='dummy':
				task = None
				# will deploy files that were scheduled to be deployed
				self.sourcetree.make_sources(bld, env, self.root)
			elif not set(env['PLATFORM']) & self.platforms or not env['ARCHITECTURE'] in self.archs:
				task = None
				# will deploy files that were scheduled to be deployed
				self.sourcetree.make_sources(bld, env, self.root)
			else:
				optim,compiler,platform,architecture,version = variant.split('-')[0:5]
				task					= bld()
				task.sname				= self.sname
				task.category			= self.category
				task.target				= self.dstname
				task.env				= env.derive()
				task.env.detach()
				task.type				= self.__class__.__name__
				task.features			= ['c', 'cxx', type]
				task.usemaster			= self.usemaster

				task.inheritedoptions	= coptions()
				task.inheritedoptions.merge(inheritedoptions)
				task.use = []
				if self.category != '3rdparty':
					task.features.append('warnall')
				else:
					task.features.append('warnnone')
				task.features.append(optim)
				task.install_path = os.path.abspath(os.path.join(env['PREFIX'],env['DEPLOY']['prefix'],env['DEPLOY'][self.install_path]))
				dps = self.depends + extradepends
				seen = set()
				while dps:
					d = dps.pop(0)
					if d == None:
						continue
					seen.add(d)
					if d not in blacklist:
						t = d.tasks[variant]
						if t:
							task.use.append(d.dstname)
							task.inheritedoptions.merge(t.inheritedoptions)
						dps += [dep for dep in d.depends if dep not in seen]
				task.options			= self.getoptions(env['PLATFORM'], env['ARCHITECTURE'])
				if env['STATIC'] or bld.static:
					task.options.defines.add('BE_STATIC')
				for d in extradepends:
					task.options.merge(d.getglobaloptions(env['PLATFORM'], env['ARCHITECTURE']))
				task.options.merge(options)
				task.options.merge(task.inheritedoptions)
				try:
					task.pchsource = self.pchsource
					task.pchheader = self.pchheader
					task.pchfullheader = self.pchfullheader
				except AttributeError:
					pass

				task.private_defines	= [d for d in task.options.defines]
				task.private_includes	= [i for i in task.options.includedir]
				task.libpath		= [os.path.abspath(l) for l in task.options.libdir]
				task.lib			= [l for l in task.options.libs]

				task.source				= self.sourcetree.make_sources(bld, env, self.root)
				if task.usemaster and task.source:
					task.features.append('master')
				task.do_install			= 1
				for kernelname, kernelsources in self.kernels.items():
					for envname in bld.env.KERNELS:
						if env.STATIC or bld.static:
							jobtype = 'cstlib'
						else:
							jobtype = 'cshlib'
						env = bld.all_envs[envname].derive()
						env.detach()
						job = bld(
								target = task.name + '.' + kernelname,
								env = env,
								type='job',
								source = kernelsources.make_sources(bld, env, self.root),
								features = ['c', 'cxx', jobtype, 'warnall', optim],
								install_path = os.path.abspath(os.path.join(env['PREFIX'],env['DEPLOY']['prefix'],env['DEPLOY']['kernel'])),
								defines = task.private_defines,
								includes = task.private_includes
							)
						job.post()
						if env.STATIC or bld.static:
							task.use.append(job.target)

			self.tasks[variant]		= task
		return self.tasks[variant]

	def scandir(self, path, local, process, platforms, archs, sourcelist = [], deploy_all=False):
		result = sources.directory()

		for file in os.listdir(path):
			if file == '.svn':
				continue
			elif file == '.cvs':
				continue
			elif os.path.isdir(os.path.join(path,file)):
				if file[0:9] == 'platform=':
					newplatforms = []
					for p in file[9:].split(','):
						for pname, pgroup in mak.allplatforms.items():
							if p in pgroup and pname in platforms:
								newplatforms.append(pname)
					result.addDirectory( self.scandir(os.path.join(path,file), os.path.join(local,file), process, newplatforms, archs, sourcelist, deploy_all), file )
				elif file[0:5] == 'arch=':
					newarchs = [a for a in file[5:].split(',') if a in archs]
					result.addDirectory( self.scandir(os.path.join(path,file), os.path.join(local,file),process, platforms, newarchs, sourcelist, deploy_all), file )
				else:
					result.addDirectory( self.scandir(os.path.join(path,file), os.path.join(local,file),process, platforms, archs, sourcelist, deploy_all), file )
			else:
				filename, ext = os.path.splitext(file)
				fileplatforms = platforms
				filearchs = archs
				if filename.find('-') != -1:
					_, specials = filename.split('-')
					if specials.startswith("p="):
						fileplatforms = []
						for p in specials[2:].split(','):
							for pname, pgroup in mak.allplatforms.items():
								if p in pgroup and pname in platforms:
									fileplatforms.append(pname)
					elif specials.startswith("a="):
						filearchs = specials[2:].split(',')

				fullname = os.path.join(path, file)
				if sourcelist:
					if fullname in sourcelist or os.path.join(local,file) in sourcelist:
						doprocess = process
					else:
						doprocess = False
				else:
					doprocess = process

				newexts = { '.ll':('.cc','.hh'), '.yy': ('.cc','.hh'), '.l':('.c', '.h'), '.y':('.c', '.h') }
				if ext in set(['.cc', '.cpp', '.cxx', '.c', '.C', '.s', '.S', '.bin', '.grit', '.m', '.mm']):
					result.addFile(sources.cppsource(file, fileplatforms, filearchs, doprocess, self.usepch))
				elif ext in set(['.rc']):
					result.addFile(sources.rcsource(file, fileplatforms, filearchs, doprocess))
				elif ext in set(['.y', '.yy']):
					cext, hext = newexts[ext]
					generatedcfile = os.path.join(path, filename+cext)
					generatedhfile = os.path.join(path, filename+hext)
					result.addFile(sources.yaccsource(file, generatedcfile, generatedhfile, fileplatforms, filearchs, doprocess))
					result.addFile(sources.generatedcppsource(filename+cext, fileplatforms, filearchs, doprocess))
					result.addFile(sources.generatedhsource(filename+hext, fileplatforms, filearchs, doprocess))
				elif ext in set(['.l', '.ll']):
					cext, hext = newexts[ext]
					generatedcfile = os.path.join(path, filename+cext)
					result.addFile(sources.lexsource(file, generatedcfile, fileplatforms, filearchs, doprocess))
					result.addFile(sources.generatedcppsource(filename+cext, fileplatforms, filearchs, doprocess))
				elif ext in set(['.def']):
					self.localoptions.deffile = fullname
					result.addFile(sources.source(file, fileplatforms, filearchs, doprocess))
					result.addFile(sources.hsource(file, fileplatforms, filearchs, doprocess))
				elif ext in set(['.h', '.hpp', '.hh', '.hxx', '.inl']):
					if file.endswith('.script'+ext):
						generatedcfile = os.path.join(path, filename+'.cc')
						result.addFile(sources.datasource(file, generatedcfile, fileplatforms, filearchs, True))
						result.addFile(sources.generatedcppsource(filename+'.cc', fileplatforms, filearchs, True, self.usepch))
					else:
						result.addFile(sources.hsource(file, fileplatforms, filearchs, doprocess))
				elif ext == '.plist':
					result.addFile(sources.deployedsource(file, '', 'root', fileplatforms, filearchs, doprocess))
				elif deploy_all:
					result.addFile(sources.deployedsource(file, '', 'data', fileplatforms, filearchs, doprocess))
		return result

	def addFile( self, filename ):
		s = sources.dummysource(os.path.split(filename)[1], mak.allplatforms.keys(), mak.allarchs[:])
		self.sourcetree.addFullFile( filename, s )

	def deploy( self, filename, outputdir, deploytype, platforms = None, archs = None ):
		if not platforms: platforms=mak.allplatforms.keys()
		if not archs: archs=mak.allarchs[:]
		self.sourcetree.addFullFile( filename, sources.deployedsource(os.path.split(filename)[1], outputdir, deploytype, platforms, archs, 1) )

	def deployDirectory( self, dirname, outputdir, deploytype, platforms = None, archs = None ):
		for file in os.listdir(os.path.join(self.root,dirname)):
			if file == '.svn':
				continue
			elif file == '.cvs':
				continue
			if os.path.isdir(os.path.join(self.root,dirname,file)):
				if file[0:9] == 'platform=':
					self.deployDirectory(os.path.join(dirname,file),os.path.join(outputdir,file),deploytype, file[9:].split(','), archs)
				elif file[0:5] == 'arch=':
					self.deployDirectory(os.path.join(dirname,file),os.path.join(outputdir,file),deploytype, platforms, file[5:].split(','))
				else:
					self.deployDirectory(os.path.join(dirname,file),os.path.join(outputdir,file),deploytype, platforms,archs)
			else:
				self.deploy(os.path.join(dirname,file),outputdir,deploytype,platforms, archs)

	def post(self, builder):
		self._post(builder)
		return self

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
				  platforms = [],
				  archs = [],
				  sources=[],
				  dstname = None,
				):
		self.install_path = 'runbin'
		module.__init__(self,
						name,
						dstname or name,
						depends,
						category,
						localoptions,
						globaloptions,
						localarchoptions,
						globalarchoptions,
						platforms,
						archs,
						sources)

	def _post(self, builder, blacklist=[]):
		for d in self.depends:
			if d not in blacklist:
				d._post(builder, blacklist)
		options = coptions()
		if builder.dynamic:
			options.defines.add('BUILDING_DLL')
			options.defines.add('_USRDLL')
			task = self.gentask(builder, 'cxxshlib', options, blacklist=blacklist)
		else:
			task = self.gentask(builder, 'cobjects', options, blacklist=blacklist)



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
				  platforms = [],
				  archs = [],
				  sources=[],
				  dstname = None,
				):
		self.install_path = 'bin'
		module.__init__(self,
						name,
						dstname or name,
						depends,
						category,
						localoptions,
						globaloptions.merge(coptions( defines = [name+'_dll'] )),
						localarchoptions,
						globalarchoptions,
						platforms,
						archs,
						sources)

	def _post(self, builder, blacklist=[]):
		for d in self.depends:
			if d not in blacklist:
				d._post(builder, blacklist)
		options = coptions()
		options.defines.add('BUILDING_DLL')
		options.defines.add('_USRDLL')
		task = self.gentask(builder, 'cshlib', options, coptions(), blacklist=blacklist)

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
				  platforms = [],
				  archs = [],
				  sources=[],
				  dstname = None,
				):
		self.install_path = 'lib'
		module.__init__(self,
						name,
						dstname or name,
						depends,
						category,
						localoptions,
						globaloptions,
						localarchoptions,
						globalarchoptions,
						platforms,
						archs,
						sources)

	def _post(self, builder, blacklist):
		for d in self.depends:
			if d not in blacklist:
				d._post(builder, blacklist)
		options = coptions()
		task = self.gentask(builder, 'cstlib', options, coptions(), blacklist=blacklist)

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
				  platforms = [],
				  archs = [],
				  sources=[],
				  dstname = None,
				):
		self.install_path = 'plugin'
		module.__init__(self,
						name,
						dstname or name,
						[mak.builder.game]+depends,
						category,
						localoptions,
						globaloptions,
						localarchoptions,
						globalarchoptions,
						platforms,
						archs,
						sources)
		try:
			mak.builder.game.plugins[name] = self
		except:
			mak.builder.game.plugins = { name:self }

	def _post(self, builder, blacklist):
		for d in self.depends:
			if d not in blacklist:
				d._post(builder, blacklist)
		if builder.env['STATIC'] or builder.static:
			options = coptions()
			task = self.gentask(builder, 'cobjects', options, coptions(), blacklist=blacklist)
		else:
			options = coptions()
			options.defines.add('BUILDING_DLL')
			options.defines.add('_USRDLL')
			task = self.gentask(builder, 'cshlib', options, coptions(), blacklist = blacklist)
			if task and not builder.env.PROJECTS:
				task.install_bindir = os.path.join(builder.env['DEPLOY']['plugin'])


""" game """
class game(plugin):
	def __init__( self,
				  name,
				  depends = [],
				  category = 'game',
				  localoptions = coptions(),
				  globaloptions = coptions(),
				  localarchoptions = {},
				  globalarchoptions = {},
				  platforms = [],
				  archs = [],
				  sources=[],
				  dstname = None,
				):
		plugin.__init__(self,
						name,
						depends,
						category,
						localoptions,
						globaloptions,
						localarchoptions,
						globalarchoptions,
						platforms,
						archs,
						sources,
						dstname)


""" executable """
class engine(module):
	def __init__( self,
				  name,
				  depends = [],
				  category = 'engine',
				  localoptions = coptions(),
				  globaloptions = coptions(),
				  localarchoptions = {},
				  globalarchoptions = {},
				  platforms = [],
				  archs = [],
				  sources=[],
				  dstname = None,
				):
		self.install_path = 'bin'
		module.__init__(self,
						name,
						dstname or name,
						depends,
						category,
						localoptions,
						globaloptions,
						localarchoptions,
						globalarchoptions,
						platforms,
						archs,
						sources)

	def _post(self, builder, blacklist=[]):
		for d in self.depends:
			if d not in blacklist:
				d._post(builder,blacklist)
		options = coptions()
		if builder.env['STATIC'] or builder.static:
			for name,d in self.plugins.items():
				if d:
					d._post(builder,[self]+blacklist)
			task = self.gentask(builder, 'cprogram', options, extradepends=list(self.plugins.values()), blacklist=[self]+blacklist)
		elif not builder.variant in self.tasks:
			task = self.gentask(builder, 'cprogram', options, blacklist=blacklist)
			for name,d in self.plugins.items():
				if d:
					d._post(builder, blacklist)

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
				  platforms = [],
				  archs = [],
				  sources=[],
				  dstname = None,
				):
		module.__init__(self,
						name,
						dstname or name,
						depends,
						category,
						localoptions,
						globaloptions,
						localarchoptions,
						globalarchoptions,
						platforms,
						archs,
						sources)
		self.usemaster = False

	def _post(self, builder, blacklist=[]):
		for d in self.depends:
			d._post(builder,  blacklist)
		task = self.gentask(builder, 'dummy')

def external(name):
	return mak.builder.m[name]
