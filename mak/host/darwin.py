# set MacOS X specific options
# detect tools : gcc, icc, clang, llvm

import os
import sys
import re
from waflib.Logs import pprint
from waflib import Options, Utils


def options(opt):
    try:
        p = Utils.subprocess.Popen(['xcode-select', '--print-path'], stdin=Utils.subprocess.PIPE, stdout=Utils.subprocess.PIPE, stderr=Utils.subprocess.PIPE)
        out = p.communicate()[0]
    except:
        out = '/Developer,/Applications/Xcode.app/Contents/Developer'
    if not isinstance(out, str):
        out = out.decode(sys.stdout.encoding)
    out = out.split('\n')[0]
    opt.add_option( '--xcode-sdks',
                    action='store',
                    default=out,
                    dest='xcode_sdks',
                    help='Paths of the different XCode SDKs')


def run_command(cmd, input=None, env=None):
    try:
        p = Utils.subprocess.Popen(cmd, stdin=Utils.subprocess.PIPE,
                                   stdout=Utils.subprocess.PIPE,
                                   stderr=Utils.subprocess.PIPE,
                                   env=env)
        if input is not None:
            p.stdin.write(input.encode())
        out, err = p.communicate()
    except Exception as e:
        return (-1, '', str(e))
    else:
        if not isinstance(out, str):
            out = out.decode(sys.stdout.encoding, errors='ignore')
        if not isinstance(err, str):
            err = err.decode(sys.stderr.encoding, errors='ignore')
        return (p.returncode, out, err)


def configure(conf):
    os.environ['PATH'] = '/System/Library/Frameworks/OpenCL.framework/Libraries:'+os.environ['PATH']
    conf.darwin_sdks = {}
    os_sdk_paths = []
    os_sdk_regexp = re.compile('([a-zA-Z]+)([0-9\\.]+)u?\\.sdk')
    for p in Options.options.xcode_sdks.split(',')[::-1]:
        if os.path.isdir(os.path.join(p, 'SDKs')):
            os_sdk_paths.append(os.path.normpath(os.path.join(p, 'SDKs')))
        try:
            for platform in os.listdir(os.path.join(p, 'Platforms')):
                os.environ['PATH'] = os.pathsep.join((os.path.join(p, 'Platforms', platform, 'Developer', 'usr', 'bin'),
                                                      os.environ['PATH']))
                s_path = os.path.normpath(os.path.join(p, 'Platforms', platform, 'Developer', 'SDKs'))
                if os.path.isdir(s_path):
                    os_sdk_paths.append(s_path)
        except OSError:
            pass
        os.environ['PATH'] = ('%s/Toolchains/XcodeDefault.xctoolchain/usr/bin:%s/usr/bin:'%(p,p))+os.environ['PATH']
    conf.find_program('file')

    archs = {
        'ppc':      'ppc',
        'ppc_7400': 'ppc',
        'ppc64':    'ppc64',
        'x86_64':   'amd64',
        'i386':     'x86',
        'arm_v6':   'armv6',
        'armv7':    'armv7a',
        'arm_v7':   'armv7a',
        'armv7s':   'armv7s',
        'arm_v7s':  'armv7s',
        'armv7k':   'armv7k',
        'arm_v7k':  'armv7k',
        'arm64':    'arm64',
    }

    for p in os_sdk_paths:
        for sdk in os.listdir(p):
            sdk_path = os.path.join(p, sdk)
            try:
                sdk_os, sdk_version = os_sdk_regexp.match(sdk).group(1, 2)
            except AttributeError:
                continue
            else:
                sdk_archs = []
                sdk_version = sdk_version.split('.')
                dylib = os.path.join(sdk_path, 'usr', 'lib', 'libc.dylib')
                if os.path.isfile(dylib):
                    r, out, err = run_command([conf.env.FILE, dylib])
                    for line in out.split('\n'):
                        line = line.strip()
                        if not line:
                            continue
                        if line.find('Mach-O universal') != -1:
                            continue
                        arch_name = line.split()[-1]
                        try:
                            arch_name = archs[arch_name]
                        except KeyError:
                            print('Unknown %s arch: %s in %s' % (sdk_os, arch_name, dylib))
                        else:
                            sdk_archs.append(arch_name)
                else:
                    tbd = os.path.join(sdk_path, 'usr', 'lib', 'libc.tbd')
                    if os.path.isfile(tbd):
                        with open(tbd, 'r') as tbd_file:
                            for line in tbd_file.readlines():
                                line = line.strip()
                                if line.startswith('archs:'):
                                    line = line.split()
                                    for a in line[2:-1]:
                                        try:
                                            sdk_archs.append(archs[a[:-1]] if a[-1] == ',' else archs[a])
                                        except KeyError:
                                            print('Unknown %s arch: %s in %s' % (sdk_os, a, dylib))
                try:
                    conf.darwin_sdks[sdk_os].append((sdk_version, sdk_archs, sdk_path))
                    conf.darwin_sdks[sdk_os].sort()
                except KeyError:
                    conf.darwin_sdks[sdk_os] = [(sdk_version, sdk_archs, sdk_path)]


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