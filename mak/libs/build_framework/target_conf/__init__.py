import os, platform
from waflib import Options, Configure
from waflib.Logs import pprint
from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext, ListContext
from waflib.Configure import conf
from . import freebsd, linux, macos, solaris, windows


def options(opt):
    gr = opt.add_option_group('configure options')
    for target in freebsd, linux, macos, solaris, windows:
        target.options(opt)
    for extra in opt.bugenginenode.make_node('extra').listdir():
        if os.path.isfile(os.path.join(opt.bugenginenode.abspath(), 'extra', extra, 'wscript')):
            opt.recurse(os.path.join(opt.bugenginenode.abspath(), 'extra', extra))
    gr.add_option('--platforms', action='store', default='', dest='platforms', help='List of platform to configure for')


def configure(conf):
    conf.env.ALL_VARIANTS = ['debug', 'profile', 'final']
    conf.platforms = []
    platforms = Options.options.platforms
    platforms = platforms.split(',') if platforms else []
    for target in freebsd, linux, macos, solaris, windows:
        if not platforms or target.__name__.split('.')[-1] in platforms:
            target.configure(conf)
    for extra in conf.bugenginenode.make_node('extra').listdir():
        if not platforms or extra in platforms:
            if os.path.isfile(os.path.join(conf.bugenginenode.abspath(), 'extra', extra, 'wscript')):
                conf.recurse(os.path.join(conf.bugenginenode.abspath(), 'extra', extra))
    for p in conf.platforms:
        configuration_list = p.get_available_compilers(conf.compilers)
        if configuration_list:
            pprint('BLUE', ' + configuring for platform %s' % p.NAME)
            for main_toolchain, sub_toolchains, platform in configuration_list:
                platform.add_toolchain(conf, main_toolchain, sub_toolchains)
    conf.env.store('.waf_toolchains.py')

