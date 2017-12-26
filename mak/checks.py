from waflib import Utils, ConfigSet, Errors, Logs
from waflib.Configure import conf
from waflib.TaskGen import feature, before_method, after_method
import platform
import os
import sys

os_name = platform.uname()[0].lower().split('-')[0]
USE_LIBRARY_CODE="""
%(include)s
#if defined(_WIN32) && !defined(__clang__)
__declspec(dllexport) int be_test()
{
    return 0;
}
#endif
int main(int argc, char *argv[])
{
    int i;
    %(function)s;
    return i;
}
"""


USE_SDK_CODE="""
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <stdio.h>

int main(int argc, const char *argv[])
{ return EXIT_SUCCESS; }
"""


@feature("link_library")
@before_method("process_source")
def link_library_test(self):
    def write_test_file(task):
        task.outputs[0].write(task.generator.code)
    bld = self.bld
    bld(rule=write_test_file, target='main.cc', code=self.code)
    bld(features='cxx cxxprogram', source='main.cc', target='app', lib=self.libname, libpath=self.libpath, includes=self.includepath, use=self.use)


@feature("link_framework")
@before_method("process_source")
def link_framework_test(self):
    def write_test_file(task):
        task.outputs[0].write(task.generator.code)
    bld = self.bld
    bld(rule=write_test_file, target='main.mm', code=self.code)
    env = self.env.derive()
    env.detach()
    env.FRAMEWORK = self.frameworks
    env.FRAMEWORK_ST = ['-framework']
    env.append_unique('CXXFLAGS', ['-F%s' % f for f in self.frameworks])
    if self.sdk:
        env.append_unique('CXXFLAGS', ['-isysroot', self.sdk])
        env.append_unique('LINKFLAGS', ['-isysroot', self.sdk, '-L%s/usr/lib'%self.sdk])
    if self.version:
        env.append_unique('CXXFLAGS', [self.version])
        env.append_unique('LINKFLAGS', [self.version])
    bld(features='cxx cxxprogram', source='main.mm', target='app', lib=self.libname, libpath=self.libpath, includes=self.includepath, env=env, use=self.use)


@conf
def check_lib(self, libname, var='', libpath=[], includepath=[], includes=[], functions=[], code=USE_LIBRARY_CODE):
    def cut(string):
        if len(string) > 19:
            return string[0:17]+'...'
        else:
            return string
    libname = Utils.to_list(libname)
    if not var: var = self.path.name
    try:
        if functions:
            functions =  '\n'.join(['i = *(int*)(&%s);'%f for f in functions])
        else:
            functions = 'i = *(int*)0;'
        self.check(
            compile_filename=[],
            features='link_library',
            msg='check for libraries %s' %cut(','.join(libname)),
            libname=libname,
            libpath=libpath,
            code=code % {
                    'include': '\n'.join(['#include <%s>'%i for i in includes]),
                    'function': functions
                },
            includepath=includepath,
            use=['debug'],
            envname=self.env.TOOLCHAIN)
    except self.errors.ConfigurationError as e:
        #Logs.pprint('YELLOW', '-%s' % var, sep=' ')
        pass
    else:
        self.env['check_%s' % var] = True
        self.env.append_unique('check_%s_libs' % var, libname)
        self.env.append_unique('check_%s_libpath' % var, libpath)
        self.env.append_unique('check_%s_includes' % var, includepath)
        Logs.pprint('GREEN', '+%s' % var, sep=' ')

    return self.env['check_%s' % var]


@conf
def check_header(self, headername, var='', libpath=[], includepath=[], code=USE_LIBRARY_CODE):
    def cut(string):
        if len(string) > 19:
            return string[0:17]+'...'
        else:
            return string
    headername = Utils.to_list(headername)
    if not var: var = self.path.name
    try:
        self.check(
            compile_filename=[],
            features='link_library',
            msg='check for header %s' %cut(','.join(headername)),
            libname=[],
            libpath=libpath,
            code='\n'.join(["#include <%s>"%h for h in headername]) + '\n' + code,
            includepath=includepath,
            use=['debug'],
            envname=self.env.TOOLCHAIN)
    except self.errors.ConfigurationError as e:
        pass
    else:
        self.env['check_%s' % var] = True
        self.env.append_unique('check_%s_includes' % var, includepath)
        self.env.append_unique('check_%s_libpath' % var, libpath)
    return self.env['%s_includess' % var]


@conf
def check_framework(self, frameworks, var='', libpath=[], includepath=[], includes=[], functions=[], code=USE_LIBRARY_CODE):
    def cut(string):
        if len(string) > 19:
            return string[0:17]+'...'
        else:
            return string
    frameworks = Utils.to_list(frameworks)
    if not var: var = self.path.name
    try:
        if functions:
            functions =  ' + '.join(['*(int*)(&%s)'%f for f in functions])
        else:
            functions = '*(int*)0'
        self.check(
            compile_filename=[],
            features='link_framework',
            msg='check for framework %s' %cut(','.join(frameworks)),
            libname=[],
            frameworks=frameworks,
            libpath=libpath,
            code=code % {
                    'include': '\n'.join(['#include <%s>'%i for i in includes]),
                    'function': functions
                },
            includepath=includepath,
            sdk='',
            version='',
            use=['debug'],
            envname=self.env.TOOLCHAIN)
    except self.errors.ConfigurationError as e:
        #Logs.pprint('YELLOW', '-%s' % var, sep=' ')
        pass
    else:
        self.env['check_%s' % var] = True
        self.env.append_unique('check_%s_frameworks' % var, frameworks)
        self.env.append_unique('check_%s_includes' % var, includepath)
        self.env.append_unique('check_%s_libpath' % var, libpath)
        self.env.append_unique('XCODE_FRAMEWORKS', frameworks)
        Logs.pprint('GREEN', '+%s' % var, sep=' ')
    return self.env['%s_frameworks' % var]


@conf
def check_sdk(self, compiler, flags, sdk, version,
              frameworks=[], libpath=[], includepath=[], code=USE_SDK_CODE):
    env = ConfigSet.ConfigSet()
    env.CXX = compiler
    env.LINK_CXX = compiler
    env.CXXFLAGS = flags
    env.LINKFLAGS = flags
    env.CXX_TGT_F = ['-c', '-o']
    env.CXXLNK_TGT_F = ['-o']

    code = '\n'.join(['#include <%s/%s.h>' % (f, f) for f in frameworks]) + '\n'+ code
    self.check(
        env=env,
        compile_filename=[],
        features='link_framework',
        msg='check for SDK %s' %os.path.split(sdk)[1],
        libname=[],
        frameworks=frameworks,
        libpath=libpath,
        code=code,
        includepath=includepath,
        sdk=sdk,
        version=version,
        use=['debug'],
        errmsg='not usable',
        compiler=compiler,
        envname=self.env.TOOLCHAIN)


@conf
def run_pkg_config(conf, name):
    sysroot = conf.env.SYSROOT or ''
    def extend_lib_path(lib_path):
        if lib_path[0] == '=':
            if sysroot:
                return os.path.join(sysroot, lib_path[2:])
            else:
                return lib_path[1:]
        else:
            return lib_path
    expand = {}
    configs = {}
    lib_paths = conf.env.SYSTEM_LIBPATHS + ['=/usr/lib', '=/usr/local/lib',
                                            '=/usr/libdata', '=/usr/local/libdata']
    lib_paths = [extend_lib_path(l) for l in lib_paths]
    for l in lib_paths:
        config_file = os.path.join(l, 'pkgconfig', name+'.pc')
        config_file = os.path.normpath(config_file)
        if os.path.isfile(config_file):
            break
    else:
        raise Errors.WafError('No pkg-config file for library %s'%name)

    if not sysroot:
        os_name = platform.uname()[0].lower().split('-')[0]
        if os_name == 'windows':
            sysroot = os.path.dirname(config_file)
            sysroot = os.path.dirname(sysroot)
            sysroot = os.path.dirname(sysroot)
            sysroot = os.path.dirname(sysroot)
            print(sysroot)
        else:
            sysroot = ''
    with open(config_file, 'r') as config:
        lines = config.readlines()
        for line in lines:
            line = line.strip()
            if not line:
                continue
            if line[0] == '#':
                continue
            pos = line.find('=')
            if pos != -1:
                var_name = line[:pos].strip()
                value = line[pos+1:].strip()
                if value[0] == '"' and value[-1] == '"':
                    value = value[1:-1]
                if sysroot and value[0] == '/':
                    value = os.path.join(sysroot, value[1:])
                value = value.replace('${', '{')
                value = value.format(value, **expand)
                expand[var_name] = value
                continue
            pos = line.find(':')
            if pos != -1:
                var_name = line[:pos].strip()
                value = line[pos+1:].strip()
                value = value.replace('${', '{')
                value = value.format(value, **expand)
                configs[var_name.strip()] = value.strip().split()
    cflags = []
    libs = []
    ldflags = []
    for f in configs.get('Cflags') or []:
        if f.startswith('-I'):
            include = f[2:]
            if include[0] == '/' and not include.startswith(sysroot):
                include = os.path.join(sysroot, include[1:])
            cflags += [conf.env.IDIRAFTER, include]
        else:
            cflags.append(f)
    for f in configs.get('Libs') or []:
        if f.startswith('-l'):
            libs.append(f[2:])
        elif f[0:2] == '-L':
            libdir = f[2:]
            if libdir[0] == '/' and not libdir.startswith(sysroot):
                libdir = os.path.join(sysroot, libdir)
            ldflags += ['-L%s' % libdir]
        else:
            ldflags.append(f)
    return cflags, libs, ldflags


@conf
def pkg_config(conf, name, var=''):
    if not var: var = conf.path.name
    cflags, libs, ldflags = conf.run_pkg_config(name)
    conf.env['check_%s' % var] = True
    conf.env['check_%s_cflags'%var] += cflags
    conf.env['check_%s_cxxflags'%var] += cflags
    conf.env['check_%s_ldflags'%var] += ldflags
    conf.env['check_%s_libs'%var] += libs


def configure(conf):
    pass

