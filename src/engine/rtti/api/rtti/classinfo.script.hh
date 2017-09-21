/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_CLASSINFO_SCRIPT_HH_
#define BE_RTTI_CLASSINFO_SCRIPT_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <core/md5.hh>
#include    <rtti/engine/helper/staticarray.hh>

namespace BugEngine { namespace RTTI
{

class Value;
struct ObjectInfo;
struct Property;
struct Method;
struct Tag;
struct ScriptingAPI;
template< typename T >
struct staticarray;

enum ClassType
{
    ClassType_Object,
    ClassType_Struct,
    ClassType_Pod,
    ClassType_Namespace,
    ClassType_Array,
    ClassType_Enum,
    ClassType_String,
    ClassType_Number,
    ClassType_Vector2,
    ClassType_Vector3,
    ClassType_Vector4,
    ClassType_Matrix,
    ClassType_Variant
};


be_tag(Index(ClassType_Namespace))
struct be_api(RTTI) Class
{
    friend struct Type;
    friend class Value;
published:
    static const istring nameConstructor();
    static const istring nameDestructor();
    static const istring nameOperatorCall();
    static const istring nameOperatorIndex();
    static const istring nameOperatorLessThan();
    static const istring nameOperatorGreaterThan();
    static const istring nameOperatorLessThanOrEqual();
    static const istring nameOperatorGreaterThanOrEqual();
    static const istring nameOperatorMultiply();
    static const istring nameOperatorDivide();
    static const istring nameOperatorModulo();
    static const istring nameOperatorAdd();
    static const istring nameOperatorSubstract();
    static const istring nameOperatorShiftLeft();
    static const istring nameOperatorShiftRight();
    static const istring nameOperatorBitwiseAnd();
    static const istring nameOperatorBitwiseOr();
    static const istring nameOperatorBitwiseXor();
    static const istring nameOperatorBitwiseNot();
    static const istring nameOperatorLogicalAnd();
    static const istring nameOperatorLogicalOr();
    static const istring nameOperatorLogicalNot();
    static const istring nameOperatorEqual();
    static const istring nameOperatorNotEqual();
    static const istring nameOperatorAssign();
    static const istring nameOperatorMultiplyAssign();
    static const istring nameOperatorDivideAssign();
    static const istring nameOperatorModuloAssign();
    static const istring nameOperatorAddAssign();
    static const istring nameOperatorSubstractAssign();
    static const istring nameOperatorShiftLeftAssign();
    static const istring nameOperatorShiftRightAssign();
    static const istring nameOperatorAndAssign();
    static const istring nameOperatorOrAssign();
    static const istring nameOperatorXorAssign();
    static const istring nameOperatorIncrement();
    static const istring nameOperatorDecrement();
    static const istring nameOperatorGet();
published:
    istring const                       name;
    u32 const                           size;
    i32 const                           offset;
    u32 const                           id;
    raw<const Class> const              owner;
    raw<const Class> const              parent;
    raw<const ObjectInfo>               objects;
    raw< const staticarray<const Tag> > tags;
    staticarray<const Property>         properties;
    staticarray<const Method>           methods;
    be_tag(Alias("?call"))
    raw<const Method>                   constructor;
public:
    raw<const ScriptingAPI>             apiMethods;
    typedef void (*CopyConstructor)(const void* source, void* destination);
    typedef void (*Destructor)(void* object);
    const CopyConstructor       copyconstructor;
    const Destructor            destructor;
published:
    typedef enum ClassType ClassType;
    Value getTag(const Type& type) const;
    Value getTag(raw<const Class> type) const;

    Value get(Value& from, istring name, bool& found) const;
    Value get(const Value& from, istring name, bool& found) const;

    bool isA(raw<const Class> klass) const;

    inamespace fullname() const;

    inline ClassType type() const       { return ClassType(id & 0xffff); }
    inline u32 index() const            { return id >> 16; }

    raw<const Property> getProperty(istring propertyName) const;
    raw<const Method> getMethod(istring methodName) const;
    raw<const ObjectInfo> getStaticProperty(istring propertyName) const;
    static Value findClass(inamespace name);
public:
    typedef void(*EnumerateCallback)(const Value& v);
    enum EnumerateRecursion
    {
        EnumerateOwn,
        EnumerateRecursive
    };
    void enumerateObjects(EnumerateRecursion recursion, EnumerateCallback callback) const;
    bool distance(raw<const Class> other, u16& result) const;
    const ObjectInfo* addObject(const istring& s, const ObjectInfo* ob);
private: // friend Value
    void copy(const void* src, void* dst) const;
    void destroy(void* src) const;
};

}

be_api(RTTI) raw<RTTI::Class> be_game_Namespace();
raw<RTTI::Class> be_game_Namespace_BugEngine();

}


/**************************************************************************************************/
#endif

