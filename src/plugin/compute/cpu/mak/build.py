def build(build_context):
    build_context.load('kernel_cpu', tooldir=[build_context.path.make_node('mak').abspath()])