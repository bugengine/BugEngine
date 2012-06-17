/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#ifndef BE_OBJECTS_SHADERS_FLOAT_SCRIPT_HH_
#define BE_OBJECTS_SHADERS_FLOAT_SCRIPT_HH_
/*****************************************************************************/
#include    <3d/shader/node.script.hh>

namespace BugEngine { namespace Float
{

class Float : public Shaders::Node
{
    BE_NOCOPY(Float)
protected:
    inline Float();
    inline ~Float();
};

class FloatUniform : public Float
{
    BE_NOCOPY(FloatUniform)
published:
    const istring name;
published:
    FloatUniform(const istring& name);
    ~FloatUniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class FloatAttribute : public Float
{
    BE_NOCOPY(FloatAttribute)
published:
    FloatAttribute();
    ~FloatAttribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class FloatVarying : public Float
{
    BE_NOCOPY(FloatVarying)
published:
    const weak<const Float> node;
published:
    FloatVarying(weak<const Float> node);
    ~FloatVarying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Float> operator /(weak<const Float> node1, weak<const Float> node2);
ref<Float> operator +(weak<const Float> node1, weak<const Float> node2);
ref<Float> operator -(weak<const Float> node1, weak<const Float> node2);



class Float2;
class Float3;
class Float4;
class Float2x2;
class Float3x2;
class Float4x2;
class Float2x3;
class Float3x3;
class Float4x3;
class Float2x4;
class Float3x4;
class Float4x4;
class Float2 : public Shaders::Node
{
    BE_NOCOPY(Float2)
protected:
    inline Float2();
    inline ~Float2();
};

class Float2Uniform : public Float2
{
    BE_NOCOPY(Float2Uniform)
published:
    const istring name;
published:
    Float2Uniform(const istring& name);
    ~Float2Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float2Attribute : public Float2
{
    BE_NOCOPY(Float2Attribute)
published:
    Float2Attribute();
    ~Float2Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float2Varying : public Float2
{
    BE_NOCOPY(Float2Varying)
published:
    const weak<const Float2> node;
published:
    Float2Varying(weak<const Float2> node);
    ~Float2Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Float2> operator /(weak<const Float2> node1, weak<const Float2> node2);
ref<Float2> operator +(weak<const Float2> node1, weak<const Float2> node2);
ref<Float2> operator -(weak<const Float2> node1, weak<const Float2> node2);
ref<Float2> operator *(weak<const Float2> node1, weak<const Float2x2> node2);
ref<Float2> operator *(weak<const Float2x2> node1, weak<const Float2> node2);
ref<Float2> operator *(weak<const Float> node1, weak<const Float2> node2);
ref<Float2> operator *(weak<const Float2> node1, weak<const Float> node2);
ref<Float2> operator /(weak<const Float> node1, weak<const Float2> node2);
ref<Float2> operator /(weak<const Float2> node1, weak<const Float> node2);
ref<Float2> operator +(weak<const Float> node1, weak<const Float2> node2);
ref<Float2> operator +(weak<const Float2> node1, weak<const Float> node2);
ref<Float2> operator -(weak<const Float> node1, weak<const Float2> node2);
ref<Float2> operator -(weak<const Float2> node1, weak<const Float> node2);



class Float3 : public Shaders::Node
{
    BE_NOCOPY(Float3)
protected:
    inline Float3();
    inline ~Float3();
};

class Float3Uniform : public Float3
{
    BE_NOCOPY(Float3Uniform)
published:
    const istring name;
published:
    Float3Uniform(const istring& name);
    ~Float3Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float3Attribute : public Float3
{
    BE_NOCOPY(Float3Attribute)
published:
    Float3Attribute();
    ~Float3Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float3Varying : public Float3
{
    BE_NOCOPY(Float3Varying)
published:
    const weak<const Float3> node;
published:
    Float3Varying(weak<const Float3> node);
    ~Float3Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Float3> operator /(weak<const Float3> node1, weak<const Float3> node2);
ref<Float3> operator +(weak<const Float3> node1, weak<const Float3> node2);
ref<Float3> operator -(weak<const Float3> node1, weak<const Float3> node2);
ref<Float3> operator *(weak<const Float3> node1, weak<const Float3x3> node2);
ref<Float3> operator *(weak<const Float3x3> node1, weak<const Float3> node2);
ref<Float3> operator *(weak<const Float> node1, weak<const Float3> node2);
ref<Float3> operator *(weak<const Float3> node1, weak<const Float> node2);
ref<Float3> operator /(weak<const Float> node1, weak<const Float3> node2);
ref<Float3> operator /(weak<const Float3> node1, weak<const Float> node2);
ref<Float3> operator +(weak<const Float> node1, weak<const Float3> node2);
ref<Float3> operator +(weak<const Float3> node1, weak<const Float> node2);
ref<Float3> operator -(weak<const Float> node1, weak<const Float3> node2);
ref<Float3> operator -(weak<const Float3> node1, weak<const Float> node2);



class Float4 : public Shaders::Node
{
    BE_NOCOPY(Float4)
protected:
    inline Float4();
    inline ~Float4();
};

class Float4Uniform : public Float4
{
    BE_NOCOPY(Float4Uniform)
published:
    const istring name;
published:
    Float4Uniform(const istring& name);
    ~Float4Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float4Attribute : public Float4
{
    BE_NOCOPY(Float4Attribute)
published:
    Float4Attribute();
    ~Float4Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float4Varying : public Float4
{
    BE_NOCOPY(Float4Varying)
published:
    const weak<const Float4> node;
published:
    Float4Varying(weak<const Float4> node);
    ~Float4Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Float4> operator /(weak<const Float4> node1, weak<const Float4> node2);
ref<Float4> operator +(weak<const Float4> node1, weak<const Float4> node2);
ref<Float4> operator -(weak<const Float4> node1, weak<const Float4> node2);
ref<Float4> operator *(weak<const Float4> node1, weak<const Float4x4> node2);
ref<Float4> operator *(weak<const Float4x4> node1, weak<const Float4> node2);
ref<Float4> operator *(weak<const Float> node1, weak<const Float4> node2);
ref<Float4> operator *(weak<const Float4> node1, weak<const Float> node2);
ref<Float4> operator /(weak<const Float> node1, weak<const Float4> node2);
ref<Float4> operator /(weak<const Float4> node1, weak<const Float> node2);
ref<Float4> operator +(weak<const Float> node1, weak<const Float4> node2);
ref<Float4> operator +(weak<const Float4> node1, weak<const Float> node2);
ref<Float4> operator -(weak<const Float> node1, weak<const Float4> node2);
ref<Float4> operator -(weak<const Float4> node1, weak<const Float> node2);



class Float2x2 : public Shaders::Node
{
    BE_NOCOPY(Float2x2)
protected:
    inline Float2x2();
    inline ~Float2x2();
};

class Float2x2Uniform : public Float2x2
{
    BE_NOCOPY(Float2x2Uniform)
published:
    const istring name;
published:
    Float2x2Uniform(const istring& name);
    ~Float2x2Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float2x2Attribute : public Float2x2
{
    BE_NOCOPY(Float2x2Attribute)
published:
    Float2x2Attribute();
    ~Float2x2Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float2x2Varying : public Float2x2
{
    BE_NOCOPY(Float2x2Varying)
published:
    const weak<const Float2x2> node;
published:
    Float2x2Varying(weak<const Float2x2> node);
    ~Float2x2Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Float2x2> operator /(weak<const Float2x2> node1, weak<const Float2x2> node2);
ref<Float2x2> operator +(weak<const Float2x2> node1, weak<const Float2x2> node2);
ref<Float2x2> operator -(weak<const Float2x2> node1, weak<const Float2x2> node2);
ref<Float2x2> operator *(weak<const Float2x2> node1, weak<const Float2x2> node2);
ref<Float2x2> operator *(weak<const Float3x2> node1, weak<const Float2x3> node2);
ref<Float2x2> operator *(weak<const Float4x2> node1, weak<const Float2x4> node2);
ref<Float2x2> operator *(weak<const Float> node1, weak<const Float2x2> node2);
ref<Float2x2> operator *(weak<const Float2x2> node1, weak<const Float> node2);
ref<Float2x2> operator /(weak<const Float> node1, weak<const Float2x2> node2);
ref<Float2x2> operator /(weak<const Float2x2> node1, weak<const Float> node2);
ref<Float2x2> operator +(weak<const Float> node1, weak<const Float2x2> node2);
ref<Float2x2> operator +(weak<const Float2x2> node1, weak<const Float> node2);
ref<Float2x2> operator -(weak<const Float> node1, weak<const Float2x2> node2);
ref<Float2x2> operator -(weak<const Float2x2> node1, weak<const Float> node2);



class Float3x2 : public Shaders::Node
{
    BE_NOCOPY(Float3x2)
protected:
    inline Float3x2();
    inline ~Float3x2();
};

class Float3x2Uniform : public Float3x2
{
    BE_NOCOPY(Float3x2Uniform)
published:
    const istring name;
published:
    Float3x2Uniform(const istring& name);
    ~Float3x2Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float3x2Attribute : public Float3x2
{
    BE_NOCOPY(Float3x2Attribute)
published:
    Float3x2Attribute();
    ~Float3x2Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float3x2Varying : public Float3x2
{
    BE_NOCOPY(Float3x2Varying)
published:
    const weak<const Float3x2> node;
published:
    Float3x2Varying(weak<const Float3x2> node);
    ~Float3x2Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Float3x2> operator /(weak<const Float3x2> node1, weak<const Float3x2> node2);
ref<Float3x2> operator +(weak<const Float3x2> node1, weak<const Float3x2> node2);
ref<Float3x2> operator -(weak<const Float3x2> node1, weak<const Float3x2> node2);
ref<Float3x2> operator *(weak<const Float2x2> node1, weak<const Float3x2> node2);
ref<Float3x2> operator *(weak<const Float3x2> node1, weak<const Float3x3> node2);
ref<Float3x2> operator *(weak<const Float4x2> node1, weak<const Float3x4> node2);
ref<Float3x2> operator *(weak<const Float> node1, weak<const Float3x2> node2);
ref<Float3x2> operator *(weak<const Float3x2> node1, weak<const Float> node2);
ref<Float3x2> operator /(weak<const Float> node1, weak<const Float3x2> node2);
ref<Float3x2> operator /(weak<const Float3x2> node1, weak<const Float> node2);
ref<Float3x2> operator +(weak<const Float> node1, weak<const Float3x2> node2);
ref<Float3x2> operator +(weak<const Float3x2> node1, weak<const Float> node2);
ref<Float3x2> operator -(weak<const Float> node1, weak<const Float3x2> node2);
ref<Float3x2> operator -(weak<const Float3x2> node1, weak<const Float> node2);



class Float4x2 : public Shaders::Node
{
    BE_NOCOPY(Float4x2)
protected:
    inline Float4x2();
    inline ~Float4x2();
};

class Float4x2Uniform : public Float4x2
{
    BE_NOCOPY(Float4x2Uniform)
published:
    const istring name;
published:
    Float4x2Uniform(const istring& name);
    ~Float4x2Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float4x2Attribute : public Float4x2
{
    BE_NOCOPY(Float4x2Attribute)
published:
    Float4x2Attribute();
    ~Float4x2Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float4x2Varying : public Float4x2
{
    BE_NOCOPY(Float4x2Varying)
published:
    const weak<const Float4x2> node;
published:
    Float4x2Varying(weak<const Float4x2> node);
    ~Float4x2Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Float4x2> operator /(weak<const Float4x2> node1, weak<const Float4x2> node2);
ref<Float4x2> operator +(weak<const Float4x2> node1, weak<const Float4x2> node2);
ref<Float4x2> operator -(weak<const Float4x2> node1, weak<const Float4x2> node2);
ref<Float4x2> operator *(weak<const Float2x2> node1, weak<const Float4x2> node2);
ref<Float4x2> operator *(weak<const Float3x2> node1, weak<const Float4x3> node2);
ref<Float4x2> operator *(weak<const Float4x2> node1, weak<const Float4x4> node2);
ref<Float4x2> operator *(weak<const Float> node1, weak<const Float4x2> node2);
ref<Float4x2> operator *(weak<const Float4x2> node1, weak<const Float> node2);
ref<Float4x2> operator /(weak<const Float> node1, weak<const Float4x2> node2);
ref<Float4x2> operator /(weak<const Float4x2> node1, weak<const Float> node2);
ref<Float4x2> operator +(weak<const Float> node1, weak<const Float4x2> node2);
ref<Float4x2> operator +(weak<const Float4x2> node1, weak<const Float> node2);
ref<Float4x2> operator -(weak<const Float> node1, weak<const Float4x2> node2);
ref<Float4x2> operator -(weak<const Float4x2> node1, weak<const Float> node2);



class Float2x3 : public Shaders::Node
{
    BE_NOCOPY(Float2x3)
protected:
    inline Float2x3();
    inline ~Float2x3();
};

class Float2x3Uniform : public Float2x3
{
    BE_NOCOPY(Float2x3Uniform)
published:
    const istring name;
published:
    Float2x3Uniform(const istring& name);
    ~Float2x3Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float2x3Attribute : public Float2x3
{
    BE_NOCOPY(Float2x3Attribute)
published:
    Float2x3Attribute();
    ~Float2x3Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float2x3Varying : public Float2x3
{
    BE_NOCOPY(Float2x3Varying)
published:
    const weak<const Float2x3> node;
published:
    Float2x3Varying(weak<const Float2x3> node);
    ~Float2x3Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Float2x3> operator /(weak<const Float2x3> node1, weak<const Float2x3> node2);
ref<Float2x3> operator +(weak<const Float2x3> node1, weak<const Float2x3> node2);
ref<Float2x3> operator -(weak<const Float2x3> node1, weak<const Float2x3> node2);
ref<Float2x3> operator *(weak<const Float2x3> node1, weak<const Float2x2> node2);
ref<Float2x3> operator *(weak<const Float3x3> node1, weak<const Float2x3> node2);
ref<Float2x3> operator *(weak<const Float4x3> node1, weak<const Float2x4> node2);
ref<Float2x3> operator *(weak<const Float> node1, weak<const Float2x3> node2);
ref<Float2x3> operator *(weak<const Float2x3> node1, weak<const Float> node2);
ref<Float2x3> operator /(weak<const Float> node1, weak<const Float2x3> node2);
ref<Float2x3> operator /(weak<const Float2x3> node1, weak<const Float> node2);
ref<Float2x3> operator +(weak<const Float> node1, weak<const Float2x3> node2);
ref<Float2x3> operator +(weak<const Float2x3> node1, weak<const Float> node2);
ref<Float2x3> operator -(weak<const Float> node1, weak<const Float2x3> node2);
ref<Float2x3> operator -(weak<const Float2x3> node1, weak<const Float> node2);



class Float3x3 : public Shaders::Node
{
    BE_NOCOPY(Float3x3)
protected:
    inline Float3x3();
    inline ~Float3x3();
};

class Float3x3Uniform : public Float3x3
{
    BE_NOCOPY(Float3x3Uniform)
published:
    const istring name;
published:
    Float3x3Uniform(const istring& name);
    ~Float3x3Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float3x3Attribute : public Float3x3
{
    BE_NOCOPY(Float3x3Attribute)
published:
    Float3x3Attribute();
    ~Float3x3Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float3x3Varying : public Float3x3
{
    BE_NOCOPY(Float3x3Varying)
published:
    const weak<const Float3x3> node;
published:
    Float3x3Varying(weak<const Float3x3> node);
    ~Float3x3Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Float3x3> operator /(weak<const Float3x3> node1, weak<const Float3x3> node2);
ref<Float3x3> operator +(weak<const Float3x3> node1, weak<const Float3x3> node2);
ref<Float3x3> operator -(weak<const Float3x3> node1, weak<const Float3x3> node2);
ref<Float3x3> operator *(weak<const Float2x3> node1, weak<const Float3x2> node2);
ref<Float3x3> operator *(weak<const Float3x3> node1, weak<const Float3x3> node2);
ref<Float3x3> operator *(weak<const Float4x3> node1, weak<const Float3x4> node2);
ref<Float3x3> operator *(weak<const Float> node1, weak<const Float3x3> node2);
ref<Float3x3> operator *(weak<const Float3x3> node1, weak<const Float> node2);
ref<Float3x3> operator /(weak<const Float> node1, weak<const Float3x3> node2);
ref<Float3x3> operator /(weak<const Float3x3> node1, weak<const Float> node2);
ref<Float3x3> operator +(weak<const Float> node1, weak<const Float3x3> node2);
ref<Float3x3> operator +(weak<const Float3x3> node1, weak<const Float> node2);
ref<Float3x3> operator -(weak<const Float> node1, weak<const Float3x3> node2);
ref<Float3x3> operator -(weak<const Float3x3> node1, weak<const Float> node2);



class Float4x3 : public Shaders::Node
{
    BE_NOCOPY(Float4x3)
protected:
    inline Float4x3();
    inline ~Float4x3();
};

class Float4x3Uniform : public Float4x3
{
    BE_NOCOPY(Float4x3Uniform)
published:
    const istring name;
published:
    Float4x3Uniform(const istring& name);
    ~Float4x3Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float4x3Attribute : public Float4x3
{
    BE_NOCOPY(Float4x3Attribute)
published:
    Float4x3Attribute();
    ~Float4x3Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float4x3Varying : public Float4x3
{
    BE_NOCOPY(Float4x3Varying)
published:
    const weak<const Float4x3> node;
published:
    Float4x3Varying(weak<const Float4x3> node);
    ~Float4x3Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Float4x3> operator /(weak<const Float4x3> node1, weak<const Float4x3> node2);
ref<Float4x3> operator +(weak<const Float4x3> node1, weak<const Float4x3> node2);
ref<Float4x3> operator -(weak<const Float4x3> node1, weak<const Float4x3> node2);
ref<Float4x3> operator *(weak<const Float2x3> node1, weak<const Float4x2> node2);
ref<Float4x3> operator *(weak<const Float3x3> node1, weak<const Float4x3> node2);
ref<Float4x3> operator *(weak<const Float4x3> node1, weak<const Float4x4> node2);
ref<Float4x3> operator *(weak<const Float> node1, weak<const Float4x3> node2);
ref<Float4x3> operator *(weak<const Float4x3> node1, weak<const Float> node2);
ref<Float4x3> operator /(weak<const Float> node1, weak<const Float4x3> node2);
ref<Float4x3> operator /(weak<const Float4x3> node1, weak<const Float> node2);
ref<Float4x3> operator +(weak<const Float> node1, weak<const Float4x3> node2);
ref<Float4x3> operator +(weak<const Float4x3> node1, weak<const Float> node2);
ref<Float4x3> operator -(weak<const Float> node1, weak<const Float4x3> node2);
ref<Float4x3> operator -(weak<const Float4x3> node1, weak<const Float> node2);



class Float2x4 : public Shaders::Node
{
    BE_NOCOPY(Float2x4)
protected:
    inline Float2x4();
    inline ~Float2x4();
};

class Float2x4Uniform : public Float2x4
{
    BE_NOCOPY(Float2x4Uniform)
published:
    const istring name;
published:
    Float2x4Uniform(const istring& name);
    ~Float2x4Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float2x4Attribute : public Float2x4
{
    BE_NOCOPY(Float2x4Attribute)
published:
    Float2x4Attribute();
    ~Float2x4Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float2x4Varying : public Float2x4
{
    BE_NOCOPY(Float2x4Varying)
published:
    const weak<const Float2x4> node;
published:
    Float2x4Varying(weak<const Float2x4> node);
    ~Float2x4Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Float2x4> operator /(weak<const Float2x4> node1, weak<const Float2x4> node2);
ref<Float2x4> operator +(weak<const Float2x4> node1, weak<const Float2x4> node2);
ref<Float2x4> operator -(weak<const Float2x4> node1, weak<const Float2x4> node2);
ref<Float2x4> operator *(weak<const Float2x4> node1, weak<const Float2x2> node2);
ref<Float2x4> operator *(weak<const Float3x4> node1, weak<const Float2x3> node2);
ref<Float2x4> operator *(weak<const Float4x4> node1, weak<const Float2x4> node2);
ref<Float2x4> operator *(weak<const Float> node1, weak<const Float2x4> node2);
ref<Float2x4> operator *(weak<const Float2x4> node1, weak<const Float> node2);
ref<Float2x4> operator /(weak<const Float> node1, weak<const Float2x4> node2);
ref<Float2x4> operator /(weak<const Float2x4> node1, weak<const Float> node2);
ref<Float2x4> operator +(weak<const Float> node1, weak<const Float2x4> node2);
ref<Float2x4> operator +(weak<const Float2x4> node1, weak<const Float> node2);
ref<Float2x4> operator -(weak<const Float> node1, weak<const Float2x4> node2);
ref<Float2x4> operator -(weak<const Float2x4> node1, weak<const Float> node2);



class Float3x4 : public Shaders::Node
{
    BE_NOCOPY(Float3x4)
protected:
    inline Float3x4();
    inline ~Float3x4();
};

class Float3x4Uniform : public Float3x4
{
    BE_NOCOPY(Float3x4Uniform)
published:
    const istring name;
published:
    Float3x4Uniform(const istring& name);
    ~Float3x4Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float3x4Attribute : public Float3x4
{
    BE_NOCOPY(Float3x4Attribute)
published:
    Float3x4Attribute();
    ~Float3x4Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float3x4Varying : public Float3x4
{
    BE_NOCOPY(Float3x4Varying)
published:
    const weak<const Float3x4> node;
published:
    Float3x4Varying(weak<const Float3x4> node);
    ~Float3x4Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Float3x4> operator /(weak<const Float3x4> node1, weak<const Float3x4> node2);
ref<Float3x4> operator +(weak<const Float3x4> node1, weak<const Float3x4> node2);
ref<Float3x4> operator -(weak<const Float3x4> node1, weak<const Float3x4> node2);
ref<Float3x4> operator *(weak<const Float2x4> node1, weak<const Float3x2> node2);
ref<Float3x4> operator *(weak<const Float3x4> node1, weak<const Float3x3> node2);
ref<Float3x4> operator *(weak<const Float4x4> node1, weak<const Float3x4> node2);
ref<Float3x4> operator *(weak<const Float> node1, weak<const Float3x4> node2);
ref<Float3x4> operator *(weak<const Float3x4> node1, weak<const Float> node2);
ref<Float3x4> operator /(weak<const Float> node1, weak<const Float3x4> node2);
ref<Float3x4> operator /(weak<const Float3x4> node1, weak<const Float> node2);
ref<Float3x4> operator +(weak<const Float> node1, weak<const Float3x4> node2);
ref<Float3x4> operator +(weak<const Float3x4> node1, weak<const Float> node2);
ref<Float3x4> operator -(weak<const Float> node1, weak<const Float3x4> node2);
ref<Float3x4> operator -(weak<const Float3x4> node1, weak<const Float> node2);



class Float4x4 : public Shaders::Node
{
    BE_NOCOPY(Float4x4)
protected:
    inline Float4x4();
    inline ~Float4x4();
};

class Float4x4Uniform : public Float4x4
{
    BE_NOCOPY(Float4x4Uniform)
published:
    const istring name;
published:
    Float4x4Uniform(const istring& name);
    ~Float4x4Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float4x4Attribute : public Float4x4
{
    BE_NOCOPY(Float4x4Attribute)
published:
    Float4x4Attribute();
    ~Float4x4Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Float4x4Varying : public Float4x4
{
    BE_NOCOPY(Float4x4Varying)
published:
    const weak<const Float4x4> node;
published:
    Float4x4Varying(weak<const Float4x4> node);
    ~Float4x4Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Float4x4> operator /(weak<const Float4x4> node1, weak<const Float4x4> node2);
ref<Float4x4> operator +(weak<const Float4x4> node1, weak<const Float4x4> node2);
ref<Float4x4> operator -(weak<const Float4x4> node1, weak<const Float4x4> node2);
ref<Float4x4> operator *(weak<const Float2x4> node1, weak<const Float4x2> node2);
ref<Float4x4> operator *(weak<const Float3x4> node1, weak<const Float4x3> node2);
ref<Float4x4> operator *(weak<const Float4x4> node1, weak<const Float4x4> node2);
ref<Float4x4> operator *(weak<const Float> node1, weak<const Float4x4> node2);
ref<Float4x4> operator *(weak<const Float4x4> node1, weak<const Float> node2);
ref<Float4x4> operator /(weak<const Float> node1, weak<const Float4x4> node2);
ref<Float4x4> operator /(weak<const Float4x4> node1, weak<const Float> node2);
ref<Float4x4> operator +(weak<const Float> node1, weak<const Float4x4> node2);
ref<Float4x4> operator +(weak<const Float4x4> node1, weak<const Float> node2);
ref<Float4x4> operator -(weak<const Float> node1, weak<const Float4x4> node2);
ref<Float4x4> operator -(weak<const Float4x4> node1, weak<const Float> node2);




}}

/*****************************************************************************/
#endif
