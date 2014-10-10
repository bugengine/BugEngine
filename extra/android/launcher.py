import os

def build(bld):
    source_node = bld.path.find_node('src/engine/launcher/src')
    resource_node = bld.path.find_node('src/engine/launcher/res')
    launcher = bld(target = 'engine.android.launcher',
                   features=['cxx', 'javac', 'dex'],
                   source_nodes = [source_node, resource_node],
                   destfile='classes.dex',
                   outdir=source_node.find_or_declare('jar'),
                   basedir=source_node.find_or_declare('jar'))

    manifest = bld.path.make_node('AndroidManifest.xml')
    resources = bld(target = 'engine.android.resource',
                   features=['aapt_resource'],
                   resource=resource_node,
                   destfile=bld.bldnode.find_or_declare('resources.apk'),
                   manifest=manifest)
    outdir = os.path.join(bld.env.PREFIX, bld.optim)
    bld.install_files(outdir, bld.path.find_or_declare(launcher.destfile))


