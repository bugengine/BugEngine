from waflib.Errors import WafError
from waflib import Options
import os


GTK3_SOURCES = 'https://github.com/GNOME/gtk/archive/3.24.22.tar.gz'
GTK3_BINARIES = 'https://github.com/bugengine/BugEngine/releases/download/prebuilt-gtk3/gtk3-3.24.22-%(platform)s-%(arch)s.tgz'


def setup_system(conf):
    try:
        conf.pkg_config('gtk+-3.0', var='gtk3')
    except Exception as e:
        return False
    else:
        conf.env.GTK3_BINARY = True
        conf.end_msg('from system')
        return True


def setup_prebuilt(conf):
    try:
        gtk3_node = conf.pkg_unpack('gtk3_bin_%(platform)s-%(arch)s-%(abi)s', GTK3_BINARIES)
        if not conf.check_package('3rdparty.gui.gtk3', gtk3_node, var='gtk3'):
            raise WafError('using GTK3 from source')
    except WafError as e:
        return False
    else:
        conf.env.GTK3_BINARY = gtk3_node.path_from(conf.package_node)
        conf.end_msg('from prebuilt')
        return True


def setup_source(conf):
    try:
        gtk3_node = conf.pkg_unpack('gtk3_src', GTK3_SOURCES, conf.path.parent.ant_glob(['patches/*.*']))
    except WafError:
        return False
    else:
        conf.env.GTK3_SOURCE = gtk3_node.path_from(conf.package_node)
        conf.end_msg('from source')
        return True


def setup(conf):
    conf.register_setup_option('bullet_package')
    found = False
    conf.start_msg_setup()
    if conf.env.PROJECTS:
        found = setup_source(conf)
    if not found and Options.options.bullet_package in ('best', 'system'):
        found = setup_system(conf)
    if not found and Options.options.bullet_package in ('best', 'prebuilt'):
        found = setup_prebuilt(conf)
    if not found and Options.options.bullet_package in ('best', 'source'):
        found = setup_source(conf)
    if not found:
        conf.end_msg('disabled', color='RED')
