/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_NAMESPACE_HH_
#define BE_RTTI_ENGINE_NAMESPACE_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>
#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/engine/helper/staticarray.hh>
#include <bugengine/rtti/engine/objectinfo.script.hh>

#define BE_REGISTER_NAMESPACE_1_NAMED(plugin, n)                                                   \
    namespace BugEngine {                                                                          \
    raw< BugEngine::RTTI::Class > be_##plugin##_Namespace();                                       \
    BE_EXPORT raw< BugEngine::RTTI::Class > be_##plugin##_Namespace_##n()                          \
    {                                                                                              \
        static RTTI::Class ci                                                                      \
           = {#n,  0, 0, RTTI::ClassType_Namespace, {0}, {0}, {0}, {0}, {0, 0}, {0, 0}, {0},       \
              {0}, 0, 0};                                                                          \
        raw< BugEngine::RTTI::Class > ptr = {&ci};                                                 \
        return ptr;                                                                                \
    }                                                                                              \
    static RTTI::ObjectInfo s_##plugin##_Namespace_##n##_ob                                        \
       = {be_##plugin##_Namespace()->objects,                                                      \
          {0},                                                                                     \
          #n,                                                                                      \
          RTTI::Value(be_##plugin##_Namespace_##n())};                                             \
    BE_EXPORT const RTTI::ObjectInfo* s_##plugin##_Namespace_##n##_r                               \
       = be_##plugin##_Namespace()->objects.set(&s_##plugin##_Namespace_##n##_ob);                 \
    }

#define BE_REGISTER_NAMESPACE_2_NAMED(plugin, n1, n2)                                              \
    namespace BugEngine {                                                                          \
    raw< BugEngine::RTTI::Class > be_##plugin##_Namespace_##n1();                                  \
    BE_EXPORT raw< BugEngine::RTTI::Class > be_##plugin##_Namespace_##n1##_##n2()                  \
    {                                                                                              \
        static RTTI::Class            ci  = {#n2,                                                  \
                                 0,                                                                \
                                 0,                                                                \
                                 RTTI::ClassType_Namespace,                                        \
                                 {be_##plugin##_Namespace_##n1().m_ptr},                           \
                                 {0},                                                              \
                                 {0},                                                              \
                                 {0},                                                              \
                                 {0, 0},                                                           \
                                 {0, 0},                                                           \
                                 {0},                                                              \
                                 {0},                                                              \
                                 0,                                                                \
                                 0};                                                               \
        raw< BugEngine::RTTI::Class > ptr = {&ci};                                                 \
        return ptr;                                                                                \
    }                                                                                              \
    static RTTI::ObjectInfo s_##plugin##_Namespace_##n1##_##n2##_ob                                \
       = {be_##plugin##_Namespace_##n1()->objects,                                                 \
          {0},                                                                                     \
          #n2,                                                                                     \
          RTTI::Value(be_##plugin##_Namespace_##n1##_##n2())};                                     \
    BE_EXPORT const RTTI::ObjectInfo* s_##plugin##_Namespace_##n1##_##n2##_r                       \
       = be_##plugin##_Namespace_##n1()->objects.set(&s_##plugin##_Namespace_##n1##_##n2##_ob);    \
    }

#define BE_REGISTER_NAMESPACE_3_NAMED(plugin, n1, n2, n3)                                          \
    namespace BugEngine {                                                                          \
    raw< BugEngine::RTTI::Class > be_##plugin##_Namespace_##n1##_##n2();                           \
    BE_EXPORT raw< BugEngine::RTTI::Class > be_##plugin##_Namespace_##n1##_##n2##_##n3()           \
    {                                                                                              \
        static RTTI::Class            ci  = {#n3,                                                  \
                                 0,                                                                \
                                 0,                                                                \
                                 RTTI::ClassType_Namespace,                                        \
                                 {be_##plugin##_Namespace_##n1##_##n2().m_ptr},                    \
                                 {0},                                                              \
                                 {0},                                                              \
                                 {0},                                                              \
                                 {0, 0},                                                           \
                                 {0, 0},                                                           \
                                 {0},                                                              \
                                 {0},                                                              \
                                 0,                                                                \
                                 0};                                                               \
        raw< BugEngine::RTTI::Class > ptr = {&ci};                                                 \
        return ptr;                                                                                \
    }                                                                                              \
    static RTTI::ObjectInfo s_##plugin##_Namespace_##n1##_##n2##_##n3##_ob                         \
       = {be_##plugin##_Namespace_##n1##_##n2()->objects,                                          \
          {0},                                                                                     \
          #n3,                                                                                     \
          RTTI::Value(be_##plugin##_Namespace_##n1##_##n2##_##n3())};                              \
    BE_EXPORT const RTTI::ObjectInfo* s_##plugin##_Namespace_##n1##_##n2##_##n3##_r                \
       = be_##plugin##_Namespace_##n1##_##n2()->objects.set(                                       \
          &s_##plugin##_Namespace_##n1##_##n2##_##n3##_ob);                                        \
    }

#define BE_REGISTER_NAMESPACE_4_NAMED(plugin, n1, n2, n3, n4)                                      \
    namespace BugEngine {                                                                          \
    raw< BugEngine::RTTI::Class > be_##plugin##_Namespace_##n1##_##n2##_##n3();                    \
    BE_EXPORT raw< BugEngine::RTTI::Class > be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4()    \
    {                                                                                              \
        static RTTI::Class            ci  = {#n4,                                                  \
                                 0,                                                                \
                                 0,                                                                \
                                 RTTI::ClassType_Namespace,                                        \
                                 {be_##plugin##_Namespace_##n1##_##n2##_##n3().m_ptr},             \
                                 {0},                                                              \
                                 {0},                                                              \
                                 {0},                                                              \
                                 {0, 0},                                                           \
                                 {0, 0},                                                           \
                                 {0},                                                              \
                                 {0},                                                              \
                                 0,                                                                \
                                 0};                                                               \
        raw< BugEngine::RTTI::Class > ptr = {&ci};                                                 \
        return ptr;                                                                                \
    }                                                                                              \
    static RTTI::ObjectInfo s_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_ob                  \
       = {be_##plugin##_Namespace_##n1##_##n2##_##n3()->objects,                                   \
          {0},                                                                                     \
          #n4,                                                                                     \
          RTTI::Value(be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4())};                       \
    BE_EXPORT const RTTI::ObjectInfo* s_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_r         \
       = be_##plugin##_Namespace_##n1##_##n2##_##n3()->objects.set(                                \
          &s_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_ob);                                 \
    }

#define BE_REGISTER_NAMESPACE_5_NAMED(plugin, n1, n2, n3, n4, n5)                                         \
    namespace BugEngine {                                                                                 \
    raw< BugEngine::RTTI::Class > be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4();                    \
    BE_EXPORT raw< BugEngine::RTTI::Class > be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5()    \
    {                                                                                                     \
        static RTTI::Class            ci  = {#n5,                                                         \
                                 0,                                                                       \
                                 0,                                                                       \
                                 RTTI::ClassType_Namespace,                                               \
                                 {be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4().m_ptr},             \
                                 {0},                                                                     \
                                 {0},                                                                     \
                                 {0},                                                                     \
                                 {0, 0},                                                                  \
                                 {0, 0},                                                                  \
                                 {0},                                                                     \
                                 {0},                                                                     \
                                 0,                                                                       \
                                 0};                                                                      \
        raw< BugEngine::RTTI::Class > ptr = {&ci};                                                        \
        return ptr;                                                                                       \
    }                                                                                                     \
    static RTTI::ObjectInfo s_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5##_ob                  \
       = {be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4()->objects,                                   \
          {0},                                                                                            \
          #n5,                                                                                            \
          RTTI::Value(be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5())};                       \
    BE_EXPORT const RTTI::ObjectInfo* s_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5##_r         \
       = be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4()->objects.set(                                \
          &s_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5##_ob);                                 \
    }

#define BE_REGISTER_NAMESPACE_1_(plugin, n1)     BE_REGISTER_NAMESPACE_1_NAMED(plugin, n1)
#define BE_REGISTER_NAMESPACE_2_(plugin, n1, n2) BE_REGISTER_NAMESPACE_2_NAMED(plugin, n1, n2)
#define BE_REGISTER_NAMESPACE_3_(plugin, n1, n2, n3)                                               \
    BE_REGISTER_NAMESPACE_3_NAMED(plugin, n1, n2, n3)
#define BE_REGISTER_NAMESPACE_4_(plugin, n1, n2, n3, n4)                                           \
    BE_REGISTER_NAMESPACE_4_NAMED(plugin, n1, n2, n3, n4)
#define BE_REGISTER_NAMESPACE_5_(plugin, n1, n2, n3, n4, n5)                                       \
    BE_REGISTER_NAMESPACE_5_NAMED(plugin, n1, n2, n3, n4, n5)

#define BE_REGISTER_NAMESPACE_1(n1)         BE_REGISTER_NAMESPACE_1_(BE_PROJECTID, n1)
#define BE_REGISTER_NAMESPACE_2(n1, n2)     BE_REGISTER_NAMESPACE_2_(BE_PROJECTID, n1, n2)
#define BE_REGISTER_NAMESPACE_3(n1, n2, n3) BE_REGISTER_NAMESPACE_3_(BE_PROJECTID, n1, n2, n3)
#define BE_REGISTER_NAMESPACE_4(n1, n2, n3, n4)                                                    \
    BE_REGISTER_NAMESPACE_4_(BE_PROJECTID, n1, n2, n3, n4)
#define BE_REGISTER_NAMESPACE_5(n1, n2, n3, n4, n5)                                                \
    BE_REGISTER_NAMESPACE_5_(BE_PROJECTID, n1, n2, n3, n4, n5)

#define BE_REGISTER_ROOT_NAMESPACE_(id, parent, name)                                              \
    namespace BugEngine {                                                                          \
    raw< RTTI::Class > be_##id##_Namespace##parent();                                              \
    raw< RTTI::Class > be_##id##_Namespace##parent##_##name()                                      \
    {                                                                                              \
        return be_##id##_Namespace();                                                              \
    }                                                                                              \
    static RTTI::ObjectInfo ob##id##parent##_##name                                                \
       = {be_##id##_Namespace##parent()->objects, {0}, #name, RTTI::Value(be_##id##_Namespace())}; \
    BE_EXPORT const RTTI::ObjectInfo* s_namespaceRegistration_##id##parent##_##name                \
       = (be_##id##_Namespace##parent()->objects.m_ptr = &ob##id##parent##_##name);                \
    }
#define BE_REGISTER_ROOT_NAMESPACE(id, ns, parent) BE_REGISTER_ROOT_NAMESPACE_(id, ns, parent)

/**************************************************************************************************/
#endif
