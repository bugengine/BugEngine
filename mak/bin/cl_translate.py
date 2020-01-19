import os
import sys
from clt import cl_parser
from optparse import OptionParser
import logging

option_decl = OptionParser()
option_decl.set_usage('clt.py [options] in_file out_file translate_script')
option_decl.add_option("-t", "--tmp", dest="tmp_dir", help="Directory to store temporary/cached files", default=".")
option_decl.add_option("-e", "--error-format", dest="error_format", help="Error message format type", default="unix")
option_decl.add_option(
    "-d", "--debug", dest="debug", help="Assume running from a debugger", default=False, action="store_true"
)
option_decl.add_option(
    "--Werror", dest="warn_error", help="Treat warning as errors", default=False, action="store_true"
)


def run():
    sys.setrecursionlimit(10000)
    (options, args) = option_decl.parse_args()
    if not args:
        option_decl.print_help()
        sys.exit(1)
    elif len(args) != 3:
        option_decl.print_help()
        sys.exit(1)
    else:
        if options.debug:
            ExceptionType = SyntaxError
        else:
            ExceptionType = Exception
        try:
            parser = cl_parser.ClParser(options.tmp_dir)
            result = parser.parse(options.error_format, args[0])
            if not result:
                sys.exit(254)
            elif result.error_count > 0:
                sys.exit(result.error_count)
            else:
                path, module = os.path.split(args[2])
                sys.path.append(path)
                s = __import__(module)
                with open(args[1], 'wb') as out_file:
                    result.write_to(s.writer(out_file))
        except (SyntaxError, ExceptionType) as exception:
            logging.exception(exception)
            sys.exit(255)


if __name__ == '__main__':
    run()
