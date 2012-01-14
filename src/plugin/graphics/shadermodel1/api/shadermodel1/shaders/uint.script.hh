/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#ifndef BE_OBJECTS_SHADERS_UINT_SCRIPT_HH_
#define BE_OBJECTS_SHADERS_UINT_SCRIPT_HH_
/*****************************************************************************/
#include    <3d/shader/node.script.hh>

namespace BugEngine { namespace Shaders
{

class Uint : public Node
{
    BE_NOCOPY(Uint)
protected:
    inline Uint();
    inline ~Uint();
};

class UintUniform : public Uint
{
    BE_NOCOPY(UintUniform)
published:
    const istring name;
published:
    UintUniform(const istring& name);
    ~UintUniform();
private:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class UintAttribute : public Uint
{
    BE_NOCOPY(UintAttribute)
published:
    UintAttribute();
    ~UintAttribute();
private:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class UintVarying : public Uint
{
    BE_NOCOPY(UintVarying)
published:
    const weak<const Uint> node;
published:
    UintVarying(weak<const Uint> node);
    ~UintVarying();
private:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

ref<Uint> operator /(weak<const Uint> node1, weak<const Uint> node2);
ref<Uint> operator +(weak<const Uint> node1, weak<const Uint> node2);
ref<Uint> operator -(weak<const Uint> node1, weak<const Uint> node2);



class Uint2;
class Uint3;
class Uint4;
class Uint2 : public Node
{
    BE_NOCOPY(Uint2)
protected:
    inline Uint2();
    inline ~Uint2();
};

class Uint2Uniform : public Uint2
{
    BE_NOCOPY(Uint2Uniform)
published:
    const istring name;
published:
    Uint2Uniform(const istring& name);
    ~Uint2Uniform();
private:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Uint2Attribute : public Uint2
{
    BE_NOCOPY(Uint2Attribute)
published:
    Uint2Attribute();
    ~Uint2Attribute();
private:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Uint2Varying : public Uint2
{
    BE_NOCOPY(Uint2Varying)
published:
    const weak<const Uint2> node;
published:
    Uint2Varying(weak<const Uint2> node);
    ~Uint2Varying();
private:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

ref<Uint2> operator /(weak<const Uint2> node1, weak<const Uint2> node2);
ref<Uint2> operator +(weak<const Uint2> node1, weak<const Uint2> node2);
ref<Uint2> operator -(weak<const Uint2> node1, weak<const Uint2> node2);
ref<Uint2> operator *(weak<const Uint> node1, weak<const Uint2> node2);
ref<Uint2> operator *(weak<const Uint2> node1, weak<const Uint> node2);
ref<Uint2> operator /(weak<const Uint> node1, weak<const Uint2> node2);
ref<Uint2> operator /(weak<const Uint2> node1, weak<const Uint> node2);
ref<Uint2> operator +(weak<const Uint> node1, weak<const Uint2> node2);
ref<Uint2> operator +(weak<const Uint2> node1, weak<const Uint> node2);
ref<Uint2> operator -(weak<const Uint> node1, weak<const Uint2> node2);
ref<Uint2> operator -(weak<const Uint2> node1, weak<const Uint> node2);



class Uint3 : public Node
{
    BE_NOCOPY(Uint3)
protected:
    inline Uint3();
    inline ~Uint3();
};

class Uint3Uniform : public Uint3
{
    BE_NOCOPY(Uint3Uniform)
published:
    const istring name;
published:
    Uint3Uniform(const istring& name);
    ~Uint3Uniform();
private:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Uint3Attribute : public Uint3
{
    BE_NOCOPY(Uint3Attribute)
published:
    Uint3Attribute();
    ~Uint3Attribute();
private:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Uint3Varying : public Uint3
{
    BE_NOCOPY(Uint3Varying)
published:
    const weak<const Uint3> node;
published:
    Uint3Varying(weak<const Uint3> node);
    ~Uint3Varying();
private:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

ref<Uint3> operator /(weak<const Uint3> node1, weak<const Uint3> node2);
ref<Uint3> operator +(weak<const Uint3> node1, weak<const Uint3> node2);
ref<Uint3> operator -(weak<const Uint3> node1, weak<const Uint3> node2);
ref<Uint3> operator *(weak<const Uint> node1, weak<const Uint3> node2);
ref<Uint3> operator *(weak<const Uint3> node1, weak<const Uint> node2);
ref<Uint3> operator /(weak<const Uint> node1, weak<const Uint3> node2);
ref<Uint3> operator /(weak<const Uint3> node1, weak<const Uint> node2);
ref<Uint3> operator +(weak<const Uint> node1, weak<const Uint3> node2);
ref<Uint3> operator +(weak<const Uint3> node1, weak<const Uint> node2);
ref<Uint3> operator -(weak<const Uint> node1, weak<const Uint3> node2);
ref<Uint3> operator -(weak<const Uint3> node1, weak<const Uint> node2);



class Uint4 : public Node
{
    BE_NOCOPY(Uint4)
protected:
    inline Uint4();
    inline ~Uint4();
};

class Uint4Uniform : public Uint4
{
    BE_NOCOPY(Uint4Uniform)
published:
    const istring name;
published:
    Uint4Uniform(const istring& name);
    ~Uint4Uniform();
private:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Uint4Attribute : public Uint4
{
    BE_NOCOPY(Uint4Attribute)
published:
    Uint4Attribute();
    ~Uint4Attribute();
private:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Uint4Varying : public Uint4
{
    BE_NOCOPY(Uint4Varying)
published:
    const weak<const Uint4> node;
published:
    Uint4Varying(weak<const Uint4> node);
    ~Uint4Varying();
private:
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

ref<Uint4> operator /(weak<const Uint4> node1, weak<const Uint4> node2);
ref<Uint4> operator +(weak<const Uint4> node1, weak<const Uint4> node2);
ref<Uint4> operator -(weak<const Uint4> node1, weak<const Uint4> node2);
ref<Uint4> operator *(weak<const Uint> node1, weak<const Uint4> node2);
ref<Uint4> operator *(weak<const Uint4> node1, weak<const Uint> node2);
ref<Uint4> operator /(weak<const Uint> node1, weak<const Uint4> node2);
ref<Uint4> operator /(weak<const Uint4> node1, weak<const Uint> node2);
ref<Uint4> operator +(weak<const Uint> node1, weak<const Uint4> node2);
ref<Uint4> operator +(weak<const Uint4> node1, weak<const Uint> node2);
ref<Uint4> operator -(weak<const Uint> node1, weak<const Uint4> node2);
ref<Uint4> operator -(weak<const Uint4> node1, weak<const Uint> node2);




}}
/*****************************************************************************/
#endif
