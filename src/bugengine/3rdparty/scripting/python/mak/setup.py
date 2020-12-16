from waflib import Errors, Options, Logs
from waflib.Configure import conf
from waflib.TaskGen import feature, before_method, after_method

PYTHON_PACKAGE = 'https://github.com/bugengine/BugEngine/releases/download/prebuilt-python/python{version}-%(platform)s.tgz'


@conf
def python_package(configuration_context, version, version_number):
    try:
        node = configuration_context.pkg_unpack(
            'python{version}-%(platform)s'.format(version=version),
            PYTHON_PACKAGE.format(version=version),
        )
    except Errors.WafError:
        raise
    else:
        configuration_context.env['PYTHON%s_BINARY' % version_number
                                  ] = node.path_from(configuration_context.package_node)
        configuration_context.env['check_python%s' % version_number] = True
        configuration_context.env['check_python%s_defines' % version_number] = ['PYTHON_LIBRARY=python%s' % version]
        return True


@feature("check_python")
@before_method("process_source")
def check_python_test(self):
    def write_test_file(task):
        task.outputs[0].write(task.generator.code)

    bld = self.bld
    bld(rule=write_test_file, target='main.cc', code=self.code)
    bld(
        features='cxx cxxprogram',
        source='main.cc',
        target='app',
        cxxflags=self.cxxflags,
        linkflags=self.ldflags,
        lib=self.libs,
        use=self.use
    )


@conf
def python_config(conf, version, var=''):
    version_number = version.replace('.', '')
    if not var: var = 'python%s' % (version_number)
    if 'posix' in conf.env.VALID_PLATFORMS:
        try:
            cflags, libs, ldflags = conf.run_pkg_config('python-%s-embed' % version)
        except Errors.WafError:
            try:
                cflags, libs, ldflags = conf.run_pkg_config('python-%s' % version)
            except Errors.WafError as error:
                cflags = ['-I/usr/include/python%s' % version]
                ldflags = []
                libs = ['python%s' % version]
        conf.check(
            compile_filename=[],
            features='check_python',
            msg='check for python %s' % version,
            cxxflags=cflags,
            libs=libs,
            ldflags=ldflags,
            use=[var],
            code="""
                #include <Python.h>
                int main() { Py_InitializeEx(0); return 0; }
            """
        )
        conf.env['check_%s' % var] = True
        for lib in libs:
            if lib.startswith('python'):
                lib_name = lib
        conf.env['check_%s_defines' % var] = ['PYTHON_LIBRARY=%s' % lib_name]
    else:
        conf.python_package(version, version_number)


def setup(conf):
    if not conf.env.PROJECTS:
        conf.recurse('tcltk/setup.py')
        conf.start_msg_setup()
        py_versions = []
        for version in Options.options.python_versions.split(','):
            try:
                conf.python_config(version)
            except Errors.WafError as e:
                pass
            else:
                conf.env.append_unique('FEATURES', 'python%s' % version)
                py_versions.append(version)
        if py_versions:
            conf.end_msg(', '.join(py_versions))
        else:
            conf.end_msg('not found', color='YELLOW')
