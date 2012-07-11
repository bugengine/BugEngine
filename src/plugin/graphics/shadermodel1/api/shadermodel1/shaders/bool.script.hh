/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#ifndef BE_OBJECTS_SHADERS_BOOL_SCRIPT_HH_
#define BE_OBJECTS_SHADERS_BOOL_SCRIPT_HH_
/*****************************************************************************/
#include    <3d/shader/node.script.hh>

namespace BugEngine { namespace Bool
{

class Bool : public Shaders::Node
{
    BE_NOCOPY(Bool)
protected:
    inline Bool();
    inline ~Bool();
};

class BoolUniform : public Bool
{
    BE_NOCOPY(BoolUniform)
published:
    const istring name;
published:
    BoolUniform(const istring& name);
    ~BoolUniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class BoolAttribute : public Bool
{
    BE_NOCOPY(BoolAttribute)
published:
    BoolAttribute();
    ~BoolAttribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class BoolVarying : public Bool
{
    BE_NOCOPY(BoolVarying)
published:
    const weak<const Bool> node;
published:
    BoolVarying(weak<const Bool> node);
    ~BoolVarying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Bool> operator /(weak<const Bool> node1, weak<const Bool> node2);
ref<Bool> operator +(weak<const Bool> node1, weak<const Bool> node2);
ref<Bool> operator -(weak<const Bool> node1, weak<const Bool> node2);



class Bool2;
class Bool3;
class Bool4;
class Bool2 : public Shaders::Node
{
    BE_NOCOPY(Bool2)
protected:
    inline Bool2();
    inline ~Bool2();
};

class Bool2Uniform : public Bool2
{
    BE_NOCOPY(Bool2Uniform)
published:
    const istring name;
published:
    Bool2Uniform(const istring& name);
    ~Bool2Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Bool2Attribute : public Bool2
{
    BE_NOCOPY(Bool2Attribute)
published:
    Bool2Attribute();
    ~Bool2Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Bool2Varying : public Bool2
{
    BE_NOCOPY(Bool2Varying)
published:
    const weak<const Bool2> node;
published:
    Bool2Varying(weak<const Bool2> node);
    ~Bool2Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Bool2> operator /(weak<const Bool2> node1, weak<const Bool2> node2);
ref<Bool2> operator +(weak<const Bool2> node1, weak<const Bool2> node2);
ref<Bool2> operator -(weak<const Bool2> node1, weak<const Bool2> node2);
ref<Bool2> operator *(weak<const Bool> node1, weak<const Bool2> node2);
ref<Bool2> operator *(weak<const Bool2> node1, weak<const Bool> node2);
ref<Bool2> operator /(weak<const Bool> node1, weak<const Bool2> node2);
ref<Bool2> operator /(weak<const Bool2> node1, weak<const Bool> node2);
ref<Bool2> operator +(weak<const Bool> node1, weak<const Bool2> node2);
ref<Bool2> operator +(weak<const Bool2> node1, weak<const Bool> node2);
ref<Bool2> operator -(weak<const Bool> node1, weak<const Bool2> node2);
ref<Bool2> operator -(weak<const Bool2> node1, weak<const Bool> node2);



class Bool3 : public Shaders::Node
{
    BE_NOCOPY(Bool3)
protected:
    inline Bool3();
    inline ~Bool3();
};

class Bool3Uniform : public Bool3
{
    BE_NOCOPY(Bool3Uniform)
published:
    const istring name;
published:
    Bool3Uniform(const istring& name);
    ~Bool3Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Bool3Attribute : public Bool3
{
    BE_NOCOPY(Bool3Attribute)
published:
    Bool3Attribute();
    ~Bool3Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Bool3Varying : public Bool3
{
    BE_NOCOPY(Bool3Varying)
published:
    const weak<const Bool3> node;
published:
    Bool3Varying(weak<const Bool3> node);
    ~Bool3Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Bool3> operator /(weak<const Bool3> node1, weak<const Bool3> node2);
ref<Bool3> operator +(weak<const Bool3> node1, weak<const Bool3> node2);
ref<Bool3> operator -(weak<const Bool3> node1, weak<const Bool3> node2);
ref<Bool3> operator *(weak<const Bool> node1, weak<const Bool3> node2);
ref<Bool3> operator *(weak<const Bool3> node1, weak<const Bool> node2);
ref<Bool3> operator /(weak<const Bool> node1, weak<const Bool3> node2);
ref<Bool3> operator /(weak<const Bool3> node1, weak<const Bool> node2);
ref<Bool3> operator +(weak<const Bool> node1, weak<const Bool3> node2);
ref<Bool3> operator +(weak<const Bool3> node1, weak<const Bool> node2);
ref<Bool3> operator -(weak<const Bool> node1, weak<const Bool3> node2);
ref<Bool3> operator -(weak<const Bool3> node1, weak<const Bool> node2);



class Bool4 : public Shaders::Node
{
    BE_NOCOPY(Bool4)
protected:
    inline Bool4();
    inline ~Bool4();
};

class Bool4Uniform : public Bool4
{
    BE_NOCOPY(Bool4Uniform)
published:
    const istring name;
published:
    Bool4Uniform(const istring& name);
    ~Bool4Uniform();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Bool4Attribute : public Bool4
{
    BE_NOCOPY(Bool4Attribute)
published:
    Bool4Attribute();
    ~Bool4Attribute();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

class Bool4Varying : public Bool4
{
    BE_NOCOPY(Bool4Varying)
published:
    const weak<const Bool4> node;
published:
    Bool4Varying(weak<const Bool4> node);
    ~Bool4Varying();
private:
    virtual void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
    virtual void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override;
};

ref<Bool4> operator /(weak<const Bool4> node1, weak<const Bool4> node2);
ref<Bool4> operator +(weak<const Bool4> node1, weak<const Bool4> node2);
ref<Bool4> operator -(weak<const Bool4> node1, weak<const Bool4> node2);
ref<Bool4> operator *(weak<const Bool> node1, weak<const Bool4> node2);
ref<Bool4> operator *(weak<const Bool4> node1, weak<const Bool> node2);
ref<Bool4> operator /(weak<const Bool> node1, weak<const Bool4> node2);
ref<Bool4> operator /(weak<const Bool4> node1, weak<const Bool> node2);
ref<Bool4> operator +(weak<const Bool> node1, weak<const Bool4> node2);
ref<Bool4> operator +(weak<const Bool4> node1, weak<const Bool> node2);
ref<Bool4> operator -(weak<const Bool> node1, weak<const Bool4> node2);
ref<Bool4> operator -(weak<const Bool4> node1, weak<const Bool> node2);




}}

/*****************************************************************************/
#endif
