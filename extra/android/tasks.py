from waflib.TaskGen import feature, before_method, after_method, extension
from waflib import Context, Task, Node, Utils, Errors, Build, TaskGen
import os

DEX_RE = '**/*.class'

class aapt_create(Task.Task):
	"""
	Create an apk file
	"""
	color   = 'PINK'
	run_str = '${AAPT} package ${AAPT_FLAGS} -M ${MANIFEST} -S ${RESOURCE_PATH} -F ${TGT}'

class copy(Task.Task):
	"""
	Copy file from input to output
	"""
	color   = 'PINK'
	
	def run(self):
		self.outputs[0].write(self.inputs[0].read())
		return 0

class aapt_package(Task.Task):
	"""
	Store files in an apk file
	"""
	color   = 'PINK'
	
	def run(self):
		bld = self.generator.bld
		root = bld.bldnode
		self.outputs[0].write(self.inputs[0].read())
		cmd = [self.env.AAPT, 'add', self.outputs[0].abspath()] + [i.path_from(root).replace('\\', '/') for i in self.inputs[1:]]
		return self.exec_command(cmd)

class jarsigner(Task.Task):
	"""
	Signs jar file
	"""
	color   = 'PINK'
	run_str = '${JARSIGNER} ${JARSIGNER_FLAGS} -signedjar ${TGT} ${SRC} ${JARSIGNER_KEY}'

class zipalign(Task.Task):
	"""
	Align zip file on 4
	"""
	color   = 'PINK'
	run_str = '${ZIPALIGN} -f 4 ${SRC} ${TGT}'

class dex(Task.Task):
	"""
	Create a dex file
	"""
	color   = 'GREEN'
	run_str = '${JAVA} -jar ${DEX} ${DEXCREATE} ${DEX_TGT_PATTERN:OUTPUT_FILES} ${DEXOPTS} ${INPUT_FILES}'

	def runnable_status(self):
		"""
		Wait for dependent tasks to be executed, then read the
		files to update the list of inputs.
		"""
		for t in self.run_after:
			if not t.hasrun:
				return Task.ASK_LATER
		if not self.inputs:
			global DEX_RE
			try:
				self.inputs = [x for x in self.outdir.ant_glob(DEX_RE, remove=False) if id(x) != id(self.outputs[0])]
				self.env.INPUT_FILES = [x.path_from(self.basedir) for x in self.inputs]
				self.env.OUTPUT_FILES = [x.path_from(self.basedir) for x in self.outputs]
			except Exception:
				raise Errors.WafError('Could not find the basedir %r for %r' % (self.basedir, self))
		return super(dex, self).runnable_status()

@feature('aapt_resource')
def aapt_resource(self):
	if 'android' in self.env.VALID_PLATFORMS:
		self.resource_task = tsk = self.create_task('aapt_create', self.resource.ant_glob('**/*'), [self.destfile])
		tsk.env.MANIFEST = self.manifest.abspath()
		tsk.env.RESOURCE_PATH = self.resource.abspath()
		tsk.dep_nodes = [self.manifest]
		nodes = [self.resource]
		while nodes:
			node = nodes.pop()
			if os.path.isdir(node.abspath()):
				nodes.extend([node.make_node(i) for i in node.listdir()])
			else:
				tsk.dep_nodes.append(node)
	

@feature('dex')
@after_method('apply_java')
@before_method('process_source')
def dex_files(self):
	"""
	Create a dex task. There can be only one dex task by task generator.
	"""
	if 'android' in self.env.VALID_PLATFORMS:
		destfile = self.destfile
		dexopts = getattr(self, 'dexopts', [])
		basedir = self.basedir
		outdir = self.outdir

		self.dex_task = tsk = self.create_task('dex')
		if not isinstance(destfile, Node.Node):
			destfile = self.path.find_or_declare(destfile)
		if not destfile:
			self.bld.fatal('invalid destfile %r for %r' % (destfile, self))
		tsk.set_outputs(destfile)
		tsk.basedir = basedir
		tsk.outdir = outdir
		tsk.cwd = outdir.abspath()

		if getattr(self, 'javac_task', None):
			tsk.set_run_after(self.javac_task)


original_install_files = Build.InstallContext.install_files
original_install_as = Build.InstallContext.install_as
def get_package_task(bld):
	try:
		package_task = bld.android_package_task
	except AttributeError:
		appname = getattr(Context.g_module, Context.APPNAME, bld.srcnode.name)
		tg = TaskGen.task_gen(bld=bld)
		package_unsigned = bld.bldnode.find_or_declare(appname + '.unsigned.apk')
		package_unaligned = bld.bldnode.find_or_declare(appname + '.unaligned.apk')
		package_final = bld.bldnode.find_or_declare(appname + '.apk')

		package_task = bld.android_package_task = tg.create_task('aapt_package', [bld.bldnode.find_or_declare('resources.apk')], [package_unsigned])
		sign_task = tg.create_task('jarsigner', [package_unsigned], [package_unaligned])
		align_task = tg.create_task('zipalign', [package_unaligned], [package_final])

		bld.add_to_group(tg)
		bld.run_task_now(tg, True)
		original_install_as(bld, os.path.join(bld.env.PREFIX, bld.optim, bld.env.DEPLOY_ROOTDIR+'.apk'), package_final)
	return package_task

def install_files(bld, dest, files, env=None, chmod=Utils.O644, relative_trick=False, cwd=None, add=True, postpone=True, package=True):
	if package:
		if not isinstance(files, list):
			files = [files]
		package_task = get_package_task(bld)
		root_path = os.path.join(bld.env.PREFIX, bld.optim)
		if not dest.startswith(root_path):
			raise Errors.WafError('Does not know how to deploy to %s'%dest)
		dest = dest[len(root_path)+1:]
		for file in files:
			filename = file.path_from(bld.bldnode)
			base = os.path.dirname(filename)
			if base != dest:
				dest_node = bld.bldnode.find_or_declare(os.path.join(dest, file.name))
				package_task.generator.create_task('copy', [file], [dest_node])
			else:
				dest_node = file
			package_task.set_inputs([dest_node])
	else:
		return original_install_files(bld, dest, files, env, chmod, relative_trick, cwd, add, postpone)

def install_as(bld, dest, source, env=None, chmod=Utils.O644, cwd=None, add=True, postpone=True, package=True):
	if package:
		package_task = get_package_task(bld)
		root_path = os.path.join(bld.env.PREFIX, bld.optim)
		if not dest.startswith(root_path):
			raise Errors.WafError('Does not know how to deploy to %s'%dest)
		dest_node = bld.bldnode.find_or_declare(dest[len(root_path)+1:])
		package_task.generator.create_task('copy', [source], [dest_node])
		package_task.set_inputs([dest_node])
	else:
		return original_install_as(bld, dest, source, env, chmod, cwd, add, postpone)

def package_file(bld, dest, source):
	package_task = get_package_task(bld)


def build(bld):
	if 'android' in bld.env.VALID_PLATFORMS:
		if bld.is_install == Build.INSTALL:
			bld.__class__.install_files = install_files
			bld.__class__.install_as = install_as
			for envname in bld.env.SUB_TOOLCHAINS:
				env = bld.all_envs[envname]
				bld.install_as(os.path.join(bld.env.PREFIX, bld.optim, env.DEPLOY_BINDIR, 'gdbserver'), bld.path.make_node('prebuilt/%s/gdbserver'%bld.env.ANDROID_ARCH))