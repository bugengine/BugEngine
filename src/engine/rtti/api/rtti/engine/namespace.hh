/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_NAMESPACE_HH_
#define BE_RTTI_ENGINE_NAMESPACE_HH_
/*****************************************************************************/
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/objectinfo.script.hh>


#define BE_REGISTER_NAMESPACE_1_NAMED(plugin, n)                                                                                                \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n()                                                                                   \
    {                                                                                                                                           \
        static RTTI::Class ci = { #n, {0}, {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0 };                                                     \
        raw<BugEngine::RTTI::Class> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                             \
    }                                                                                                                                           \
    static RTTI::ObjectInfo s_##plugin##_Namespace_##n_ob =                                                                                     \
        { be_##plugin##_Namespace()->objects, {0}, #n, RTTI::Value(be_##plugin##_Namespace_##n) };                                              \
    BE_EXPORT const RTTI::ObjectInfo* s_##plugin##_Namespace_##n##_r = be_##plugin##_Namespace()->objects.set(&s_##plugin##_Namespace_##n_ob);  \
}

#define BE_REGISTER_NAMESPACE_2_NAMED(plugin, n1, n2)                                                                                           \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1();                                                                                 \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1##_##n2()                                                                           \
    {                                                                                                                                           \
        static RTTI::Class ci = { #n2, {be_##plugin##_Namespace_##n1().m_ptr}, {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0 };                 \
        raw<BugEngine::RTTI::Class> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                             \
    }                                                                                                                                           \
    static RTTI::ObjectInfo s_##plugin##_Namespace_##n1##_##n2##_ob =                                                                           \
        { be_##plugin##_Namespace_##n1()->objects, {0}, #n2, RTTI::Value(be_##plugin##_Namespace_##n1##_##n2()) };                              \
    BE_EXPORT const RTTI::ObjectInfo* s_##plugin##_Namespace_##n1##_##n2##_r =                                                                  \
        be_##plugin##_Namespace_##n1()->objects.set(&s_##plugin##_Namespace_##n1##_##n2##_ob);                                                  \
}

#define BE_REGISTER_NAMESPACE_3_NAMED(plugin, n1, n2, n3)                                                                                       \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1##_##n2();                                                                          \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1##_##n2##_##n3()                                                                    \
    {                                                                                                                                           \
        static RTTI::Class ci = { #n3, {be_##plugin##_Namespace_##n1##_##n2().m_ptr}, {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0 };          \
        raw<BugEngine::RTTI::Class> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                             \
    }                                                                                                                                           \
    static RTTI::ObjectInfo s_##plugin##_Namespace_##n1##_##n2##_##n3##_ob =                                                                    \
        { be_##plugin##_Namespace_##n1##_##n2()->objects, {0}, #n3, RTTI::Value(be_##plugin##_Namespace_##n1##_##n2##_##n3()) };                \
    BE_EXPORT const RTTI::ObjectInfo* s_##plugin##_Namespace_##n1##_##n2##_##n3##_r =                                                           \
        be_##plugin##_Namespace_##n1##_##n2()->objects.set(&s_##plugin##_Namespace_##n1##_##n2##_##n3##_ob);                                    \
}

#define BE_REGISTER_NAMESPACE_4_NAMED(plugin, n1, n2, n3, n4)                                                                                   \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1##_##n2##_##n3();                                                                   \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4()                                                             \
    {                                                                                                                                           \
        static RTTI::Class ci = { #n4, {be_##plugin##_Namespace_##n1##_##n2##_##n3().m_ptr}, {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0 };   \
        raw<BugEngine::RTTI::Class> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                             \
    }                                                                                                                                           \
    static RTTI::ObjectInfo s_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_ob =                                                             \
        { be_##plugin##_Namespace_##n1##_##n2##_##n3()->objects, {0}, #n4, RTTI::Value(be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4()) };  \
    BE_EXPORT const RTTI::ObjectInfo* s_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_r =                                                    \
        be_##plugin##_Namespace_##n1##_##n2##_##n3()->objects.set(&s_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_ob);                      \
}

#define BE_REGISTER_NAMESPACE_5_NAMED(plugin,n1, n2, n3, n4, n5)                                                                                \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4();                                                            \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5()                                                      \
    {                                                                                                                                           \
        static RTTI::Class ci =                                                                                                                 \
            {                                                                                                                                   \
                #n5,                                                                                                                            \
                {be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4().m_ptr},                                                                    \
                {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0 };                                                                                \
        raw<BugEngine::RTTI::Class> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                             \
    }                                                                                                                                           \
    static RTTI::ObjectInfo s_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5##_ob =                                                      \
        {                                                                                                                                       \
            be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4()->objects,                                                                       \
            {0},                                                                                                                                \
            #n5,                                                                                                                                \
            RTTI::Value(be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5())                                                             \
        };                                                                                                                                      \
    BE_EXPORT const RTTI::ObjectInfo* s_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5##_r =                                             \
        be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4()->objects.set(&s_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5##_ob);        \
}


#define BE_REGISTER_NAMESPACE_1_(plugin, n1)                            \
    BE_REGISTER_NAMESPACE_1_NAMED(plugin, n1)
#define BE_REGISTER_NAMESPACE_2_(plugin, n1, n2)                        \
    BE_REGISTER_NAMESPACE_2_NAMED(plugin, n1, n2)
#define BE_REGISTER_NAMESPACE_3_(plugin, n1, n2, n3)                    \
    BE_REGISTER_NAMESPACE_3_NAMED(plugin, n1, n2, n3)
#define BE_REGISTER_NAMESPACE_4_(plugin, n1, n2, n3, n4)                \
    BE_REGISTER_NAMESPACE_4_NAMED(plugin, n1, n2, n3, n4)
#define BE_REGISTER_NAMESPACE_5_(plugin, n1, n2, n3, n4, n5)            \
    BE_REGISTER_NAMESPACE_5_NAMED(plugin, n1, n2, n3, n4, n5)


#define BE_REGISTER_NAMESPACE_1(n1)                                     \
    BE_REGISTER_NAMESPACE_1_(BE_PROJECTID, n1)
#define BE_REGISTER_NAMESPACE_2(n1, n2)                                 \
    BE_REGISTER_NAMESPACE_2_(BE_PROJECTID, n1, n2)
#define BE_REGISTER_NAMESPACE_3(n1, n2, n3)                             \
    BE_REGISTER_NAMESPACE_3_(BE_PROJECTID, n1, n2, n3)
#define BE_REGISTER_NAMESPACE_4(n1, n2, n3, n4)                         \
    BE_REGISTER_NAMESPACE_4_(BE_PROJECTID, n1, n2, n3, n4)
#define BE_REGISTER_NAMESPACE_5(n1, n2, n3, n4, n5)                     \
    BE_REGISTER_NAMESPACE_5_(BE_PROJECTID, n1, n2, n3, n4, n5)

/*****************************************************************************/
#endif


