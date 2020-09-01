PCRE_SOURCES = 'https://ftp.pcre.org/pub/pcre/pcre-8.44.tar.gz'


def setup(conf):
    pcre_node = conf.pkg_unpack('pcre_src', PCRE_SOURCES, conf.path.ant_glob(['patches/*.*']))
    conf.env.PCRE_SOURCE = pcre_node.path_from(conf.package_node)
