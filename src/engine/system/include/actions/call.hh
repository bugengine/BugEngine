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

#ifndef BE_SYSTEM_CALL_HH_
#define BE_SYSTEM_CALL_HH_
/*****************************************************************************/
#include    <system/action.hh>

namespace BugEngine { namespace Actions
{


class SYSTEMEXPORT Call : public Action
{
private:
    Object::MetaClass::Method*   m_method;
public:
    Call();
    Call(Object::MetaClass::Method* method);
    ~Call();

    virtual void initialize(ActionContext* context) const override;
    virtual bool oneturn(ActionContext* context) const override;

    be_metaclass(SYSTEMEXPORT,Call,Action)
        refptr<Call> createWithMethod(Object::MetaClass::Method* method);
    be_properties
        be_classmethod(createWithMethod);

        be_property(method)
            [be_read(m_method)]
            [be_write(m_method)];
    be_end
};

}}

/*****************************************************************************/
#endif
