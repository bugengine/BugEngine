from waflib import Options, Utils, Errors
from waflib.Configure import conf
from waflib.TaskGen import feature, before_method, after_method
import os


@conf
def python_module(bld, name, depends, path, conditions):
    # TODO
    #module_list, module_multiarch = bld.module(name, path, depends, [], platforms,
    #    features=['cxx', 'cxxshlib', 'bugengine:python_module'],
    #    build_features=[],
    #    extra_includes=[], extra_defines=[],
    #    extra_public_includes=[], extra_public_defines=[],
    #    use_master=True, warnings=True, export_all=False, root_namespace='BugEngine')
    #if module_list[0].preprocess:
    #    module_list[0].preprocess.env.PLUGIN = name.replace('.', '_')
    #for module in module_list:
    #    module.env.cxxshlib_PATTERN = module.env.pymodule_PATTERN
    #if module_multiarch:
    #    module_multiarch.env.cxxshlib_PATTERN = module.env.pymodule_PATTERN
    return


@feature('bugengine:python_module')
@after_method('install_step')
def install_python_module(self):
    if not self.env.PROJECTS and not self.env.ENV_PREFIX:                                         #no multiarch
        self.install_files(
            os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_RUNBINDIR),
            [self.postlink_task.outputs[0]], Utils.O755
        )
        if self.env.CC_NAME == 'msvc':
            self.install_files(
                os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_RUNBINDIR),
                [self.link_task.outputs[1]]
            )


def build(bld):
    bld.env.PYTHON_VERSIONS = Options.options.python_versions.split(',')
    for version in bld.env.PYTHON_VERSIONS:
        bld.recurse('tcltk/build.py')
        version_number = version.replace('.', '')
        path = bld.env['PYTHON%s_BINARY' % version_number]
        if path:
            path = bld.package_node.make_node(path)
            bld.thirdparty(
                'bugengine.3rdparty.scripting.python%s' % version_number,
                var='python%s' % version_number,
                source_node=path,
                private_use=['bugengine.3rdparty.scripting.tcltk'],
                feature='python'
            )
        else:
            bld.thirdparty(
                'bugengine.3rdparty.scripting.python%s' % version_number,
                var='python%s' % version_number,
                private_use=['bugengine.3rdparty.scripting.tcltk'],
                feature='python'
            )