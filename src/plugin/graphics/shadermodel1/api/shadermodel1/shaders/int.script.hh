/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#ifndef BE_OBJECTS_SHADERS_INT_SCRIPT_HH_
#define BE_OBJECTS_SHADERS_INT_SCRIPT_HH_
/*****************************************************************************/
#include    <3d/shader/node.script.hh>

namespace BugEngine { namespace Int
{

class Int : public Shaders::Node
{
    BE_NOCOPY(Int)
protected:
    inline Int();
    inline ~Int();
};

class IntUniform : public Int
{
    BE_NOCOPY(IntUniform)
published:
    const istring name;
published:
    IntUniform(const istring& name);
    ~IntUniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class IntAttribute : public Int
{
    BE_NOCOPY(IntAttribute)
published:
    IntAttribute();
    ~IntAttribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class IntVarying : public Int
{
    BE_NOCOPY(IntVarying)
published:
    const weak<const Int> node;
published:
    IntVarying(weak<const Int> node);
    ~IntVarying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Int> operator /(weak<const Int> node1, weak<const Int> node2);
ref<Int> operator +(weak<const Int> node1, weak<const Int> node2);
ref<Int> operator -(weak<const Int> node1, weak<const Int> node2);



class Int2;
class Int3;
class Int4;
class Int2 : public Shaders::Node
{
    BE_NOCOPY(Int2)
protected:
    inline Int2();
    inline ~Int2();
};

class Int2Uniform : public Int2
{
    BE_NOCOPY(Int2Uniform)
published:
    const istring name;
published:
    Int2Uniform(const istring& name);
    ~Int2Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Int2Attribute : public Int2
{
    BE_NOCOPY(Int2Attribute)
published:
    Int2Attribute();
    ~Int2Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Int2Varying : public Int2
{
    BE_NOCOPY(Int2Varying)
published:
    const weak<const Int2> node;
published:
    Int2Varying(weak<const Int2> node);
    ~Int2Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Int2> operator /(weak<const Int2> node1, weak<const Int2> node2);
ref<Int2> operator +(weak<const Int2> node1, weak<const Int2> node2);
ref<Int2> operator -(weak<const Int2> node1, weak<const Int2> node2);
ref<Int2> operator *(weak<const Int> node1, weak<const Int2> node2);
ref<Int2> operator *(weak<const Int2> node1, weak<const Int> node2);
ref<Int2> operator /(weak<const Int> node1, weak<const Int2> node2);
ref<Int2> operator /(weak<const Int2> node1, weak<const Int> node2);
ref<Int2> operator +(weak<const Int> node1, weak<const Int2> node2);
ref<Int2> operator +(weak<const Int2> node1, weak<const Int> node2);
ref<Int2> operator -(weak<const Int> node1, weak<const Int2> node2);
ref<Int2> operator -(weak<const Int2> node1, weak<const Int> node2);



class Int3 : public Shaders::Node
{
    BE_NOCOPY(Int3)
protected:
    inline Int3();
    inline ~Int3();
};

class Int3Uniform : public Int3
{
    BE_NOCOPY(Int3Uniform)
published:
    const istring name;
published:
    Int3Uniform(const istring& name);
    ~Int3Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Int3Attribute : public Int3
{
    BE_NOCOPY(Int3Attribute)
published:
    Int3Attribute();
    ~Int3Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Int3Varying : public Int3
{
    BE_NOCOPY(Int3Varying)
published:
    const weak<const Int3> node;
published:
    Int3Varying(weak<const Int3> node);
    ~Int3Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Int3> operator /(weak<const Int3> node1, weak<const Int3> node2);
ref<Int3> operator +(weak<const Int3> node1, weak<const Int3> node2);
ref<Int3> operator -(weak<const Int3> node1, weak<const Int3> node2);
ref<Int3> operator *(weak<const Int> node1, weak<const Int3> node2);
ref<Int3> operator *(weak<const Int3> node1, weak<const Int> node2);
ref<Int3> operator /(weak<const Int> node1, weak<const Int3> node2);
ref<Int3> operator /(weak<const Int3> node1, weak<const Int> node2);
ref<Int3> operator +(weak<const Int> node1, weak<const Int3> node2);
ref<Int3> operator +(weak<const Int3> node1, weak<const Int> node2);
ref<Int3> operator -(weak<const Int> node1, weak<const Int3> node2);
ref<Int3> operator -(weak<const Int3> node1, weak<const Int> node2);



class Int4 : public Shaders::Node
{
    BE_NOCOPY(Int4)
protected:
    inline Int4();
    inline ~Int4();
};

class Int4Uniform : public Int4
{
    BE_NOCOPY(Int4Uniform)
published:
    const istring name;
published:
    Int4Uniform(const istring& name);
    ~Int4Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Int4Attribute : public Int4
{
    BE_NOCOPY(Int4Attribute)
published:
    Int4Attribute();
    ~Int4Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Int4Varying : public Int4
{
    BE_NOCOPY(Int4Varying)
published:
    const weak<const Int4> node;
published:
    Int4Varying(weak<const Int4> node);
    ~Int4Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Int4> operator /(weak<const Int4> node1, weak<const Int4> node2);
ref<Int4> operator +(weak<const Int4> node1, weak<const Int4> node2);
ref<Int4> operator -(weak<const Int4> node1, weak<const Int4> node2);
ref<Int4> operator *(weak<const Int> node1, weak<const Int4> node2);
ref<Int4> operator *(weak<const Int4> node1, weak<const Int> node2);
ref<Int4> operator /(weak<const Int> node1, weak<const Int4> node2);
ref<Int4> operator /(weak<const Int4> node1, weak<const Int> node2);
ref<Int4> operator +(weak<const Int> node1, weak<const Int4> node2);
ref<Int4> operator +(weak<const Int4> node1, weak<const Int> node2);
ref<Int4> operator -(weak<const Int> node1, weak<const Int4> node2);
ref<Int4> operator -(weak<const Int4> node1, weak<const Int> node2);




}}

/*****************************************************************************/
#endif
