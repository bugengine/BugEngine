import os
from be_typing import TYPE_CHECKING


def configure(configuration_context):
    # type: (Configure.ConfigurationContext) -> None
    "Recursively calls configure on host and all targets to create all available toolchains"
    configuration_context.recurse('host/host.py')
    extra = configuration_context.bugenginenode.make_node('extra')
    for extra_platform in extra.listdir():
        directory = extra.make_node(extra_platform).abspath()
        if os.path.isdir(directory) and os.path.isfile(os.path.join(directory, 'wscript')):
            configuration_context.recurse(extra.make_node(extra_platform).abspath(), name='host_configure')
    tool_dir = os.path.join(configuration_context.bugenginenode.abspath(), 'mak', 'tools')
    configuration_context.load('flex', tooldir=[tool_dir])
    configuration_context.load('bison', tooldir=[tool_dir])
    configuration_context.recurse('compiler/compiler.py')
    configuration_context.recurse('target/target.py')
    configuration_context.env.ALL_TOOLCHAINS.sort(key=lambda x: x.split('-'))


if TYPE_CHECKING:
    from waflib import Configure