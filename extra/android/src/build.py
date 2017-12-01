import os
from waflib.TaskGen import feature

def build(bld):
    #bld.platforms.append(bld.external('3rdparty.android.stl-gabi++'))
    bld.platforms.append(bld.external('3rdparty.android.libc++'))

    root = bld.path.parent
    source_node = root.find_node('src/engine/launcher/src')
    resource_node = root.find_node('src/engine/launcher/res')
    launcher = bld(target = 'engine.android.launcher',
                   features=['cxx', 'javac', 'dex'],
                   source_nodes = [source_node, resource_node],
                   destfile='classes.dex',
                   outdir=source_node.find_or_declare('jar'),
                   basedir=source_node.find_or_declare('jar'))

    manifest = root.make_node('AndroidManifest.xml')
    resources = bld(target = 'engine.android.resource',
                   features=['aapt_resource'],
                   resource=resource_node,
                   destfile=root.find_or_declare('resources.apk'),
                   manifest=manifest)
    outdir = os.path.join(bld.env.PREFIX, bld.optim)
    launcher.install_files(outdir, bld.path.find_or_declare(launcher.destfile))


def plugin(bld):
    pass

