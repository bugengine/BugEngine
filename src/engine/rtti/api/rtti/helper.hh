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

#ifndef BE_RTTI_HELPER_HH_
#define BE_RTTI_HELPER_HH_
/*****************************************************************************/
#include    <rtti/object.hh>
#include    <rtti/metaclass.hh>
#include    <rtti/method.hh>
#include    <rtti/autoregistration.hh>
#include    <rtti/properties/propertybuilder.hh>
#include    <rtti/methods/methodbuilder.hh>

#define be_metaclass(exportrule,_name,_parent)                                          \
    public:                                                                             \
    typedef _name   self_t;                                                             \
    typedef _parent super_t;                                                            \
    class exportrule MetaClass : public _parent::MetaClass                              \
    {                                                                                   \
        friend class _name;                                                             \
    private:                                                                            \
        typedef _parent::MetaClass  super_t;                                            \
        MetaClass();                                                                    \
    protected:                                                                          \
    public:                                                                             \
        MetaClass(const BugEngine::inamespace& name, const MetaClass* parent);          \
        ~MetaClass();

#define be_properties                                                                   \
    };                                                                                  \
    static BugEngine::RTTI::AutoRegister<self_t> s_autoRegistration;                    \
    static const BugEngine::inamespace& getClassName();                                 \
    static const self_t::MetaClass* static_metaclass();                                 \
    const self_t::MetaClass* metaclass() const override;                                \
private:                                                                                \
    static void registerMetaClass(self_t::MetaClass* mc)                                \
    {                                                                                   \
        self_t::MetaClass::init(mc);

#define be_read(_field)                                                                 \
    BugEngine::RTTI::_::createReadFieldFromOffset<self_t, static_cast<size_t>(offsetof(self_t,_field))>(&(reinterpret_cast<self_t*>(0)->_field))
#define be_get(_method)                                                                 \
    BugEngine::RTTI::_::createHelperFromGetter<>(&self_t::_method).operator()<&self_t::_method>()
#define be_write(_field)                                                                \
    BugEngine::RTTI::_::createWriteFieldFromOffset<self_t, static_cast<size_t>(offsetof(self_t,_field))>(&(reinterpret_cast<self_t*>(0)->_field))
#define be_set(_method)                                                                 \
    BugEngine::RTTI::_::createHelperFromSetter<>(&self_t::_method).operator()<&self_t::_method>()
#define be_property(_name)                                                              \
    BugEngine::RTTI::_::PropertyBuilder<void, void>(mc, #_name)

#define be_method(name)                                                                 \

#define be_classmethod(name)

#define be_end                                                                          \
    }

#define be_metaclass_impl(_namespace,_cls)                                                                  \
    BugEngine::RTTI::AutoRegister<_cls> _cls::s_autoRegistration;                                           \
    _cls::MetaClass::MetaClass() :                                                                          \
        _cls::super_t::MetaClass( _cls::self_t::getClassName(),                                             \
                                  _cls::super_t::static_metaclass())                                        \
    {                                                                                                       \
    }                                                                                                       \
    _cls::MetaClass::MetaClass(const BugEngine::inamespace& name, const MetaClass* parent) :                \
        _cls::super_t::MetaClass( name, parent )                                                            \
    {                                                                                                       \
    }                                                                                                       \
    _cls::MetaClass::~MetaClass()                                                                           \
    {                                                                                                       \
    }                                                                                                       \
    const BugEngine::inamespace& _cls::getClassName()                                                       \
    {                                                                                                       \
        static BugEngine::inamespace s_name = BugEngine::inamespace(_namespace)+BugEngine::istring(#_cls);  \
        return s_name;                                                                                      \
    }                                                                                                       \
    const _cls::MetaClass* _cls::static_metaclass()                                                         \
    {                                                                                                       \
        static self_t::MetaClass* s_metaclass;                                                              \
        if(!s_metaclass)                                                                                    \
        {                                                                                                   \
            super_t::static_metaclass();                                                                    \
            if(!s_metaclass)                                                                                \
            {                                                                                               \
                s_metaclass = new self_t::MetaClass();                                                      \
                self_t::registerMetaClass(s_metaclass);                                                     \
            }                                                                                               \
        }                                                                                                   \
        return s_metaclass;                                                                                 \
    }                                                                                                       \
    const _cls::MetaClass* _cls::metaclass() const                                                          \
    {                                                                                                       \
        return static_metaclass();                                                                          \
    }


#define be_abstractmetaclass_impl(_namespace,_cls) be_metaclass_impl(_namespace,_cls)

/*****************************************************************************/
#endif
