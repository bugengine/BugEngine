from be_typing import TYPE_CHECKING


def setup(configuration_context):
    # type: (ConfigurationContext) -> None
    if 'pc' in configuration_context.env.VALID_PLATFORMS:
        configuration_context.recurse('glib/setup.py')
        configuration_context.recurse('gobject/setup.py')
        configuration_context.recurse('gtk/setup.py')


if TYPE_CHECKING:
    from waflib.Configure import ConfigurationContext
