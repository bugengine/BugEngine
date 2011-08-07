/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shaders/double.script.hh>
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{
/* Type *************************************************************/
Double::Double()
{
}
Double::~Double()
{
}

/* Uniform **********************************************************/
DoubleUniform::DoubleUniform(const istring& name)
    :   name(name)
{
}
DoubleUniform::~DoubleUniform()
{
}
void DoubleUniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double);
}
void DoubleUniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
DoubleAttribute::DoubleAttribute()
{
}
DoubleAttribute::~DoubleAttribute()
{
}
void DoubleAttribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double);
}
void DoubleAttribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
DoubleVarying::DoubleVarying(weak<const Double> node)
    :   node(node)
{
}
DoubleVarying::~DoubleVarying()
{
}
void DoubleVarying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void DoubleVarying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Doublemul : public Double
{
private:
    weak<const Double> node1;
    weak<const Double> node2;
public:
    Doublemul(weak<const Double> node1, weak<const Double> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Double, node1, node2);
        }
    }
};
ref<Double> operator *(weak<const Double> node1, weak<const Double> node2)
{
    return ref<Doublemul>::create(gameArena(), node1, node2);
}

class Doublediv : public Double
{
private:
    weak<const Double> node1;
    weak<const Double> node2;
public:
    Doublediv(weak<const Double> node1, weak<const Double> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Double, node1, node2);
        }
    }
};
ref<Double> operator /(weak<const Double> node1, weak<const Double> node2)
{
    return ref<Doublediv>::create(gameArena(), node1, node2);
}

class Doubleadd : public Double
{
private:
    weak<const Double> node1;
    weak<const Double> node2;
public:
    Doubleadd(weak<const Double> node1, weak<const Double> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Double, node1, node2);
        }
    }
};
ref<Double> operator +(weak<const Double> node1, weak<const Double> node2)
{
    return ref<Doubleadd>::create(gameArena(), node1, node2);
}

class Doublesub : public Double
{
private:
    weak<const Double> node1;
    weak<const Double> node2;
public:
    Doublesub(weak<const Double> node1, weak<const Double> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Double, node1, node2);
        }
    }
};
ref<Double> operator -(weak<const Double> node1, weak<const Double> node2)
{
    return ref<Doublesub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Double2::Double2()
{
}
Double2::~Double2()
{
}

/* Uniform **********************************************************/
Double2Uniform::Double2Uniform(const istring& name)
    :   name(name)
{
}
Double2Uniform::~Double2Uniform()
{
}
void Double2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double2);
}
void Double2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double2Attribute::Double2Attribute()
{
}
Double2Attribute::~Double2Attribute()
{
}
void Double2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double2);
}
void Double2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double2Varying::Double2Varying(weak<const Double2> node)
    :   node(node)
{
}
Double2Varying::~Double2Varying()
{
}
void Double2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Double2mul : public Double2
{
private:
    weak<const Double2> node1;
    weak<const Double2> node2;
public:
    Double2mul(weak<const Double2> node1, weak<const Double2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Double2, node1, node2);
        }
    }
};
ref<Double2> operator *(weak<const Double2> node1, weak<const Double2> node2)
{
    return ref<Double2mul>::create(gameArena(), node1, node2);
}

class Double2div : public Double2
{
private:
    weak<const Double2> node1;
    weak<const Double2> node2;
public:
    Double2div(weak<const Double2> node1, weak<const Double2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Double2, node1, node2);
        }
    }
};
ref<Double2> operator /(weak<const Double2> node1, weak<const Double2> node2)
{
    return ref<Double2div>::create(gameArena(), node1, node2);
}

class Double2add : public Double2
{
private:
    weak<const Double2> node1;
    weak<const Double2> node2;
public:
    Double2add(weak<const Double2> node1, weak<const Double2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Double2, node1, node2);
        }
    }
};
ref<Double2> operator +(weak<const Double2> node1, weak<const Double2> node2)
{
    return ref<Double2add>::create(gameArena(), node1, node2);
}

class Double2sub : public Double2
{
private:
    weak<const Double2> node1;
    weak<const Double2> node2;
public:
    Double2sub(weak<const Double2> node1, weak<const Double2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Double2, node1, node2);
        }
    }
};
ref<Double2> operator -(weak<const Double2> node1, weak<const Double2> node2)
{
    return ref<Double2sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Double3::Double3()
{
}
Double3::~Double3()
{
}

/* Uniform **********************************************************/
Double3Uniform::Double3Uniform(const istring& name)
    :   name(name)
{
}
Double3Uniform::~Double3Uniform()
{
}
void Double3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double3);
}
void Double3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double3Attribute::Double3Attribute()
{
}
Double3Attribute::~Double3Attribute()
{
}
void Double3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double3);
}
void Double3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double3Varying::Double3Varying(weak<const Double3> node)
    :   node(node)
{
}
Double3Varying::~Double3Varying()
{
}
void Double3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Double3mul : public Double3
{
private:
    weak<const Double3> node1;
    weak<const Double3> node2;
public:
    Double3mul(weak<const Double3> node1, weak<const Double3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Double3, node1, node2);
        }
    }
};
ref<Double3> operator *(weak<const Double3> node1, weak<const Double3> node2)
{
    return ref<Double3mul>::create(gameArena(), node1, node2);
}

class Double3div : public Double3
{
private:
    weak<const Double3> node1;
    weak<const Double3> node2;
public:
    Double3div(weak<const Double3> node1, weak<const Double3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Double3, node1, node2);
        }
    }
};
ref<Double3> operator /(weak<const Double3> node1, weak<const Double3> node2)
{
    return ref<Double3div>::create(gameArena(), node1, node2);
}

class Double3add : public Double3
{
private:
    weak<const Double3> node1;
    weak<const Double3> node2;
public:
    Double3add(weak<const Double3> node1, weak<const Double3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Double3, node1, node2);
        }
    }
};
ref<Double3> operator +(weak<const Double3> node1, weak<const Double3> node2)
{
    return ref<Double3add>::create(gameArena(), node1, node2);
}

class Double3sub : public Double3
{
private:
    weak<const Double3> node1;
    weak<const Double3> node2;
public:
    Double3sub(weak<const Double3> node1, weak<const Double3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Double3, node1, node2);
        }
    }
};
ref<Double3> operator -(weak<const Double3> node1, weak<const Double3> node2)
{
    return ref<Double3sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Double4::Double4()
{
}
Double4::~Double4()
{
}

/* Uniform **********************************************************/
Double4Uniform::Double4Uniform(const istring& name)
    :   name(name)
{
}
Double4Uniform::~Double4Uniform()
{
}
void Double4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double4);
}
void Double4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double4Attribute::Double4Attribute()
{
}
Double4Attribute::~Double4Attribute()
{
}
void Double4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double4);
}
void Double4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double4Varying::Double4Varying(weak<const Double4> node)
    :   node(node)
{
}
Double4Varying::~Double4Varying()
{
}
void Double4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Double4mul : public Double4
{
private:
    weak<const Double4> node1;
    weak<const Double4> node2;
public:
    Double4mul(weak<const Double4> node1, weak<const Double4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Double4, node1, node2);
        }
    }
};
ref<Double4> operator *(weak<const Double4> node1, weak<const Double4> node2)
{
    return ref<Double4mul>::create(gameArena(), node1, node2);
}

class Double4div : public Double4
{
private:
    weak<const Double4> node1;
    weak<const Double4> node2;
public:
    Double4div(weak<const Double4> node1, weak<const Double4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Double4, node1, node2);
        }
    }
};
ref<Double4> operator /(weak<const Double4> node1, weak<const Double4> node2)
{
    return ref<Double4div>::create(gameArena(), node1, node2);
}

class Double4add : public Double4
{
private:
    weak<const Double4> node1;
    weak<const Double4> node2;
public:
    Double4add(weak<const Double4> node1, weak<const Double4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Double4, node1, node2);
        }
    }
};
ref<Double4> operator +(weak<const Double4> node1, weak<const Double4> node2)
{
    return ref<Double4add>::create(gameArena(), node1, node2);
}

class Double4sub : public Double4
{
private:
    weak<const Double4> node1;
    weak<const Double4> node2;
public:
    Double4sub(weak<const Double4> node1, weak<const Double4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Double4, node1, node2);
        }
    }
};
ref<Double4> operator -(weak<const Double4> node1, weak<const Double4> node2)
{
    return ref<Double4sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Double2x2::Double2x2()
{
}
Double2x2::~Double2x2()
{
}

/* Uniform **********************************************************/
Double2x2Uniform::Double2x2Uniform(const istring& name)
    :   name(name)
{
}
Double2x2Uniform::~Double2x2Uniform()
{
}
void Double2x2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double2x2);
}
void Double2x2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double2x2Attribute::Double2x2Attribute()
{
}
Double2x2Attribute::~Double2x2Attribute()
{
}
void Double2x2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double2x2);
}
void Double2x2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double2x2Varying::Double2x2Varying(weak<const Double2x2> node)
    :   node(node)
{
}
Double2x2Varying::~Double2x2Varying()
{
}
void Double2x2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double2x2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double2x2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Double2x2mul : public Double2x2
{
private:
    weak<const Double2x2> node1;
    weak<const Double2x2> node2;
public:
    Double2x2mul(weak<const Double2x2> node1, weak<const Double2x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Double2x2, node1, node2);
        }
    }
};
ref<Double2x2> operator *(weak<const Double2x2> node1, weak<const Double2x2> node2)
{
    return ref<Double2x2mul>::create(gameArena(), node1, node2);
}

class Double2x2div : public Double2x2
{
private:
    weak<const Double2x2> node1;
    weak<const Double2x2> node2;
public:
    Double2x2div(weak<const Double2x2> node1, weak<const Double2x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Double2x2, node1, node2);
        }
    }
};
ref<Double2x2> operator /(weak<const Double2x2> node1, weak<const Double2x2> node2)
{
    return ref<Double2x2div>::create(gameArena(), node1, node2);
}

class Double2x2add : public Double2x2
{
private:
    weak<const Double2x2> node1;
    weak<const Double2x2> node2;
public:
    Double2x2add(weak<const Double2x2> node1, weak<const Double2x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Double2x2, node1, node2);
        }
    }
};
ref<Double2x2> operator +(weak<const Double2x2> node1, weak<const Double2x2> node2)
{
    return ref<Double2x2add>::create(gameArena(), node1, node2);
}

class Double2x2sub : public Double2x2
{
private:
    weak<const Double2x2> node1;
    weak<const Double2x2> node2;
public:
    Double2x2sub(weak<const Double2x2> node1, weak<const Double2x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Double2x2, node1, node2);
        }
    }
};
ref<Double2x2> operator -(weak<const Double2x2> node1, weak<const Double2x2> node2)
{
    return ref<Double2x2sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Double3x2::Double3x2()
{
}
Double3x2::~Double3x2()
{
}

/* Uniform **********************************************************/
Double3x2Uniform::Double3x2Uniform(const istring& name)
    :   name(name)
{
}
Double3x2Uniform::~Double3x2Uniform()
{
}
void Double3x2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double3x2);
}
void Double3x2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double3x2Attribute::Double3x2Attribute()
{
}
Double3x2Attribute::~Double3x2Attribute()
{
}
void Double3x2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double3x2);
}
void Double3x2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double3x2Varying::Double3x2Varying(weak<const Double3x2> node)
    :   node(node)
{
}
Double3x2Varying::~Double3x2Varying()
{
}
void Double3x2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double3x2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double3x2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Double3x2mul : public Double3x2
{
private:
    weak<const Double3x2> node1;
    weak<const Double3x2> node2;
public:
    Double3x2mul(weak<const Double3x2> node1, weak<const Double3x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Double3x2, node1, node2);
        }
    }
};
ref<Double3x2> operator *(weak<const Double3x2> node1, weak<const Double3x2> node2)
{
    return ref<Double3x2mul>::create(gameArena(), node1, node2);
}

class Double3x2div : public Double3x2
{
private:
    weak<const Double3x2> node1;
    weak<const Double3x2> node2;
public:
    Double3x2div(weak<const Double3x2> node1, weak<const Double3x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Double3x2, node1, node2);
        }
    }
};
ref<Double3x2> operator /(weak<const Double3x2> node1, weak<const Double3x2> node2)
{
    return ref<Double3x2div>::create(gameArena(), node1, node2);
}

class Double3x2add : public Double3x2
{
private:
    weak<const Double3x2> node1;
    weak<const Double3x2> node2;
public:
    Double3x2add(weak<const Double3x2> node1, weak<const Double3x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Double3x2, node1, node2);
        }
    }
};
ref<Double3x2> operator +(weak<const Double3x2> node1, weak<const Double3x2> node2)
{
    return ref<Double3x2add>::create(gameArena(), node1, node2);
}

class Double3x2sub : public Double3x2
{
private:
    weak<const Double3x2> node1;
    weak<const Double3x2> node2;
public:
    Double3x2sub(weak<const Double3x2> node1, weak<const Double3x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Double3x2, node1, node2);
        }
    }
};
ref<Double3x2> operator -(weak<const Double3x2> node1, weak<const Double3x2> node2)
{
    return ref<Double3x2sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Double4x2::Double4x2()
{
}
Double4x2::~Double4x2()
{
}

/* Uniform **********************************************************/
Double4x2Uniform::Double4x2Uniform(const istring& name)
    :   name(name)
{
}
Double4x2Uniform::~Double4x2Uniform()
{
}
void Double4x2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double4x2);
}
void Double4x2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double4x2Attribute::Double4x2Attribute()
{
}
Double4x2Attribute::~Double4x2Attribute()
{
}
void Double4x2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double4x2);
}
void Double4x2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double4x2Varying::Double4x2Varying(weak<const Double4x2> node)
    :   node(node)
{
}
Double4x2Varying::~Double4x2Varying()
{
}
void Double4x2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double4x2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double4x2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Double4x2mul : public Double4x2
{
private:
    weak<const Double4x2> node1;
    weak<const Double4x2> node2;
public:
    Double4x2mul(weak<const Double4x2> node1, weak<const Double4x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Double4x2, node1, node2);
        }
    }
};
ref<Double4x2> operator *(weak<const Double4x2> node1, weak<const Double4x2> node2)
{
    return ref<Double4x2mul>::create(gameArena(), node1, node2);
}

class Double4x2div : public Double4x2
{
private:
    weak<const Double4x2> node1;
    weak<const Double4x2> node2;
public:
    Double4x2div(weak<const Double4x2> node1, weak<const Double4x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Double4x2, node1, node2);
        }
    }
};
ref<Double4x2> operator /(weak<const Double4x2> node1, weak<const Double4x2> node2)
{
    return ref<Double4x2div>::create(gameArena(), node1, node2);
}

class Double4x2add : public Double4x2
{
private:
    weak<const Double4x2> node1;
    weak<const Double4x2> node2;
public:
    Double4x2add(weak<const Double4x2> node1, weak<const Double4x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Double4x2, node1, node2);
        }
    }
};
ref<Double4x2> operator +(weak<const Double4x2> node1, weak<const Double4x2> node2)
{
    return ref<Double4x2add>::create(gameArena(), node1, node2);
}

class Double4x2sub : public Double4x2
{
private:
    weak<const Double4x2> node1;
    weak<const Double4x2> node2;
public:
    Double4x2sub(weak<const Double4x2> node1, weak<const Double4x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Double4x2, node1, node2);
        }
    }
};
ref<Double4x2> operator -(weak<const Double4x2> node1, weak<const Double4x2> node2)
{
    return ref<Double4x2sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Double2x3::Double2x3()
{
}
Double2x3::~Double2x3()
{
}

/* Uniform **********************************************************/
Double2x3Uniform::Double2x3Uniform(const istring& name)
    :   name(name)
{
}
Double2x3Uniform::~Double2x3Uniform()
{
}
void Double2x3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double2x3);
}
void Double2x3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double2x3Attribute::Double2x3Attribute()
{
}
Double2x3Attribute::~Double2x3Attribute()
{
}
void Double2x3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double2x3);
}
void Double2x3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double2x3Varying::Double2x3Varying(weak<const Double2x3> node)
    :   node(node)
{
}
Double2x3Varying::~Double2x3Varying()
{
}
void Double2x3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double2x3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double2x3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Double2x3mul : public Double2x3
{
private:
    weak<const Double2x3> node1;
    weak<const Double2x3> node2;
public:
    Double2x3mul(weak<const Double2x3> node1, weak<const Double2x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Double2x3, node1, node2);
        }
    }
};
ref<Double2x3> operator *(weak<const Double2x3> node1, weak<const Double2x3> node2)
{
    return ref<Double2x3mul>::create(gameArena(), node1, node2);
}

class Double2x3div : public Double2x3
{
private:
    weak<const Double2x3> node1;
    weak<const Double2x3> node2;
public:
    Double2x3div(weak<const Double2x3> node1, weak<const Double2x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Double2x3, node1, node2);
        }
    }
};
ref<Double2x3> operator /(weak<const Double2x3> node1, weak<const Double2x3> node2)
{
    return ref<Double2x3div>::create(gameArena(), node1, node2);
}

class Double2x3add : public Double2x3
{
private:
    weak<const Double2x3> node1;
    weak<const Double2x3> node2;
public:
    Double2x3add(weak<const Double2x3> node1, weak<const Double2x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Double2x3, node1, node2);
        }
    }
};
ref<Double2x3> operator +(weak<const Double2x3> node1, weak<const Double2x3> node2)
{
    return ref<Double2x3add>::create(gameArena(), node1, node2);
}

class Double2x3sub : public Double2x3
{
private:
    weak<const Double2x3> node1;
    weak<const Double2x3> node2;
public:
    Double2x3sub(weak<const Double2x3> node1, weak<const Double2x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Double2x3, node1, node2);
        }
    }
};
ref<Double2x3> operator -(weak<const Double2x3> node1, weak<const Double2x3> node2)
{
    return ref<Double2x3sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Double3x3::Double3x3()
{
}
Double3x3::~Double3x3()
{
}

/* Uniform **********************************************************/
Double3x3Uniform::Double3x3Uniform(const istring& name)
    :   name(name)
{
}
Double3x3Uniform::~Double3x3Uniform()
{
}
void Double3x3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double3x3);
}
void Double3x3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double3x3Attribute::Double3x3Attribute()
{
}
Double3x3Attribute::~Double3x3Attribute()
{
}
void Double3x3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double3x3);
}
void Double3x3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double3x3Varying::Double3x3Varying(weak<const Double3x3> node)
    :   node(node)
{
}
Double3x3Varying::~Double3x3Varying()
{
}
void Double3x3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double3x3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double3x3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Double3x3mul : public Double3x3
{
private:
    weak<const Double3x3> node1;
    weak<const Double3x3> node2;
public:
    Double3x3mul(weak<const Double3x3> node1, weak<const Double3x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Double3x3, node1, node2);
        }
    }
};
ref<Double3x3> operator *(weak<const Double3x3> node1, weak<const Double3x3> node2)
{
    return ref<Double3x3mul>::create(gameArena(), node1, node2);
}

class Double3x3div : public Double3x3
{
private:
    weak<const Double3x3> node1;
    weak<const Double3x3> node2;
public:
    Double3x3div(weak<const Double3x3> node1, weak<const Double3x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Double3x3, node1, node2);
        }
    }
};
ref<Double3x3> operator /(weak<const Double3x3> node1, weak<const Double3x3> node2)
{
    return ref<Double3x3div>::create(gameArena(), node1, node2);
}

class Double3x3add : public Double3x3
{
private:
    weak<const Double3x3> node1;
    weak<const Double3x3> node2;
public:
    Double3x3add(weak<const Double3x3> node1, weak<const Double3x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Double3x3, node1, node2);
        }
    }
};
ref<Double3x3> operator +(weak<const Double3x3> node1, weak<const Double3x3> node2)
{
    return ref<Double3x3add>::create(gameArena(), node1, node2);
}

class Double3x3sub : public Double3x3
{
private:
    weak<const Double3x3> node1;
    weak<const Double3x3> node2;
public:
    Double3x3sub(weak<const Double3x3> node1, weak<const Double3x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Double3x3, node1, node2);
        }
    }
};
ref<Double3x3> operator -(weak<const Double3x3> node1, weak<const Double3x3> node2)
{
    return ref<Double3x3sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Double4x3::Double4x3()
{
}
Double4x3::~Double4x3()
{
}

/* Uniform **********************************************************/
Double4x3Uniform::Double4x3Uniform(const istring& name)
    :   name(name)
{
}
Double4x3Uniform::~Double4x3Uniform()
{
}
void Double4x3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double4x3);
}
void Double4x3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double4x3Attribute::Double4x3Attribute()
{
}
Double4x3Attribute::~Double4x3Attribute()
{
}
void Double4x3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double4x3);
}
void Double4x3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double4x3Varying::Double4x3Varying(weak<const Double4x3> node)
    :   node(node)
{
}
Double4x3Varying::~Double4x3Varying()
{
}
void Double4x3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double4x3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double4x3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Double4x3mul : public Double4x3
{
private:
    weak<const Double4x3> node1;
    weak<const Double4x3> node2;
public:
    Double4x3mul(weak<const Double4x3> node1, weak<const Double4x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Double4x3, node1, node2);
        }
    }
};
ref<Double4x3> operator *(weak<const Double4x3> node1, weak<const Double4x3> node2)
{
    return ref<Double4x3mul>::create(gameArena(), node1, node2);
}

class Double4x3div : public Double4x3
{
private:
    weak<const Double4x3> node1;
    weak<const Double4x3> node2;
public:
    Double4x3div(weak<const Double4x3> node1, weak<const Double4x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Double4x3, node1, node2);
        }
    }
};
ref<Double4x3> operator /(weak<const Double4x3> node1, weak<const Double4x3> node2)
{
    return ref<Double4x3div>::create(gameArena(), node1, node2);
}

class Double4x3add : public Double4x3
{
private:
    weak<const Double4x3> node1;
    weak<const Double4x3> node2;
public:
    Double4x3add(weak<const Double4x3> node1, weak<const Double4x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Double4x3, node1, node2);
        }
    }
};
ref<Double4x3> operator +(weak<const Double4x3> node1, weak<const Double4x3> node2)
{
    return ref<Double4x3add>::create(gameArena(), node1, node2);
}

class Double4x3sub : public Double4x3
{
private:
    weak<const Double4x3> node1;
    weak<const Double4x3> node2;
public:
    Double4x3sub(weak<const Double4x3> node1, weak<const Double4x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Double4x3, node1, node2);
        }
    }
};
ref<Double4x3> operator -(weak<const Double4x3> node1, weak<const Double4x3> node2)
{
    return ref<Double4x3sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Double2x4::Double2x4()
{
}
Double2x4::~Double2x4()
{
}

/* Uniform **********************************************************/
Double2x4Uniform::Double2x4Uniform(const istring& name)
    :   name(name)
{
}
Double2x4Uniform::~Double2x4Uniform()
{
}
void Double2x4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double2x4);
}
void Double2x4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double2x4Attribute::Double2x4Attribute()
{
}
Double2x4Attribute::~Double2x4Attribute()
{
}
void Double2x4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double2x4);
}
void Double2x4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double2x4Varying::Double2x4Varying(weak<const Double2x4> node)
    :   node(node)
{
}
Double2x4Varying::~Double2x4Varying()
{
}
void Double2x4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double2x4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double2x4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Double2x4mul : public Double2x4
{
private:
    weak<const Double2x4> node1;
    weak<const Double2x4> node2;
public:
    Double2x4mul(weak<const Double2x4> node1, weak<const Double2x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Double2x4, node1, node2);
        }
    }
};
ref<Double2x4> operator *(weak<const Double2x4> node1, weak<const Double2x4> node2)
{
    return ref<Double2x4mul>::create(gameArena(), node1, node2);
}

class Double2x4div : public Double2x4
{
private:
    weak<const Double2x4> node1;
    weak<const Double2x4> node2;
public:
    Double2x4div(weak<const Double2x4> node1, weak<const Double2x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Double2x4, node1, node2);
        }
    }
};
ref<Double2x4> operator /(weak<const Double2x4> node1, weak<const Double2x4> node2)
{
    return ref<Double2x4div>::create(gameArena(), node1, node2);
}

class Double2x4add : public Double2x4
{
private:
    weak<const Double2x4> node1;
    weak<const Double2x4> node2;
public:
    Double2x4add(weak<const Double2x4> node1, weak<const Double2x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Double2x4, node1, node2);
        }
    }
};
ref<Double2x4> operator +(weak<const Double2x4> node1, weak<const Double2x4> node2)
{
    return ref<Double2x4add>::create(gameArena(), node1, node2);
}

class Double2x4sub : public Double2x4
{
private:
    weak<const Double2x4> node1;
    weak<const Double2x4> node2;
public:
    Double2x4sub(weak<const Double2x4> node1, weak<const Double2x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Double2x4, node1, node2);
        }
    }
};
ref<Double2x4> operator -(weak<const Double2x4> node1, weak<const Double2x4> node2)
{
    return ref<Double2x4sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Double3x4::Double3x4()
{
}
Double3x4::~Double3x4()
{
}

/* Uniform **********************************************************/
Double3x4Uniform::Double3x4Uniform(const istring& name)
    :   name(name)
{
}
Double3x4Uniform::~Double3x4Uniform()
{
}
void Double3x4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double3x4);
}
void Double3x4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double3x4Attribute::Double3x4Attribute()
{
}
Double3x4Attribute::~Double3x4Attribute()
{
}
void Double3x4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double3x4);
}
void Double3x4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double3x4Varying::Double3x4Varying(weak<const Double3x4> node)
    :   node(node)
{
}
Double3x4Varying::~Double3x4Varying()
{
}
void Double3x4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double3x4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double3x4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Double3x4mul : public Double3x4
{
private:
    weak<const Double3x4> node1;
    weak<const Double3x4> node2;
public:
    Double3x4mul(weak<const Double3x4> node1, weak<const Double3x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Double3x4, node1, node2);
        }
    }
};
ref<Double3x4> operator *(weak<const Double3x4> node1, weak<const Double3x4> node2)
{
    return ref<Double3x4mul>::create(gameArena(), node1, node2);
}

class Double3x4div : public Double3x4
{
private:
    weak<const Double3x4> node1;
    weak<const Double3x4> node2;
public:
    Double3x4div(weak<const Double3x4> node1, weak<const Double3x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Double3x4, node1, node2);
        }
    }
};
ref<Double3x4> operator /(weak<const Double3x4> node1, weak<const Double3x4> node2)
{
    return ref<Double3x4div>::create(gameArena(), node1, node2);
}

class Double3x4add : public Double3x4
{
private:
    weak<const Double3x4> node1;
    weak<const Double3x4> node2;
public:
    Double3x4add(weak<const Double3x4> node1, weak<const Double3x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Double3x4, node1, node2);
        }
    }
};
ref<Double3x4> operator +(weak<const Double3x4> node1, weak<const Double3x4> node2)
{
    return ref<Double3x4add>::create(gameArena(), node1, node2);
}

class Double3x4sub : public Double3x4
{
private:
    weak<const Double3x4> node1;
    weak<const Double3x4> node2;
public:
    Double3x4sub(weak<const Double3x4> node1, weak<const Double3x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Double3x4, node1, node2);
        }
    }
};
ref<Double3x4> operator -(weak<const Double3x4> node1, weak<const Double3x4> node2)
{
    return ref<Double3x4sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Double4x4::Double4x4()
{
}
Double4x4::~Double4x4()
{
}

/* Uniform **********************************************************/
Double4x4Uniform::Double4x4Uniform(const istring& name)
    :   name(name)
{
}
Double4x4Uniform::~Double4x4Uniform()
{
}
void Double4x4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Double4x4);
}
void Double4x4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Double4x4Attribute::Double4x4Attribute()
{
}
Double4x4Attribute::~Double4x4Attribute()
{
}
void Double4x4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Double4x4);
}
void Double4x4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Double4x4Varying::Double4x4Varying(weak<const Double4x4> node)
    :   node(node)
{
}
Double4x4Varying::~Double4x4Varying()
{
}
void Double4x4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Double4x4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Double4x4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Double4x4mul : public Double4x4
{
private:
    weak<const Double4x4> node1;
    weak<const Double4x4> node2;
public:
    Double4x4mul(weak<const Double4x4> node1, weak<const Double4x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Double4x4, node1, node2);
        }
    }
};
ref<Double4x4> operator *(weak<const Double4x4> node1, weak<const Double4x4> node2)
{
    return ref<Double4x4mul>::create(gameArena(), node1, node2);
}

class Double4x4div : public Double4x4
{
private:
    weak<const Double4x4> node1;
    weak<const Double4x4> node2;
public:
    Double4x4div(weak<const Double4x4> node1, weak<const Double4x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Double4x4, node1, node2);
        }
    }
};
ref<Double4x4> operator /(weak<const Double4x4> node1, weak<const Double4x4> node2)
{
    return ref<Double4x4div>::create(gameArena(), node1, node2);
}

class Double4x4add : public Double4x4
{
private:
    weak<const Double4x4> node1;
    weak<const Double4x4> node2;
public:
    Double4x4add(weak<const Double4x4> node1, weak<const Double4x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Double4x4, node1, node2);
        }
    }
};
ref<Double4x4> operator +(weak<const Double4x4> node1, weak<const Double4x4> node2)
{
    return ref<Double4x4add>::create(gameArena(), node1, node2);
}

class Double4x4sub : public Double4x4
{
private:
    weak<const Double4x4> node1;
    weak<const Double4x4> node2;
public:
    Double4x4sub(weak<const Double4x4> node1, weak<const Double4x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Double4x4, node1, node2);
        }
    }
};
ref<Double4x4> operator -(weak<const Double4x4> node1, weak<const Double4x4> node2)
{
    return ref<Double4x4sub>::create(gameArena(), node1, node2);
}




}}}
