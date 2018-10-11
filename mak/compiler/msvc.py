from waflib import Configure, Utils
from waflib.Configure import conf
from waflib.Tools import msvc
import os
import re


class MSVC(Configure.ConfigurationContext.Compiler):
    def __init__(self, cl, name, version, target_arch, arch, bat, args, path, includes, libdirs):
        self.NAMES = [name, 'msvc']
        p = os.pathsep.join([os.environ.get('PATH', '')] + path)
        flags = ['/I%s'%i for i in includes] + ['/LIBPATH:%i' for l in libdirs]
        Configure.ConfigurationContext.Compiler.__init__(self, cl, cl, version, 'windows-%s'%name, arch, {}, {'PATH': p})
        self.batfile = bat
        self.path = path
        self.args = args
        self.arch_name = target_arch
        self.includes = [os.path.join(i, target_arch) for i in includes if os.path.isdir(os.path.join(i, target_arch))] + includes
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
                                                    '/Oy', '/GT', '/GF', '/FD', '/GS-', '/Gy',
                                                    '/GR-'])
        conf.env.append_unique('CFLAGS_profile', ['/DNDEBUG', '/MT', '/Ox', '/Ob2', '/Oi', '/Ot',
                                                  '/Oy', '/GT', '/GF', '/FD', '/GS-', '/Gy',
                                                  '/GR-'])
        conf.env.append_unique('CXXFLAGS_profile', ['/DNDEBUG', '/D_HAS_EXCEPTIONS=0', '/MT', '/Ox',
                                                    '/Ob2', '/Oi', '/Ot', '/Oy', '/GT',
                                                    '/GF', '/FD', '/GS-', '/Gy', '/GR-'])
        conf.env.append_unique('LINKFLAGS_profile', ['/DEBUG', '/INCREMENTAL:no'])
        conf.env.append_unique('ARFLAGS_profile', ['/LTCG'])

        conf.env.append_unique('CPPFLAGS_final', ['/DNDEBUG', '/MT', '/Ox', '/Ob2', '/Oi', '/Ot',
                                                  '/Oy', '/GT', '/GF', '/FD', '/GS-', '/Gy',
                                                  '/GR-'])
        conf.env.append_unique('CFLAGS_final', ['/DNDEBUG', '/MT', '/Ox', '/Ob2', '/Oi', '/Ot',
                                                '/Oy', '/GT', '/GF', '/FD', '/GS-', '/Gy',
                                                '/GR-'])
        conf.env.append_unique('CXXFLAGS_final', ['/DNDEBUG', '/D_HAS_EXCEPTIONS=0', '/MT', '/Ox',
                                                  '/Ob2', '/Oi', '/Ot', '/Oy', '/GT', '/GF',
                                                  '/FD', '/GS-', '/Gy', '/GR-'])
        conf.env.append_unique('LINKFLAGS_final', ['/DEBUG', '/INCREMENTAL:no'])
        conf.env.append_unique('ARFLAGS_final', ['/LTCG'])

    def set_warning_options(self, conf):
        if self.NAMES[0] == 'intel':
            conf.env.append_unique('CXXFLAGS', ['/Zc:forScope'])
            if self.version_number >= (11,):
                warning = ['/W4', '/Qdiag-disable:remark']
            else:
                warning = ['/W3']
        else:
            warning = ['/W4']
        conf.env.append_unique('CFLAGS_warnall', warning + ['/D_CRT_SECURE_NO_WARNINGS=1', '/WX'])
        conf.env.append_unique('CFLAGS_warnnone', ['/D_CRT_SECURE_NO_WARNINGS=1', '/W0'])
        conf.env.append_unique('CXXFLAGS_warnall', warning + ['/D_CRT_SECURE_NO_WARNINGS=1', '/WX'])
        conf.env.append_unique('CXXFLAGS_warnnone', ['/D_CRT_SECURE_NO_WARNINGS=1', '/W0'])
        if self.NAMES[0] == 'msvc' and self.version_number >= (14,):
            conf.env.append_unique('CFLAGS_warnall', ['/D_ALLOW_RTCc_IN_STL=1'])
            conf.env.append_unique('CXXFLAGS_warnall', ['/D_ALLOW_RTCc_IN_STL=1'])
            conf.env.append_unique('CFLAGS_warnnone', ['/D_ALLOW_RTCc_IN_STL=1'])
            conf.env.append_unique('CXXFLAGS_warnnone', ['/D_ALLOW_RTCc_IN_STL=1'])

    def load_tools(self, conf, platform):
        env = conf.env
        version = '%s %s'%(self.NAMES[0], self.version)
        version_number = float(self.version.replace('Exp', ''))
        env.NO_MSVC_DETECT = 1
        env.PATH = self.path
        env.INCLUDES = self.includes
        env.LIBPATH = self.libdirs
        env.MSVC_COMPILER = self.NAMES[0]
        env.MSVC_VERSION = version_number
        env.MSVC_MANIFEST = 0
        env.MSVC_TARGETS = [self.arch_name]
        env.MSVC_BATFILE = [self.batfile, self.args]
        env.COMPILER_NAME='msvc'
        env.COMPILER_TARGET='windows-win32-msvc-%s'%version
        conf.load('msvc')
        if ((self.NAMES[0] == 'msvc' and version_number >= 8)
         or (self.NAMES[0] == 'wsdk' and version_number >= 6)
         or (self.NAMES[0] == 'intel' and version_number >= 11)):
            env.append_unique('LINKFLAGS', ['/MANIFEST:NO'])

    def load_in_env(self, conf, platform):
        Configure.ConfigurationContext.Compiler.load_in_env(self, conf, platform)
        env = conf.env
        env.IDIRAFTER='/I'
        env.CC_CPP = Utils.to_list(env.CC) + ['/nologo', '/TC', '/X', '/E']
        env.CC_CPP_SRC_F = ''
        env.CC_CPP_TGT_F = ['/Fo'] # will actually be ignored
        env.append_unique('STLIB', [conf.bugenginenode.make_node('mak/compiler/msvc/set_conditional').abspath()])
        if os_platform().endswith('64'):
            conf.find_program('cdb64', var='CDB', mandatory=False)
        else:
            conf.find_program('cdb', var='CDB', mandatory=False)


all_icl_platforms = (
    ('intel64', 'intel64', 'amd64'),
    ('em64t', 'em64t', 'amd64'),
    ('em64t_native', 'intel64', 'amd64'),
    ('ia32e', 'ia32e', 'amd64'),
    ('ia32', 'ia32', 'x86'),
    ('itanium', 'itanium', 'ia64'),
    ('ia64', 'ia64', 'ia64'),
)
@conf
def gather_icl_versions(conf, versions):
    """
    Checks ICL compilers

    :param versions: list to modify
    :type versions: list
    """
    version_pattern = re.compile('^...?.?\....?.?')
    version_pattern_old = re.compile('^..')
    try:
        all_versions = Utils.winreg.OpenKey(Utils.winreg.HKEY_LOCAL_MACHINE, r'SOFTWARE\Wow6432node\Intel\Compilers\C++')
    except OSError:
        try:
            all_versions = Utils.winreg.OpenKey(Utils.winreg.HKEY_LOCAL_MACHINE, r'SOFTWARE\Intel\Compilers\C++')
        except OSError:
            return
    index = 0
    while 1:
        try:
            version = Utils.winreg.EnumKey(all_versions, index)
        except OSError:
            break
        index += 1
        if version_pattern.match(version):
            version_str = '%s.%s' % (version[0:2], version[2])
        elif version_pattern_old.match(version):
            version_str = '%s.%s' % (version[0], version[1])
        else:
            continue
        targets = {}
        for target, target_arg, arch in all_icl_platforms:
            try:
                icl_version = Utils.winreg.OpenKey(all_versions, version+'\\'+target)
                path,type = Utils.winreg.QueryValueEx(icl_version,'ProductDir')
            except OSError:
                continue
            else:
                batch_file=os.path.join(path,'bin','iclvars.bat')
                if os.path.isfile(batch_file):
                    targets[target_arg] = msvc.target_compiler(conf, 'intel', arch, version_str, target_arg, batch_file)
        versions['intel ' + version_str] = targets


@conf
def gather_intel_composer_versions(conf, versions):
    version_pattern_suite = re.compile('^...?.?\..?.?')
    try:
        all_versions = Utils.winreg.OpenKey(Utils.winreg.HKEY_LOCAL_MACHINE, r'SOFTWARE\Wow6432node\Intel\Suites')
    except OSError:
        try:
            all_versions = Utils.winreg.OpenKey(Utils.winreg.HKEY_LOCAL_MACHINE, r'SOFTWARE\Intel\Suites')
        except OSError:
            return
    index = 0
    while 1:
        try:
            version = Utils.winreg.EnumKey(all_versions, index)
        except OSError:
            break
        index += 1
        if version_pattern_suite.match(version):
            version_str = '%s.%s' % (version[0:2], version[3])
        else:
            continue
        all_minor_versions = Utils.winreg.OpenKey(all_versions, version)
        minor_index = 0
        while 1:
            try:
                minor_version = Utils.winreg.EnumKey(all_minor_versions, minor_index)
            except OSError:
                break
            else:
                minor_index += 1
                targets = {}
                for target, target_arg, arch in all_icl_platforms:
                    try:
                        # check if target is installed
                        Utils.winreg.OpenKey(all_minor_versions, '%s\\C++\\%s' % (minor_version, target))
                        # retrieve ProductDir
                        icl_version = Utils.winreg.OpenKey(all_minor_versions, '%s\\C++' % minor_version)
                        path,type = Utils.winreg.QueryValueEx(icl_version,'ProductDir')
                    except OSError:
                        continue
                    else:
                        batch_file=os.path.join(path,'bin','iclvars.bat')
                        if os.path.isfile(batch_file):
                            targets[target_arg] = msvc.target_compiler(conf, 'intel', arch, version_str, target_arg, batch_file)
                versions['intel ' + version_str] = targets

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
        for version, targets in sorted(versions.items()):
            name, version = version.split()
            for target_name, target_compiler in sorted(targets.items()):
                target_compiler.evaluate()
                if target_compiler.is_valid:
                    cl = conf.detect_executable(name == 'intel' and 'icl' or 'cl', target_compiler.bindirs)
                    c = MSVC(cl, name, version, target_name,
                             target_compiler.cpu, target_compiler.bat, target_compiler.bat_target,
                             target_compiler.bindirs, target_compiler.incdirs, target_compiler.libdirs)
                    if c.name() in seen:
                        continue
                    seen.add(c.name())
                    conf.compilers.append(c)
    conf.end_msg('done')
