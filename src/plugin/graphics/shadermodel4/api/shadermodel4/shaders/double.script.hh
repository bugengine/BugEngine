/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#ifndef BE_OBJECTS_SHADERS_DOUBLE_SCRIPT_HH_
#define BE_OBJECTS_SHADERS_DOUBLE_SCRIPT_HH_
/*****************************************************************************/
#include    <3d/shader/node.script.hh>

namespace BugEngine { namespace Double
{

class Double : public Shaders::Node
{
    BE_NOCOPY(Double)
protected:
    inline Double();
    inline ~Double();
};

class DoubleUniform : public Double
{
    BE_NOCOPY(DoubleUniform)
published:
    const istring name;
published:
    DoubleUniform(const istring& name);
    ~DoubleUniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class DoubleAttribute : public Double
{
    BE_NOCOPY(DoubleAttribute)
published:
    DoubleAttribute();
    ~DoubleAttribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class DoubleVarying : public Double
{
    BE_NOCOPY(DoubleVarying)
published:
    const weak<const Double> node;
published:
    DoubleVarying(weak<const Double> node);
    ~DoubleVarying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Double> operator /(weak<const Double> node1, weak<const Double> node2);
ref<Double> operator +(weak<const Double> node1, weak<const Double> node2);
ref<Double> operator -(weak<const Double> node1, weak<const Double> node2);



class Double2;
class Double3;
class Double4;
class Double2x2;
class Double3x2;
class Double4x2;
class Double2x3;
class Double3x3;
class Double4x3;
class Double2x4;
class Double3x4;
class Double4x4;
class Double2 : public Shaders::Node
{
    BE_NOCOPY(Double2)
protected:
    inline Double2();
    inline ~Double2();
};

class Double2Uniform : public Double2
{
    BE_NOCOPY(Double2Uniform)
published:
    const istring name;
published:
    Double2Uniform(const istring& name);
    ~Double2Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double2Attribute : public Double2
{
    BE_NOCOPY(Double2Attribute)
published:
    Double2Attribute();
    ~Double2Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double2Varying : public Double2
{
    BE_NOCOPY(Double2Varying)
published:
    const weak<const Double2> node;
published:
    Double2Varying(weak<const Double2> node);
    ~Double2Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Double2> operator /(weak<const Double2> node1, weak<const Double2> node2);
ref<Double2> operator +(weak<const Double2> node1, weak<const Double2> node2);
ref<Double2> operator -(weak<const Double2> node1, weak<const Double2> node2);
ref<Double2> operator *(weak<const Double2> node1, weak<const Double2x2> node2);
ref<Double2> operator *(weak<const Double2x2> node1, weak<const Double2> node2);
ref<Double2> operator *(weak<const Double> node1, weak<const Double2> node2);
ref<Double2> operator *(weak<const Double2> node1, weak<const Double> node2);
ref<Double2> operator /(weak<const Double> node1, weak<const Double2> node2);
ref<Double2> operator /(weak<const Double2> node1, weak<const Double> node2);
ref<Double2> operator +(weak<const Double> node1, weak<const Double2> node2);
ref<Double2> operator +(weak<const Double2> node1, weak<const Double> node2);
ref<Double2> operator -(weak<const Double> node1, weak<const Double2> node2);
ref<Double2> operator -(weak<const Double2> node1, weak<const Double> node2);



class Double3 : public Shaders::Node
{
    BE_NOCOPY(Double3)
protected:
    inline Double3();
    inline ~Double3();
};

class Double3Uniform : public Double3
{
    BE_NOCOPY(Double3Uniform)
published:
    const istring name;
published:
    Double3Uniform(const istring& name);
    ~Double3Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double3Attribute : public Double3
{
    BE_NOCOPY(Double3Attribute)
published:
    Double3Attribute();
    ~Double3Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double3Varying : public Double3
{
    BE_NOCOPY(Double3Varying)
published:
    const weak<const Double3> node;
published:
    Double3Varying(weak<const Double3> node);
    ~Double3Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Double3> operator /(weak<const Double3> node1, weak<const Double3> node2);
ref<Double3> operator +(weak<const Double3> node1, weak<const Double3> node2);
ref<Double3> operator -(weak<const Double3> node1, weak<const Double3> node2);
ref<Double3> operator *(weak<const Double3> node1, weak<const Double3x3> node2);
ref<Double3> operator *(weak<const Double3x3> node1, weak<const Double3> node2);
ref<Double3> operator *(weak<const Double> node1, weak<const Double3> node2);
ref<Double3> operator *(weak<const Double3> node1, weak<const Double> node2);
ref<Double3> operator /(weak<const Double> node1, weak<const Double3> node2);
ref<Double3> operator /(weak<const Double3> node1, weak<const Double> node2);
ref<Double3> operator +(weak<const Double> node1, weak<const Double3> node2);
ref<Double3> operator +(weak<const Double3> node1, weak<const Double> node2);
ref<Double3> operator -(weak<const Double> node1, weak<const Double3> node2);
ref<Double3> operator -(weak<const Double3> node1, weak<const Double> node2);



class Double4 : public Shaders::Node
{
    BE_NOCOPY(Double4)
protected:
    inline Double4();
    inline ~Double4();
};

class Double4Uniform : public Double4
{
    BE_NOCOPY(Double4Uniform)
published:
    const istring name;
published:
    Double4Uniform(const istring& name);
    ~Double4Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double4Attribute : public Double4
{
    BE_NOCOPY(Double4Attribute)
published:
    Double4Attribute();
    ~Double4Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double4Varying : public Double4
{
    BE_NOCOPY(Double4Varying)
published:
    const weak<const Double4> node;
published:
    Double4Varying(weak<const Double4> node);
    ~Double4Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Double4> operator /(weak<const Double4> node1, weak<const Double4> node2);
ref<Double4> operator +(weak<const Double4> node1, weak<const Double4> node2);
ref<Double4> operator -(weak<const Double4> node1, weak<const Double4> node2);
ref<Double4> operator *(weak<const Double4> node1, weak<const Double4x4> node2);
ref<Double4> operator *(weak<const Double4x4> node1, weak<const Double4> node2);
ref<Double4> operator *(weak<const Double> node1, weak<const Double4> node2);
ref<Double4> operator *(weak<const Double4> node1, weak<const Double> node2);
ref<Double4> operator /(weak<const Double> node1, weak<const Double4> node2);
ref<Double4> operator /(weak<const Double4> node1, weak<const Double> node2);
ref<Double4> operator +(weak<const Double> node1, weak<const Double4> node2);
ref<Double4> operator +(weak<const Double4> node1, weak<const Double> node2);
ref<Double4> operator -(weak<const Double> node1, weak<const Double4> node2);
ref<Double4> operator -(weak<const Double4> node1, weak<const Double> node2);



class Double2x2 : public Shaders::Node
{
    BE_NOCOPY(Double2x2)
protected:
    inline Double2x2();
    inline ~Double2x2();
};

class Double2x2Uniform : public Double2x2
{
    BE_NOCOPY(Double2x2Uniform)
published:
    const istring name;
published:
    Double2x2Uniform(const istring& name);
    ~Double2x2Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double2x2Attribute : public Double2x2
{
    BE_NOCOPY(Double2x2Attribute)
published:
    Double2x2Attribute();
    ~Double2x2Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double2x2Varying : public Double2x2
{
    BE_NOCOPY(Double2x2Varying)
published:
    const weak<const Double2x2> node;
published:
    Double2x2Varying(weak<const Double2x2> node);
    ~Double2x2Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Double2x2> operator /(weak<const Double2x2> node1, weak<const Double2x2> node2);
ref<Double2x2> operator +(weak<const Double2x2> node1, weak<const Double2x2> node2);
ref<Double2x2> operator -(weak<const Double2x2> node1, weak<const Double2x2> node2);
ref<Double2x2> operator *(weak<const Double2x2> node1, weak<const Double2x2> node2);
ref<Double2x2> operator *(weak<const Double3x2> node1, weak<const Double2x3> node2);
ref<Double2x2> operator *(weak<const Double4x2> node1, weak<const Double2x4> node2);
ref<Double2x2> operator *(weak<const Double> node1, weak<const Double2x2> node2);
ref<Double2x2> operator *(weak<const Double2x2> node1, weak<const Double> node2);
ref<Double2x2> operator /(weak<const Double> node1, weak<const Double2x2> node2);
ref<Double2x2> operator /(weak<const Double2x2> node1, weak<const Double> node2);
ref<Double2x2> operator +(weak<const Double> node1, weak<const Double2x2> node2);
ref<Double2x2> operator +(weak<const Double2x2> node1, weak<const Double> node2);
ref<Double2x2> operator -(weak<const Double> node1, weak<const Double2x2> node2);
ref<Double2x2> operator -(weak<const Double2x2> node1, weak<const Double> node2);



class Double3x2 : public Shaders::Node
{
    BE_NOCOPY(Double3x2)
protected:
    inline Double3x2();
    inline ~Double3x2();
};

class Double3x2Uniform : public Double3x2
{
    BE_NOCOPY(Double3x2Uniform)
published:
    const istring name;
published:
    Double3x2Uniform(const istring& name);
    ~Double3x2Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double3x2Attribute : public Double3x2
{
    BE_NOCOPY(Double3x2Attribute)
published:
    Double3x2Attribute();
    ~Double3x2Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double3x2Varying : public Double3x2
{
    BE_NOCOPY(Double3x2Varying)
published:
    const weak<const Double3x2> node;
published:
    Double3x2Varying(weak<const Double3x2> node);
    ~Double3x2Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Double3x2> operator /(weak<const Double3x2> node1, weak<const Double3x2> node2);
ref<Double3x2> operator +(weak<const Double3x2> node1, weak<const Double3x2> node2);
ref<Double3x2> operator -(weak<const Double3x2> node1, weak<const Double3x2> node2);
ref<Double3x2> operator *(weak<const Double2x2> node1, weak<const Double3x2> node2);
ref<Double3x2> operator *(weak<const Double3x2> node1, weak<const Double3x3> node2);
ref<Double3x2> operator *(weak<const Double4x2> node1, weak<const Double3x4> node2);
ref<Double3x2> operator *(weak<const Double> node1, weak<const Double3x2> node2);
ref<Double3x2> operator *(weak<const Double3x2> node1, weak<const Double> node2);
ref<Double3x2> operator /(weak<const Double> node1, weak<const Double3x2> node2);
ref<Double3x2> operator /(weak<const Double3x2> node1, weak<const Double> node2);
ref<Double3x2> operator +(weak<const Double> node1, weak<const Double3x2> node2);
ref<Double3x2> operator +(weak<const Double3x2> node1, weak<const Double> node2);
ref<Double3x2> operator -(weak<const Double> node1, weak<const Double3x2> node2);
ref<Double3x2> operator -(weak<const Double3x2> node1, weak<const Double> node2);



class Double4x2 : public Shaders::Node
{
    BE_NOCOPY(Double4x2)
protected:
    inline Double4x2();
    inline ~Double4x2();
};

class Double4x2Uniform : public Double4x2
{
    BE_NOCOPY(Double4x2Uniform)
published:
    const istring name;
published:
    Double4x2Uniform(const istring& name);
    ~Double4x2Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double4x2Attribute : public Double4x2
{
    BE_NOCOPY(Double4x2Attribute)
published:
    Double4x2Attribute();
    ~Double4x2Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double4x2Varying : public Double4x2
{
    BE_NOCOPY(Double4x2Varying)
published:
    const weak<const Double4x2> node;
published:
    Double4x2Varying(weak<const Double4x2> node);
    ~Double4x2Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Double4x2> operator /(weak<const Double4x2> node1, weak<const Double4x2> node2);
ref<Double4x2> operator +(weak<const Double4x2> node1, weak<const Double4x2> node2);
ref<Double4x2> operator -(weak<const Double4x2> node1, weak<const Double4x2> node2);
ref<Double4x2> operator *(weak<const Double2x2> node1, weak<const Double4x2> node2);
ref<Double4x2> operator *(weak<const Double3x2> node1, weak<const Double4x3> node2);
ref<Double4x2> operator *(weak<const Double4x2> node1, weak<const Double4x4> node2);
ref<Double4x2> operator *(weak<const Double> node1, weak<const Double4x2> node2);
ref<Double4x2> operator *(weak<const Double4x2> node1, weak<const Double> node2);
ref<Double4x2> operator /(weak<const Double> node1, weak<const Double4x2> node2);
ref<Double4x2> operator /(weak<const Double4x2> node1, weak<const Double> node2);
ref<Double4x2> operator +(weak<const Double> node1, weak<const Double4x2> node2);
ref<Double4x2> operator +(weak<const Double4x2> node1, weak<const Double> node2);
ref<Double4x2> operator -(weak<const Double> node1, weak<const Double4x2> node2);
ref<Double4x2> operator -(weak<const Double4x2> node1, weak<const Double> node2);



class Double2x3 : public Shaders::Node
{
    BE_NOCOPY(Double2x3)
protected:
    inline Double2x3();
    inline ~Double2x3();
};

class Double2x3Uniform : public Double2x3
{
    BE_NOCOPY(Double2x3Uniform)
published:
    const istring name;
published:
    Double2x3Uniform(const istring& name);
    ~Double2x3Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double2x3Attribute : public Double2x3
{
    BE_NOCOPY(Double2x3Attribute)
published:
    Double2x3Attribute();
    ~Double2x3Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double2x3Varying : public Double2x3
{
    BE_NOCOPY(Double2x3Varying)
published:
    const weak<const Double2x3> node;
published:
    Double2x3Varying(weak<const Double2x3> node);
    ~Double2x3Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Double2x3> operator /(weak<const Double2x3> node1, weak<const Double2x3> node2);
ref<Double2x3> operator +(weak<const Double2x3> node1, weak<const Double2x3> node2);
ref<Double2x3> operator -(weak<const Double2x3> node1, weak<const Double2x3> node2);
ref<Double2x3> operator *(weak<const Double2x3> node1, weak<const Double2x2> node2);
ref<Double2x3> operator *(weak<const Double3x3> node1, weak<const Double2x3> node2);
ref<Double2x3> operator *(weak<const Double4x3> node1, weak<const Double2x4> node2);
ref<Double2x3> operator *(weak<const Double> node1, weak<const Double2x3> node2);
ref<Double2x3> operator *(weak<const Double2x3> node1, weak<const Double> node2);
ref<Double2x3> operator /(weak<const Double> node1, weak<const Double2x3> node2);
ref<Double2x3> operator /(weak<const Double2x3> node1, weak<const Double> node2);
ref<Double2x3> operator +(weak<const Double> node1, weak<const Double2x3> node2);
ref<Double2x3> operator +(weak<const Double2x3> node1, weak<const Double> node2);
ref<Double2x3> operator -(weak<const Double> node1, weak<const Double2x3> node2);
ref<Double2x3> operator -(weak<const Double2x3> node1, weak<const Double> node2);



class Double3x3 : public Shaders::Node
{
    BE_NOCOPY(Double3x3)
protected:
    inline Double3x3();
    inline ~Double3x3();
};

class Double3x3Uniform : public Double3x3
{
    BE_NOCOPY(Double3x3Uniform)
published:
    const istring name;
published:
    Double3x3Uniform(const istring& name);
    ~Double3x3Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double3x3Attribute : public Double3x3
{
    BE_NOCOPY(Double3x3Attribute)
published:
    Double3x3Attribute();
    ~Double3x3Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double3x3Varying : public Double3x3
{
    BE_NOCOPY(Double3x3Varying)
published:
    const weak<const Double3x3> node;
published:
    Double3x3Varying(weak<const Double3x3> node);
    ~Double3x3Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Double3x3> operator /(weak<const Double3x3> node1, weak<const Double3x3> node2);
ref<Double3x3> operator +(weak<const Double3x3> node1, weak<const Double3x3> node2);
ref<Double3x3> operator -(weak<const Double3x3> node1, weak<const Double3x3> node2);
ref<Double3x3> operator *(weak<const Double2x3> node1, weak<const Double3x2> node2);
ref<Double3x3> operator *(weak<const Double3x3> node1, weak<const Double3x3> node2);
ref<Double3x3> operator *(weak<const Double4x3> node1, weak<const Double3x4> node2);
ref<Double3x3> operator *(weak<const Double> node1, weak<const Double3x3> node2);
ref<Double3x3> operator *(weak<const Double3x3> node1, weak<const Double> node2);
ref<Double3x3> operator /(weak<const Double> node1, weak<const Double3x3> node2);
ref<Double3x3> operator /(weak<const Double3x3> node1, weak<const Double> node2);
ref<Double3x3> operator +(weak<const Double> node1, weak<const Double3x3> node2);
ref<Double3x3> operator +(weak<const Double3x3> node1, weak<const Double> node2);
ref<Double3x3> operator -(weak<const Double> node1, weak<const Double3x3> node2);
ref<Double3x3> operator -(weak<const Double3x3> node1, weak<const Double> node2);



class Double4x3 : public Shaders::Node
{
    BE_NOCOPY(Double4x3)
protected:
    inline Double4x3();
    inline ~Double4x3();
};

class Double4x3Uniform : public Double4x3
{
    BE_NOCOPY(Double4x3Uniform)
published:
    const istring name;
published:
    Double4x3Uniform(const istring& name);
    ~Double4x3Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double4x3Attribute : public Double4x3
{
    BE_NOCOPY(Double4x3Attribute)
published:
    Double4x3Attribute();
    ~Double4x3Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double4x3Varying : public Double4x3
{
    BE_NOCOPY(Double4x3Varying)
published:
    const weak<const Double4x3> node;
published:
    Double4x3Varying(weak<const Double4x3> node);
    ~Double4x3Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Double4x3> operator /(weak<const Double4x3> node1, weak<const Double4x3> node2);
ref<Double4x3> operator +(weak<const Double4x3> node1, weak<const Double4x3> node2);
ref<Double4x3> operator -(weak<const Double4x3> node1, weak<const Double4x3> node2);
ref<Double4x3> operator *(weak<const Double2x3> node1, weak<const Double4x2> node2);
ref<Double4x3> operator *(weak<const Double3x3> node1, weak<const Double4x3> node2);
ref<Double4x3> operator *(weak<const Double4x3> node1, weak<const Double4x4> node2);
ref<Double4x3> operator *(weak<const Double> node1, weak<const Double4x3> node2);
ref<Double4x3> operator *(weak<const Double4x3> node1, weak<const Double> node2);
ref<Double4x3> operator /(weak<const Double> node1, weak<const Double4x3> node2);
ref<Double4x3> operator /(weak<const Double4x3> node1, weak<const Double> node2);
ref<Double4x3> operator +(weak<const Double> node1, weak<const Double4x3> node2);
ref<Double4x3> operator +(weak<const Double4x3> node1, weak<const Double> node2);
ref<Double4x3> operator -(weak<const Double> node1, weak<const Double4x3> node2);
ref<Double4x3> operator -(weak<const Double4x3> node1, weak<const Double> node2);



class Double2x4 : public Shaders::Node
{
    BE_NOCOPY(Double2x4)
protected:
    inline Double2x4();
    inline ~Double2x4();
};

class Double2x4Uniform : public Double2x4
{
    BE_NOCOPY(Double2x4Uniform)
published:
    const istring name;
published:
    Double2x4Uniform(const istring& name);
    ~Double2x4Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double2x4Attribute : public Double2x4
{
    BE_NOCOPY(Double2x4Attribute)
published:
    Double2x4Attribute();
    ~Double2x4Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double2x4Varying : public Double2x4
{
    BE_NOCOPY(Double2x4Varying)
published:
    const weak<const Double2x4> node;
published:
    Double2x4Varying(weak<const Double2x4> node);
    ~Double2x4Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Double2x4> operator /(weak<const Double2x4> node1, weak<const Double2x4> node2);
ref<Double2x4> operator +(weak<const Double2x4> node1, weak<const Double2x4> node2);
ref<Double2x4> operator -(weak<const Double2x4> node1, weak<const Double2x4> node2);
ref<Double2x4> operator *(weak<const Double2x4> node1, weak<const Double2x2> node2);
ref<Double2x4> operator *(weak<const Double3x4> node1, weak<const Double2x3> node2);
ref<Double2x4> operator *(weak<const Double4x4> node1, weak<const Double2x4> node2);
ref<Double2x4> operator *(weak<const Double> node1, weak<const Double2x4> node2);
ref<Double2x4> operator *(weak<const Double2x4> node1, weak<const Double> node2);
ref<Double2x4> operator /(weak<const Double> node1, weak<const Double2x4> node2);
ref<Double2x4> operator /(weak<const Double2x4> node1, weak<const Double> node2);
ref<Double2x4> operator +(weak<const Double> node1, weak<const Double2x4> node2);
ref<Double2x4> operator +(weak<const Double2x4> node1, weak<const Double> node2);
ref<Double2x4> operator -(weak<const Double> node1, weak<const Double2x4> node2);
ref<Double2x4> operator -(weak<const Double2x4> node1, weak<const Double> node2);



class Double3x4 : public Shaders::Node
{
    BE_NOCOPY(Double3x4)
protected:
    inline Double3x4();
    inline ~Double3x4();
};

class Double3x4Uniform : public Double3x4
{
    BE_NOCOPY(Double3x4Uniform)
published:
    const istring name;
published:
    Double3x4Uniform(const istring& name);
    ~Double3x4Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double3x4Attribute : public Double3x4
{
    BE_NOCOPY(Double3x4Attribute)
published:
    Double3x4Attribute();
    ~Double3x4Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double3x4Varying : public Double3x4
{
    BE_NOCOPY(Double3x4Varying)
published:
    const weak<const Double3x4> node;
published:
    Double3x4Varying(weak<const Double3x4> node);
    ~Double3x4Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Double3x4> operator /(weak<const Double3x4> node1, weak<const Double3x4> node2);
ref<Double3x4> operator +(weak<const Double3x4> node1, weak<const Double3x4> node2);
ref<Double3x4> operator -(weak<const Double3x4> node1, weak<const Double3x4> node2);
ref<Double3x4> operator *(weak<const Double2x4> node1, weak<const Double3x2> node2);
ref<Double3x4> operator *(weak<const Double3x4> node1, weak<const Double3x3> node2);
ref<Double3x4> operator *(weak<const Double4x4> node1, weak<const Double3x4> node2);
ref<Double3x4> operator *(weak<const Double> node1, weak<const Double3x4> node2);
ref<Double3x4> operator *(weak<const Double3x4> node1, weak<const Double> node2);
ref<Double3x4> operator /(weak<const Double> node1, weak<const Double3x4> node2);
ref<Double3x4> operator /(weak<const Double3x4> node1, weak<const Double> node2);
ref<Double3x4> operator +(weak<const Double> node1, weak<const Double3x4> node2);
ref<Double3x4> operator +(weak<const Double3x4> node1, weak<const Double> node2);
ref<Double3x4> operator -(weak<const Double> node1, weak<const Double3x4> node2);
ref<Double3x4> operator -(weak<const Double3x4> node1, weak<const Double> node2);



class Double4x4 : public Shaders::Node
{
    BE_NOCOPY(Double4x4)
protected:
    inline Double4x4();
    inline ~Double4x4();
};

class Double4x4Uniform : public Double4x4
{
    BE_NOCOPY(Double4x4Uniform)
published:
    const istring name;
published:
    Double4x4Uniform(const istring& name);
    ~Double4x4Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double4x4Attribute : public Double4x4
{
    BE_NOCOPY(Double4x4Attribute)
published:
    Double4x4Attribute();
    ~Double4x4Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Double4x4Varying : public Double4x4
{
    BE_NOCOPY(Double4x4Varying)
published:
    const weak<const Double4x4> node;
published:
    Double4x4Varying(weak<const Double4x4> node);
    ~Double4x4Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Double4x4> operator /(weak<const Double4x4> node1, weak<const Double4x4> node2);
ref<Double4x4> operator +(weak<const Double4x4> node1, weak<const Double4x4> node2);
ref<Double4x4> operator -(weak<const Double4x4> node1, weak<const Double4x4> node2);
ref<Double4x4> operator *(weak<const Double2x4> node1, weak<const Double4x2> node2);
ref<Double4x4> operator *(weak<const Double3x4> node1, weak<const Double4x3> node2);
ref<Double4x4> operator *(weak<const Double4x4> node1, weak<const Double4x4> node2);
ref<Double4x4> operator *(weak<const Double> node1, weak<const Double4x4> node2);
ref<Double4x4> operator *(weak<const Double4x4> node1, weak<const Double> node2);
ref<Double4x4> operator /(weak<const Double> node1, weak<const Double4x4> node2);
ref<Double4x4> operator /(weak<const Double4x4> node1, weak<const Double> node2);
ref<Double4x4> operator +(weak<const Double> node1, weak<const Double4x4> node2);
ref<Double4x4> operator +(weak<const Double4x4> node1, weak<const Double> node2);
ref<Double4x4> operator -(weak<const Double> node1, weak<const Double4x4> node2);
ref<Double4x4> operator -(weak<const Double4x4> node1, weak<const Double> node2);




}}

/*****************************************************************************/
#endif
