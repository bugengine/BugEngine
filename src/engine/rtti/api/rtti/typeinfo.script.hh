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

/// Represents the type of an object
/*!
 * The Type struct stores information about the type of an object; it holds
 * information about the class of an object, its indirection, its constness,
 * and the constness of the indirection itself.
 * Just as in \e C++, the constness can be applied to different parts of a type;
 * a pointer to a \e const value is different from a \e const pointer to a value,
 * which is itself different from a \e const pointer to a \e const value.
 * The constness of the object is then split in two parts; a bit in \e indirection
 * indicates the constness of the object pointed at. the \e const field indicates
 * the constness of the indirection.
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
        IndirectionMask = 0x0f,                     ///< Mask that covers all the bits used for the indirection without the \e MutableBit
        MutableBit      = 0x10,                     ///< Constness bit; when this bit is set in the indirection flag,
                                                    ///< the object pointed to is \e mutable; otherwise it is \e const

        Class           = 0,                        ///< The type represents an immediate value without indirection
        ConstRawPtr     = 1,                        ///< The type represents a raw pointer to a \e const object
        ConstWeakPtr    = 2,                        ///< The type represents a weak pointer to a \e const object
        ConstRefPtr     = 3,                        ///< The type represents a refcounted pointer to a \e const object
 
        RawPtr          = ConstRawPtr | MutableBit, ///< The type represents a raw pointer to a \e mutable object
        WeakPtr         = ConstWeakPtr | MutableBit,///< The type represents a weak pointer to a \e mutable object
        RefPtr          = ConstRefPtr | MutableBit  ///< The type represents a refcounted pointer to a \e mutable object
    };
    /// Constness of the object
    /*!
     * Unlike the constness bit in the \e Indirection, the Constness enumeration
     * represents the constness of the 
     */
    enum Constness
    {
        Const = 0,      ///< The type represents a const object; 
        Mutable = 1     ///< The type represents a const object; 
    };
    /// 
    /*!
     */
    enum MakeConstType
    {
        MakeConst       ///< Specifies that the type to copy should be made const
    };

    raw<const RTTI::Class>  metaclass;
    Indirection             indirection;
    Constness               constness;

    static inline Type makeType(raw<const RTTI::Class> klass, Indirection indirection, Constness constness = Mutable)
    {
        Type info = { klass, indirection, constness };
        return info;
    }
    static inline Type makeType(const Type& type, MakeConstType /*constify*/)
    {
        Type info = { type.metaclass, type.indirection, Const };
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

