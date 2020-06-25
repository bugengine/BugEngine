from waflib.Logs import pprint
from waflib.Errors import WafError
from waflib import Options
import os

FT_SOURCES = 'https://download.savannah.gnu.org/releases/freetype/freetype-2.10.2.tar.gz'
FT_BINARIES = 'https://github.com/bugengine/BugEngine/releases/download/v0.0/freetype-2.10.2-%(platform)s-%(arch)s.tgz'


def setup(conf):
    conf.register_setup_option('freetype_src')
    if conf.env.PROJECTS or Options.options.freetype_src:
        conf.env.FREETYPE_SOURCE = conf.pkg_unpack('freetype_src', FT_SOURCES).path_from(conf.package_node)
        pprint('BLUE', '=freetype', sep=' ')
    elif 'posix' in conf.env.VALID_PLATFORMS:
        try:
            conf.pkg_config('freetype2', var='freetype')
        except Exception as e:
            conf.env.FREETYPE_SOURCE = conf.pkg_unpack('freetype_src', FT_SOURCES).path_from(conf.package_node)
            pprint('BLUE', '=freetype', sep=' ')
        else:
            conf.env.FREETYPE_BINARY = True
            pprint('GREEN', '+freetype', sep=' ')
    else:
        try:
            freetype_node = conf.pkg_unpack('freetype_bin_%(platform)s-%(arch)s', FT_BINARIES)
            if not conf.check_package(
                '3rdparty.system.freetype',
                freetype_node,
                var='freetype',
                includes=['ft2build.h', 'freetype/freetype.h', 'freetype/t1tables.h'],
                functions=['FT_Get_PS_Font_Info']
            ):
                raise WafError('using freetype from source')
            conf.env.FREETYPE_BINARY = freetype_node.path_from(conf.package_node)
        except WafError:
            conf.env.FREETYPE_SOURCE = conf.pkg_unpack('freetype_src', FT_SOURCES).path_from(conf.package_node)
            pprint('BLUE', '=freetype', sep=' ')
