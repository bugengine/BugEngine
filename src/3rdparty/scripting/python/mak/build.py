from waflib import Options, Utils, Errors
from waflib.Configure import conf
from waflib.TaskGen import feature, before_method, after_method
import os


@conf
def python_module(bld, name, depends, path, platforms=[]):
    if not bld.env.PROJECTS:
        for p in platforms:
            if p not in bld.env.VALID_PLATFORMS:
                return
    module_list, module_multiarch = bld.module(name, path, depends, [], platforms,
        features=['cxx', 'cxxshlib', 'python_module'],
        build_features=[],
        extra_includes=[], extra_defines=[],
        extra_public_includes=[], extra_public_defines=[],
        use_master=True, warnings=True, export_all=False, root_namespace='BugEngine')
    if module_list[0].preprocess:
        module_list[0].preprocess.env.PLUGIN = name.replace('.', '_')
    for module in module_list:
        module.env.cxxshlib_PATTERN = module.env.pymodule_PATTERN
    if module_multiarch:
        module_multiarch.env.cxxshlib_PATTERN = module.env.pymodule_PATTERN


@feature('python_module')
@after_method('install_step')
def install_python_module(self):
    if not self.env.PROJECTS and not self.env.ENV_PREFIX: #no multiarch
        self.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_RUNBINDIR),
                           [self.postlink_task.outputs[0]],
                           Utils.O755)
        if self.env.CC_NAME == 'msvc':
            self.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_RUNBINDIR),
                               [self.link_task.outputs[1]])


def build(bld):
    bld.env.PYTHON_VERSIONS = Options.options.python_versions.split(',')
    for version in bld.env.PYTHON_VERSIONS:
        try:
            bld.recurse('../../python%s/mak/build.py' % (version.replace('.', '')))
        except Errors.WafError as e:
            pass

