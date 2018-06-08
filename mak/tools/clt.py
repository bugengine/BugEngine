import os
import sys
sys.path.append(os.path.split(os.path.split(os.path.dirname(os.path.realpath(__file__)))[0])[0])
from mak.libs.clt import cl_parser
from optparse import OptionParser
import traceback
try:
    import cPickle as pickle
except ImportError:
    import pickle


option_decl = OptionParser()
option_decl.set_usage('clt.py [options] in_file out_file translate_script')
option_decl.add_option("-t", "--tmp", dest="tmp_dir", help="Directory to store temporary/cached files", default=".")
option_decl.add_option("-e", "--error-format", dest="error_format", help="Error message format type", default="unix")


if __name__ == '__main__':
    (options, args) = option_decl.parse_args()
    if not args:
        option_decl.print_help()
        sys.exit(1)
    elif len(args) != 3:
        option_decl.print_help()
        sys.exit(1)
    else:
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
        except Exception as e:
            print(e)
            traceback.print_exc()
            sys.exit(255)

