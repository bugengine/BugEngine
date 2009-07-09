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

#ifndef BE_RTTI_METHOD_HH_
#define BE_RTTI_METHOD_HH_
/*****************************************************************************/
#include    <rtti/object.hh>
#include    <rtti/metaclass.hh>

#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

class RTTIEXPORT Method : public Object
{
protected:
    class RTTIEXPORT MetaClass : public RTTI::MetaClass
    {
    public:
        MetaClass();
        MetaClass(const inamespace& name, const MetaClass* parent, bool registerClass);
        ~MetaClass();
    };
public:
    Method();
    virtual ~Method();

    virtual const MetaClass* metaclass() const override;
    static  const MetaClass* static_metaclass();
private:
    Method(const Method& other);
    Method& operator=(const Method& other);
};

}}

/*****************************************************************************/
#endif
