import platform
from waflib.TaskGen import feature, before_method, after_method
from waflib.Configure import conf

def options(opt):
    # The options for MSVC are not used
    #opt.load('msvc')
    from waflib.Tools import msvc

def configure(conf):
    from waflib.Tools import msvc
    conf.start_msg('Looking for msvc compilers')
    conf.env.append_unique('useful_defines', ['__INTEL_COMPILER', '__clang__', '_MSC_VER'])
    try:
        conf.get_msvc_versions()
    except:
        pass
    conf.end_msg('done')

@conf
def load_msvc(self, version, target_arch):
    self.env.MSVC_VERSIONS = [version]
    self.env.MSVC_TARGETS = [target_arch]
    self.env.COMPILER_NAME='msvc'
    self.env.COMPILER_TARGET='windows-win32-msvc-%s'%version
    if platform.machine().endswith('64'):
        self.find_program('cdb64', var='CDB', mandatory=False)
    else:
        self.find_program('cdb', var='CDB', mandatory=False)
    self.load('msvc')

def build(bld):
    pass

@feature('c', 'cxx')
@after_method('process_source')
def apply_pdb_flag(self):
    if self.env.CC_NAME == 'msvc':
        for task in getattr(self, 'compiled_tasks', []):
            if task:
                task.env.append_unique('CFLAGS', '/Fd%s'%task.outputs[0].change_ext('.pdb').abspath())
                task.env.append_unique('CXXFLAGS', '/Fd%s'%task.outputs[0].change_ext('.pdb').abspath())
