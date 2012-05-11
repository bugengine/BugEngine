/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_VALUE_HH_
#define BE_PACKAGE_NODES_VALUE_HH_
/*****************************************************************************/

namespace BugEngine
{
class File;
class Folder;
}

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Value : public minitl::refcountable
{
protected:
    Value();
public:
    ~Value();

    virtual bool        isCompatible(const RTTI::Type& type) const = 0;
    virtual RTTI::Value as(const RTTI::Type& type) const = 0;
};


class Reference;

class ReferenceValue : public Value
{
private:
    ref<const Reference> m_value;
public:
    ReferenceValue(ref<const Reference> value);
    ~ReferenceValue();

    virtual bool        isCompatible(const RTTI::Type& type) const override;
    virtual RTTI::Value as(const RTTI::Type& type) const override;
};

class BoolValue : public Value
{
private:
    bool m_value;
public:
    BoolValue(bool value);
    ~BoolValue();

    virtual bool        isCompatible(const RTTI::Type& type) const override;
    virtual RTTI::Value as(const RTTI::Type& type) const override;
};


class IntValue : public Value
{
private:
    i64 m_value;
public:
    IntValue(i64 param);
    ~IntValue();

    virtual bool        isCompatible(const RTTI::Type& type) const override;
    virtual RTTI::Value as(const RTTI::Type& type) const override;
};


class FloatValue : public Value
{
private:
    double m_value;
public:
    FloatValue(double value);
    ~FloatValue();

    virtual bool        isCompatible(const RTTI::Type& type) const override;
    virtual RTTI::Value as(const RTTI::Type& type) const override;
};


class StringValue : public Value
{
private:
    const char* m_value;
public:
    StringValue(const char* value);
    ~StringValue();

    virtual bool        isCompatible(const RTTI::Type& type) const override;
    virtual RTTI::Value as(const RTTI::Type& type) const override;
};


class FileValue : public Value
{
private:
    weak<const File> m_value;
public:
    FileValue(weak<Folder> folder, const char* value);
    ~FileValue();

    virtual bool        isCompatible(const RTTI::Type& type) const override;
    virtual RTTI::Value as(const RTTI::Type& type) const override;
};

}}}

/*****************************************************************************/
#endif
