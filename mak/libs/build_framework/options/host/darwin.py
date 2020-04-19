import os
import sys
from waflib import Utils


def options(opt):
    sdks = ['/%s' % d for d in os.listdir('/') if d.startswith('Developer')]
    sdks += [
        os.path.join('/', 'Applications', d, 'Contents', 'Developer') for d in os.listdir('/Applications')
        if d.startswith('Xcode')
    ]
    try:
        p = Utils.subprocess.Popen(['xcode-select', '--print-path'],
                                   stdin=Utils.subprocess.PIPE,
                                   stdout=Utils.subprocess.PIPE,
                                   stderr=Utils.subprocess.PIPE)
        out = p.communicate()[0]
    except Exception:
        pass
    else:
        if not isinstance(out, str):
            out = out.decode(sys.stdout.encoding)
        out = out.split('\n')[0]
        if out not in sdks:
            sdks.append(out)
    opt.add_option('--xcode-sdks',
                   action='store',
                   default=','.join(sdks),
                   dest='xcode_sdks',
                   help='Paths of the different XCode SDKs')

