from cpp import parser
from optparse import OptionParser
import os
import traceback

options = OptionParser()
options.set_usage('ddf.py [options] input out_def out_instances out_doc')
options.add_option("-d", dest="macro_file", action="append", help="Add the content of <macrofile> to the macros, one macro per line")
options.add_option("-p", "--pch", dest="pch", help="Insert an include for precompiled header at the start of the file")
options.add_option("-m", "--module", dest="module", help="Module root")
options.add_option("-t", "--tmp", dest="tmp_dir", help="Directory to store temporary/cached files", default=".")

if __name__ == '__main__':
    (options, args) = options.parse_args()
    if not args:
        options.print_help()
        exit(1)
    elif len(args) != 4:
        options.print_help()
        exit(1)
    else:
        try:
            result = parser.parse(args[0], args[1], args[2], args[3],
                                  os.path.join(options.tmp_dir, 'cpp_grammar.pickle'),
                                  options.macro_file, options.pch, options.module)
            exit(result)
        except Exception as e:
            print(e)
            traceback.print_exc()
            exit(1)

