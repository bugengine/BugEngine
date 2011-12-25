/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_VALUE_HH_
#define BE_PACKAGE_NODES_VALUE_HH_
/*****************************************************************************/

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Value : public minitl::refcountable
{
protected:
    Value();
public:
    ~Value();

    virtual TypeInfo getType() const = 0;
};


class Reference;

class ReferenceValue : public Value
{
private:
    ref<const Reference> m_value;
public:
    ReferenceValue(ref<const Reference> value);
    ~ReferenceValue();

    virtual TypeInfo getType() const override;
};

class BoolValue : public Value
{
private:
    bool m_value;
public:
    BoolValue(bool value);
    ~BoolValue();

    virtual TypeInfo getType() const override;
};


class IntValue : public Value
{
private:
    i64 m_value;
public:
    IntValue(i64 param);
    ~IntValue();

    virtual TypeInfo getType() const override;
};


class FloatValue : public Value
{
private:
    double m_value;
public:
    FloatValue(double value);
    ~FloatValue();

    virtual TypeInfo getType() const override;
};


class StringValue : public Value
{
private:
    istring m_value;
public:
    StringValue(const istring& value);
    ~StringValue();

    virtual TypeInfo getType() const override;
};

}}}

/*****************************************************************************/
#endif
