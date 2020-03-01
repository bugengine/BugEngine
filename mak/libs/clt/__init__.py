from . import cl_lexer, cl_parser, cl_ast, cl_messages


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
        cl_messages.init_arguments(argument_context)

    import os
    import sys
    from argparse import ArgumentParser
    import logging

    argument_context = ArgumentParser()
    init_arguments(argument_context)

    sys.setrecursionlimit(10000)
    arguments = argument_context.parse_args()
    logger = cl_messages.load_arguments(arguments)

    ExceptionType = Exception
    if arguments.debug:
        ExceptionType = SyntaxError
    try:
        parser = cl_parser.ClParser(arguments.tmp_dir)
        result = parser.parse(logger, arguments.input)
        if not result:
            sys.exit(254)
        elif logger._error_count > 0:
            sys.exit(logger._error_count)
        else:
            path, module = os.path.split(arguments.processor)
            sys.path.append(path)
            s = __import__(module)
            with open(arguments.output, 'wb') as out_file:
                result.write_document(s.writer(out_file).create_document())
    except (SyntaxError, ExceptionType) as exception:
        logging.exception(exception)
        sys.exit(255)


from be_typing import TYPE_CHECKING
if TYPE_CHECKING:
    from optparse import OptionParser