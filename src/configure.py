def configure(configuration_context):
    configuration_context.recurse('plugin/compute/cpu/configure.py', name='configure')
    configuration_context.recurse('plugin/compute/opencl/configure.py', name='configure')
    configuration_context.recurse('plugin/compute/cuda/configure.py', name='configure')


def setup(configuration_context):
    configuration_context.recurse('plugin/compute/cpu/configure.py', name='setup', once=False)
    configuration_context.recurse('plugin/compute/opencl/configure.py', name='setup', once=False)
    configuration_context.recurse('plugin/compute/cuda/configure.py', name='setup', once=False)