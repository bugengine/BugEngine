from waflib import Utils
from waflib.TaskGen import feature, before_method, after_method
import os


@feature('kernel')
@after_method('install_step')
def install_kernel(self):
    if not self.env.ENV_PREFIX and not self.bld.env.STATIC: #no multiarch, no static
        self.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_KERNELDIR),
                           [self.postlink_task.outputs[0]],
                           Utils.O755)
        if self.env.CC_NAME == 'msvc':
            self.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_KERNELDIR),
                               [self.link_task.outputs[1]])


@feature('plugin')
@after_method('install_step')
def install_plugin(self):
    if ('cshlib' in self.features) or ('cxxshlib' in self.features):
        if not self.env.ENV_PREFIX: #no multiarch
            self.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_PLUGINDIR),
                               [self.postlink_task.outputs[0]],
                               Utils.O755)
            if self.env.CC_NAME == 'msvc':
                self.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_PLUGINDIR),
                                   [self.link_task.outputs[1]])


@feature('shared_lib')
@after_method('install_step')
def install_shared_lib(self):
    if ('cshlib' in self.features) or ('cxxshlib' in self.features):
        if not self.env.ENV_PREFIX: #no multiarch
            self.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_RUNBINDIR),
                               [self.postlink_task.outputs[0]],
                               Utils.O755)
            if self.env.CC_NAME == 'msvc':
                self.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_RUNBINDIR),
                                   [self.link_task.outputs[1]])


@feature('launcher')
@after_method('install_step')
def install_program(self):
    if not self.env.ENV_PREFIX: #no multiarch
        self.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_BINDIR),
                           [self.postlink_task.outputs[0]],
                           chmod=Utils.O755)
        if self.env.CC_NAME == 'msvc':
            self.install_files(os.path.join(self.bld.env.PREFIX, self.bld.optim, self.bld.env.DEPLOY_BINDIR),
                               [self.link_task.outputs[1]])


@feature('game')
@after_method('install_step')
def install_game(self):
    pass #also plugin


def build(bld):
    pass

