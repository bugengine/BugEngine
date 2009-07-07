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

#ifndef BE_RTTI_METACLASS_HH_
#define BE_RTTI_METACLASS_HH_
/*****************************************************************************/
#include    <rtti/object.hh>

namespace BugEngine
{

class Property;

class RTTIEXPORT Object::MetaClass : public Object
{
private:
    class MetaMetaClass;
public:
    typedef minitl::map< istring, refptr<const Property> >      PropertyMap;
    typedef PropertyMap::iterator                               PropertyIterator;
    typedef PropertyMap::const_iterator                         PropertyConstIterator;
private:
    static MetaMetaClass*   s_metaMetaClass;
    istring                 m_name;
    refptr<const MetaClass> m_parent;
protected:
    PropertyMap             m_properties;
public:
    MetaClass(const inamespace& name, const MetaClass* parent, bool registerClass = true);
    virtual ~MetaClass();

    const istring& name() const;
    virtual const MetaClass* metaclass() const override;
    virtual const MetaClass* parent() const;

    void addProperty(const istring& name, refptr<const Property> property);
    const Property* getProperty(const istring& name) const;
protected:
    static void init(MetaClass* mc);
private:
    MetaClass(const MetaClass& other);
    MetaClass& operator=(const MetaClass& other);
};

}

/*****************************************************************************/
#endif
