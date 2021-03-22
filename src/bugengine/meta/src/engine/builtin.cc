/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/meta/stdafx.h>
#include <bugengine/meta/classinfo.script.hh>
#include <bugengine/meta/engine/array.factory.hh>
#include <bugengine/meta/engine/helper/method.hh>
#include <bugengine/meta/engine/namespace.hh>

namespace BugEngine { namespace Meta {

template <>
BE_EXPORT raw< const Meta::Class > ClassID< void >::klass()
{
    static Meta::Class s_void = {"void",
                                 0,
                                 0,
                                 0,
                                 {0},
                                 {0},
                                 {0},
                                 {0},
                                 {0, 0},
                                 {0, 0},
                                 {0},
                                 {0},
                                 &Meta::nullconstructor< 0 >,
                                 &Meta::nulldestructor};
    raw< Meta::Class > ci     = {&s_void};
    return ci;
}

template <>
BE_EXPORT raw< const Meta::Class > ClassID< minitl::pointer >::klass()
{
    static Meta::Class s_pointer
        = {"pointer", 0,      0,      0,   {0}, {be_class< void >().m_ptr},  {0},
           {0},       {0, 0}, {0, 0}, {0}, {0}, &Meta::nullconstructor< 0 >, &Meta::nulldestructor};
    raw< Meta::Class > ci = {&s_pointer};
    return ci;
}

template <>
BE_EXPORT raw< const Meta::Class > ClassID< minitl::refcountable >::klass()
{
    static Meta::Class s_refcountable = {"refcountable",
                                         0,
                                         0,
                                         0,
                                         {0},
                                         {be_class< minitl::pointer >().m_ptr},
                                         {0},
                                         {0},
                                         {0, 0},
                                         {0, 0},
                                         {0},
                                         {0},
                                         &Meta::nullconstructor< 0 >,
                                         &Meta::nulldestructor};
    raw< Meta::Class > ci             = {&s_refcountable};
    return ci;
}

static Type s_numericTypes[]
    = {be_type< bool >(), be_type< u8 >(),    be_type< u16 >(),   be_type< u32 >(),
       be_type< u64 >(),  be_type< i8 >(),    be_type< i16 >(),   be_type< i32 >(),
       be_type< i64 >(),  be_type< float >(), be_type< double >()};

static Type s_stringTypes[] = {be_type< istring >(), be_type< inamespace >(),
                               be_type< ifilename >(), be_type< ipath >(), be_type< text >()};

const Type& getTypeFromIndex(ClassIndex_Numeric index)
{
    return s_numericTypes[index];
}
const Type& getTypeFromIndex(ClassIndex_String index)
{
    return s_stringTypes[index];
}

}}  // namespace BugEngine::Meta
