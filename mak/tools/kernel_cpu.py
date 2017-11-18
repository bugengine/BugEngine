import os
import sys
sys.path.append(os.path.split(os.path.split(os.path.dirname(os.path.realpath(__file__)))[0])[0])
from mak.libs.cpp import parser
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
option_decl.set_usage('kernel_cpu.py [options] input output')
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


template="""
%(pch)s
#include    <kernel/compilers.hh>
#include    <kernel/simd.hh>
#include    <kernel/input/cpp/input.hh>
#include    <plugin/dynobjectlist.hh>
#include    <minitl/array.hh>
#include    <cpu/memorybuffer.hh>

using namespace Kernel;

#include    "%(source)s"

struct Parameter
{
    void* begin;
    void* end;
};

_BE_PLUGIN_EXPORT void _kmain(const u32 index, const u32 total,
                              const minitl::array< minitl::weak<const BugEngine::Kernel::IMemoryBuffer> >& argv)
{
    kmain(index, total,
          %(args)s
    );
}
_BE_REGISTER_PLUGIN(BE_KERNEL_ID, BE_KERNEL_NAME);
_BE_REGISTER_METHOD(BE_KERNEL_ID, _kmain);
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
                name = arg.name
                for t in arg.tags:
                    if t[0] == 'kernel_param':
                        print(t[1])
                        bugengine_names = {
                            'segment':      '::BugEngine::Kernel::Segment',
                            'segments':     '::BugEngine::Kernel::Segments',
                            'stream':       '::BugEngine::Kernel::Stream',
                            'texture1d':    '::BugEngine::Kernel::Texture1D',
                            'texture2d':    '::BugEngine::Kernel::Texture2D',
                            'texture3d':    '::BugEngine::Kernel::Texture3D',
                        }
                        try:
                            bugengine_name = bugengine_names[t[1][1]]
                        except KeyError:
                            raise Exception('invalid kernel input type: %s:\n'
                                            'type %s is not a valid kernel type' % (name, t[1][0]))
                        args.append((arg.name, arg.type, bugengine_name, t[1][1], t[1][0]))
                        break
                else:
                    raise Exception('invalid kernel input type: %s\n'
                                    'mark it with be_in, be_out or be_inout' % name)

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
