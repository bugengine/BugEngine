/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TYPEINFO_SCRIPT_HH_
#define BE_RTTI_TYPEINFO_SCRIPT_HH_
/*****************************************************************************/

namespace BugEngine { namespace RTTI
{

class Value;
struct Class;

/// Represents the type of an object packed in a \ref BugEngine::RTTI::Value
/*!
 * The Type struct stores meta-information about an object; it holds
 * information about the class of an object stored in the \ref RTTI::Value,
 * its indirection (and thus how to access it), and the constness of the object.
 *
 * All objects passed to script are packed in an opaque structure; the type
 * allows to modify the value that is packed and the members of this value.
 *
 * \par Constness
 * Just as in \e C++, the constness can be applied to different parts of a type;
 * a pointer to a \e const value is different from a \e const pointer to a value,
 * which is itself different from a \e const pointer to a \e const value.
 * The constness of the object is then split in two parts; the \ref access
 * indicates the constness of the object pointed at. the \ref constness indicates
 * the constness of the variable.
 *
 * \sa
 * BugEngine::RTTI::Value
 */
struct be_api(RTTI) Type
{
    friend class Value;

    /// Indirection to the object
    /*!
     * Represents the type of indirection to the object.
     * The values are sorted according to compatibility; a value holding a \e RefPtr
     * can be unpacked as a \e WeakPtr or a \e RawPtr. It is not possible however
     * to convert the other way around; a \e WeakPtr can not be promoted to a \e RefPtr.
     */
    enum Indirection
    {
        Value   = 0,    ///< The type represents an immediate value without indirection
        RawPtr  = 1,    ///< The type represents a raw pointer to an object
        WeakPtr = 2,    ///< The type represents a weak pointer to an object
        RefPtr  = 3     ///< The type represents a refcounted pointer to an object
    };
    /// Constness of the value
    /*!
     * Convenience enum to indicate of a value is const or mutable.
     */
    enum Constness
    {
        Const = 0,      ///< The type represents a const object or a const indirection to an object
        Mutable = 1     ///< The type represents a mutable object or a mutable indirection to an object
    };
    /// Helper enumeration.
    /*!
     * This enumeration is used in \ref makeType to create a copy of a type but change its constness.
     */
    enum MakeConstType
    {
        MakeConst       ///< Specifies that the type to copy should be made const
    };

    /// Class of the object
    /*!
     * The metaclass contains the meta-information about the packed object:
     * the list of properties, methods, etc.
     * \sa
     *  BugEngine::RTTI::Class
     */
    raw<const Class>    metaclass;
    /// Indirection of the object
    /*!
     * Indicates how to access the object. The value is one of \ref Indirection.
     */
    u16                 indirection;
    /// Access right to the object
    /*!
     * access will be either Const or Mutable. It indicates the constness of the
     * object pointed at. If the access is \ref Mutable, it is possible to modify
     * the properties of the object and to call methods that are not marked
     * \e const. If the access is \ref Const then the properties are read-only
     * and only \e const methods can be called.
     */
    u8                  access;
    /// Access right to the indirection
    /*!
     * indirection will be either Const or Mutable. It indicates the constness of the
     * pointer to the object. If the indirection is \ref Mutable, it is possible to
     * make the value point to another object. If the indirection is \ref Const
     * then it is not possible to change the value to point to another object.
     */
    u8                  constness;

    /// Creates a type
    /*!
     * Creates a type from the given parameters
     * \returns A type description
     * \param klass
     *  The metaclass of the created type
     * \param indirection
     *  The indirection of the created type
     * \param access
     *  The access rights to the value
     * \param constness
     *  The access rights to the indirection itself
     */
    static inline Type makeType(raw<const Class> klass, Indirection indirection, Constness access, Constness constness)
    {
        Type info = { klass, (u16)indirection, (u8)access, (u8)constness };
        return info;
    }

    /// Creates a copy of a type
    /*!
     * Creates a copy of the type description but alters the constness of the indirection
     * \returns A modify copy of the type
     */
    static inline Type makeType(
            const Type& type,               ///< type to copy
            MakeConstType constify          ///< make const
        )
    {
        be_forceuse(constify);
        Type info = { type.metaclass, type.indirection, type.access, Const };
        return info;
    }

    /// Calculates the size of a value of this type.
    /*!
     * \returns
     *  Either the size of a pointer or the size of the object itself
     *  depending on the indirection to the object.
     */
    u32                 size() const;

    /// Returns the name of this type
    /*!
     * Builds the type name depending on the access and indirection.
     * \returns
     *   A formatted string including the indirection, access right and constness of this type.
     */
    minitl::format<>    name() const;
    /// Calculate the distance to another type
    /*!
     * Two types can be compatible but not necessarily equivalent. In order to
     * find the best version of an overloaded method, it is necessary to calculate
     * how close the type of the real parameter is from the type expected by a method.
     * This method returns 0 in case the two types are equal; it returns a value above 0
     * if they don't exactly match but are compatible.
     * It returns a very large value for types that are not compatible.
     * A type is compatible with another one if:
     *  - the metaclass inherits from the compatible type
     *  - the access is less permissive than the compatible type
     *  - the indirection type is compatible
     *
     * \param other Type to which to try compatibility
     * \returns
     *  The distance between type other and this, or MaxTypeDistance if this
     *  is not compatible with type
     */
    u32                 distance(const Type& other) const;

    /// Max distance constant
    /*!
     * The maximum distance between two types, when they are not compatible
     * \deprecated
     */
    enum MaxTypeDistanceType
    {
        MaxTypeDistance = 1000000 ///< The maximum distance to indicate two types are not compatible
    };
public:
    bool isA(const Type& other) const;
private:
    Type& operator=(const Type&);
    void*               rawget(const void*) const;
    void                copy(const void* source, void* dest) const;
    void                create(void* obj) const;
    void                destroy(void* obj) const;
};

}}

/*****************************************************************************/
#endif

