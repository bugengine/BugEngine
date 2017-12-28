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
option_decl.set_usage('kernel_task.py kernel_name kernel_input task_output.cc task_output.hh')
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


template_h = """
/* BugEngine <bugengine.devel@gmail.com> / 2008-2015
   see LICENSE for detail */
#ifndef BE_%(PLUGIN)s_%(NAME)s_SCRIPT_HH_
#define BE_%(PLUGIN)s_%(NAME)s_SCRIPT_HH_
/**************************************************************************************************/
%(pch)s
#include    <scheduler/kernel/kernel.script.hh>
#include    <scheduler/task/itask.hh>
#include    <scheduler/kernel/product.hh>
#include    <scheduler/kernel/parameters/parameters.hh>
#include    <scheduler/task/kerneltask.hh>
#include    <kernel/colors.hh>
%(includes)s

%(Namespace)s

class %(Name)sTask : public minitl::refcountable
{
private:
    %(argument_field)s
    ref<BugEngine::Task::KernelTask> m_task;
    %(callbacks)s
private:
    class Kernel : public BugEngine::Kernel::Kernel
    {
    public:
        Kernel();
        ~Kernel();
    };
private:
    static ref<Kernel> s_kernel;
    minitl::array< weak<BugEngine::Kernel::IParameter> > makeParameters() const;
published:
    %(argument_outs)s
    %(Name)sTask(%(argument_params)s);
    ~%(Name)sTask();
};

%(end_Namespace)s

/**************************************************************************************************/
#endif
"""


template_cc = """
%(pch)s
#include "%(header)s"

BugEngine::Kernel::Kernel::KernelList&  getKernelList_%(module)s();

%(Namespace)s

%(Name)sTask::Kernel::Kernel()
    :   BugEngine::Kernel::Kernel("%(plugin)s.%(kernel_full_name)s")
{
    getKernelList_%(module)s().push_back(*this);
}

%(Name)sTask::Kernel::~Kernel()
{
    KernelList::item::unhook();
}

ref<%(Name)sTask::Kernel> %(Name)sTask::s_kernel = ref<%(Name)sTask::Kernel>::create(BugEngine::Arena::game());

%(Name)sTask::%(Name)sTask(%(argument_params)s)
    :   %(argument_assign)s
    ,   m_task(ref<BugEngine::Task::KernelTask>::create(BugEngine::Arena::task(),
                                                        "%(kernel_full_name)s",
                                                        BugEngine::Colors::Red::Red,
                                                        BugEngine::Scheduler::High,
                                                        s_kernel,
                                                        makeParameters()))
    ,   %(callback_assign)s
    ,   %(argument_out_assign)s
{
}

%(Name)sTask::~%(Name)sTask()
{
}

minitl::array< weak<BugEngine::Kernel::IParameter> > %(Name)sTask::makeParameters() const
{
    minitl::array< weak<BugEngine::Kernel::IParameter> > result(BugEngine::Arena::task(), %(argument_count)d);
    %(argument_result_assign)s
    return result;
}

%(end_Namespace)s
"""


if __name__ == '__main__':
    (options, arguments) = option_decl.parse_args()
    if not arguments:
        option_decl.print_help()
        sys.exit(1)
    elif len(arguments) != 4:
        option_decl.print_help()
        sys.exit(1)
    else:
        try:
            result = parser.parse(arguments[1], os.path.join(options.tmp_dir, 'cpp_grammar.pickle'),
                                  options.macro_file, options.module)
            kernel_namespace = ['Kernels'] + arguments[0].split('.')
            kernel_name = kernel_namespace[-1]
            kernel_full_name = kernel_namespace[1:]
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
                        bugengine_names = {
                            'be_segment':      '::BugEngine::Kernel::Segment',
                            'be_segments':     '::BugEngine::Kernel::Segments',
                            'be_stream':       '::BugEngine::Kernel::Stream',
                            'be_texture1d':    '::BugEngine::Kernel::Texture1D',
                            'be_texture2d':    '::BugEngine::Kernel::Texture2D',
                            'be_texture3d':    '::BugEngine::Kernel::Texture3D',
                        }
                        try:
                            bugengine_name = bugengine_names[t[1]]
                        except KeyError:
                            raise Exception('invalid kernel input type: %s:\n'
                                            'type %s is not a valid kernel type' % (name, t[1]))
                        args.append((arg.name, arg.type, bugengine_name, t[1]))
                        break
                else:
                    raise Exception('invalid kernel input type: %s\n'
                                    'mark it with be_in, be_out or be_inout' % name)

            params = {
                'header': arguments[3],
                'Namespace':' { '.join('namespace %s' % n.capitalize() for n in kernel_namespace[:-1]) + '\n{\n',
                'end_Namespace': '}'* (len(kernel_namespace) -1),
                'name':     kernel_name,
                'Name':     kernel_name.capitalize(),
                'NAME':     kernel_name.upper(),
                'kernel_full_name': '.'.join(kernel_full_name),
                'pch':      '#include <%s>\n'%options.pch if options.pch else '',
                'PLUGIN':   options.module.upper(),
                'module':   options.module,
                'plugin':   options.module.replace('_', '.'),
                'includes': '\n'.join(result.includes),
                'argument_count': len(args),
                'argument_field':
                    '\n    '.join(('weak< const BugEngine::Kernel::Product< %s<minitl::remove_const< %s >::type> > > const m_%s;' % (arg[2], arg[1], arg[0])
                                   for arg in args)),
                'callbacks':
                    '\n    '.join(('BugEngine::Task::ITask::CallbackConnection const m_%sChain;' % (arg[0])
                                   for arg in args)),
                'argument_result_assign':
                    '\n    '.join(('result[%d] = m_%s->parameter();' % (i, arg[0])
                                   for i, arg in enumerate(args) if arg[3] not in ('product'))),
                'argument_outs':
                    '\n    '.join(('ref< const BugEngine::Kernel::Product< %s<minitl::remove_const< %s >::type> > > const %s;' % (arg[2], arg[1], arg[0])
                                   for arg in args)),
                'argument_params':
                    ', '.join(('weak< const BugEngine::Kernel::Product< %s<minitl::remove_const< %s >::type> > > %s' % (arg[2], arg[1], arg[0])
                                   for arg in args)),
                'argument_assign':
                    '\n    ,   '.join(('m_%s(%s)' % (arg[0], arg[0])
                                           for arg in args)),
                'callback_assign':
                    '\n    ,   '.join(('m_%sChain(%s->producer(), m_task->startCallback())' % (arg[0], arg[0])
                                           for arg in args)),
                'argument_out_assign':
                    '\n    ,   '.join(('%s(ref< const BugEngine::Kernel::Product< %s<minitl::remove_const< %s >::type> > >::create(BugEngine::Arena::task(), %s, m_task))' % (arg[0], arg[2], arg[1], arg[0])
                                           for arg in args)),
            }
            with open(arguments[2], 'w') as out:
                out.write(template_cc % params)
            with open(arguments[3], 'w') as out:
                out.write(template_h % params)
            sys.exit(result.error_count)
        except Exception as e:
            print(e)
            traceback.print_exc()
            sys.exit(1)
