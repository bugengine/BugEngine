/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#ifndef BE_GRAPHICS_OBJECTS_SHADERS_INT_SCRIPT_HH_
#define BE_GRAPHICS_OBJECTS_SHADERS_INT_SCRIPT_HH_
/*****************************************************************************/
#include    <graphics/objects/shaders/node.script.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

class Int : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Int2 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Int3 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Int4 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};


}}}
/*****************************************************************************/
#endif
