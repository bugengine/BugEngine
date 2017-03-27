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
option_decl.set_usage('kernel.py input output')
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


template = """
/* BugEngine <bugengine.devel@gmail.com> / 2008-2015
   see LICENSE for detail */
#ifndef BE_%(PLUGIN)s_%(NAME)s_SCRIPT_HH_
#define BE_%(PLUGIN)s_%(NAME)s_SCRIPT_HH_
/**************************************************************************************************/
%(pch)s
#include    <scheduler/kernel/kernel.script.hh>
#include    <scheduler/task/itask.hh>
#include    <scheduler/kernel/product.factory.hh>
#include    <scheduler/task/kerneltask.hh>
#include    <kernel/colors.hh>
%(includes)s

namespace Kernels
{

class %(Name)s : public BugEngine::Kernel::KernelDescription
{
private:
    %(argument_field)s
    scoped< BugEngine::Task::ITask > const m_kernelTask;
    %(callbacks)s
private:
    minitl::array< weak<const BugEngine::Kernel::IStream> > makeParameters() const
    {
        minitl::array< weak<const BugEngine::Kernel::IStream> > result(BugEngine::Arena::task(), %(argument_count)d);
        %(argument_result_assign)s
        return result;
    }
published:
    %(argument_outs)s
    %(Name)s(%(argument_params)s)
        :   KernelDescription("%(plugin)s.%(name)s")
        ,   %(argument_assign)s
        ,   m_kernelTask(scoped<BugEngine::Task::KernelTask>::create(BugEngine::Arena::general(),
                                                                     "%(plugin)s.%(name)s",
                                                                     BugEngine::Colors::Red::Red,
                                                                     BugEngine::Scheduler::High,
                                                                     this,
                                                                     makeParameters()))
        ,   %(callback_assign)s
        ,   %(argument_out_assign)s
    {
    }
};

}

/**************************************************************************************************/
#endif
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
                'name':     kernel_name,
                'Name':     kernel_name.capitalize(),
                'NAME':     kernel_name.upper(),
                'pch':      '#include <%s>\n'%options.pch if options.pch else '',
                'PLUGIN':   options.module.upper(),
                'plugin':   options.module.replace('_', '.'),
                'includes': '\n'.join(result.includes),
                'argument_count': len(args),
                'argument_field':
                    '\n    '.join(('BugEngine::Kernel::Product< minitl::remove_const< %s >::type > const m_%s;' % (arg_type, arg_name)
                                   for arg_name, arg_type, out_type in args)),
                'callbacks':
                    '\n    '.join(('BugEngine::Task::ITask::CallbackConnection const m_%sChain;' % (arg_name)
                                   for arg_name, arg_type, out_type in args)),
                'argument_result_assign':
                    '\n        '.join(('result[%d] = m_%s.stream;' % (i, arg[0])
                                   for i, arg in enumerate(args) if arg[2] not in ('product'))),
                'argument_outs':
                    '\n    '.join(('BugEngine::Kernel::Product< minitl::remove_const< %s >::type > const %s;' % (arg_type, arg_name)
                                   for arg_name, arg_type, out_type in args)),
                'argument_params':
                    ', '.join(('const BugEngine::Kernel::Product< minitl::remove_const< %s >::type >& %s' % (arg_type, arg_name)
                                   for arg_name, arg_type, out_type in args)),
                'argument_assign':
                    '\n        ,   '.join(('m_%s(%s)' % (arg_name, arg_name)
                                           for arg_name, arg_type, out_type in args)),
                'callback_assign':
                    '\n        ,   '.join(('m_%sChain(%s.producer, m_kernelTask->startCallback())' % (arg_name, arg_name)
                                           for arg_name, arg_type, out_type in args)),
                'argument_out_assign':
                    '\n        ,   '.join(('%s(%s.stream, m_kernelTask)' % (arg_name, arg_name)
                                           for arg_name, arg_type, out_type in args if out_type not in ('product'))),
            }
            with open(arguments[1], 'w') as out:
                out.write(template % params)
            sys.exit(result.error_count)
        except Exception as e:
            print(e)
            traceback.print_exc()
            sys.exit(1)
