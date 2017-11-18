#!/usr/bin/env python
# encoding: utf-8

from waflib import Task, Errors
from waflib.TaskGen import feature
import os
import sys
from waflib import Task

class copy(Task.Task):
    color   = 'PINK'
    def run(self):
        self.outputs[0].write(self.inputs[0].read())
    def scan(self):
        return ([], [])


@feature('kernel_build')
def kernel_build_cpu_source(self):
    for kernel, source in getattr(self, 'kernels', []):
        name = self.name
        internal_kernels = {}
        original_source = source
        fname, fext = os.path.splitext(source.name)
        if fext == '.cc':
            pass
        elif fext == '.cl':
            source = self.make_bld_node('src', source.parent, source.name[:-2]+'cc')
            t = self.create_task('copy', [original_source], [source])
        else:
            raise Errors.WafError('Unknown file extension: %s' % source.path_from(self.bld.srcnode))
        for env in self.bld.multiarch_envs:
            for variant in [''] + env.KERNEL_OPTIM_VARIANTS:
                target_prefix = (env.ENV_PREFIX + '.') if env.ENV_PREFIX else ''
                t = self.bld(
                    env = env.derive(),
                    target = target_prefix + name + '.' + '.'.join(kernel) + ".cpu" + variant,
                    features = ['cxx', self.bld.env.STATIC and 'cxxobjects' or 'cxxshlib', 'kernel'],
                    extra_use = self.extra_use,
                    defines = self.defines + [
                            'BE_BUILD_KERNEL=1',
                            'BE_KERNEL_ID=%s_%s'%(name.replace('.', '_'),'_'.join(kernel)),
                            'BE_KERNEL_NAME=%s.%s'%(name, '.'.join(kernel)),
                            'BE_KERNEL_TARGET=cpp',
                            'BE_KERNEL_ARCH=%s' % variant],
                    includes = self.includes,
                    source = [source],
                    use = self.use + ['plugin.compute.cpu'] + ([variant] if variant else []))
                t.env.PLUGIN = self.env.PLUGIN
        #        t.post()
        #        if target_prefix:
        #            try:
        #                internal_kernels[variant].append((kernel_name, kernels, kernelsources))
        #            except KeyError:
        #                internal_kernels[variant] = [(kernel_name, kernels, kernelsources)]
        #for kernel, kernel_deps, kernel_sources in internal_kernels:
        #    if kernel_deps:
        #        bld(target=name + '.' + kernel, features=['multiarch'], use=kernel_deps)
