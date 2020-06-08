# LLVM IR/Spir parser
from . import ir_parser, ir_messages
from .ir_codegen import IrccGenerator, IrccType
from . import generators


def run():
    # type: () -> None
    def init_arguments(argument_context):
        # type: (ArgumentParser) -> None
        argument_context.add_argument(
            "-t", "--tmp", dest="tmp_dir", help="Directory to store temporary/cached files", default="."
        )
        argument_context.add_argument(
            "-d", "--debug", dest="debug", help="Assume running from a debugger", default=False, action="store_true"
        )
        argument_context.add_argument("input", help="Input source file", metavar="IN")
        argument_context.add_argument("output", help="Destination filename", metavar="OUT")
        argument_context.add_argument("processor", help="AST processor module", metavar="MODULE")
        ir_messages.init_arguments(argument_context)

    import os
    import sys
    from argparse import ArgumentParser
    import logging

    argument_context = ArgumentParser()
    init_arguments(argument_context)

    arguments = argument_context.parse_args()
    logger = ir_messages.load_arguments(arguments)

    ExceptionType = Exception
    if arguments.debug:
        ExceptionType = SyntaxError
    try:
        parser = ir_parser.IrParser(arguments.tmp_dir)
        result = parser.parse(logger, arguments.input)
        if not result:
            sys.exit(1)
        elif logger._error_count > 0:
            sys.exit(logger._error_count)
        else:
            path, module = os.path.split(arguments.processor)
            sys.path.append(path)
            s = __import__(module)
            with open(arguments.output, 'w') as out_file:
                result.visit(s.generators(out_file))
    except (SyntaxError, ExceptionType) as exception:
        logging.exception(exception)
        sys.exit(255)


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from argparse import ArgumentParser