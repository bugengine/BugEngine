from waflib import Configure
from waflib.TaskGen import feature, before_method, after_method
from waflib.Tools import msvc
import os


class MSVC(Configure.ConfigurationContext.Compiler):
    def __init__(self, cl, name, version, target_arch, arch, bat, args, path, includes, libdirs):
        self.NAMES = [name, 'msvc']
        p = os.pathsep.join([os.environ.get('PATH', '')] + path)
        flags = ['/I%s'%i for i in includes] + ['/LIBPATH:%i' for l in libdirs]
        Configure.ConfigurationContext.Compiler.__init__(self, cl, cl, version, 'windows-%s'%name, arch, {}, {'PATH': p})
        self.batfile = bat
        self.args = args
        self.arch_name = target_arch
        self.includes = includes
        self.libdirs = libdirs
        self.target = self.platform

    def set_optimisation_options(self, conf):
        conf.env.append_unique('CPPFLAGS_debug', ['/Od', '/Ob1', '/EHsc', '/RTC1', '/RTCc', '/Zi',
                                                  '/MTd', '/D_DEBUG'])
        conf.env.append_unique('CFLAGS_debug', ['/Od', '/Ob1', '/EHsc', '/RTC1', '/RTCc', '/Zi',
                                                '/MTd', '/D_DEBUG'])
        conf.env.append_unique('CXXFLAGS_debug', ['/Od', '/Ob1', '/EHsc', '/RTC1', '/RTCc', '/Zi',
                                                  '/MTd', '/D_DEBUG', '/GR'])
        conf.env.append_unique('LINKFLAGS_debug', ['/DEBUG', '/INCREMENTAL:no'])
        conf.env.append_unique('ARFLAGS_debug', [])

        conf.env.append_unique('CPPFLAGS_profile', ['/DNDEBUG', '/MT', '/Ox', '/Ob2', '/Oi', '/Ot',
                                                    '/Oy', '/GT', '/GL', '/GF', '/FD', '/GS-', '/Gy',
                                                    '/GR-'])
        conf.env.append_unique('CFLAGS_profile', ['/DNDEBUG', '/MT', '/Ox', '/Ob2', '/Oi', '/Ot',
                                                  '/Oy', '/GT', '/GL', '/GF', '/FD', '/GS-', '/Gy',
                                                  '/GR-'])
        conf.env.append_unique('CXXFLAGS_profile', ['/DNDEBUG', '/D_HAS_EXCEPTIONS=0', '/MT', '/Ox',
                                                    '/Ob2', '/Oi', '/Ot', '/Oy', '/GT', '/GL',
                                                    '/GF', '/FD', '/GS-', '/Gy', '/GR-'])
        conf.env.append_unique('LINKFLAGS_profile', ['/DEBUG', '/LTCG', '/INCREMENTAL:no'])
        conf.env.append_unique('ARFLAGS_profile', ['/LTCG'])

        conf.env.append_unique('CPPFLAGS_final', ['/DNDEBUG', '/MT', '/Ox', '/Ob2', '/Oi', '/Ot',
                                                  '/Oy', '/GT', '/GL', '/GF', '/FD', '/GS-', '/Gy',
                                                  '/GR-'])
        conf.env.append_unique('CFLAGS_final', ['/DNDEBUG', '/MT', '/Ox', '/Ob2', '/Oi', '/Ot',
                                                '/Oy', '/GT', '/GL', '/GF', '/FD', '/GS-', '/Gy',
                                                '/GR-'])
        conf.env.append_unique('CXXFLAGS_final', ['/DNDEBUG', '/D_HAS_EXCEPTIONS=0', '/MT', '/Ox',
                                                  '/Ob2', '/Oi', '/Ot', '/Oy', '/GT', '/GL', '/GF',
                                                  '/FD', '/GS-', '/Gy', '/GR-'])
        conf.env.append_unique('LINKFLAGS_final', ['/DEBUG', '/LTCG', '/INCREMENTAL:no'])
        conf.env.append_unique('ARFLAGS_final', ['/LTCG'])

    def set_warning_options(self, conf):
        conf.env.append_unique('CFLAGS_warnall', ['/D_CRT_SECURE_NO_WARNINGS=1', '/W4', '/WX'])
        conf.env.append_unique('CFLAGS_warnnone', ['/D_CRT_SECURE_NO_WARNINGS=1', '/W0'])
        conf.env.append_unique('CXXFLAGS_warnall', ['/D_CRT_SECURE_NO_WARNINGS=1', '/W4', '/WX'])
        conf.env.append_unique('CXXFLAGS_warnnone', ['/D_CRT_SECURE_NO_WARNINGS=1', '/W0'])
        if self.NAMES[0] == 'msvc' and self.version_number >= (14,):
            conf.env.append_unique('CFLAGS_warnall', ['/D_ALLOW_RTCc_IN_STL=1'])
            conf.env.append_unique('CXXFLAGS_warnall', ['/D_ALLOW_RTCc_IN_STL=1'])
            conf.env.append_unique('CFLAGS_warnnone', ['/D_ALLOW_RTCc_IN_STL=1'])
            conf.env.append_unique('CXXFLAGS_warnnone', ['/D_ALLOW_RTCc_IN_STL=1'])

    def load_tools(self, conf, platform):
        env = conf.env
        version = '%s %s'%(self.NAMES[0], self.version)
        env.MSVC_VERSIONS = [version]
        env.MSVC_TARGETS = [self.arch_name]
        env.MSVC_BATFILE = [self.batfile, self.args]
        env.COMPILER_NAME='msvc'
        env.COMPILER_TARGET='windows-win32-msvc-%s'%version
        conf.load('msvc')

    def load_in_env(self, conf, platform):
        Configure.ConfigurationContext.Compiler.load_in_env(self, conf, platform)
        env = conf.env
        env.IDIRAFTER='/I'
        env.CC_CPP = env.CC
        env.CC_CPP_SRC_F = ''
        env.CC_CPP_TGT_F = ['/TC', '-X', '/P', '/Fi']
        if os_platform().endswith('64'):
            conf.find_program('cdb64', var='CDB', mandatory=False)
        else:
            conf.find_program('cdb', var='CDB', mandatory=False)


def os_platform():
    true_platform = os.environ['PROCESSOR_ARCHITECTURE']
    try:
            true_platform = os.environ["PROCESSOR_ARCHITEW6432"]
    except KeyError:
            pass
            #true_platform not assigned to if this does not exist
    return true_platform


def options(opt):
    # The options for MSVC are not used
    #opt.load('msvc')
    from waflib.Tools import msvc


def configure(conf):
    seen = set([])
    from waflib.Tools import msvc
    conf.env.append_unique('useful_defines', ['__INTEL_COMPILER', '__clang__', '_MSC_VER'])
    conf.start_msg('Looking for msvc compilers')
    try:
        versions = conf.get_msvc_versions()
    except Exception as e:
        pass
    else:
        for version, targets in versions:
            name, version = version.split()
            for target_name, target in targets:
                arch, flags = target
                batfile, args, path, includes, libdirs = flags
                cl = conf.detect_executable(name == 'intel' and 'icl' or 'cl', path)
                c = MSVC(cl, name, version, target_name, arch, batfile, args, path, includes, libdirs)
                if c.name() in seen:
                    continue
                seen.add(c.name())
                conf.compilers.append(c)
    conf.end_msg('done')


def build(bld):
    msvc.wrap_class('cpp')


@feature('c', 'cxx')
@after_method('process_source')
def apply_pdb_flag(self):
    if self.env.CC_NAME == 'msvc':
        for task in getattr(self, 'compiled_tasks', []):
            if task:
                task.env.append_unique('CFLAGS', '/Fd%s'%task.outputs[0].change_ext('.pdb').abspath())
                task.env.append_unique('CXXFLAGS', '/Fd%s'%task.outputs[0].change_ext('.pdb').abspath())
