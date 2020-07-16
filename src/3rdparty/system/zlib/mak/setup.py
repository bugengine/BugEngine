from waflib.Errors import WafError
from waflib import Options
import os

ZLIB_SOURCES = 'https://zlib.net/zlib-1.2.11.tar.gz'
ZLIB_BINARIES = 'https://github.com/bugengine/BugEngine/releases/download/prebuilt-%(platform)s-%(abi)s/zlib-1.2.11-%(platform)s-%(arch)s-%(abi)s.tgz'
MINIZIP_BINARIES = 'https://github.com/bugengine/BugEngine/releases/download/prebuilt-%(platform)s-%(abi)s/minizip-1.2.11-%(platform)s-%(arch)s-%(abi)s.tgz'


def setup_system_zlib(conf):
    try:
        conf.pkg_config('zlib', var='zlib')
    except Exception as e:
        return False
    else:
        conf.env.ZLIB_BINARY = True
        conf.end_msg('from system')
        return True


def setup_system_minizip(conf):
    try:
        conf.pkg_config('minizip', var='minizip')
    except Exception as e:
        return False
    else:
        conf.env.MINIZIP_BINARY = True
        conf.end_msg('from system')
        return True


def setup_prebuilt_zlib(conf):
    try:
        zlib_node = conf.pkg_unpack('zlib_bin_%(platform)s-%(arch)s-%(abi)s', ZLIB_BINARIES)
        if not conf.check_package('3rdparty.system.zlib', zlib_node, var='zlib'):
            raise WafError('using zlib from source')
    except WafError:
        return False
    else:
        conf.env.ZLIB_BINARY = zlib_node.path_from(conf.package_node)
        conf.end_msg('from prebuilt')
        return True


def setup_prebuilt_minizip(conf):
    try:
        minizip_node = conf.pkg_unpack('minizip_bin_%(platform)s-%(arch)s-%(abi)s', MINIZIP_BINARIES)
        if not conf.check_package('3rdparty.system.minizip', minizip_node, var='minizip'):
            raise WafError('using minizip from source')
    except WafError:
        return False
    else:
        conf.env.MINIZIP_BINARY = minizip_node.path_from(conf.package_node)
        conf.end_msg('from prebuilt')
        return True


def setup_source_zlib(conf):
    try:
        conf.env.ZLIB_SOURCE = conf.pkg_unpack('zlib_src', ZLIB_SOURCES,
                                               conf.path.parent.ant_glob(['patches/*.*'])).path_from(conf.package_node)
    except WafError:
        return False
    else:
        conf.end_msg('from source')
        return True


def setup_source_minizip(conf):
    try:
        conf.env.MINIZIP_SOURCE = conf.pkg_unpack('zlib_src', ZLIB_SOURCES,
                                                  conf.path.parent.ant_glob(['patches/*.*']
                                                                            )).path_from(conf.package_node)
    except WafError:
        return False
    else:
        conf.end_msg('from source')
        return True


def setup(conf):
    conf.register_setup_option('zlib_package')
    conf.register_setup_option('minizip_package')
    conf.start_msg_setup('zlib')
    found = False
    if conf.env.PROJECTS:
        found = setup_source_zlib(conf)
    if not found and Options.options.zlib_package in ('best', 'system'):
        found = setup_system_zlib(conf)
    if not found and Options.options.zlib_package in ('best', 'prebuilt'):
        found = setup_prebuilt_zlib(conf)
    if not found and Options.options.zlib_package in ('best', 'source'):
        found = setup_source_zlib(conf)
    if not found:
        conf.end_msg('disabled', color='RED')

    conf.start_msg_setup('minizip')
    found = False
    if conf.env.PROJECTS:
        found = setup_source_minizip(conf)
    if not found and Options.options.minizip_package in ('best', 'system'):
        found = setup_system_minizip(conf)
    if not found and Options.options.minizip_package in ('best', 'prebuilt'):
        found = setup_prebuilt_minizip(conf)
    if not found and Options.options.minizip_package in ('best', 'source'):
        found = setup_source_minizip(conf)
    if not found:
        conf.end_msg('disabled', color='RED')
