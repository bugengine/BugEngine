from . import messages, parser
from argparse import ArgumentParser
import glrp


def init_arguments():
    # type: () -> ArgumentParser
    argument_context = ArgumentParser()
    argument_context.add_argument(
        "-o",
        "--optimized",
        dest="optimized",
        help="Skip table file generation and assume up to date",
        default=False,
        action="store_true",
    )
    argument_context.add_argument(
        "-t",
        "--tmp",
        dest="tmp_dir",
        help="Directory to store temporary/cached files",
        default=".",
    )
    argument_context.add_argument(
        "-d",
        "--debug",
        dest="debug",
        help="Assume running from a debugger",
        default=False,
        action="store_true",
    )
    argument_context.add_argument(
        "-m",
        "--macros",
        dest="macros",
        help="Preprocessor macros to expand",
        default='',
        action="store",
    )
    argument_context.add_argument(
        "-x",
        dest="lang",
        help="Source code language",
        choices=('auto', 'c', 'c++', 'objc', 'objc++'),
        default='auto',
        action="store"
    )
    argument_context.add_argument(
        "--std",
        dest="std",
        help="Language standard to parse",
        choices=(
            'c89',
            'c99',
            'c11',
            'c17',
            'c++98',
            'c++03',
            'c++11',
            'c++14',
            'c++17',
            'c++20',
            'c++23',
        ),
        default='c++98',
        action="store"
    )
    argument_context.add_argument(
        "input",
        help="Input source file",
        metavar="IN",
    )
    messages.init_arguments(argument_context)
    return argument_context


def run(argument_context):
    # type: (ArgumentParser) -> None
    import os
    import sys
    import logging

    arguments = argument_context.parse_args()
    logger = messages.load_arguments(arguments)

    ExceptionType = Exception
    if arguments.debug:
        ExceptionType = SyntaxError
    try:
        mode = glrp.LOAD_OPTIMIZED if arguments.optimized else glrp.LOAD_CACHE
        p = parser.parser(arguments.lang, os.path.splitext(arguments.input)[1], arguments.std)(arguments.tmp_dir, mode)
        result = p.parse(arguments.input)
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