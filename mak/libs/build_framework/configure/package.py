import os
from be_typing import TYPE_CHECKING


def configure(configuration_context):
    # type: (Configure.ConfigurationContext) -> None
    configuration_context.setenv('packages')
    configuration_context.env['PACKAGE_REPOS'] = {}
    configuration_context.variant = ''


if TYPE_CHECKING:
    from waflib import Configure