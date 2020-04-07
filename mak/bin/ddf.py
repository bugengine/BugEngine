import os
import sys
sys.path.append(os.path.join(os.path.dirname(os.path.dirname(sys.argv[0])), 'libs'))
from cpp import parser
from optparse import OptionParser
import traceback
try:
    from cStringIO import StringIO
except ImportError:
    try:
        from StringIO import StringIO
    except ImportError:
        from io import StringIO

option_decl = OptionParser()
option_decl.set_usage('ddf.py [options] input out_def out_instances out_doc out_namespaces')
option_decl.add_option(
    "-d", dest="macro_file", action="append", help="Add the content of <macrofile> to the macros, one macro per line"
)
option_decl.add_option(
    "-p", "--pch", dest="pch", help="Insert an include for precompiled header at the start of the file"
)
option_decl.add_option("-m", "--module", dest="module", help="Module root")
option_decl.add_option("-t", "--tmp", dest="tmp_dir", help="Directory to store temporary/cached files", default=".")

if __name__ == '__main__':
    (options, args) = option_decl.parse_args()
    if not args:
        option_decl.print_help()
        sys.exit(1)
    elif len(args) != 5:
        option_decl.print_help()
        sys.exit(1)
    else:
        try:
            result = parser.parse(
                args[0], os.path.join(options.tmp_dir, 'cpp_grammar.pickle'), options.macro_file, options.module
            )
            classes = StringIO()
            instances = StringIO()
            docs = StringIO()
            if options.pch:
                classes.write('#include <%s>\n' % options.pch)
                instances.write('#include <%s>\n' % options.pch)
            result.dump(classes, instances, args[0])
            with open(args[1], 'w') as c:
                c.write(classes.getvalue())
            with open(args[2], 'w') as i:
                i.write(instances.getvalue())
            with open(args[3], 'w') as d:
                d.write(docs.getvalue())
            with open(args[4], 'wb') as namespace_file:
                result.write_namespaces(namespace_file)
            sys.exit(result.error_count)
        except Exception as e:
            print(e)
            traceback.print_exc()
            sys.exit(1)
