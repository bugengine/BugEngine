/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#ifndef BE_GRAPHICS_OBJECTS_SHADERS_UINT_SCRIPT_HH_
#define BE_GRAPHICS_OBJECTS_SHADERS_UINT_SCRIPT_HH_
/*****************************************************************************/
#include    <graphics/objects/shaders/node.script.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};

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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};

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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};

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
    virtual void buildDeclarations(IShaderBuilder& stream) const override;
    virtual void buildDefinitions(IShaderBuilder& stream) const override;
};


}}}
/*****************************************************************************/
#endif
