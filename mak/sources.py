import os


class directory:
	def __init__(self):
		self.directories = {}
		self.files = []
		
	def addDirectory(self, dir, name):
		self.directories[name] = dir

	def addFile(self, file):
		self.files.append(file)
		self.files.sort()
		
	def addFullFile(self, filename, file):
		pathlist = []
		head = filename
		tail = filename
		while head != '' and tail != '':
			head,tail = os.path.split(head)
			if tail != '':
				pathlist.append(tail)
		pathlist.reverse()
		pwd = self
		for dir in pathlist[:-1]:
			if pwd.directories.has_key(dir):
				pwd = pwd.directories[dir]
			else:
				nextpwd = directory()
				pwd.addDirectory(nextpwd, dir)
				pwd = nextpwd
		pwd.addFile(file)

	def make_sources(self, bld, env, prefix, relative = ''):
		result = []
		for name, d in self.directories.iteritems():
			result += d.make_sources(bld, env, os.path.join(prefix, name), os.path.join(relative, name))
		for f in self.files:
			f.make_source(bld, env, prefix, relative, result)
		return result
		
	def __repr__(self):
		result =  ','.join(repr(value) for value in self.files)
		result += ';'+';'.join(name+':'+repr(value) for name,value in self.directories.iteritems())
		return result
		

class source:
	def __init__( self, filename, platforms, archs, process ):
		self.filename	= filename
		self.platforms	= platforms
		self.archs		= archs
		self.process	= process
	def make_source(self, bld, env, prefix, relative, result):
		if self.process and env['PLATFORM'] in self.platforms and env['ARCHITECTURE'] in self.archs:
			result.append(os.path.join(prefix,	self.filename))
	def __cmp__(self,other):
		return cmp(self.filename,other.filename)
	def __repr__(self):
		return self.filename
	def generated(self):
		return False

class cppsource(source):
	def __init__( self, filename, platforms, archs, process, usepch = True ):
		source.__init__( self, filename, platforms, archs, process )
		self.usepch = usepch

class generatedcppsource(cppsource):
	def __init__( self, filename, platforms, archs, process):
		cppsource.__init__( self, filename, platforms, archs, process, False )
	def make_source(self, bld, env, prefix, relative, result):
		pass
	def generated(self):
		return True

class hsource(source):
	def __init__( self, filename, platforms, archs, process ):
		source.__init__( self, filename, platforms, archs, process )
	def make_source(self, bld, env, prefix, relative, result):
		pass

class generatedhsource(source):
	def __init__( self, filename, platforms, archs, process ):
		source.__init__( self, filename, platforms, archs, process )
	def make_source(self, bld, env, prefix, relative, result):
		pass
	def generated(self):
		return True

class cssource(source):
	def __init__( self, filename, platforms, archs, process ):
		source.__init__( self, filename, platforms, archs, process )
	def make_source(self, bld, env, prefix, relative, result):
		pass
		
class deployedsource(source):
	def __init__( self, filename, outdir, type, platforms, archs, process ):
		source.__init__( self, filename, platforms, archs, process )
		self.outdir = outdir
		self.type = type
	def make_source(self, bld, env, prefix, relative, result):
		if self.process and env['PLATFORM'] in self.platforms and env['ARCHITECTURE'] in self.archs:
			bld.install_files(os.path.join(env['PREFIX'], env['DEPLOY']['prefix'], env['DEPLOY'][self.type], self.outdir), os.path.join(prefix, self.filename), env=env)

class lexsource(source):
	def __init__( self, filename, generatedcpp, platforms, archs, process ):
		source.__init__( self, filename, platforms, archs, process )
		self.generatedcpp = generatedcpp

class yaccsource(source):
	def __init__( self, filename, generatedcpp, generatedh, platforms, archs, process ):
		source.__init__( self, filename,platforms, archs, process )
		self.generatedcpp = generatedcpp
		self.generatedh = generatedh

