import os
import shlex
from be_typing import TYPE_CHECKING


def get_jail_sysroots():
    try:
        with open('/etc/jail.conf', 'r') as jail_conf:
            jail = jail_conf.readlines()
    except (OSError, IOError):
        return []
    else:
        roots = []
        for line in jail:
            line = line.split('#')[0].strip()
            line = line.split('=')
            if len(line) == 2:
                if line[0].strip() == 'path':
                    roots.append(shlex.split(line[1][:-1])[0])
        return roots


def options(option_context):
    # type: (Options.OptionsContext) -> None
    gr = option_context.add_option_group('configure options')
    gr.add_option('--platforms', action='store', default='', dest='platforms', help="List of platform to configure for")
    sysroots = []
    for path in ('/opt/sys', '/opt/sysroots'):
        try:
            dirs = os.listdir(path)
        except OSError:
            pass
        else:
            for dir in dirs:
                absolute_path = os.path.join(path, dir)
                if os.path.isdir(absolute_path):
                    sysroots.append(absolute_path)

    gr.add_option(
        '--sysroot',
        action='append',
        default=sysroots + get_jail_sysroots(),
        dest='sysroots',
        help="List of directories that map to a platform's sysroot"
    )

    for target in os.listdir(option_context.path.abspath()):
        option_context.recurse(target) # conveniently will not recurse in this module again


if TYPE_CHECKING:
    from waflib import Options
