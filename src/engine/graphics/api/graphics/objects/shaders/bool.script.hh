/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#ifndef BE_GRAPHICS_OBJECTS_SHADERS_BOOL_SCRIPT_HH_
#define BE_GRAPHICS_OBJECTS_SHADERS_BOOL_SCRIPT_HH_
/*****************************************************************************/
#include    <graphics/objects/shaders/node.script.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

class Bool : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Bool2 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Bool3 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Bool4 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};


}}}
/*****************************************************************************/
#endif
