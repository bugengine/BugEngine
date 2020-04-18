import os
from waflib import Options
from waflib.Logs import pprint
from be_typing import TYPE_CHECKING


def configure(configuration_context):
    # type: (Configure.ConfigurationContext) -> None
    configuration_context.env.ALL_VARIANTS = ['debug', 'profile', 'final']
    configuration_context.platforms = []
    platforms = Options.options.platforms
    platforms = platforms.split(',') if platforms else[]
    configuration_context.recurse('platform.py')
    for target in os.listdir(configuration_context.path.abspath()):
        if not platforms or os.path.splitext(target)[0] in platforms:
            configuration_context.recurse(target)
    for extra in configuration_context.bugenginenode.make_node('extra').listdir():
        if not platforms or extra in platforms:
            if os.path.isfile(os.path.join(configuration_context.bugenginenode.abspath(), 'extra', extra, 'wscript')):
                configuration_context.recurse(os.path.join(configuration_context.bugenginenode.abspath(), 'extra', extra))
    for p in configuration_context.platforms:
        configuration_list = p.get_available_compilers(configuration_context.compilers)
        if configuration_list:
            pprint('BLUE', ' + configuring for platform %s' % p.NAME)
            for main_toolchain, sub_toolchains, platform in configuration_list:
                platform.add_toolchain(configuration_context, main_toolchain, sub_toolchains)
    configuration_context.env.store('.waf_toolchains.py')


if TYPE_CHECKING:
    from waflib import Configure