/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#ifndef BE_GRAPHICS_OBJECTS_SHADERS_FLOAT_SCRIPT_HH_
#define BE_GRAPHICS_OBJECTS_SHADERS_FLOAT_SCRIPT_HH_
/*****************************************************************************/
#include    <graphics/objects/shaders/node.script.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

class Float : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};

class Float2 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};

class Float3 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};

class Float4 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};

class Float2x2 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};

class Float3x2 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};

class Float4x2 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};

class Float2x3 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};

class Float3x3 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};

class Float4x3 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};

class Float2x4 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};

class Float3x4 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};

class Float4x4 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};


}}}
/*****************************************************************************/
#endif
