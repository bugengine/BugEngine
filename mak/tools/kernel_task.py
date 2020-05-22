#!/usr/bin/env python
# encoding: utf-8

from waflib import Task
from waflib.TaskGen import extension
try:
    import cPickle as pickle
except ImportError:
    import pickle

TEMPLATE_CLASS_H = """
class %(KernelName)sTask : public minitl::refcountable
{
private:
    %(argument_field)s
    ref<BugEngine::Task::KernelTask> m_task;
    %(callbacks)s
private:
    minitl::array< weak<BugEngine::KernelScheduler::IParameter> > makeParameters() const;
published:
    %(argument_outs)s
    %(KernelName)sTask(%(argument_params)s);
    ~%(KernelName)sTask();
};
"""

TEMPLATE_H = """
/* BugEngine <bugengine.devel@gmail.com> / 2008-2015
   see LICENSE for detail */
#ifndef BE_%(PLUGIN)s_%(NAME)s_SCRIPT_HH_
#define BE_%(PLUGIN)s_%(NAME)s_SCRIPT_HH_
/**************************************************************************************************/
%(pch)s
#include    <bugengine/scheduler/kernel/kernel.script.hh>
#include    <bugengine/scheduler/task/itask.hh>
#include    <bugengine/scheduler/kernel/product.hh>
#include    <bugengine/scheduler/kernel/parameters/parameters.hh>
#include    <bugengine/scheduler/task/kerneltask.hh>
#include    <bugengine/kernel/colors.hh>
%(includes)s

%(Namespace)s

class %(Name)sKernel : public BugEngine::KernelScheduler::Kernel
{
public:
    %(Name)sKernel();
    ~%(Name)sKernel();

    static ref<%(Name)sKernel> s_kernel;
};

%(Tasks_H)s

%(end_Namespace)s

/**************************************************************************************************/
#endif
"""

TEMPLATE_CLASS_CC = """
%(KernelName)sTask::%(KernelName)sTask(%(argument_params)s)
    :   %(argument_assign)sm_task(ref<BugEngine::Task::KernelTask>::create(BugEngine::Arena::task(),
                                                        "%(kernel_full_name)s.%(KernelName)s",
                                                        BugEngine::KernelScheduler::GPUType,
                                                        BugEngine::Colors::Red::Red,
                                                        BugEngine::Scheduler::High,
                                                        %(Name)sKernel::s_kernel,
                                                        makeParameters()))%(callback_assign)s%(argument_out_assign)s
{
}

%(KernelName)sTask::~%(KernelName)sTask()
{
}

minitl::array< weak<BugEngine::KernelScheduler::IParameter> > %(KernelName)sTask::makeParameters() const
{
    minitl::array< weak<BugEngine::KernelScheduler::IParameter> > result(BugEngine::Arena::task(), %(argument_count)d);
    %(argument_result_assign)s
    return result;
}

"""

TEMPLATE_CC = """
%(pch)s
#include "%(header)s"

BugEngine::KernelScheduler::Kernel::KernelList&  getKernelList_%(module)s();

%(Namespace)s

%(Name)sKernel::%(Name)sKernel()
    :   BugEngine::KernelScheduler::Kernel("%(plugin)s.%(kernel_full_name)s")
{
    getKernelList_%(module)s().push_back(*this);
}

%(Name)sKernel::~%(Name)sKernel()
{
    KernelList::item::unhook();
}

ref< %(Name)sKernel > %(Name)sKernel::s_kernel = ref< %(Name)sKernel >::create(BugEngine::Arena::task());

%(Tasks_CC)s

%(end_Namespace)s
"""


class kernel_task(Task.Task):
    color = 'PINK'

    def sig_vars(self):
        self.m.update(TEMPLATE_CLASS_CC.encode('utf-8'))
        self.m.update(TEMPLATE_CC.encode('utf-8'))

    def scan(self):
        return ([], [])

    def run(self):
        with open(self.inputs[0].abspath(), 'rb') as input_file:
            kernel_name, includes, source, kernel_methods = pickle.load(input_file)

        kernel_namespace = ['Kernels'] + kernel_name.split('.')
        kernel_name = kernel_namespace[-1]
        kernel_full_name = kernel_namespace[1:]

        params = {
            'header': self.outputs[1].abspath(),
            'Namespace': ' { '.join('namespace %s' % n.capitalize() for n in kernel_namespace[:-1]) + '\n{\n',
            'end_Namespace': '}' * (len(kernel_namespace) - 1),
            'name': kernel_name,
            'Name': kernel_name.capitalize(),
            'NAME': kernel_name.upper(),
            'kernel_full_name': '.'.join(kernel_full_name),
            'pch': '#include <%s>\n' % self.generator.pchstop if self.generator.pchstop else '',
            'PLUGIN': self.generator.env.PLUGIN.upper(),
            'module': self.generator.env.PLUGIN,
            'plugin': self.generator.env.PLUGIN.replace('_', '.'),
            'includes': '\n'.join(includes),
        }
        tasks_cc = []
        tasks_h = []
        for method, args in kernel_methods:
            argument_assign = '\n    ,   '.join(('m_%s(%s)' % (arg[0], arg[0]) for arg in args))
            callback_assign = '\n    ,   '.join(
                ('m_%sChain(%s->producer(), m_task->startCallback())' % (arg[0], arg[0]) for arg in args)
            )

            argument_out_assign = '\n    ,   '.join(
                (
                    '%s(ref< const BugEngine::KernelScheduler::Product< BugEngine::KernelScheduler::ParamTypeToKernelType< %s >::Type > >::create(BugEngine::Arena::task(), %s, m_task))'
                    % (arg[0], arg[1], arg[0]) for arg in args
                )
            )
            task_params = {
                'Name':
                    kernel_name.capitalize(),
                'kernel_full_name':
                    '.'.join(kernel_full_name),
                'KernelName':
                    method.name.capitalize(),
                'argument_count':
                    len(args),
                'argument_field':
                    '\n    '.join(
                        (
                            'weak< const BugEngine::KernelScheduler::Product< BugEngine::KernelScheduler::ParamTypeToKernelType< %s >::Type > > const m_%s;'
                            % (arg[1], arg[0]) for arg in args
                        )
                    ),
                'callbacks':
                    '\n    '.join(
                        ('BugEngine::Task::ITask::CallbackConnection const m_%sChain;' % (arg[0]) for arg in args)
                    ),
                'argument_result_assign':
                    '\n    '.join(('result[%d] = m_%s->parameter();' % (i, arg[0]) for i, arg in enumerate(args))),
                'argument_outs':
                    '\n    '.join(
                        (
                            'ref< const BugEngine::KernelScheduler::Product< BugEngine::KernelScheduler::ParamTypeToKernelType< %s >::Type > > const %s;'
                            % (arg[1], arg[0]) for arg in args
                        )
                    ),
                'argument_params':
                    ', '.join(
                        (
                            'weak< const BugEngine::KernelScheduler::Product< BugEngine::KernelScheduler::ParamTypeToKernelType< %s >::Type > > %s'
                            % (arg[1], arg[0]) for arg in args
                        )
                    ),
                'argument_assign':
                    argument_assign and (argument_assign + '\n    ,   ') or '/* no arguments */\n        ',
                'callback_assign':
                    callback_assign and ('\n    ,   ' + callback_assign) or '\n        /* no callbacks */',
                'argument_out_assign':
                    argument_out_assign and ('\n    ,   ' + argument_out_assign) or '\n        /* no arguments out */',
            }
            tasks_cc.append(TEMPLATE_CLASS_CC % task_params)
            tasks_h.append(TEMPLATE_CLASS_H % task_params)

        params['Tasks_CC'] = '\n\n'.join(tasks_cc)
        params['Tasks_H'] = '\n\n'.join(tasks_h)
        with open(self.outputs[0].abspath(), 'w') as out:
            out.write(TEMPLATE_CC % params)
        with open(self.outputs[1].abspath(), 'w') as out:
            out.write(TEMPLATE_H % params)


@extension('.ast')
def kernel_generate(task_gen, ast_node):
    outs = [ast_node.change_ext('task.cc'), ast_node.change_ext('task.script.hh')]
    task = task_gen.create_task('kernel_task', [ast_node], outs)
    try:
        task_gen.out_sources.append(outs[0])
    except AttributeError:
        task_gen.out_sources = [outs[0]]
    task_gen.source.append(outs[1])
