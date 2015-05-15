# set MacOS X specific options
# detect tools : gcc, icc, clang, llvm

import os, sys
from waflib.Logs import pprint
from waflib import Options, Utils


def options(opt):
    try:
        p = Utils.subprocess.Popen(['xcode-select', '--print-path'], stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
        p.stdin.write('\n'.encode())
        out = p.communicate()[0]
    except:
        out = '/Developper,/Applications/Xcode.app/Contents/Developer'
    if not isinstance(out, str):
        out = out.decode(sys.stdout.encoding)
    out = out.split('\n')[0]
    opt.add_option( '--xcode-sdks',
                    action='store',
                    default=out,
                    dest='xcode_sdks',
                    help='Paths of the different XCode SDKs')

def configure(conf):
    os.environ['PATH'] = '/System/Library/Frameworks/OpenCL.framework/Libraries:'+os.environ['PATH']
    for p in Options.options.xcode_sdks.split(',')[::-1]:
        try:
            for platform in os.listdir(os.path.join(p, 'Platforms')):
                os.environ['PATH'] = ('%s/Platforms/%s/Developer/usr/bin:'%(p, platform))+os.environ['PATH']

        except:
            pass
        os.environ['PATH'] = ('%s/Toolchains/XcodeDefault.xctoolchain/usr/bin:%s/usr/bin:'%(p,p))+os.environ['PATH']

def build(bld):
    for p in Options.options.xcode_sdks.split(',')[::-1]:
        try:
            for platform in os.listdir(os.path.join(p, 'Platforms')):
                os.environ['PATH'] = ('%s/Platforms/%s/Developer/usr/bin:'%(p, platform))+os.environ['PATH']

        except:
            pass
        os.environ['PATH'] = ('%s/Toolchains/XcodeDefault.xctoolchain/usr/bin:%s/usr/bin:'%(p,p))+os.environ['PATH']

def plugins(bld):
    pass