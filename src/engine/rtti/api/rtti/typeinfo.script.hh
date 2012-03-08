/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_SCRIPT_HH_
#define BE_RTTI_TYPEINFO_SCRIPT_HH_
/*****************************************************************************/

namespace BugEngine
{

class Value;

namespace RTTI
{
struct Class;
}

/// Represents the type of an object packed in an \ref RTTI::Value
/*!
 * The Type struct stores meta-information about the type of an object; it holds
 * information about the class of an object stored in the \ref RTTI::Value,
 * its indirection (and thus how to access it), and the constness of the object
 *
 * All objects passed to script are packed in an opaque structure; the type
 * allows to modify the value that is packed or the members of this value.
 *
 * \par Constness
 * Just as in \e C++, the constness can be applied to different parts of a type;
 * a pointer to a \e const value is different from a \e const pointer to a value,
 * which is itself different from a \e const pointer to a \e const value.
 * The constness of the object is then split in two parts; a bit in \e indirection
 * indicates the constness of the object pointed at. the \e const field indicates
 * the constness of the indirection.
 *
 * \sa
 * BugEngine::RTTI::Value
 */
struct be_api(RTTI) Type
{
    friend class Value;

    /// Indirection to the object
    /*!
     * Represents the type of indirection to the object; it also indicates the constness
     * of the object pointed to.
     */
    enum Indirection
    {
        Value   = 0,    ///< The type represents an immediate value without indirection
        RawPtr  = 1,    ///< The type represents a raw pointer to an object
        WeakPtr = 2,    ///< The type represents a weak pointer to an object
        RefPtr  = 3     ///< The type represents a refcounted pointer to an object
    };
    /// Constness of the value
    enum Constness
    {
        Mutable = 0,    ///< The type represents a mutable object
        Const = 1       ///< The type represents a const object
    };
    /// Helper enumeration.
    /*!
     * This enumeration is used in \ref makeType to create a copy of a type but change its constness.
     */
    enum MakeConstType
    {
        MakeConst       ///< Specifies that the type to copy should be made const
    };

    raw<const RTTI::Class>  metaclass;
    u16                     indirection;
    u8                      access;
    u8                      constness;

    static inline Type makeType(raw<const RTTI::Class> klass, Indirection indirection, Constness access, Constness constness)
    {
        Type info = { klass, (u16)indirection, (u8)access, (u8)constness };
        return info;
    }
    static inline Type makeType(const Type& type, MakeConstType /*constify*/)
    {
        Type info = { type.metaclass, type.indirection, type.access, Const };
        return info;
    }

    u32                 size() const;
    minitl::format<>    name() const;
    u32                 distance(const Type& other) const;
public:
    enum { MaxTypeDistance = 1000000 };
private:
    Type& operator=(const Type&);
    void*               rawget(const void*) const;
    void                copy(const void* source, void* dest) const;
    void                create(void* obj) const;
    void                destroy(void* obj) const;
};

}

/*****************************************************************************/
#endif

