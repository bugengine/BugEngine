/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DATA_PARSE_TYPE_HH_
#define BE_DATA_PARSE_TYPE_HH_
/*****************************************************************************/
#include    <parse/node.hh>

namespace BugEngine { namespace Data { namespace Parse
{

class Context;

class Type : public Node
{
private:
    virtual void  dolink(Context& context) const override = 0;
    virtual Value doeval(Context& context) const override = 0;
public:
    class Any;
    class Bool;
    class Int;
    class Float;
    class String;
    class Vector;
    class Map;

    ~Type();
protected:
    Type();
};

class Type::Any : public Type
{
public:
    Any();
    ~Any();
protected:
    void dolink(Context& context) const override;
    Value doeval(Context& context) const override;
};

class Type::Bool : public Type
{
public:
    Bool();
    ~Bool();
protected:
    void dolink(Context& context) const override;
    Value doeval(Context& context) const override;
};

class Type::Int : public Type
{
public:
    Int();
    ~Int();
protected:
    void dolink(Context& context) const override;
    Value doeval(Context& context) const override;
};

class Type::Float : public Type
{
public:
    Float();
    ~Float();
protected:
    void dolink(Context& context) const override;
    Value doeval(Context& context) const override;
};

class Type::String : public Type
{
public:
    String();
    ~String();
protected:
    void dolink(Context& context) const override;
    Value doeval(Context& context) const override;
};

class Type::Vector : public Type
{
private:
    Type*    m_cellType;
public:
    Vector(Type* cellType);
    ~Vector();
protected:
    void dolink(Context& context) const override;
    Value doeval(Context& context) const override;
};

class Type::Map : public Type
{
private:
    Type*    m_keyType;
    Type*    m_valueType;
public:
    Map(Type* keyType, Type* valueType);
    ~Map();
protected:
    void dolink(Context& context) const override;
    Value doeval(Context& context) const override;
};

}}}

/*****************************************************************************/
#endif
