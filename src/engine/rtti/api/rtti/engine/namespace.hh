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
        static RTTI::ObjectInfo ob = { be_##plugin##_Namespace(), {0}, #n, RTTI::Value(&ci) };                                                  \
        static raw<const RTTI::ObjectInfo> result = {be_##plugin##_Namespace()->objects.set(&ob)};                                              \
        be_forceuse(result);                                                                                                                    \
        raw<BugEngine::RTTI::Class> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                             \
    }                                                                                                                                           \
    static raw<BugEngine::RTTI::Class> s_##plugin##_##n = be_##plugin##_Namespace_##n();                                                        \
}

#define BE_REGISTER_NAMESPACE_2_NAMED(plugin, n1, n2)                                                                                           \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1##_##n2()                                                                           \
    {                                                                                                                                           \
        static RTTI::Class ci = { #n2, be_##plugin##_Namespace_##n1(), {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0 };                         \
        static RTTI::ObjectInfo ob = { be_##plugin##_Namespace_##n1()->objects, {0}, #n2, RTTI::Value(&ci) };                                   \
        static raw<const RTTI::ObjectInfo> result = {be_##plugin##_Namespace_##n1()->objects.set(&ob)};                                         \
        be_forceuse(result);                                                                                                                    \
        raw<BugEngine::RTTI::Class> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                             \
    }                                                                                                                                           \
    static raw<BugEngine::RTTI::Class> s_##plugin##_##n1##_##n2=be_##plugin##_Namespace_##n1##_##n2();                                          \
}

#define BE_REGISTER_NAMESPACE_3_NAMED(plugin, n1, n2, n3)                                                                                       \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1##_##n2##_##n3()                                                                    \
    {                                                                                                                                           \
        static RTTI::Class ci = { #n3, be_##plugin##_Namespace_##n1##_##n2(), {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0 };                  \
        static RTTI::ObjectInfo ob = { be_##plugin##_Namespace_##n1##_##n2(), {0}, #n3, RTTI::Value(&ci) };                                     \
        static raw<const RTTI::ObjectInfo> result = {be_##plugin##_Namespace_##n1##_##n2()->objects.set(&ob)};                                  \
        be_forceuse(result);                                                                                                                    \
        raw<BugEngine::RTTI::Class> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                             \
    }                                                                                                                                           \
    static raw<BugEngine::RTTI::Class> s_##plugin##_##n1##_##n2##_##n3=be_##plugin##_Namespace_##n1##_##n2##_##n3();                            \
}

#define BE_REGISTER_NAMESPACE_4_NAMED(plugin, n1, n2, n3, n4)                                                                                   \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4()                                                             \
    {                                                                                                                                           \
        static RTTI::Class ci = { #n4, be_##plugin##_Namespace_##n1##_##n2##_##n3(), {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0 };           \
        static RTTI::ObjectInfo ob = { be_##plugin##_Namespace_##n1##_##n2##_##n3(), {0}, #n4, RTTI::Value(&ci) };                              \
        static raw<const RTTI::ObjectInfo> result = {be_##plugin##_Namespace_##n1##_##n2##_##n3()->objects.set(&ob)};                           \
        be_forceuse(result);                                                                                                                    \
        raw<BugEngine::RTTI::Class> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                             \
    }                                                                                                                                           \
    static raw<BugEngine::RTTI::Class> s_##plugin##_##n1##_##n2##_##n3##_##n4=be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4();              \
}

#define BE_REGISTER_NAMESPACE_5_NAMED(plugin,n1, n2, n3, n4, n5)                                                                                \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5()                                                      \
    {                                                                                                                                           \
        static RTTI::Class ci = { #n5, be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4(), {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0 };    \
        static RTTI::ObjectInfo ob = { be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4(), {0}, #n5, RTTI::Value(&ci) };                       \
        static raw<const RTTI::ObjectInfo> result = {be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4()->objects.set(&ob)};                    \
        be_forceuse(result);                                                                                                                    \
        raw<BugEngine::RTTI::Class> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                             \
    }                                                                                                                                           \
    static raw<BugEngine::RTTI::Class> s_##plugin##_##n1##_##n2##_##n3##_##n4##_##n5=be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5();\
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
    BE_REGISTER_NAMESPACE_1_(BE_PROJECTSHORTNAME, n1)
#define BE_REGISTER_NAMESPACE_2(n1, n2)                                 \
    BE_REGISTER_NAMESPACE_2_(BE_PROJECTSHORTNAME, n1, n2)
#define BE_REGISTER_NAMESPACE_3(n1, n2, n3)                             \
    BE_REGISTER_NAMESPACE_3_(BE_PROJECTSHORTNAME, n1, n2, n3)
#define BE_REGISTER_NAMESPACE_4(n1, n2, n3, n4)                         \
    BE_REGISTER_NAMESPACE_4_(BE_PROJECTSHORTNAME, n1, n2, n3, n4)
#define BE_REGISTER_NAMESPACE_5(n1, n2, n3, n4, n5)                     \
    BE_REGISTER_NAMESPACE_5_(BE_PROJECTSHORTNAME, n1, n2, n3, n4, n5)

/*****************************************************************************/
#endif


