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

#ifndef BE_RTTI_PACKAGE_HH_
#define BE_RTTI_PACKAGE_HH_
/*****************************************************************************/
#include    <rtti/helper.hh>
#include    <core/utils/singleton.hh>

namespace BugEngine { namespace RTTI
{

class Namespace;

class RTTIEXPORT Package : public Object
{
protected:
    size_t                                  m_refcount;
    istring                                 m_name;
    refptr<RTTI::Namespace>                 m_namespace;
    Package*                                m_parent;
    minitl::map< istring, refptr<Package> > m_children;
private:
    virtual void doload() = 0;
    virtual void dounload() = 0;
private:
    Package();
public:
    Package(const istring& name, Package* parent);
    ~Package();

    void load();
    void unload();

    virtual const Namespace*    getNamespace() const = 0;

    const istring& name() const { return m_name; }

    static Package* get(const inamespace& name);

    be_metaclass(RTTIEXPORT,Package,Object)
        virtual refptr<Package> create(const ipath& name) const;
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
