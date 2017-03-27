import os
import sys
sys.path.append(os.path.dirname(os.path.realpath(__file__)))
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
option_decl.set_usage('kernel_preprocess.py [options] input output')
option_decl.add_option("-d", dest="macro_file", action="append", help="Add the content of <macrofile> to the macros, one macro per line")
option_decl.add_option("-p", "--pch", dest="pch", help="Insert an include for precompiled header at the start of the file")
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

def split_type(name):
    name = name.strip()
    template_begin = name.find('<')
    if template_begin == -1:
        raise Exception('invalid kernel input type: %s' % name)
    template_name = name[0:template_begin].strip()
    type_name = name[template_begin+1:-1].strip()
    if template_name not in ['segment']:
        raise Exception('invalid kernel input type: %s' % name)
    return (template_name, type_name)



template="""
%(pch)s
#include    <kernel/compilers.hh>
#include    <kernel/simd.hh>
#include    <kernel/input.hh>
#include    <plugin/dynobjectlist.hh>

using namespace Kernel;

#include    "%(source)s"

struct Parameter
{
    void* begin;
    void* end;
};

_BE_PLUGIN_EXPORT void _kmain(const u32 index, const u32 total, Parameter argv[])
{
    kmain(index, total,
          %(args)s
    );
}
_BE_REGISTER_PLUGIN(BE_KERNELID, BE_KERNELNAME);
_BE_REGISTER_METHOD(BE_KERNELID, _kmain);
"""

if __name__ == '__main__':
    (options, arguments) = option_decl.parse_args()
    if not arguments:
        option_decl.print_help()
        sys.exit(1)
    elif len(arguments) != 2:
        option_decl.print_help()
        sys.exit(1)
    else:
        try:
            result = parser.parse(arguments[0], os.path.join(options.tmp_dir, 'cpp_grammar.pickle'),
                                  options.macro_file, options.module)
            kernel_name = os.path.splitext(os.path.basename(arguments[0]))[0]
            for m in result.objects:
                if m.name_cpp == 'kmain':
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
                template_name, type_name = split_type(arg.type)
                args.append((arg.name, type_name, template_name))

            params = {
                'pch':      '#include <%s>\n'%options.pch if options.pch else '',
                'source':   arguments[0],
                'args':     ',\n          '.join('%s< %s >((%s*)argv[%d].begin, (%s*)argv[%d].end)'
                                                 % (arg[2], arg[1], arg[1], i, arg[1], i)
                                                 for i, arg in enumerate(args))
            }
            with open(arguments[1], 'w') as out:
                out.write(template % params)
            sys.exit(result.error_count)
        except Exception as e:
            print(e)
            traceback.print_exc()
            sys.exit(1)
