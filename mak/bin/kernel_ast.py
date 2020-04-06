import os
import sys
sys.path.append(os.path.join(os.path.dirname(os.path.dirname(sys.argv[0])), 'libs'))
from cpp import parser, tree
from optparse import OptionParser
import traceback
try:
    import cPickle as pickle
except ImportError:
    import pickle

option_decl = OptionParser()
option_decl.set_usage('kernel_ast.py kernel_name kernel_input task_output.ast')
option_decl.add_option(
    "-d", dest="macro_file", action="append", help="Add the content of <macrofile> to the macros, one macro per line"
)
option_decl.add_option("-m", "--module", dest="module", help="Module root")
option_decl.add_option("-t", "--tmp", dest="tmp_dir", help="Directory to store temporary/cached files", default=".")

global_macro_map = {
    "__declspec": True,
    "__attribute__": True,
    "CALLBACK": False,
    "WINAPI": False,
    "__cdecl": False,
    "__fastcall": False,
    "__stdcall": False,
    "PASCAL": False,
}

if __name__ == '__main__':
    (options, arguments) = option_decl.parse_args()
    if not arguments:
        option_decl.print_help()
        sys.exit(1)
    elif len(arguments) != 3:
        option_decl.print_help()
        sys.exit(1)
    else:
        try:
            result = parser.parse(
                arguments[1], os.path.join(options.tmp_dir, 'cpp_grammar.pickle'), options.macro_file, options.module
            )
            if not result:
                sys.exit(1)
            for m in result.objects:
                if isinstance(m, tree.OverloadedMethod) and m.name_cpp == 'kmain':
                    break
            else:
                raise Exception("could not locate method kmain in kernel")
            if len(m.overloads) > 1:
                raise Exception("cannot overload metod kmain")
            m = m.overloads[0]
            args = []
            arg0 = m.parameters[0]
            if arg0.type.strip() != 'u32' and arg0.type.strip() != 'const u32':
                raise Exception("invalid signature for method kmain")
            arg1 = m.parameters[1]
            if arg1.type.strip() != 'u32' and arg1.type.strip() != 'const u32':
                raise Exception("invalid signature for method kmain")
            for arg in m.parameters[2:]:
                args.append((arg.name, arg.type))

            with open(arguments[2], 'wb') as out_file:
                pickle.dump((arguments[0], m, args, result.includes, arguments[1]), out_file)

            sys.exit(result.error_count)
        except Exception as e:
            print(e)
            traceback.print_exc()
            sys.exit(1)
