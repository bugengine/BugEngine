# set MacOS X specific options
# detect tools : gcc, icc, clang, llvm

import os
import sys
import shlex
from waflib import Options, Utils
from be_typing import TYPE_CHECKING


def configure(conf):
    environ = getattr(conf, 'environ', os.environ)
    environ['PATH'] = '/System/Library/Frameworks/OpenCL.framework/Libraries:' + environ['PATH']
    conf.darwin_sdks = {}
    conf.os_sdk_paths = []
    for p in Options.options.xcode_sdks.split(',')[::-1]:
        if os.path.isdir(os.path.join(p, 'SDKs')):
            conf.os_sdk_paths.append(os.path.normpath(os.path.join(p, 'SDKs')))
        try:
            for platform in os.listdir(os.path.join(p, 'Platforms')):
                conf.env.append_value('EXTRA_PATH',
                                      [os.path.join(p, 'Platforms', platform, 'Developer', 'usr', 'bin')])
                s_path = os.path.normpath(os.path.join(p, 'Platforms', platform, 'Developer', 'SDKs'))
                if os.path.isdir(s_path):
                    os_sdk_paths.append(s_path)
        except OSError:
            pass
        conf.env.append_value('EXTRA_PATH', ['%s/Toolchains/XcodeDefault.xctoolchain/usr/bin'%p,
                                             '%s/usr/bin' % p])
    # find valid code signing identity
    process = Utils.subprocess.Popen(['security', 'find-identity', '-p', 'codesigning', '-v'],
                                     stdout=Utils.subprocess.PIPE,
                                     stderr=Utils.subprocess.STDOUT)
    output, error = process.communicate()
    if not isinstance(output, str):
        output = output.decode(sys.stdout.encoding)
    for line in output.split('\n'):
        line = shlex.split(line.strip())
        if len(line) == 3:
            guid = line[1]
            identity = line[2]
            dev = identity.split(':')[0]
            conf.env.append_unique('MAC_SIGNING_IDENTITIES', [(dev, guid, identity)])


if TYPE_CHECKING:
    from waflib import Configure
