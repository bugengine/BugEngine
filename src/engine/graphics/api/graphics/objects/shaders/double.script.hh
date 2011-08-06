/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#ifndef BE_GRAPHICS_OBJECTS_SHADERS_DOUBLE_SCRIPT_HH_
#define BE_GRAPHICS_OBJECTS_SHADERS_DOUBLE_SCRIPT_HH_
/*****************************************************************************/
#include    <graphics/objects/shaders/node.script.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{

class Double : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Double2 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Double3 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Double4 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Double2x2 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Double3x2 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Double4x2 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Double2x3 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Double3x3 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Double4x3 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Double2x4 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Double3x4 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};

class Double4x4 : public Node
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
    virtual void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
    virtual void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override;
};


}}}
/*****************************************************************************/
#endif
