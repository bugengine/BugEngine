from be_typing import TYPE_CHECKING


def setup(configuration_context):
    # type: (ConfigurationContext) -> None
    configuration_context.recurse('glib/setup.py')
    configuration_context.recurse('gobject/setup.py')
    configuration_context.recurse('gtk/setup.py')


if TYPE_CHECKING:
    from waflib.Configure import ConfigurationContext
