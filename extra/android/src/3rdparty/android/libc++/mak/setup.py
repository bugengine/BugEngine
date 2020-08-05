from waflib import Errors
from waflib.TaskGen import feature
import os

LIBCPP_BINARY = 'https://github.com/bugengine/BugEngine/releases/download/prebuilt-libc++/libc++-%(platform)s-%(arch)s-%(abi)s.tgz'


@feature('cxxtest')
def cxx_link_libcpp(task_gen):
    if task_gen.env['check_libc++']:
        task_gen.env.append_value('LIB', task_gen.env['check_libc++_libs'])
        task_gen.env.append_value('LIBPATH', task_gen.env['check_libc++_libpath'])


def setup(configuration_context):
    configuration_context.start_msg_setup()
    try:
        node = configuration_context.pkg_unpack('libcpp_%(platform)s_%(arch)s_%(abi)s', LIBCPP_BINARY)
        paths = [os.path.join(node.abspath(), 'bin.%s' % a) for a in configuration_context.env.VALID_ARCHITECTURES]
        paths += [os.path.join(node.abspath(), 'lib.%s' % a) for a in configuration_context.env.VALID_ARCHITECTURES]
        paths = [p for p in paths if os.path.isdir(p)]
        if not configuration_context.check_package('c++', node, libpath=paths, var='libc++'):
            raise Errors.WafError('package not setup')
    except Errors.WafError as e:
        configuration_context.end_msg(str(e), color='RED')
    else:
        configuration_context.env.LIBCPP_BINARY = node.path_from(configuration_context.package_node)
        configuration_context.end_msg('from prebuilt')
