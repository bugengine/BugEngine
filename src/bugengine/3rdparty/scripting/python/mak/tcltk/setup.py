TCLTK_PACKAGE = 'https://github.com/bugengine/BugEngine/releases/download/prebuilt-python/tcltk-%(platform)s.tgz'


def setup(configuration_context):
    if 'macos' in configuration_context.env.PLATFORMS:
        try:
            node = configuration_context.pkg_unpack(
                'tcltk-%(platform)s',
                TCLTK_PACKAGE,
            )
        except WafError:
            pass
        else:
            conf.env.TCLTK_BINARY = node.path_from(configuration_context.package_node)
            conf.env.check_tcltk = True
