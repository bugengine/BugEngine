from waflib.Errors import WafError
from waflib import Options
import os

BULLET_SOURCES = 'https://github.com/bulletphysics/bullet3/archive/2.87.tar.gz'
BULLET_BINARIES = 'https://github.com/bugengine/BugEngine/releases/download/prebuilt-%(platform)s-%(abi)s/bullet-2.87-%(platform)s-%(arch)s-%(abi)s.tgz'


def setup_pkgconfig(conf):
    if conf.env.CXX_NAME != 'sun':     # sunCC does not have the same ABI on Linux
        try:
            conf.pkg_config('bullet')
        except Exception as e:
            return False
        else:
            conf.env.BULLET_BINARY = True
            conf.end_msg('from pkg-config')
            return True
    else:
        return False


def setup_system(conf):
    if conf.check_lib(['BulletSoftBody', 'BulletDynamics', 'BulletCollision', 'LinearMath'],
                      var='bullet',
                      includepath=['=/usr/include/bullet', '=/usr/local/include/bullet'],
                      includes=['LinearMath/btAlignedAllocator.h'],
                      functions=['btAlignedAllocSetCustom']):
        conf.env.BULLET_BINARY = True
        conf.end_msg('from system')
        return True
    else:
        return False


def setup_prebuilt(conf):
    try:
        bullet_node = conf.pkg_unpack('bullet_bin_%(platform)s-%(arch)s-%(abi)s', BULLET_BINARIES)
        if not conf.check_package('bugengine.3rdparty.physics.bullet', bullet_node, var='bullet'):
            raise WafError('using bullet from source')
    except WafError as e:
        return False
    else:
        conf.env.BULLET_BINARY = bullet_node.path_from(conf.package_node)
        conf.end_msg('from prebuilt')
        return True


def setup_source(conf):
    try:
        bullet_node = conf.pkg_unpack('bullet_src', BULLET_SOURCES, conf.path.parent.ant_glob(['patches/*.*']))
    except WafError:
        return False
    else:
        conf.env.BULLET_SOURCE = bullet_node.path_from(conf.package_node)
        conf.end_msg('from source')
        return True


def setup(conf):
    conf.register_setup_option('bullet_package')
    found = False
    conf.start_msg_setup()
    if conf.env.PROJECTS:
        found = setup_source(conf)
    if not found and Options.options.bullet_package in ('best', 'pkgconfig'):
        found = setup_pkgconfig(conf)
    if not found and Options.options.bullet_package in ('best', 'system'):
        found = setup_system(conf)
    if not found and Options.options.bullet_package in ('best', 'prebuilt'):
        found = setup_prebuilt(conf)
    if not found and Options.options.bullet_package in ('best', 'source'):
        found = setup_source(conf)
    if not found:
        conf.end_msg('disabled', color='RED')
