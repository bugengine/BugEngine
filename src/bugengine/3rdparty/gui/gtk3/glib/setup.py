from be_typing import TYPE_CHECKING


def setup(configuration_context):
    # type: (ConfigurationContext) -> None
    configuration_context.recurse('../libiconv/setup.py')
    configuration_context.recurse('../pcre/setup.py')


if TYPE_CHECKING:
    from waflib.Configure import ConfigurationContext
