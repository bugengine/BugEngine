from waflib.Logs import pprint
from waflib.Errors import WafError
from waflib import Options
import os

BULLET_SOURCES = 'https://github.com/bulletphysics/bullet3/archive/2.87.tar.gz'
BULLET_BINARIES = 'https://github.com/bugengine/BugEngine/releases/download/v0.0/bullet-2.87-%(platform)s-%(arch)s-%(abi)s.tgz'


def setup(conf):
    conf.register_setup_option('bullet_src')
    if conf.env.CXX_NAME != 'sun' and 'posix' in conf.env.VALID_PLATFORMS and not Options.options.bullet_src:
        try:
            conf.pkg_config('bullet')
        except Exception as e:
            pass
        else:
            conf.env.BULLET_BINARY = True
            pprint('GREEN', '+bullet', sep=' ')
    if not conf.env.BULLET_BINARY:
        if not Options.options.bullet_src:
            try:
                bullet_node = conf.pkg_unpack('bullet_bin_%(platform)s-%(arch)s-%(abi)s', BULLET_BINARIES)
                if not conf.check_package('3rdparty.physics.bullet', bullet_node, var='bullet'):
                    raise WafError('using bullet from source')
            except WafError:
                pass
            else:
                conf.env.BULLET_BINARY = bullet_node.path_from(conf.package_node)
                return

        bullet_node = conf.pkg_unpack('bullet_src', BULLET_SOURCES, conf.path.parent.ant_glob(['patches/*.*']))
        conf.env.BULLET_SOURCE = bullet_node.path_from(conf.package_node)
        pprint('BLUE', '=bullet', sep=' ')