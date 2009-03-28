/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

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
    refptr<Type>    m_cellType;
public:
    Vector(refptr<Type> cellType);
    ~Vector();
protected:
    void dolink(Context& context) const override;
    Value doeval(Context& context) const override;
};

class Type::Map : public Type
{
private:
    refptr<Type>    m_keyType;
    refptr<Type>    m_valueType;
public:
    Map(refptr<Type> keyType, refptr<Type> valueType);
    ~Map();
protected:
    void dolink(Context& context) const override;
    Value doeval(Context& context) const override;
};

}}}

/*****************************************************************************/
#endif
