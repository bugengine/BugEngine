from waflib import Options

def options(opt):
    gr = opt.add_option_group('configure options')
    gr.add_option( '--compilers',
                    action='store',
                    default='',
                    dest='compilers',
                    help='List of compilers to configure for')
    for path in opt.path.make_node('compiler').listdir():
        opt.recurse('compiler/%s'%path)

def configure(conf):
    compilers = Options.options.compilers
    compilers = compilers.split(',') if compilers else []
    for path in conf.path.make_node('compiler').listdir():
        if not compilers or path[:-3] in compilers:
            conf.recurse('compiler/%s'%path)

def build(bld):
    compilers = Options.options.compilers
    compilers = compilers.split(',') if compilers else []
    for path in bld.path.make_node('compiler').listdir():
        if not compilers or path[:-3] in compilers:
            bld.recurse('compiler/%s'%path)
