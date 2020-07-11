import os
from be_typing import TYPE_CHECKING
from waflib import Options, Configure


@Configure.conf
def register_setup_option(configuration_context, option_name):
    # type: (Configure.ConfigurationContext, str) -> None
    configuration_context.env.append_value('SETUP_OPTIONS', [(option_name, getattr(Options.options, option_name))])


@Configure.conf
def start_msg_setup(configuration_context, third_party_name=None):
    # type: (Configure.ConfigurationContext, Optional[str]) -> None
    configuration_context.start_msg(
        '  %s.%s' % (
            configuration_context.path.parent.parent.name,
            third_party_name if third_party_name is not None else configuration_context.path.parent.name
        )
    )


def setup(configuration_context):
    # type: (Configure.ConfigurationContext) -> None
    "setup step before the build: recursively calls setup on every third party library"
    extra = configuration_context.bugenginenode.make_node('extra')
    if configuration_context.env.VALID_PLATFORMS:
        extra_dir = os.path.join(extra.abspath(), configuration_context.env.VALID_PLATFORMS[0])
        if os.path.isdir(extra_dir):
            configuration_context.recurse(extra_dir, once=False)


def multiarch_setup(configuration_context):
    # type: (Configure.ConfigurationContext) -> None
    configuration_context.recurse('checks.py')
    configuration_context.recurse('package.py')
    configuration_context.fun = 'setup'
    if configuration_context.env.SUB_TOOLCHAINS:
        for t in configuration_context.env.SUB_TOOLCHAINS:
            try:
                configuration_context.start_msg('Setting up environment')
                configuration_context.end_msg(t)
                configuration_context.setenv(t + '.setup', configuration_context.all_envs[t])
                configuration_context.env.TOOLCHAIN = configuration_context.env.TOOLCHAIN + '.setup'
                configuration_context.recurse(configuration_context.run_dir, once=False)
            except Exception as e:
                raise
            else:
                configuration_context.env.BUGENGINE_SETUP = True
            finally:
                configuration_context.variant = configuration_context.bugengine_variant
        configuration_context.setenv(
            configuration_context.bugengine_variant + '.setup',
            configuration_context.all_envs[configuration_context.bugengine_variant]
        )
        configuration_context.env.SUB_TOOLCHAINS = [t + '.setup' for t in configuration_context.env.SUB_TOOLCHAINS]
        configuration_context.env.BUGENGINE_SETUP = True
    else:
        t = configuration_context.bugengine_variant
        try:
            configuration_context.start_msg('Setting up environment')
            configuration_context.end_msg(t)
            configuration_context.setenv(t + '.setup', configuration_context.all_envs[t])
            configuration_context.env.TOOLCHAIN = configuration_context.env.TOOLCHAIN + '.setup'
            configuration_context.recurse(configuration_context.run_dir, once=False)
        except Exception as e:
            raise
        else:
            configuration_context.env.BUGENGINE_SETUP = True


if TYPE_CHECKING:
    from waflib import Configure
    from typing import Optional