import os
try:
	import hashlib
	createMd5=hashlib.md5
except:
	import md5
	createMd5=md5.new

class directory:
	def __init__(self):
		self.directories = {}
		self.files = []

	def addDirectory(self, dir, name):
		self.directories[name] = dir
		dir.prefix = name

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
			if dir in pwd.directories:
				pwd = pwd.directories[dir]
			else:
				nextpwd = directory()
				pwd.addDirectory(nextpwd, dir)
				pwd = nextpwd
		pwd.addFile(file)

	def make_sources(self, bld, env, prefix, relative = ''):
		result = []
		for name, d in self.directories.items():
			result += d.make_sources(bld, env, os.path.join(prefix, d.prefix), os.path.join(relative, d.prefix))
		for f in self.files:
			f.make_source(bld, env, prefix, relative, result)
		return result

	def hash(self, md5 = None):
		if not md5:
			md5 = createMd5()
		for value in self.files:
			md5.update(','.encode())
			value.hash(md5)
		md5.update(';'.encode())
		for name, value in self.directories.items():
			md5.update(name.encode())
			md5.update(':'.encode())
			value.hash(md5)
		return md5.digest()


class source:
	def __init__( self, filename, platforms, archs, process ):
		self.filename	= filename
		self.platforms	= sorted([p for p in platforms])
		self.archs		= archs
		self.process	= process
	def make_source(self, bld, env, prefix, relative, result):
		if self.process and set(env['PLATFORM']) & set(self.platforms) and env['ARCHITECTURE'] in self.archs:
			result.append(os.path.join(prefix,	self.filename))
	def __lt__(self,other):
		return self.filename < other.filename
	def hash(self, md5):
		md5.update(self.filename.encode())
	def generated(self):
		return False

class dummysource(source):
	def __init__( self, filename, platforms, archs ):
		source.__init__( self, filename, platforms, archs, False )

class rcsource(source):
	def __init__( self, filename, platforms, archs, process ):
		source.__init__( self, filename, platforms, archs, process )

class cppsource(source):
	def __init__( self, filename, platforms, archs, process, usepch = True ):
		source.__init__( self, filename, platforms, archs, process )
		self.usepch = usepch

class generatedcppsource(cppsource):
	def __init__( self, filename, platforms, archs, process, usepch = False ):
		cppsource.__init__( self, filename, platforms, archs, process, usepch )
	def make_source(self, bld, env, prefix, relative, result):
		pass
	def generated(self):
		return True

class hsource(source):
	def __init__( self, filename, platforms, archs, process ):
		source.__init__( self, filename, platforms, archs, process )
	def make_source(self, bld, env, prefix, relative, result):
		pass

class generatedhsource(hsource):
	def __init__( self, filename, platforms, archs, process ):
		hsource.__init__( self, filename, platforms, archs, process )
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
		if self.process and set(env['PLATFORM']) & set(self.platforms) and env['ARCHITECTURE'] in self.archs:
			bld.install_files(os.path.join(env['PREFIX'], env['DEPLOY']['prefix'], env['DEPLOY'][self.type], self.outdir), os.path.join(prefix, self.filename), env=env)

class datasource(source):
	def __init__( self, filename, generatedcpp, platforms, archs, process ):
		source.__init__( self, filename, platforms, archs, process )
		self.generatedcpp = generatedcpp

class lexsource(source):
	def __init__( self, filename, generatedcpp, platforms, archs, process ):
		source.__init__( self, filename, platforms, archs, process )
		self.generatedcpp = generatedcpp

class yaccsource(source):
	def __init__( self, filename, generatedcpp, generatedh, platforms, archs, process ):
		source.__init__( self, filename,platforms, archs, process )
		self.generatedcpp = generatedcpp
		self.generatedh = generatedh

