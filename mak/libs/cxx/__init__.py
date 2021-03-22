from . import cxx_lexer, cxx_parser, cxx_grammar, cxx_ast, cxx_messages
from argparse import ArgumentParser


def init_arguments():
    # type: () -> ArgumentParser
    argument_context = ArgumentParser()
    argument_context.add_argument(
        "-t", "--tmp", dest="tmp_dir", help="Directory to store temporary/cached files", default="."
    )
    argument_context.add_argument(
        "-d", "--debug", dest="debug", help="Assume running from a debugger", default=False, action="store_true"
    )
    argument_context.add_argument(
        "-m", "--macros", dest="macros", help="Preprocessor macros to expand", default='', action="store"
    )
    argument_context.add_argument(
        "--std",
        dest="std",
        help="Language standard to parse",
        choices=('c++98', 'c++03', 'c++11', 'c++14', 'c++17', 'c++20', 'c++23'),
        default='c++98',
        action="store"
    )
    argument_context.add_argument("input", help="Input source file", metavar="IN")
    cxx_messages.init_arguments(argument_context)
    return argument_context


def run(argument_context):
    # type: (ArgumentParser) -> None
    import os
    import sys
    import logging

    arguments = argument_context.parse_args()
    logger = cxx_messages.load_arguments(arguments)

    ExceptionType = Exception
    if arguments.debug:
        ExceptionType = SyntaxError
    try:
        parser = cxx_parser.parser(arguments.std)(arguments.tmp_dir)
        result = parser.parse(arguments.input)
        if not result:
            sys.exit(0)
        elif logger._error_count > 0:
            sys.exit(logger._error_count)
        else:
            return
    except (SyntaxError, ExceptionType) as exception:
        logging.exception(exception)
        sys.exit(255)


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from optparse import OptionParser
    from types import ModuleType