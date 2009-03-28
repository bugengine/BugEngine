/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#include    <system/stdafx.h>
#include    <actions/sleep.hh>

namespace BugEngine { namespace Actions
{

ABSTRACTMETACLASS_IMPL("Actions",Sleep);

Sleep::Sleep()
{
}

Sleep::~Sleep()
{
}

void Sleep::initialize(ActionContext* context) const
{
    ActionContext::StackFrame frame(context, 1, 0);
    frame[0] = Value(0.0);
}

bool Sleep::oneturn(ActionContext* context) const
{
    ActionContext::StackFrame frame(context, 1, 0);
    double currenttime(frame[0].as<double>());
    currenttime += context->deltaTime();
    return currenttime >= frame[-1].as<double>();
}

}}
