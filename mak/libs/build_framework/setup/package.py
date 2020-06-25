from waflib.Configure import conf
from waflib import Errors, Utils
import tarfile
import shutil
import sys
import os
import tempfile
from patch import fromfile
if sys.version_info < (3, ):
    import urllib2 as request
else:
    from urllib import request


@conf
def get_package_node(context, package_id):
    args = {}
    if context.env.VALID_PLATFORMS:
        args['platform'] = context.env.VALID_PLATFORMS[0]
    if context.env.VALID_ARCHITECTURES:
        args['arch'] = context.env.VALID_ARCHITECTURES[0]
    if context.env.COMPILER_NAME:
        args['compiler'] = context.env.COMPILER_NAME
    if context.env.COMPILER_ABI:
        args['abi'] = context.env.COMPILER_ABI
    specific_package_id = package_id % (args)
    try:
        package_path = context.all_envs['packages'].PACKAGE_REPOS[specific_package_id]
        return context.package_node.make_node(package_path)
    except KeyError:
        args['arch'] = 'multiarch'
        multiarch_package_id = package_id % (args)
        try:
            package_path = context.all_envs['packages'].PACKAGE_REPOS[specific_package_id]
            return context.package_node.make_node(package_path)
        except KeyError:
            raise Errors.WafError('no package %s setup' % package_id)


@conf
def pkg_unpack(configuration_context, package_id_template, package_url, patch_list=[]):
    args = {}
    if configuration_context.env.VALID_PLATFORMS:
        args['platform'] = configuration_context.env.VALID_PLATFORMS[0]
    if configuration_context.env.VALID_ARCHITECTURES:
        args['arch'] = configuration_context.env.VALID_ARCHITECTURES[0]
    if configuration_context.env.COMPILER_NAME:
        args['compiler'] = configuration_context.env.COMPILER_NAME
    if configuration_context.env.COMPILER_ABI:
        args['abi'] = configuration_context.env.COMPILER_ABI
    try:
        package_id = package_id_template % args
        if package_id in configuration_context.package_env.PACKAGE_REPOS:
            pkg_node = configuration_context.package_node.make_node(
                configuration_context.package_env.PACKAGE_REPOS[package_id]
            )
            if pkg_node.isdir():
                return pkg_node
        try:
            pkg = request.urlopen(package_url % args)
        except Exception as e:
            args['arch'] = 'multiarch'
            package_id = package_id_template % args
            if package_id in configuration_context.package_env.PACKAGE_REPOS:
                pkg_node = configuration_context.package_node.make_node(
                    configuration_context.package_env.PACKAGE_REPOS[package_id]
                )
                if pkg_node.isdir():
                    return pkg_node
            pkg = request.urlopen(package_url % args)
    except Exception as e:
        raise Errors.WafError('failed to download package "%s": %s' % (package_url, e))

    try:
        shutil.rmtree(os.path.join(configuration_context.package_node.abspath(), package_id))
    except OSError:
        pass

    with tempfile.TemporaryFile(mode='w+b') as archive_file:
        archive_file.write(pkg.read())
        archive_file.seek(0)
        archive = tarfile.open(fileobj=archive_file, mode='r')
        root_path = ''
        info = archive.getmembers()[0]
        root_path = info.name
        archive.extractall(path=os.path.join(configuration_context.package_node.abspath(), package_id))
        for patch in patch_list:
            patch = fromfile(patch.abspath())
            patch.apply(1, configuration_context.package_node.make_node(os.path.join(package_id, root_path)).abspath())
    configuration_context.package_env.PACKAGE_REPOS[package_id] = os.path.join(package_id, root_path)
    return configuration_context.package_node.make_node(os.path.join(package_id, root_path))


def multiarch_setup(configuration_context):
    configuration_context.package_env = configuration_context.all_envs['packages']
    configuration_context.package_node = configuration_context.bldnode.make_node('packages')
    configuration_context.package_node.mkdir()
