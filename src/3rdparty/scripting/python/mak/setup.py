from waflib import Errors, Options, Logs
from waflib.Configure import conf
from waflib.TaskGen import feature, before_method, after_method


@feature("check_python")
@before_method("process_source")
def check_python_test(self):
    def write_test_file(task):
        task.outputs[0].write(task.generator.code)
    bld = self.bld
    bld(rule=write_test_file, target='main.cc', code=self.code)
    bld(features='cxx cxxprogram', source='main.cc', target='app',
        cxxflags=self.cxxflags, linkflags=self.ldflags, lib=self.libs, use=self.use)


@conf
def python_config(conf, version, var=''):
    version_number = version.replace('.', '')
    if not var: var = 'python%s'%(version_number)
    if 'posix' in conf.env.VALID_PLATFORMS:
        try:
            cflags, libs, ldflags = conf.run_pkg_config('python-%s'%version)
        except Errors.WafError as error:
            cflags = ['-I/usr/include/python%s'%version]
            ldflags=[]
            libs = ['python%s'%version]
        conf.check(
            compile_filename=[],
            features='check_python',
            msg='check for python %s'%version,
            cxxflags=cflags,
            libs=libs,
            ldflags=ldflags,
            use=[var],
            code="""
                #include <Python.h>
                int main() { Py_Initialize(); return 0; }
            """)
        conf.env['check_%s' % var] = True
        for lib in libs:
            if lib.startswith('python'):
                lib_name = lib
        conf.env['check_%s_defines' % var] = ['PYTHON_LIBRARY=%s' % lib_name]
    elif 'macosx' in conf.env.VALID_PLATFORMS:
        conf.recurse('../python%s/mak/setup.py' % version_number, name='setup_python', once=False)
    elif 'windows' in conf.env.VALID_PLATFORMS:
        conf.recurse('../python%s/mak/setup.py' % version_number, name='setup_python', once=False)
    else:
        raise Errors.WafError('TODO')


def setup(conf):
    py_versions = []
    for version in Options.options.python_versions.split(','):
        try:
            conf.python_config(version)
        except Errors.WafError:
            pass
        else:
            conf.env.append_unique('FEATURES', 'python%s'%version)
            py_versions.append(version)
    if py_versions:
        Logs.pprint('GREEN', '+python (%s)'% ', '.join(py_versions), sep=' ')
    else:
        Logs.pprint('YELLOW', '-python', sep=' ')

