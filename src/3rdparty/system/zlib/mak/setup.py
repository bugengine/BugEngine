from waflib.Logs import pprint
from waflib.Errors import WafError
from waflib import Options
import os

ZLIB_SOURCES = 'https://zlib.net/zlib-1.2.11.tar.gz'
ZLIB_BINARIES = 'https://github.com/bugengine/BugEngine/releases/download/prebuilt-zlib/zlib-1.2.11-%(platform)s-%(arch)s-%(abi)s.tgz'
MINIZIP_BINARIES = 'https://github.com/bugengine/BugEngine/releases/download/prebuilt-zlib/minizip-1.2.11-%(platform)s-%(arch)s-%(abi)s.tgz'


def setup(conf):
    conf.register_setup_option('zlib_src')
    conf.register_setup_option('minizip_src')
    if conf.env.PROJECTS or Options.options.zlib_src:
        conf.env.ZLIB_SOURCE = conf.pkg_unpack('zlib_src', ZLIB_SOURCES,
                                               conf.path.parent.ant_glob(['patches/*.*'])).path_from(conf.package_node)
        pprint('BLUE', '=zlib', sep=' ')
    elif 'posix' in conf.env.VALID_PLATFORMS:
        try:
            conf.pkg_config('zlib', var='zlib')
        except Exception as e:
            conf.env.ZLIB_SOURCE = conf.pkg_unpack(
                'zlib_src', ZLIB_SOURCES, conf.path.parent.ant_glob(['patches/*.*'])
            ).path_from(conf.package_node)
            pprint('BLUE', '=zlib', sep=' ')
        else:
            conf.env.ZLIB_BINARY = True
            pprint('GREEN', '+zlib', sep=' ')
    else:
        try:
            zlib_node = conf.pkg_unpack('zlib_bin_%(platform)s-%(arch)s-%(abi)s', ZLIB_BINARIES)
            if not conf.check_package('3rdparty.system.zlib', zlib_node, var='zlib'):
                raise WafError('using zlib from source')
            conf.env.ZLIB_BINARY = zlib_node.path_from(conf.package_node)
        except WafError:
            conf.env.ZLIB_SOURCE = conf.pkg_unpack(
                'zlib_src', ZLIB_SOURCES, conf.path.parent.ant_glob(['patches/*.*'])
            ).path_from(conf.package_node)
            pprint('BLUE', '=zlib', sep=' ')

    if conf.env.PROJECTS or Options.options.minizip_src:
        conf.env.MINIZIP_SOURCE = conf.pkg_unpack('zlib_src', ZLIB_SOURCES,
                                                  conf.path.parent.ant_glob(['patches/*.*']
                                                                            )).path_from(conf.package_node)
        pprint('BLUE', '=minizip', sep=' ')
    elif 'posix' in conf.env.VALID_PLATFORMS:
        try:
            conf.pkg_config('minizip', var='minizip')
        except Exception as e:
            conf.env.MINIZIP_SOURCE = conf.pkg_unpack(
                'zlib_src', ZLIB_SOURCES, conf.path.parent.ant_glob(['patches/*.*'])
            ).path_from(conf.package_node)
            pprint('BLUE', '=minizip', sep=' ')
        else:
            conf.env.MINIZIP_BINARY = True
            pprint('GREEN', '+minizip', sep=' ')
    else:
        try:
            minizip_node = conf.pkg_unpack('minizip_bin_%(platform)s-%(arch)s-%(abi)s', MINIZIP_BINARIES)
            if not conf.check_package('3rdparty.system.minizip', minizip_node, var='minizip'):
                raise WafError('using minizip from source')
            conf.env.MINIZIP_BINARY = minizip_node.path_from(conf.package_node)
        except WafError:
            conf.env.MINIZIP_SOURCE = conf.pkg_unpack(
                'zlib_src', ZLIB_SOURCES, conf.path.parent.ant_glob(['patches/*.*'])
            ).path_from(conf.package_node)
            pprint('BLUE', '=minizip', sep=' ')
