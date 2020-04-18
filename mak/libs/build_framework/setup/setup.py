import os
from be_typing import TYPE_CHECKING

def setup_toolchain(conf, toolchain):
    conf.setenv(toolchain + '.setup', conf.all_envs[toolchain])
    conf.env.BUGENGINE_SETUP = True
    extra = conf.bugenginenode.make_node('extra')
    conf.start_msg('Setting up environment')
    conf.end_msg(conf.env.TOOLCHAIN)
    if conf.env.VALID_PLATFORMS:
        extra_dir = os.path.join(extra.abspath(), conf.env.VALID_PLATFORMS[0])
        if os.path.isdir(extra_dir):
            conf.recurse(extra_dir, name='setup', once=False)

def setup(conf):
    # type: (Configure.ConfigurationContext) -> None
    "setup step before the build: recursively calls setup on every third party library"
    conf.recurse('checks.py')
    if conf.env.SUB_TOOLCHAINS:
        conf.setenv(conf.env.TOOLCHAIN + '.setup', conf.all_envs[conf.env.TOOLCHAIN])
        for t in conf.env.SUB_TOOLCHAINS:
            try:
                setup_toolchain(conf, t)
            finally:
                conf.setenv(conf.bugengine_variant)
    else:
        setup_toolchain(conf, conf.env.TOOLCHAIN)


if TYPE_CHECKING:
    from waflib import Configure