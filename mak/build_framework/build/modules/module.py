import os
import re
from waflib import Errors
from waflib.Configure import conf
from waflib.TaskGen import feature, before_method, extension

COMPILE_EXTENSIONS = ['cxx', 'cpp', 'cc', 'c', 'rc', 'm', 'mm', 'def', 'masm']


def safe_name(name):
    name = name.replace('-', '_')
    name = name.replace('+', 'x')
    return name


@conf
def add_feature(self, feature, env=None):
    if env:
        env.append_unique('FEATURES', feature)
    else:
        for env in self.multiarch_envs:
            env.append_unique('FEATURES', feature)


def get_source_nodes(build_context, path, name):
    if path is None:
        path = build_context.path
        for n in name.split('.'):
            path = path.find_node(n)
            if not path:
                raise Errors.WafError('could not find module %s in %s' % (name, build_context.path.abspath()))
    source_nodes = [path]
    if path.is_child_of(build_context.bugenginenode):
        relative_path = path.path_from(build_context.bugenginenode)
        for platform in build_context.bugenginenode.find_node('extra').listdir():
            if build_context.env.PROJECTS or platform in build_context.env.VALID_PLATFORMS:
                node = build_context.bugenginenode.find_node('extra').find_node(platform).find_node(relative_path)
                if node:
                    source_nodes.append(node)
    return source_nodes


@conf
def preprocess(build_context, name, path, root_namespace, plugin_name, extra_features=[]):
    source_nodes = get_source_nodes(build_context, path, name)

    pp_env = build_context.common_env.derive()
    pp_env.PLUGIN = plugin_name.replace('.', '_')

    preprocess_sources = []
    if build_context.env.PROJECTS:
        globs = ['nothing']
    else:
        globs = ['src/**/*.yy', 'src/**/*.ll', 'src/**/*.plist', 'api/**/*.script.hh', 'include/**/*.script.hh']
    for source_node in source_nodes:
        preprocess_sources += source_node.ant_glob(globs)

    api = source_node.find_node('api')
    include = source_node.find_node('include')
    pchstop = source_node.find_node('api/bugengine/%s/stdafx.h' % name)
    if pchstop:
        pchstop = pchstop.path_from(api)
    else:
        pchstop = source_node.find_node('api/%s/stdafx.h' % name)
        if pchstop:
            pchstop = pchstop.path_from(api)
        else:
            pchstop = source_node.find_node('include/stdafx.h')
            if pchstop:
                pchstop = pchstop.path_from(include)

    preprocess = build_context(
        env=pp_env,
        target=name + '.pp',
        parent=name,
        features=['bugengine:preprocess'] + extra_features,
        pchstop=pchstop,
        source=preprocess_sources,
        kernels=[],
        source_nodes=source_nodes,
        root_namespace=root_namespace
    )

    for source_node in source_nodes:
        if os.path.isdir(os.path.join(source_node.abspath(), 'kernels')):
            kernelspath = source_node.make_node('kernels')
            for kernel in kernelspath.ant_glob('**'):
                kernel_name = os.path.splitext(kernel.path_from(kernelspath))[0]
                kernel_name = re.split('[\\\\/]', kernel_name)
                preprocess.kernels.append(
                    (
                        kernel_name, kernel,
                        preprocess.make_bld_node('src/kernels', None, '%s.ast' % (os.path.join(*kernel_name)))
                    )
                )

    return preprocess


@conf
def module(
    build_context, name, env, path, depends, private_depends, features, source_list, extra_includes,
    extra_public_includes, extra_defines, extra_public_defines, conditions, **kw_args
):
    for condition in conditions:
        if condition not in env.FEATURES:
            return None
    source_nodes = get_source_nodes(build_context, path, name)
    source_filter = ['src/**/*.%s' % ext for ext in COMPILE_EXTENSIONS]
    includes = []
    api = []
    platform_specific = ['']
    platform_specific += ['.%s' % p for p in env.VALID_PLATFORMS]
    platform_specific += ['.%s' % a for a in env.VALID_ARCHITECTURES]
    platform_specific += ['.%s.%s' % (p, a) for p in env.VALID_PLATFORMS for a in env.VALID_ARCHITECTURES]
    if source_list is None:
        source_list = []
        for node in source_nodes:
            source_list.append(node.ant_glob(source_filter))
            for suffix in platform_specific:
                if node.find_node('include%s' % suffix):
                    includes.append(node.find_node('include%s' % suffix))
                if node.find_node('api%s' % suffix):
                    api.append(node.find_node('api%s' % suffix))
    elif source_list:
        source_list = source_nodes[0].ant_glob(source_list)
    else:
        source_list = []
    if not build_context.env.PROJECTS:
        preprocess = build_context.get_tgen_by_name('%s.pp' % name)
    else:
        preprocess = None

    module_path = None
    if path:
        path_node = source_nodes[0]
        if path_node.is_child_of(build_context.path):
            module_path = path_node.path_from(build_context.path).replace('/', '.').replace('\\', '. ')
        elif path_node.is_child_of(build_context.package_node):
            pass
        elif path_node.is_child_of(build_context.bugenginenode):
            module_path = path_node.path_from(build_context.bugenginenode).replace('/', '.').replace('\\', '. ')
    task_gen = build_context(
        env=env.derive(),
        target=env.ENV_PREFIX % name,
        target_name=name,
        features=features[:],
        use=[env.ENV_PREFIX % d for d in depends],
        private_use=[env.ENV_PREFIX % d for d in private_depends],
        uselib=[build_context.__class__.optim] + (build_context.env.STATIC and ['static'] or ['dynamic']),
        preprocess=preprocess,
        source_nodes=source_nodes,
        source=source_list,
        defines=[
            'building_%s' % safe_name(name.split('.')[-1]),
            'BE_PROJECTID=%s' % name.replace('.', '_'),
            'BE_PROJECTNAME=%s' % name
        ] + extra_defines,
        export_defines=extra_public_defines[:],
        includes=extra_includes + includes + api +
        [build_context.bugenginenode, build_context.srcnode, build_context.bldnode],
        export_includes=extra_public_includes + api,
    )
    if module_path is not None:
        task_gen.module_path = module_path
    return task_gen


@conf
def multiarch(build_context, name, arch_modules):
    arch_modules = [m for m in arch_modules if m is not None]
    if arch_modules:
        if len(build_context.multiarch_envs) == 1:
            task_gen = arch_modules[0]
        else:
            task_gen = build_context(
                target=name,
                features=['bugengine:multiarch'],
                use=[arch_module.target for arch_module in arch_modules],
            )
        return task_gen
    else:
        return None


@extension('.cl')
def handle_cl(self, cl_file):
    pass


def build(build_context):
    pass
