from waflib.Errors import WafError
from waflib import Options
import os

GTK3_BINARIES = 'https://github.com/bugengine/BugEngine/releases/download/prebuilt-gtk3/gtk+-3.0-%(platform)s-%(arch)s.tgz'


def setup_pkgconfig(conf):
    try:
        conf.pkg_config('gtk+-3.0', var='gtk3')
    except Exception as e:
        return False
    else:
        conf.env.GTK3_BINARY = True
        conf.end_msg('from pkg-config')
        return True


def setup_system(conf):
    if conf.check_lib(
        [
            'gtk-3.0', 'gdk-3.0', 'pangocairo-1.0', 'pango-1.0', 'atk-1.0', 'cairo-gobject', 'cairo', 'gdk_pixbuf-2.0',
            'gobject-2.0', 'glib-2.0', 'intl'
        ],
        var='gtk3',
        includes=['gtk/gtk.h'],
        includepath=sum(
            [
                ['=/usr/include/%s' % lib, '=/usr/local/include/%s' % lib] for lib in [
                    'gtk-3.0',
                    'pango-1.0',
                    'glib-2.0',
                    'cairo',
                    'gdk-pixbuf-2.0',
                    'atk-1.0',
                    'harfbuzz',
                ]
            ], []
        ) + [
            '=/usr/lib/glib-2.0/include', '=/usr/local/lib/glib-2.0/include', '=/usr/lib64/glib-2.0/include',
            '=/usr/local/lib64/glib-2.0/include', '=/usr/lib32/glib-2.0/include', '=/usr/local/lib32/glib-2.0/include'
        ],
        functions=['gtk_application_window_new']
    ):
        conf.env.GTK3_BINARY = True
        conf.end_msg('from system')
        return True
    return False


def setup_prebuilt(conf):
    try:
        gtk3_node = conf.pkg_unpack('gtk3_bin_%(platform)s-%(arch)s', GTK3_BINARIES)
        if not conf.check_package(
            ['gtk-3.0', 'gdk-3.0', 'gobject-2.0', 'glib-2.0'],
            gtk3_node,
            var='gtk3',
            includes=['gtk/gtk.h'],
            functions=['gtk_application_window_new']
        ):
            raise WafError('using GTK3 from source')
    except WafError as e:
        return False
    else:
        conf.env.GTK3_BINARY = gtk3_node.path_from(conf.package_node)
        conf.end_msg('from prebuilt')
        return True


def setup_source(conf):
    return False


def setup(conf):
    conf.register_setup_option('gtk3_package')
    if conf.env.PROJECTS:
        return
    found = False
    if 'pc' in conf.env.VALID_PLATFORMS:
        conf.start_msg_setup()
        if not found and Options.options.gtk3_package in ('best', 'pkgconfig'):
            found = setup_pkgconfig(conf)
        if not found and Options.options.gtk3_package in ('best', 'system'):
            found = setup_system(conf)
        if not found and Options.options.gtk3_package in ('best', 'prebuilt'):
            found = setup_prebuilt(conf)
        if not found and Options.options.gtk3_package in ('best', 'source'):
            found = setup_source(conf)
        if not found:
            conf.end_msg('disabled - editor will not be built', color='RED')
