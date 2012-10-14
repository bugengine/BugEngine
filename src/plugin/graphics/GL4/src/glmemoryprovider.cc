/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <GL4/stdafx.h>
#include    <GL4/glmemoryprovider.hh>

namespace BugEngine { namespace OpenGL
{

GLMemoryProvider::GLMemoryProvider()
    :   IMemoryProvider("OpenGL")
{
}

GLMemoryProvider::~GLMemoryProvider()
{
}

Kernel::KernelParameter GLMemoryProvider::getKernelParameterFromBank(weak<const Kernel::IMemoryBank> bank) const
{
    be_forceuse(bank);
    Kernel::KernelParameter parameter = { 0, 0 };
    return parameter;
}

}}
