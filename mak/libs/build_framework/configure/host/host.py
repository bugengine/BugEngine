import platform
from be_typing import TYPE_CHECKING


def configure(configuration_context):
    os_name = platform.uname()[0].lower().split('-')[0]
    configuration_context.env.HOST = os_name
    configuration_context.recurse('%s.py' % os_name)


if TYPE_CHECKING:
    from waflib import Configure
