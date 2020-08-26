from waflib.Errors import WafError
from waflib import Options
import os

FT_SOURCES = 'https://download.savannah.gnu.org/releases/freetype/freetype-2.10.2.tar.gz'
FT_BINARIES = 'https://github.com/bugengine/BugEngine/releases/download/prebuilt-%(platform)s-%(abi)s/freetype-2.10.2-%(platform)s-%(arch)s-%(abi)s.tgz'


def setup_pkgconfig(conf):
    try:
        conf.pkg_config('freetype2', var='freetype')
    except WafError as e:
        return False
    else:
        conf.env.FREETYPE_BINARY = True
        conf.end_msg('from pkg-config')
        return True


def setup_system(conf):
    if 'windows' not in conf.env.VALID_PLATFORMS and conf.check_lib(
        'freetype',
        var='freetype',
        includes=['ft2build.h', 'freetype/freetype.h', 'freetype/t1tables.h'],
        includepath=['=/usr/include/freetype2', '=/usr/local/include/freetype2'],
        functions=['FT_Get_PS_Font_Info']
    ):
        conf.env.FREETYPE_BINARY = True
        conf.end_msg('from system')
        return True
    return False


def setup_prebuilt(conf):
    try:
        freetype_node = conf.pkg_unpack('freetype_bin_%(platform)s-%(arch)s-%(abi)s', FT_BINARIES)
        if not conf.check_package(
            'bugengine.3rdparty.system.freetype',
            freetype_node,
            var='freetype',
            includes=['ft2build.h', 'freetype/freetype.h', 'freetype/t1tables.h'],
            functions=['FT_Get_PS_Font_Info']
        ):
            raise WafError('using freetype from source')
        conf.env.FREETYPE_BINARY = freetype_node.path_from(conf.package_node)
        conf.end_msg('from prebuilt')
        return True
    except WafError:
        return False


def setup_source(conf):
    try:
        conf.env.FREETYPE_SOURCE = conf.pkg_unpack('freetype_src', FT_SOURCES).path_from(conf.package_node)
    except WafError:
        return False
    else:
        conf.end_msg('from source')
        return True


def setup(conf):
    conf.register_setup_option('freetype_package')
    conf.start_msg_setup()
    found = False
    if conf.env.PROJECTS:
        found = setup_source(conf)
    if not found and Options.options.freetype_package in ('best', 'pkgconfig'):
        found = setup_pkgconfig(conf)
    if not found and Options.options.freetype_package in ('best', 'system'):
        found = setup_system(conf)
    if not found and Options.options.freetype_package in ('best', 'prebuilt'):
        found = setup_prebuilt(conf)
    if not found and Options.options.freetype_package in ('best', 'source'):
        found = setup_source(conf)
    if not found:
        conf.end_msg('disabled', color='RED')
