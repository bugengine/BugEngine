from waflib import Errors, Utils, Options
from waflib.Configure import conf
from waflib.TaskGen import feature, before_method, after_method
import os
import sys


@feature("check_python")
@before_method("process_source")
def check_python_test(self):
    def write_test_file(task):
        task.outputs[0].write(task.generator.code)
    bld = self.bld
    bld(rule=write_test_file, target='main.cc', code=self.code)
    bld(features='cxx cxxprogram', source='main.cc', target='app',
        cxxflags=self.cxxflags, linkflags=self.ldflags, use=self.use)


@conf
def python_module(bld, name, depends, path):
    module_list, module_multiarch = bld.module(name, path, depends, [],
        features=['cxx', 'cxxshlib', 'python_module'],
        build_features=[],
        extra_includes=[], extra_defines=[],
        extra_public_includes=[], extra_public_defines=[],
        use_master=True, warnings=True, export_all=False)
    for module in module_list:
        module.env.cxxshlib_PATTERN = module.env.pymodule_PATTERN
    if module_multiarch:
        module_multiarch.env.cxxshlib_PATTERN = module.env.pymodule_PATTERN


@conf
def python_config(conf, version, var=''):
    version_number = version.replace('.', '')
    if not var: var = '3rdparty.python%s'%(version_number)
    if 'posix' in conf.env.VALID_PLATFORMS:
        cflags, libs = conf.run_pkg_config('python-%s'%version)
        conf.env['CFLAGS_%s'%var] = cflags
        conf.env['CXXFLAGS_%s'%var] = cflags
        conf.check(
            compile_filename=[],
            features='check_python',
            msg='check for python %s'%version,
            cxxflags=cflags,
            ldflags=libs,
            use=[var],
            code="""
                #include <Python.h>
                int main() { Py_Initialize(); return 0; }
            """)
        for lib in libs:
            if lib.startswith('-lpython'):
                lib_name = lib[2:]
        conf.env['DEFINES_%s'%var] = ['PYTHON_LIBRARY="%s"'%lib_name]
    elif 'macosx' in conf.env.VALID_PLATFORMS:
        conf.recurse('../python%s/python%s.py' % (version_number, version_number), name='setup_python', once=False)
    elif 'windows' in conf.env.VALID_PLATFORMS:
        conf.recurse('../python%s/python%s.py' % (version_number, version_number), name='setup_python', once=False)
    else:
        raise Errors.WafError('TODO')


def options(opt):
    gr = opt.add_option_group('configure options')
    gr.add_option('--python-versions',
                  action='store',
                  dest='python_versions',
                  help='List of Python version to support in plugins',
                  default='2.5,2.6,2.7,3.0,3.1,3.2,3.3,3.4,3.5')


def setup(conf):
    for version in Options.options.python_versions.split(','):
        try:
            conf.python_config(version)
        except Errors.WafError:
            pass
        else:
            conf.env.append_unique('FEATURES', 'python%s'%version)
            conf.detected.append('Python %s'%version)


def build(bld):
    bld.env.PYTHON_VERSIONS = Options.options.python_versions.split(',')
    for version in bld.env.PYTHON_VERSIONS:
        try:
            bld.recurse('../python%s/python%s.py' % (version.replace('.', ''), version.replace('.', '')))
        except:
            pass


@feature('python_module')
@after_method('apply_link')
def install_python_module(self):
    if self.bld.is_install:
        if not self.env.ENV_PREFIX: #no multiarch
            self.bld.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_RUNBINDIR), [self.link_task.outputs[0]])
            if self.env.CC_NAME == 'msvc':
                self.bld.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_RUNBINDIR), [self.link_task.outputs[1]])

