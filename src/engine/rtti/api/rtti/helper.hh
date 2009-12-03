/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_HELPER_HH_
#define BE_RTTI_HELPER_HH_
/*****************************************************************************/
#include    <rtti/object.hh>
#include    <rtti/metaclass.hh>
#include    <rtti/method.hh>
#include    <rtti/autoregistration.hh>
#include    <rtti/properties/propertybuilder.hh>
#include    <rtti/methods/methodbuilder.hh>

#define be_metaclass(exportrule,_name,_parent)                                                             \
    public:                                                                                                 \
    typedef _name   self_t;                                                                                 \
    typedef _parent super_t;                                                                                \
    class be_api(exportrule) MetaClass : public _parent::MetaClass                                          \
    {                                                                                                       \
        friend class _name;                                                                                 \
        friend class minitl::ref<MetaClass>;                                                                \
    private:                                                                                                \
        typedef _parent::MetaClass  super_t;                                                                \
        MetaClass();                                                                                        \
    protected:                                                                                              \
        class be_api(exportrule) MetaMetaClass : public _parent::MetaClass::MetaMetaClass                   \
        {                                                                                                   \
            friend class _name;                                                                             \
            friend class _name::MetaClass;                                                                  \
        public:                                                                                             \
            MetaMetaClass(const BugEngine::inamespace& name,                                                \
                          ref<const BugEngine::Object::MetaClass> parent);                                  \
            ~MetaMetaClass();                                                                               \
        };                                                                                                  \
    public:                                                                                                 \
        MetaClass(const BugEngine::inamespace& name, ref<const MetaClass> parent, ref<MetaMetaClass> meta); \
        ~MetaClass();                                                                                       \
        ref<BugEngine::Object> create() const override;

#define be_properties                                                                                      \
    };                                                                                                      \
    static ref<MetaClass> s_metaclass;                                                                      \
    static BugEngine::RTTI::_::AutoRegister<self_t> s_autoRegistration;                                     \
    static const BugEngine::inamespace& getClassName();                                                     \
    static ref<const self_t::MetaClass> static_metaclass();                                                 \
    ref<const BugEngine::RTTI::MetaClass> metaclass() const override;                                       \
private:                                                                                                    \
    static void registerMetaClass(weak<self_t::MetaClass> mc)                                               \
    {                                                                                                       \
        self_t::MetaClass::init(mc);

#define be_read(_field)                                                                                     \
    BugEngine::RTTI::_::createReadFieldFromOffset<self_t, static_cast<size_t>(offsetof(self_t,_field))>(&(reinterpret_cast<self_t*>(0)->_field))
#define be_get(_method)                                                                                     \
    BugEngine::RTTI::_::createHelperFromGetter<>(&self_t::_method).operator()<&self_t::_method>()
#define be_write(_field)                                                                                    \
    BugEngine::RTTI::_::createWriteFieldFromOffset<self_t, static_cast<size_t>(offsetof(self_t,_field))>(&(reinterpret_cast<self_t*>(0)->_field))
#define be_set(_method)                                                                                     \
    BugEngine::RTTI::_::createHelperFromSetter<>(&self_t::_method).operator()<&self_t::_method>()
#define be_property(_name)                                                                                  \
    BugEngine::RTTI::_::PropertyBuilder<void, void>(mc, #_name)

#define be_method(name)                                                                 \
    mc->addMethod(#name, BugEngine::RTTI::_::createMethodBuilder(&self_t::name).buildMethod<&self_t::name>())
#define be_classmethod(name)                                                            \
    mc->m_metaclass->addMethod(#name, BugEngine::RTTI::_::createMethodBuilder(&self_t::MetaClass::name).buildMethod<&self_t::MetaClass::name>())

#define be_end                                                                          \
    }

#define be_metaclass_impl_(_namespace,_cls,_code)                                                          \
    ref<_cls::MetaClass> _cls::s_metaclass;                                                                 \
    BugEngine::RTTI::_::AutoRegister<_cls> _cls::s_autoRegistration;                                        \
    _cls::MetaClass::MetaMetaClass::MetaMetaClass( const BugEngine::inamespace& name,                       \
                                                   ref<const BugEngine::Object::MetaClass> parent) :        \
        _cls::super_t::MetaClass::MetaMetaClass( name, parent )                                             \
    {                                                                                                       \
    }                                                                                                       \
    _cls::MetaClass::MetaMetaClass::~MetaMetaClass()                                                        \
    {                                                                                                       \
    }                                                                                                       \
    _cls::MetaClass::MetaClass()                                                                            \
        : _cls::super_t::MetaClass( _cls::self_t::getClassName(),                                           \
                                    _cls::super_t::static_metaclass(),                                      \
                                    ref<MetaMetaClass>::create( #_cls "Class",                              \
                                                       _cls::super_t::static_metaclass()->metaclass()))     \
    {                                                                                                       \
    }                                                                                                       \
    _cls::MetaClass::MetaClass( const BugEngine::inamespace& name,                                          \
                                ref<const MetaClass> parent,                                                \
                                ref<MetaMetaClass> meta) :                                                  \
        _cls::super_t::MetaClass( name, parent, meta )                                                      \
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
    ref<const _cls::MetaClass> _cls::static_metaclass()                                                     \
    {                                                                                                       \
        if(!s_metaclass)                                                                                    \
        {                                                                                                   \
            super_t::static_metaclass();                                                                    \
            if(!s_metaclass)                                                                                \
            {                                                                                               \
                s_metaclass = ref<self_t::MetaClass>::create();                                             \
                self_t::registerMetaClass(s_metaclass);                                                     \
            }                                                                                               \
        }                                                                                                   \
        return s_metaclass;                                                                                 \
    }                                                                                                       \
    ref<const BugEngine::RTTI::MetaClass> _cls::metaclass() const                                           \
    {                                                                                                       \
        return static_metaclass();                                                                          \
    }                                                                                                       \
    ref<BugEngine::Object> _cls::MetaClass::create() const                                                  \
    {                                                                                                       \
        _code;                                                                                              \
    }                                                                                                       \
    class _cls


#define be_metaclass_impl(_namespace,_cls)          be_metaclass_impl_(_namespace,_cls,return ref<_cls>::create())
#define be_abstractmetaclass_impl(_namespace,_cls)  be_metaclass_impl_(_namespace,_cls, throw 0)

/*****************************************************************************/
#endif
