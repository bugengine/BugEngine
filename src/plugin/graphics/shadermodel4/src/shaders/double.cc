/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <shadermodel4/stdafx.h>
#include    <shadermodel4/shaders/double.script.hh>
#include    <3d/shader/ishaderbuilder.hh>

namespace BugEngine { namespace Shaders
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

class DoubledivDouble : public Double
{
private:
    weak<const Double> node1;
    weak<const Double> node2;
public:
    DoubledivDouble(weak<const Double> node1, weak<const Double> node2)
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
    return ref<DoubledivDouble>::create(gameArena(), node1, node2);
}

class DoubleaddDouble : public Double
{
private:
    weak<const Double> node1;
    weak<const Double> node2;
public:
    DoubleaddDouble(weak<const Double> node1, weak<const Double> node2)
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
    return ref<DoubleaddDouble>::create(gameArena(), node1, node2);
}

class DoublesubDouble : public Double
{
private:
    weak<const Double> node1;
    weak<const Double> node2;
public:
    DoublesubDouble(weak<const Double> node1, weak<const Double> node2)
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
    return ref<DoublesubDouble>::create(gameArena(), node1, node2);
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

class Double2divDouble2 : public Double2
{
private:
    weak<const Double2> node1;
    weak<const Double2> node2;
public:
    Double2divDouble2(weak<const Double2> node1, weak<const Double2> node2)
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
    return ref<Double2divDouble2>::create(gameArena(), node1, node2);
}

class Double2addDouble2 : public Double2
{
private:
    weak<const Double2> node1;
    weak<const Double2> node2;
public:
    Double2addDouble2(weak<const Double2> node1, weak<const Double2> node2)
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
    return ref<Double2addDouble2>::create(gameArena(), node1, node2);
}

class Double2subDouble2 : public Double2
{
private:
    weak<const Double2> node1;
    weak<const Double2> node2;
public:
    Double2subDouble2(weak<const Double2> node1, weak<const Double2> node2)
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
    return ref<Double2subDouble2>::create(gameArena(), node1, node2);
}

class Double2mulDouble2x2 : public Double2
{
private:
    weak<const Double2> node1;
    weak<const Double2x2> node2;
public:
    Double2mulDouble2x2(weak<const Double2> node1, weak<const Double2x2> node2)
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
ref<Double2> operator *(weak<const Double2> node1, weak<const Double2x2> node2)
{
    return ref<Double2mulDouble2x2>::create(gameArena(), node1, node2);
}

class Double2x2mulDouble2 : public Double2
{
private:
    weak<const Double2x2> node1;
    weak<const Double2> node2;
public:
    Double2x2mulDouble2(weak<const Double2x2> node1, weak<const Double2> node2)
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
ref<Double2> operator *(weak<const Double2x2> node1, weak<const Double2> node2)
{
    return ref<Double2x2mulDouble2>::create(gameArena(), node1, node2);
}

class DoublemulDouble2 : public Double2
{
private:
    weak<const Double> node1;
    weak<const Double2> node2;
public:
    DoublemulDouble2(weak<const Double> node1, weak<const Double2> node2)
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
ref<Double2> operator *(weak<const Double> node1, weak<const Double2> node2)
{
    return ref<DoublemulDouble2>::create(gameArena(), node1, node2);
}

class Double2mulDouble : public Double2
{
private:
    weak<const Double2> node1;
    weak<const Double> node2;
public:
    Double2mulDouble(weak<const Double2> node1, weak<const Double> node2)
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
ref<Double2> operator *(weak<const Double2> node1, weak<const Double> node2)
{
    return ref<Double2mulDouble>::create(gameArena(), node1, node2);
}

class DoubledivDouble2 : public Double2
{
private:
    weak<const Double> node1;
    weak<const Double2> node2;
public:
    DoubledivDouble2(weak<const Double> node1, weak<const Double2> node2)
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
ref<Double2> operator /(weak<const Double> node1, weak<const Double2> node2)
{
    return ref<DoubledivDouble2>::create(gameArena(), node1, node2);
}

class Double2divDouble : public Double2
{
private:
    weak<const Double2> node1;
    weak<const Double> node2;
public:
    Double2divDouble(weak<const Double2> node1, weak<const Double> node2)
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
ref<Double2> operator /(weak<const Double2> node1, weak<const Double> node2)
{
    return ref<Double2divDouble>::create(gameArena(), node1, node2);
}

class DoubleaddDouble2 : public Double2
{
private:
    weak<const Double> node1;
    weak<const Double2> node2;
public:
    DoubleaddDouble2(weak<const Double> node1, weak<const Double2> node2)
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
ref<Double2> operator +(weak<const Double> node1, weak<const Double2> node2)
{
    return ref<DoubleaddDouble2>::create(gameArena(), node1, node2);
}

class Double2addDouble : public Double2
{
private:
    weak<const Double2> node1;
    weak<const Double> node2;
public:
    Double2addDouble(weak<const Double2> node1, weak<const Double> node2)
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
ref<Double2> operator +(weak<const Double2> node1, weak<const Double> node2)
{
    return ref<Double2addDouble>::create(gameArena(), node1, node2);
}

class DoublesubDouble2 : public Double2
{
private:
    weak<const Double> node1;
    weak<const Double2> node2;
public:
    DoublesubDouble2(weak<const Double> node1, weak<const Double2> node2)
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
ref<Double2> operator -(weak<const Double> node1, weak<const Double2> node2)
{
    return ref<DoublesubDouble2>::create(gameArena(), node1, node2);
}

class Double2subDouble : public Double2
{
private:
    weak<const Double2> node1;
    weak<const Double> node2;
public:
    Double2subDouble(weak<const Double2> node1, weak<const Double> node2)
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
ref<Double2> operator -(weak<const Double2> node1, weak<const Double> node2)
{
    return ref<Double2subDouble>::create(gameArena(), node1, node2);
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

class Double3divDouble3 : public Double3
{
private:
    weak<const Double3> node1;
    weak<const Double3> node2;
public:
    Double3divDouble3(weak<const Double3> node1, weak<const Double3> node2)
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
    return ref<Double3divDouble3>::create(gameArena(), node1, node2);
}

class Double3addDouble3 : public Double3
{
private:
    weak<const Double3> node1;
    weak<const Double3> node2;
public:
    Double3addDouble3(weak<const Double3> node1, weak<const Double3> node2)
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
    return ref<Double3addDouble3>::create(gameArena(), node1, node2);
}

class Double3subDouble3 : public Double3
{
private:
    weak<const Double3> node1;
    weak<const Double3> node2;
public:
    Double3subDouble3(weak<const Double3> node1, weak<const Double3> node2)
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
    return ref<Double3subDouble3>::create(gameArena(), node1, node2);
}

class Double3mulDouble3x3 : public Double3
{
private:
    weak<const Double3> node1;
    weak<const Double3x3> node2;
public:
    Double3mulDouble3x3(weak<const Double3> node1, weak<const Double3x3> node2)
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
ref<Double3> operator *(weak<const Double3> node1, weak<const Double3x3> node2)
{
    return ref<Double3mulDouble3x3>::create(gameArena(), node1, node2);
}

class Double3x3mulDouble3 : public Double3
{
private:
    weak<const Double3x3> node1;
    weak<const Double3> node2;
public:
    Double3x3mulDouble3(weak<const Double3x3> node1, weak<const Double3> node2)
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
ref<Double3> operator *(weak<const Double3x3> node1, weak<const Double3> node2)
{
    return ref<Double3x3mulDouble3>::create(gameArena(), node1, node2);
}

class DoublemulDouble3 : public Double3
{
private:
    weak<const Double> node1;
    weak<const Double3> node2;
public:
    DoublemulDouble3(weak<const Double> node1, weak<const Double3> node2)
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
ref<Double3> operator *(weak<const Double> node1, weak<const Double3> node2)
{
    return ref<DoublemulDouble3>::create(gameArena(), node1, node2);
}

class Double3mulDouble : public Double3
{
private:
    weak<const Double3> node1;
    weak<const Double> node2;
public:
    Double3mulDouble(weak<const Double3> node1, weak<const Double> node2)
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
ref<Double3> operator *(weak<const Double3> node1, weak<const Double> node2)
{
    return ref<Double3mulDouble>::create(gameArena(), node1, node2);
}

class DoubledivDouble3 : public Double3
{
private:
    weak<const Double> node1;
    weak<const Double3> node2;
public:
    DoubledivDouble3(weak<const Double> node1, weak<const Double3> node2)
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
ref<Double3> operator /(weak<const Double> node1, weak<const Double3> node2)
{
    return ref<DoubledivDouble3>::create(gameArena(), node1, node2);
}

class Double3divDouble : public Double3
{
private:
    weak<const Double3> node1;
    weak<const Double> node2;
public:
    Double3divDouble(weak<const Double3> node1, weak<const Double> node2)
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
ref<Double3> operator /(weak<const Double3> node1, weak<const Double> node2)
{
    return ref<Double3divDouble>::create(gameArena(), node1, node2);
}

class DoubleaddDouble3 : public Double3
{
private:
    weak<const Double> node1;
    weak<const Double3> node2;
public:
    DoubleaddDouble3(weak<const Double> node1, weak<const Double3> node2)
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
ref<Double3> operator +(weak<const Double> node1, weak<const Double3> node2)
{
    return ref<DoubleaddDouble3>::create(gameArena(), node1, node2);
}

class Double3addDouble : public Double3
{
private:
    weak<const Double3> node1;
    weak<const Double> node2;
public:
    Double3addDouble(weak<const Double3> node1, weak<const Double> node2)
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
ref<Double3> operator +(weak<const Double3> node1, weak<const Double> node2)
{
    return ref<Double3addDouble>::create(gameArena(), node1, node2);
}

class DoublesubDouble3 : public Double3
{
private:
    weak<const Double> node1;
    weak<const Double3> node2;
public:
    DoublesubDouble3(weak<const Double> node1, weak<const Double3> node2)
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
ref<Double3> operator -(weak<const Double> node1, weak<const Double3> node2)
{
    return ref<DoublesubDouble3>::create(gameArena(), node1, node2);
}

class Double3subDouble : public Double3
{
private:
    weak<const Double3> node1;
    weak<const Double> node2;
public:
    Double3subDouble(weak<const Double3> node1, weak<const Double> node2)
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
ref<Double3> operator -(weak<const Double3> node1, weak<const Double> node2)
{
    return ref<Double3subDouble>::create(gameArena(), node1, node2);
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

class Double4divDouble4 : public Double4
{
private:
    weak<const Double4> node1;
    weak<const Double4> node2;
public:
    Double4divDouble4(weak<const Double4> node1, weak<const Double4> node2)
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
    return ref<Double4divDouble4>::create(gameArena(), node1, node2);
}

class Double4addDouble4 : public Double4
{
private:
    weak<const Double4> node1;
    weak<const Double4> node2;
public:
    Double4addDouble4(weak<const Double4> node1, weak<const Double4> node2)
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
    return ref<Double4addDouble4>::create(gameArena(), node1, node2);
}

class Double4subDouble4 : public Double4
{
private:
    weak<const Double4> node1;
    weak<const Double4> node2;
public:
    Double4subDouble4(weak<const Double4> node1, weak<const Double4> node2)
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
    return ref<Double4subDouble4>::create(gameArena(), node1, node2);
}

class Double4mulDouble4x4 : public Double4
{
private:
    weak<const Double4> node1;
    weak<const Double4x4> node2;
public:
    Double4mulDouble4x4(weak<const Double4> node1, weak<const Double4x4> node2)
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
ref<Double4> operator *(weak<const Double4> node1, weak<const Double4x4> node2)
{
    return ref<Double4mulDouble4x4>::create(gameArena(), node1, node2);
}

class Double4x4mulDouble4 : public Double4
{
private:
    weak<const Double4x4> node1;
    weak<const Double4> node2;
public:
    Double4x4mulDouble4(weak<const Double4x4> node1, weak<const Double4> node2)
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
ref<Double4> operator *(weak<const Double4x4> node1, weak<const Double4> node2)
{
    return ref<Double4x4mulDouble4>::create(gameArena(), node1, node2);
}

class DoublemulDouble4 : public Double4
{
private:
    weak<const Double> node1;
    weak<const Double4> node2;
public:
    DoublemulDouble4(weak<const Double> node1, weak<const Double4> node2)
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
ref<Double4> operator *(weak<const Double> node1, weak<const Double4> node2)
{
    return ref<DoublemulDouble4>::create(gameArena(), node1, node2);
}

class Double4mulDouble : public Double4
{
private:
    weak<const Double4> node1;
    weak<const Double> node2;
public:
    Double4mulDouble(weak<const Double4> node1, weak<const Double> node2)
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
ref<Double4> operator *(weak<const Double4> node1, weak<const Double> node2)
{
    return ref<Double4mulDouble>::create(gameArena(), node1, node2);
}

class DoubledivDouble4 : public Double4
{
private:
    weak<const Double> node1;
    weak<const Double4> node2;
public:
    DoubledivDouble4(weak<const Double> node1, weak<const Double4> node2)
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
ref<Double4> operator /(weak<const Double> node1, weak<const Double4> node2)
{
    return ref<DoubledivDouble4>::create(gameArena(), node1, node2);
}

class Double4divDouble : public Double4
{
private:
    weak<const Double4> node1;
    weak<const Double> node2;
public:
    Double4divDouble(weak<const Double4> node1, weak<const Double> node2)
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
ref<Double4> operator /(weak<const Double4> node1, weak<const Double> node2)
{
    return ref<Double4divDouble>::create(gameArena(), node1, node2);
}

class DoubleaddDouble4 : public Double4
{
private:
    weak<const Double> node1;
    weak<const Double4> node2;
public:
    DoubleaddDouble4(weak<const Double> node1, weak<const Double4> node2)
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
ref<Double4> operator +(weak<const Double> node1, weak<const Double4> node2)
{
    return ref<DoubleaddDouble4>::create(gameArena(), node1, node2);
}

class Double4addDouble : public Double4
{
private:
    weak<const Double4> node1;
    weak<const Double> node2;
public:
    Double4addDouble(weak<const Double4> node1, weak<const Double> node2)
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
ref<Double4> operator +(weak<const Double4> node1, weak<const Double> node2)
{
    return ref<Double4addDouble>::create(gameArena(), node1, node2);
}

class DoublesubDouble4 : public Double4
{
private:
    weak<const Double> node1;
    weak<const Double4> node2;
public:
    DoublesubDouble4(weak<const Double> node1, weak<const Double4> node2)
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
ref<Double4> operator -(weak<const Double> node1, weak<const Double4> node2)
{
    return ref<DoublesubDouble4>::create(gameArena(), node1, node2);
}

class Double4subDouble : public Double4
{
private:
    weak<const Double4> node1;
    weak<const Double> node2;
public:
    Double4subDouble(weak<const Double4> node1, weak<const Double> node2)
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
ref<Double4> operator -(weak<const Double4> node1, weak<const Double> node2)
{
    return ref<Double4subDouble>::create(gameArena(), node1, node2);
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

class Double2x2divDouble2x2 : public Double2x2
{
private:
    weak<const Double2x2> node1;
    weak<const Double2x2> node2;
public:
    Double2x2divDouble2x2(weak<const Double2x2> node1, weak<const Double2x2> node2)
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
    return ref<Double2x2divDouble2x2>::create(gameArena(), node1, node2);
}

class Double2x2addDouble2x2 : public Double2x2
{
private:
    weak<const Double2x2> node1;
    weak<const Double2x2> node2;
public:
    Double2x2addDouble2x2(weak<const Double2x2> node1, weak<const Double2x2> node2)
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
    return ref<Double2x2addDouble2x2>::create(gameArena(), node1, node2);
}

class Double2x2subDouble2x2 : public Double2x2
{
private:
    weak<const Double2x2> node1;
    weak<const Double2x2> node2;
public:
    Double2x2subDouble2x2(weak<const Double2x2> node1, weak<const Double2x2> node2)
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
    return ref<Double2x2subDouble2x2>::create(gameArena(), node1, node2);
}

class Double2x2mulDouble2x2 : public Double2x2
{
private:
    weak<const Double2x2> node1;
    weak<const Double2x2> node2;
public:
    Double2x2mulDouble2x2(weak<const Double2x2> node1, weak<const Double2x2> node2)
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
    return ref<Double2x2mulDouble2x2>::create(gameArena(), node1, node2);
}

class Double3x2mulDouble2x3 : public Double2x2
{
private:
    weak<const Double3x2> node1;
    weak<const Double2x3> node2;
public:
    Double3x2mulDouble2x3(weak<const Double3x2> node1, weak<const Double2x3> node2)
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
ref<Double2x2> operator *(weak<const Double3x2> node1, weak<const Double2x3> node2)
{
    return ref<Double3x2mulDouble2x3>::create(gameArena(), node1, node2);
}

class Double4x2mulDouble2x4 : public Double2x2
{
private:
    weak<const Double4x2> node1;
    weak<const Double2x4> node2;
public:
    Double4x2mulDouble2x4(weak<const Double4x2> node1, weak<const Double2x4> node2)
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
ref<Double2x2> operator *(weak<const Double4x2> node1, weak<const Double2x4> node2)
{
    return ref<Double4x2mulDouble2x4>::create(gameArena(), node1, node2);
}

class DoublemulDouble2x2 : public Double2x2
{
private:
    weak<const Double> node1;
    weak<const Double2x2> node2;
public:
    DoublemulDouble2x2(weak<const Double> node1, weak<const Double2x2> node2)
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
ref<Double2x2> operator *(weak<const Double> node1, weak<const Double2x2> node2)
{
    return ref<DoublemulDouble2x2>::create(gameArena(), node1, node2);
}

class Double2x2mulDouble : public Double2x2
{
private:
    weak<const Double2x2> node1;
    weak<const Double> node2;
public:
    Double2x2mulDouble(weak<const Double2x2> node1, weak<const Double> node2)
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
ref<Double2x2> operator *(weak<const Double2x2> node1, weak<const Double> node2)
{
    return ref<Double2x2mulDouble>::create(gameArena(), node1, node2);
}

class DoubledivDouble2x2 : public Double2x2
{
private:
    weak<const Double> node1;
    weak<const Double2x2> node2;
public:
    DoubledivDouble2x2(weak<const Double> node1, weak<const Double2x2> node2)
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
ref<Double2x2> operator /(weak<const Double> node1, weak<const Double2x2> node2)
{
    return ref<DoubledivDouble2x2>::create(gameArena(), node1, node2);
}

class Double2x2divDouble : public Double2x2
{
private:
    weak<const Double2x2> node1;
    weak<const Double> node2;
public:
    Double2x2divDouble(weak<const Double2x2> node1, weak<const Double> node2)
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
ref<Double2x2> operator /(weak<const Double2x2> node1, weak<const Double> node2)
{
    return ref<Double2x2divDouble>::create(gameArena(), node1, node2);
}

class DoubleaddDouble2x2 : public Double2x2
{
private:
    weak<const Double> node1;
    weak<const Double2x2> node2;
public:
    DoubleaddDouble2x2(weak<const Double> node1, weak<const Double2x2> node2)
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
ref<Double2x2> operator +(weak<const Double> node1, weak<const Double2x2> node2)
{
    return ref<DoubleaddDouble2x2>::create(gameArena(), node1, node2);
}

class Double2x2addDouble : public Double2x2
{
private:
    weak<const Double2x2> node1;
    weak<const Double> node2;
public:
    Double2x2addDouble(weak<const Double2x2> node1, weak<const Double> node2)
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
ref<Double2x2> operator +(weak<const Double2x2> node1, weak<const Double> node2)
{
    return ref<Double2x2addDouble>::create(gameArena(), node1, node2);
}

class DoublesubDouble2x2 : public Double2x2
{
private:
    weak<const Double> node1;
    weak<const Double2x2> node2;
public:
    DoublesubDouble2x2(weak<const Double> node1, weak<const Double2x2> node2)
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
ref<Double2x2> operator -(weak<const Double> node1, weak<const Double2x2> node2)
{
    return ref<DoublesubDouble2x2>::create(gameArena(), node1, node2);
}

class Double2x2subDouble : public Double2x2
{
private:
    weak<const Double2x2> node1;
    weak<const Double> node2;
public:
    Double2x2subDouble(weak<const Double2x2> node1, weak<const Double> node2)
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
ref<Double2x2> operator -(weak<const Double2x2> node1, weak<const Double> node2)
{
    return ref<Double2x2subDouble>::create(gameArena(), node1, node2);
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

class Double3x2divDouble3x2 : public Double3x2
{
private:
    weak<const Double3x2> node1;
    weak<const Double3x2> node2;
public:
    Double3x2divDouble3x2(weak<const Double3x2> node1, weak<const Double3x2> node2)
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
    return ref<Double3x2divDouble3x2>::create(gameArena(), node1, node2);
}

class Double3x2addDouble3x2 : public Double3x2
{
private:
    weak<const Double3x2> node1;
    weak<const Double3x2> node2;
public:
    Double3x2addDouble3x2(weak<const Double3x2> node1, weak<const Double3x2> node2)
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
    return ref<Double3x2addDouble3x2>::create(gameArena(), node1, node2);
}

class Double3x2subDouble3x2 : public Double3x2
{
private:
    weak<const Double3x2> node1;
    weak<const Double3x2> node2;
public:
    Double3x2subDouble3x2(weak<const Double3x2> node1, weak<const Double3x2> node2)
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
    return ref<Double3x2subDouble3x2>::create(gameArena(), node1, node2);
}

class Double2x2mulDouble3x2 : public Double3x2
{
private:
    weak<const Double2x2> node1;
    weak<const Double3x2> node2;
public:
    Double2x2mulDouble3x2(weak<const Double2x2> node1, weak<const Double3x2> node2)
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
ref<Double3x2> operator *(weak<const Double2x2> node1, weak<const Double3x2> node2)
{
    return ref<Double2x2mulDouble3x2>::create(gameArena(), node1, node2);
}

class Double3x2mulDouble3x3 : public Double3x2
{
private:
    weak<const Double3x2> node1;
    weak<const Double3x3> node2;
public:
    Double3x2mulDouble3x3(weak<const Double3x2> node1, weak<const Double3x3> node2)
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
ref<Double3x2> operator *(weak<const Double3x2> node1, weak<const Double3x3> node2)
{
    return ref<Double3x2mulDouble3x3>::create(gameArena(), node1, node2);
}

class Double4x2mulDouble3x4 : public Double3x2
{
private:
    weak<const Double4x2> node1;
    weak<const Double3x4> node2;
public:
    Double4x2mulDouble3x4(weak<const Double4x2> node1, weak<const Double3x4> node2)
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
ref<Double3x2> operator *(weak<const Double4x2> node1, weak<const Double3x4> node2)
{
    return ref<Double4x2mulDouble3x4>::create(gameArena(), node1, node2);
}

class DoublemulDouble3x2 : public Double3x2
{
private:
    weak<const Double> node1;
    weak<const Double3x2> node2;
public:
    DoublemulDouble3x2(weak<const Double> node1, weak<const Double3x2> node2)
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
ref<Double3x2> operator *(weak<const Double> node1, weak<const Double3x2> node2)
{
    return ref<DoublemulDouble3x2>::create(gameArena(), node1, node2);
}

class Double3x2mulDouble : public Double3x2
{
private:
    weak<const Double3x2> node1;
    weak<const Double> node2;
public:
    Double3x2mulDouble(weak<const Double3x2> node1, weak<const Double> node2)
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
ref<Double3x2> operator *(weak<const Double3x2> node1, weak<const Double> node2)
{
    return ref<Double3x2mulDouble>::create(gameArena(), node1, node2);
}

class DoubledivDouble3x2 : public Double3x2
{
private:
    weak<const Double> node1;
    weak<const Double3x2> node2;
public:
    DoubledivDouble3x2(weak<const Double> node1, weak<const Double3x2> node2)
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
ref<Double3x2> operator /(weak<const Double> node1, weak<const Double3x2> node2)
{
    return ref<DoubledivDouble3x2>::create(gameArena(), node1, node2);
}

class Double3x2divDouble : public Double3x2
{
private:
    weak<const Double3x2> node1;
    weak<const Double> node2;
public:
    Double3x2divDouble(weak<const Double3x2> node1, weak<const Double> node2)
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
ref<Double3x2> operator /(weak<const Double3x2> node1, weak<const Double> node2)
{
    return ref<Double3x2divDouble>::create(gameArena(), node1, node2);
}

class DoubleaddDouble3x2 : public Double3x2
{
private:
    weak<const Double> node1;
    weak<const Double3x2> node2;
public:
    DoubleaddDouble3x2(weak<const Double> node1, weak<const Double3x2> node2)
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
ref<Double3x2> operator +(weak<const Double> node1, weak<const Double3x2> node2)
{
    return ref<DoubleaddDouble3x2>::create(gameArena(), node1, node2);
}

class Double3x2addDouble : public Double3x2
{
private:
    weak<const Double3x2> node1;
    weak<const Double> node2;
public:
    Double3x2addDouble(weak<const Double3x2> node1, weak<const Double> node2)
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
ref<Double3x2> operator +(weak<const Double3x2> node1, weak<const Double> node2)
{
    return ref<Double3x2addDouble>::create(gameArena(), node1, node2);
}

class DoublesubDouble3x2 : public Double3x2
{
private:
    weak<const Double> node1;
    weak<const Double3x2> node2;
public:
    DoublesubDouble3x2(weak<const Double> node1, weak<const Double3x2> node2)
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
ref<Double3x2> operator -(weak<const Double> node1, weak<const Double3x2> node2)
{
    return ref<DoublesubDouble3x2>::create(gameArena(), node1, node2);
}

class Double3x2subDouble : public Double3x2
{
private:
    weak<const Double3x2> node1;
    weak<const Double> node2;
public:
    Double3x2subDouble(weak<const Double3x2> node1, weak<const Double> node2)
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
ref<Double3x2> operator -(weak<const Double3x2> node1, weak<const Double> node2)
{
    return ref<Double3x2subDouble>::create(gameArena(), node1, node2);
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

class Double4x2divDouble4x2 : public Double4x2
{
private:
    weak<const Double4x2> node1;
    weak<const Double4x2> node2;
public:
    Double4x2divDouble4x2(weak<const Double4x2> node1, weak<const Double4x2> node2)
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
    return ref<Double4x2divDouble4x2>::create(gameArena(), node1, node2);
}

class Double4x2addDouble4x2 : public Double4x2
{
private:
    weak<const Double4x2> node1;
    weak<const Double4x2> node2;
public:
    Double4x2addDouble4x2(weak<const Double4x2> node1, weak<const Double4x2> node2)
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
    return ref<Double4x2addDouble4x2>::create(gameArena(), node1, node2);
}

class Double4x2subDouble4x2 : public Double4x2
{
private:
    weak<const Double4x2> node1;
    weak<const Double4x2> node2;
public:
    Double4x2subDouble4x2(weak<const Double4x2> node1, weak<const Double4x2> node2)
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
    return ref<Double4x2subDouble4x2>::create(gameArena(), node1, node2);
}

class Double2x2mulDouble4x2 : public Double4x2
{
private:
    weak<const Double2x2> node1;
    weak<const Double4x2> node2;
public:
    Double2x2mulDouble4x2(weak<const Double2x2> node1, weak<const Double4x2> node2)
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
ref<Double4x2> operator *(weak<const Double2x2> node1, weak<const Double4x2> node2)
{
    return ref<Double2x2mulDouble4x2>::create(gameArena(), node1, node2);
}

class Double3x2mulDouble4x3 : public Double4x2
{
private:
    weak<const Double3x2> node1;
    weak<const Double4x3> node2;
public:
    Double3x2mulDouble4x3(weak<const Double3x2> node1, weak<const Double4x3> node2)
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
ref<Double4x2> operator *(weak<const Double3x2> node1, weak<const Double4x3> node2)
{
    return ref<Double3x2mulDouble4x3>::create(gameArena(), node1, node2);
}

class Double4x2mulDouble4x4 : public Double4x2
{
private:
    weak<const Double4x2> node1;
    weak<const Double4x4> node2;
public:
    Double4x2mulDouble4x4(weak<const Double4x2> node1, weak<const Double4x4> node2)
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
ref<Double4x2> operator *(weak<const Double4x2> node1, weak<const Double4x4> node2)
{
    return ref<Double4x2mulDouble4x4>::create(gameArena(), node1, node2);
}

class DoublemulDouble4x2 : public Double4x2
{
private:
    weak<const Double> node1;
    weak<const Double4x2> node2;
public:
    DoublemulDouble4x2(weak<const Double> node1, weak<const Double4x2> node2)
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
ref<Double4x2> operator *(weak<const Double> node1, weak<const Double4x2> node2)
{
    return ref<DoublemulDouble4x2>::create(gameArena(), node1, node2);
}

class Double4x2mulDouble : public Double4x2
{
private:
    weak<const Double4x2> node1;
    weak<const Double> node2;
public:
    Double4x2mulDouble(weak<const Double4x2> node1, weak<const Double> node2)
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
ref<Double4x2> operator *(weak<const Double4x2> node1, weak<const Double> node2)
{
    return ref<Double4x2mulDouble>::create(gameArena(), node1, node2);
}

class DoubledivDouble4x2 : public Double4x2
{
private:
    weak<const Double> node1;
    weak<const Double4x2> node2;
public:
    DoubledivDouble4x2(weak<const Double> node1, weak<const Double4x2> node2)
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
ref<Double4x2> operator /(weak<const Double> node1, weak<const Double4x2> node2)
{
    return ref<DoubledivDouble4x2>::create(gameArena(), node1, node2);
}

class Double4x2divDouble : public Double4x2
{
private:
    weak<const Double4x2> node1;
    weak<const Double> node2;
public:
    Double4x2divDouble(weak<const Double4x2> node1, weak<const Double> node2)
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
ref<Double4x2> operator /(weak<const Double4x2> node1, weak<const Double> node2)
{
    return ref<Double4x2divDouble>::create(gameArena(), node1, node2);
}

class DoubleaddDouble4x2 : public Double4x2
{
private:
    weak<const Double> node1;
    weak<const Double4x2> node2;
public:
    DoubleaddDouble4x2(weak<const Double> node1, weak<const Double4x2> node2)
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
ref<Double4x2> operator +(weak<const Double> node1, weak<const Double4x2> node2)
{
    return ref<DoubleaddDouble4x2>::create(gameArena(), node1, node2);
}

class Double4x2addDouble : public Double4x2
{
private:
    weak<const Double4x2> node1;
    weak<const Double> node2;
public:
    Double4x2addDouble(weak<const Double4x2> node1, weak<const Double> node2)
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
ref<Double4x2> operator +(weak<const Double4x2> node1, weak<const Double> node2)
{
    return ref<Double4x2addDouble>::create(gameArena(), node1, node2);
}

class DoublesubDouble4x2 : public Double4x2
{
private:
    weak<const Double> node1;
    weak<const Double4x2> node2;
public:
    DoublesubDouble4x2(weak<const Double> node1, weak<const Double4x2> node2)
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
ref<Double4x2> operator -(weak<const Double> node1, weak<const Double4x2> node2)
{
    return ref<DoublesubDouble4x2>::create(gameArena(), node1, node2);
}

class Double4x2subDouble : public Double4x2
{
private:
    weak<const Double4x2> node1;
    weak<const Double> node2;
public:
    Double4x2subDouble(weak<const Double4x2> node1, weak<const Double> node2)
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
ref<Double4x2> operator -(weak<const Double4x2> node1, weak<const Double> node2)
{
    return ref<Double4x2subDouble>::create(gameArena(), node1, node2);
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

class Double2x3divDouble2x3 : public Double2x3
{
private:
    weak<const Double2x3> node1;
    weak<const Double2x3> node2;
public:
    Double2x3divDouble2x3(weak<const Double2x3> node1, weak<const Double2x3> node2)
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
    return ref<Double2x3divDouble2x3>::create(gameArena(), node1, node2);
}

class Double2x3addDouble2x3 : public Double2x3
{
private:
    weak<const Double2x3> node1;
    weak<const Double2x3> node2;
public:
    Double2x3addDouble2x3(weak<const Double2x3> node1, weak<const Double2x3> node2)
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
    return ref<Double2x3addDouble2x3>::create(gameArena(), node1, node2);
}

class Double2x3subDouble2x3 : public Double2x3
{
private:
    weak<const Double2x3> node1;
    weak<const Double2x3> node2;
public:
    Double2x3subDouble2x3(weak<const Double2x3> node1, weak<const Double2x3> node2)
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
    return ref<Double2x3subDouble2x3>::create(gameArena(), node1, node2);
}

class Double2x3mulDouble2x2 : public Double2x3
{
private:
    weak<const Double2x3> node1;
    weak<const Double2x2> node2;
public:
    Double2x3mulDouble2x2(weak<const Double2x3> node1, weak<const Double2x2> node2)
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
ref<Double2x3> operator *(weak<const Double2x3> node1, weak<const Double2x2> node2)
{
    return ref<Double2x3mulDouble2x2>::create(gameArena(), node1, node2);
}

class Double3x3mulDouble2x3 : public Double2x3
{
private:
    weak<const Double3x3> node1;
    weak<const Double2x3> node2;
public:
    Double3x3mulDouble2x3(weak<const Double3x3> node1, weak<const Double2x3> node2)
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
ref<Double2x3> operator *(weak<const Double3x3> node1, weak<const Double2x3> node2)
{
    return ref<Double3x3mulDouble2x3>::create(gameArena(), node1, node2);
}

class Double4x3mulDouble2x4 : public Double2x3
{
private:
    weak<const Double4x3> node1;
    weak<const Double2x4> node2;
public:
    Double4x3mulDouble2x4(weak<const Double4x3> node1, weak<const Double2x4> node2)
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
ref<Double2x3> operator *(weak<const Double4x3> node1, weak<const Double2x4> node2)
{
    return ref<Double4x3mulDouble2x4>::create(gameArena(), node1, node2);
}

class DoublemulDouble2x3 : public Double2x3
{
private:
    weak<const Double> node1;
    weak<const Double2x3> node2;
public:
    DoublemulDouble2x3(weak<const Double> node1, weak<const Double2x3> node2)
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
ref<Double2x3> operator *(weak<const Double> node1, weak<const Double2x3> node2)
{
    return ref<DoublemulDouble2x3>::create(gameArena(), node1, node2);
}

class Double2x3mulDouble : public Double2x3
{
private:
    weak<const Double2x3> node1;
    weak<const Double> node2;
public:
    Double2x3mulDouble(weak<const Double2x3> node1, weak<const Double> node2)
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
ref<Double2x3> operator *(weak<const Double2x3> node1, weak<const Double> node2)
{
    return ref<Double2x3mulDouble>::create(gameArena(), node1, node2);
}

class DoubledivDouble2x3 : public Double2x3
{
private:
    weak<const Double> node1;
    weak<const Double2x3> node2;
public:
    DoubledivDouble2x3(weak<const Double> node1, weak<const Double2x3> node2)
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
ref<Double2x3> operator /(weak<const Double> node1, weak<const Double2x3> node2)
{
    return ref<DoubledivDouble2x3>::create(gameArena(), node1, node2);
}

class Double2x3divDouble : public Double2x3
{
private:
    weak<const Double2x3> node1;
    weak<const Double> node2;
public:
    Double2x3divDouble(weak<const Double2x3> node1, weak<const Double> node2)
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
ref<Double2x3> operator /(weak<const Double2x3> node1, weak<const Double> node2)
{
    return ref<Double2x3divDouble>::create(gameArena(), node1, node2);
}

class DoubleaddDouble2x3 : public Double2x3
{
private:
    weak<const Double> node1;
    weak<const Double2x3> node2;
public:
    DoubleaddDouble2x3(weak<const Double> node1, weak<const Double2x3> node2)
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
ref<Double2x3> operator +(weak<const Double> node1, weak<const Double2x3> node2)
{
    return ref<DoubleaddDouble2x3>::create(gameArena(), node1, node2);
}

class Double2x3addDouble : public Double2x3
{
private:
    weak<const Double2x3> node1;
    weak<const Double> node2;
public:
    Double2x3addDouble(weak<const Double2x3> node1, weak<const Double> node2)
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
ref<Double2x3> operator +(weak<const Double2x3> node1, weak<const Double> node2)
{
    return ref<Double2x3addDouble>::create(gameArena(), node1, node2);
}

class DoublesubDouble2x3 : public Double2x3
{
private:
    weak<const Double> node1;
    weak<const Double2x3> node2;
public:
    DoublesubDouble2x3(weak<const Double> node1, weak<const Double2x3> node2)
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
ref<Double2x3> operator -(weak<const Double> node1, weak<const Double2x3> node2)
{
    return ref<DoublesubDouble2x3>::create(gameArena(), node1, node2);
}

class Double2x3subDouble : public Double2x3
{
private:
    weak<const Double2x3> node1;
    weak<const Double> node2;
public:
    Double2x3subDouble(weak<const Double2x3> node1, weak<const Double> node2)
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
ref<Double2x3> operator -(weak<const Double2x3> node1, weak<const Double> node2)
{
    return ref<Double2x3subDouble>::create(gameArena(), node1, node2);
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

class Double3x3divDouble3x3 : public Double3x3
{
private:
    weak<const Double3x3> node1;
    weak<const Double3x3> node2;
public:
    Double3x3divDouble3x3(weak<const Double3x3> node1, weak<const Double3x3> node2)
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
    return ref<Double3x3divDouble3x3>::create(gameArena(), node1, node2);
}

class Double3x3addDouble3x3 : public Double3x3
{
private:
    weak<const Double3x3> node1;
    weak<const Double3x3> node2;
public:
    Double3x3addDouble3x3(weak<const Double3x3> node1, weak<const Double3x3> node2)
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
    return ref<Double3x3addDouble3x3>::create(gameArena(), node1, node2);
}

class Double3x3subDouble3x3 : public Double3x3
{
private:
    weak<const Double3x3> node1;
    weak<const Double3x3> node2;
public:
    Double3x3subDouble3x3(weak<const Double3x3> node1, weak<const Double3x3> node2)
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
    return ref<Double3x3subDouble3x3>::create(gameArena(), node1, node2);
}

class Double2x3mulDouble3x2 : public Double3x3
{
private:
    weak<const Double2x3> node1;
    weak<const Double3x2> node2;
public:
    Double2x3mulDouble3x2(weak<const Double2x3> node1, weak<const Double3x2> node2)
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
ref<Double3x3> operator *(weak<const Double2x3> node1, weak<const Double3x2> node2)
{
    return ref<Double2x3mulDouble3x2>::create(gameArena(), node1, node2);
}

class Double3x3mulDouble3x3 : public Double3x3
{
private:
    weak<const Double3x3> node1;
    weak<const Double3x3> node2;
public:
    Double3x3mulDouble3x3(weak<const Double3x3> node1, weak<const Double3x3> node2)
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
    return ref<Double3x3mulDouble3x3>::create(gameArena(), node1, node2);
}

class Double4x3mulDouble3x4 : public Double3x3
{
private:
    weak<const Double4x3> node1;
    weak<const Double3x4> node2;
public:
    Double4x3mulDouble3x4(weak<const Double4x3> node1, weak<const Double3x4> node2)
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
ref<Double3x3> operator *(weak<const Double4x3> node1, weak<const Double3x4> node2)
{
    return ref<Double4x3mulDouble3x4>::create(gameArena(), node1, node2);
}

class DoublemulDouble3x3 : public Double3x3
{
private:
    weak<const Double> node1;
    weak<const Double3x3> node2;
public:
    DoublemulDouble3x3(weak<const Double> node1, weak<const Double3x3> node2)
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
ref<Double3x3> operator *(weak<const Double> node1, weak<const Double3x3> node2)
{
    return ref<DoublemulDouble3x3>::create(gameArena(), node1, node2);
}

class Double3x3mulDouble : public Double3x3
{
private:
    weak<const Double3x3> node1;
    weak<const Double> node2;
public:
    Double3x3mulDouble(weak<const Double3x3> node1, weak<const Double> node2)
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
ref<Double3x3> operator *(weak<const Double3x3> node1, weak<const Double> node2)
{
    return ref<Double3x3mulDouble>::create(gameArena(), node1, node2);
}

class DoubledivDouble3x3 : public Double3x3
{
private:
    weak<const Double> node1;
    weak<const Double3x3> node2;
public:
    DoubledivDouble3x3(weak<const Double> node1, weak<const Double3x3> node2)
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
ref<Double3x3> operator /(weak<const Double> node1, weak<const Double3x3> node2)
{
    return ref<DoubledivDouble3x3>::create(gameArena(), node1, node2);
}

class Double3x3divDouble : public Double3x3
{
private:
    weak<const Double3x3> node1;
    weak<const Double> node2;
public:
    Double3x3divDouble(weak<const Double3x3> node1, weak<const Double> node2)
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
ref<Double3x3> operator /(weak<const Double3x3> node1, weak<const Double> node2)
{
    return ref<Double3x3divDouble>::create(gameArena(), node1, node2);
}

class DoubleaddDouble3x3 : public Double3x3
{
private:
    weak<const Double> node1;
    weak<const Double3x3> node2;
public:
    DoubleaddDouble3x3(weak<const Double> node1, weak<const Double3x3> node2)
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
ref<Double3x3> operator +(weak<const Double> node1, weak<const Double3x3> node2)
{
    return ref<DoubleaddDouble3x3>::create(gameArena(), node1, node2);
}

class Double3x3addDouble : public Double3x3
{
private:
    weak<const Double3x3> node1;
    weak<const Double> node2;
public:
    Double3x3addDouble(weak<const Double3x3> node1, weak<const Double> node2)
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
ref<Double3x3> operator +(weak<const Double3x3> node1, weak<const Double> node2)
{
    return ref<Double3x3addDouble>::create(gameArena(), node1, node2);
}

class DoublesubDouble3x3 : public Double3x3
{
private:
    weak<const Double> node1;
    weak<const Double3x3> node2;
public:
    DoublesubDouble3x3(weak<const Double> node1, weak<const Double3x3> node2)
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
ref<Double3x3> operator -(weak<const Double> node1, weak<const Double3x3> node2)
{
    return ref<DoublesubDouble3x3>::create(gameArena(), node1, node2);
}

class Double3x3subDouble : public Double3x3
{
private:
    weak<const Double3x3> node1;
    weak<const Double> node2;
public:
    Double3x3subDouble(weak<const Double3x3> node1, weak<const Double> node2)
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
ref<Double3x3> operator -(weak<const Double3x3> node1, weak<const Double> node2)
{
    return ref<Double3x3subDouble>::create(gameArena(), node1, node2);
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

class Double4x3divDouble4x3 : public Double4x3
{
private:
    weak<const Double4x3> node1;
    weak<const Double4x3> node2;
public:
    Double4x3divDouble4x3(weak<const Double4x3> node1, weak<const Double4x3> node2)
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
    return ref<Double4x3divDouble4x3>::create(gameArena(), node1, node2);
}

class Double4x3addDouble4x3 : public Double4x3
{
private:
    weak<const Double4x3> node1;
    weak<const Double4x3> node2;
public:
    Double4x3addDouble4x3(weak<const Double4x3> node1, weak<const Double4x3> node2)
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
    return ref<Double4x3addDouble4x3>::create(gameArena(), node1, node2);
}

class Double4x3subDouble4x3 : public Double4x3
{
private:
    weak<const Double4x3> node1;
    weak<const Double4x3> node2;
public:
    Double4x3subDouble4x3(weak<const Double4x3> node1, weak<const Double4x3> node2)
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
    return ref<Double4x3subDouble4x3>::create(gameArena(), node1, node2);
}

class Double2x3mulDouble4x2 : public Double4x3
{
private:
    weak<const Double2x3> node1;
    weak<const Double4x2> node2;
public:
    Double2x3mulDouble4x2(weak<const Double2x3> node1, weak<const Double4x2> node2)
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
ref<Double4x3> operator *(weak<const Double2x3> node1, weak<const Double4x2> node2)
{
    return ref<Double2x3mulDouble4x2>::create(gameArena(), node1, node2);
}

class Double3x3mulDouble4x3 : public Double4x3
{
private:
    weak<const Double3x3> node1;
    weak<const Double4x3> node2;
public:
    Double3x3mulDouble4x3(weak<const Double3x3> node1, weak<const Double4x3> node2)
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
ref<Double4x3> operator *(weak<const Double3x3> node1, weak<const Double4x3> node2)
{
    return ref<Double3x3mulDouble4x3>::create(gameArena(), node1, node2);
}

class Double4x3mulDouble4x4 : public Double4x3
{
private:
    weak<const Double4x3> node1;
    weak<const Double4x4> node2;
public:
    Double4x3mulDouble4x4(weak<const Double4x3> node1, weak<const Double4x4> node2)
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
ref<Double4x3> operator *(weak<const Double4x3> node1, weak<const Double4x4> node2)
{
    return ref<Double4x3mulDouble4x4>::create(gameArena(), node1, node2);
}

class DoublemulDouble4x3 : public Double4x3
{
private:
    weak<const Double> node1;
    weak<const Double4x3> node2;
public:
    DoublemulDouble4x3(weak<const Double> node1, weak<const Double4x3> node2)
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
ref<Double4x3> operator *(weak<const Double> node1, weak<const Double4x3> node2)
{
    return ref<DoublemulDouble4x3>::create(gameArena(), node1, node2);
}

class Double4x3mulDouble : public Double4x3
{
private:
    weak<const Double4x3> node1;
    weak<const Double> node2;
public:
    Double4x3mulDouble(weak<const Double4x3> node1, weak<const Double> node2)
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
ref<Double4x3> operator *(weak<const Double4x3> node1, weak<const Double> node2)
{
    return ref<Double4x3mulDouble>::create(gameArena(), node1, node2);
}

class DoubledivDouble4x3 : public Double4x3
{
private:
    weak<const Double> node1;
    weak<const Double4x3> node2;
public:
    DoubledivDouble4x3(weak<const Double> node1, weak<const Double4x3> node2)
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
ref<Double4x3> operator /(weak<const Double> node1, weak<const Double4x3> node2)
{
    return ref<DoubledivDouble4x3>::create(gameArena(), node1, node2);
}

class Double4x3divDouble : public Double4x3
{
private:
    weak<const Double4x3> node1;
    weak<const Double> node2;
public:
    Double4x3divDouble(weak<const Double4x3> node1, weak<const Double> node2)
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
ref<Double4x3> operator /(weak<const Double4x3> node1, weak<const Double> node2)
{
    return ref<Double4x3divDouble>::create(gameArena(), node1, node2);
}

class DoubleaddDouble4x3 : public Double4x3
{
private:
    weak<const Double> node1;
    weak<const Double4x3> node2;
public:
    DoubleaddDouble4x3(weak<const Double> node1, weak<const Double4x3> node2)
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
ref<Double4x3> operator +(weak<const Double> node1, weak<const Double4x3> node2)
{
    return ref<DoubleaddDouble4x3>::create(gameArena(), node1, node2);
}

class Double4x3addDouble : public Double4x3
{
private:
    weak<const Double4x3> node1;
    weak<const Double> node2;
public:
    Double4x3addDouble(weak<const Double4x3> node1, weak<const Double> node2)
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
ref<Double4x3> operator +(weak<const Double4x3> node1, weak<const Double> node2)
{
    return ref<Double4x3addDouble>::create(gameArena(), node1, node2);
}

class DoublesubDouble4x3 : public Double4x3
{
private:
    weak<const Double> node1;
    weak<const Double4x3> node2;
public:
    DoublesubDouble4x3(weak<const Double> node1, weak<const Double4x3> node2)
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
ref<Double4x3> operator -(weak<const Double> node1, weak<const Double4x3> node2)
{
    return ref<DoublesubDouble4x3>::create(gameArena(), node1, node2);
}

class Double4x3subDouble : public Double4x3
{
private:
    weak<const Double4x3> node1;
    weak<const Double> node2;
public:
    Double4x3subDouble(weak<const Double4x3> node1, weak<const Double> node2)
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
ref<Double4x3> operator -(weak<const Double4x3> node1, weak<const Double> node2)
{
    return ref<Double4x3subDouble>::create(gameArena(), node1, node2);
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

class Double2x4divDouble2x4 : public Double2x4
{
private:
    weak<const Double2x4> node1;
    weak<const Double2x4> node2;
public:
    Double2x4divDouble2x4(weak<const Double2x4> node1, weak<const Double2x4> node2)
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
    return ref<Double2x4divDouble2x4>::create(gameArena(), node1, node2);
}

class Double2x4addDouble2x4 : public Double2x4
{
private:
    weak<const Double2x4> node1;
    weak<const Double2x4> node2;
public:
    Double2x4addDouble2x4(weak<const Double2x4> node1, weak<const Double2x4> node2)
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
    return ref<Double2x4addDouble2x4>::create(gameArena(), node1, node2);
}

class Double2x4subDouble2x4 : public Double2x4
{
private:
    weak<const Double2x4> node1;
    weak<const Double2x4> node2;
public:
    Double2x4subDouble2x4(weak<const Double2x4> node1, weak<const Double2x4> node2)
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
    return ref<Double2x4subDouble2x4>::create(gameArena(), node1, node2);
}

class Double2x4mulDouble2x2 : public Double2x4
{
private:
    weak<const Double2x4> node1;
    weak<const Double2x2> node2;
public:
    Double2x4mulDouble2x2(weak<const Double2x4> node1, weak<const Double2x2> node2)
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
ref<Double2x4> operator *(weak<const Double2x4> node1, weak<const Double2x2> node2)
{
    return ref<Double2x4mulDouble2x2>::create(gameArena(), node1, node2);
}

class Double3x4mulDouble2x3 : public Double2x4
{
private:
    weak<const Double3x4> node1;
    weak<const Double2x3> node2;
public:
    Double3x4mulDouble2x3(weak<const Double3x4> node1, weak<const Double2x3> node2)
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
ref<Double2x4> operator *(weak<const Double3x4> node1, weak<const Double2x3> node2)
{
    return ref<Double3x4mulDouble2x3>::create(gameArena(), node1, node2);
}

class Double4x4mulDouble2x4 : public Double2x4
{
private:
    weak<const Double4x4> node1;
    weak<const Double2x4> node2;
public:
    Double4x4mulDouble2x4(weak<const Double4x4> node1, weak<const Double2x4> node2)
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
ref<Double2x4> operator *(weak<const Double4x4> node1, weak<const Double2x4> node2)
{
    return ref<Double4x4mulDouble2x4>::create(gameArena(), node1, node2);
}

class DoublemulDouble2x4 : public Double2x4
{
private:
    weak<const Double> node1;
    weak<const Double2x4> node2;
public:
    DoublemulDouble2x4(weak<const Double> node1, weak<const Double2x4> node2)
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
ref<Double2x4> operator *(weak<const Double> node1, weak<const Double2x4> node2)
{
    return ref<DoublemulDouble2x4>::create(gameArena(), node1, node2);
}

class Double2x4mulDouble : public Double2x4
{
private:
    weak<const Double2x4> node1;
    weak<const Double> node2;
public:
    Double2x4mulDouble(weak<const Double2x4> node1, weak<const Double> node2)
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
ref<Double2x4> operator *(weak<const Double2x4> node1, weak<const Double> node2)
{
    return ref<Double2x4mulDouble>::create(gameArena(), node1, node2);
}

class DoubledivDouble2x4 : public Double2x4
{
private:
    weak<const Double> node1;
    weak<const Double2x4> node2;
public:
    DoubledivDouble2x4(weak<const Double> node1, weak<const Double2x4> node2)
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
ref<Double2x4> operator /(weak<const Double> node1, weak<const Double2x4> node2)
{
    return ref<DoubledivDouble2x4>::create(gameArena(), node1, node2);
}

class Double2x4divDouble : public Double2x4
{
private:
    weak<const Double2x4> node1;
    weak<const Double> node2;
public:
    Double2x4divDouble(weak<const Double2x4> node1, weak<const Double> node2)
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
ref<Double2x4> operator /(weak<const Double2x4> node1, weak<const Double> node2)
{
    return ref<Double2x4divDouble>::create(gameArena(), node1, node2);
}

class DoubleaddDouble2x4 : public Double2x4
{
private:
    weak<const Double> node1;
    weak<const Double2x4> node2;
public:
    DoubleaddDouble2x4(weak<const Double> node1, weak<const Double2x4> node2)
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
ref<Double2x4> operator +(weak<const Double> node1, weak<const Double2x4> node2)
{
    return ref<DoubleaddDouble2x4>::create(gameArena(), node1, node2);
}

class Double2x4addDouble : public Double2x4
{
private:
    weak<const Double2x4> node1;
    weak<const Double> node2;
public:
    Double2x4addDouble(weak<const Double2x4> node1, weak<const Double> node2)
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
ref<Double2x4> operator +(weak<const Double2x4> node1, weak<const Double> node2)
{
    return ref<Double2x4addDouble>::create(gameArena(), node1, node2);
}

class DoublesubDouble2x4 : public Double2x4
{
private:
    weak<const Double> node1;
    weak<const Double2x4> node2;
public:
    DoublesubDouble2x4(weak<const Double> node1, weak<const Double2x4> node2)
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
ref<Double2x4> operator -(weak<const Double> node1, weak<const Double2x4> node2)
{
    return ref<DoublesubDouble2x4>::create(gameArena(), node1, node2);
}

class Double2x4subDouble : public Double2x4
{
private:
    weak<const Double2x4> node1;
    weak<const Double> node2;
public:
    Double2x4subDouble(weak<const Double2x4> node1, weak<const Double> node2)
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
ref<Double2x4> operator -(weak<const Double2x4> node1, weak<const Double> node2)
{
    return ref<Double2x4subDouble>::create(gameArena(), node1, node2);
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

class Double3x4divDouble3x4 : public Double3x4
{
private:
    weak<const Double3x4> node1;
    weak<const Double3x4> node2;
public:
    Double3x4divDouble3x4(weak<const Double3x4> node1, weak<const Double3x4> node2)
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
    return ref<Double3x4divDouble3x4>::create(gameArena(), node1, node2);
}

class Double3x4addDouble3x4 : public Double3x4
{
private:
    weak<const Double3x4> node1;
    weak<const Double3x4> node2;
public:
    Double3x4addDouble3x4(weak<const Double3x4> node1, weak<const Double3x4> node2)
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
    return ref<Double3x4addDouble3x4>::create(gameArena(), node1, node2);
}

class Double3x4subDouble3x4 : public Double3x4
{
private:
    weak<const Double3x4> node1;
    weak<const Double3x4> node2;
public:
    Double3x4subDouble3x4(weak<const Double3x4> node1, weak<const Double3x4> node2)
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
    return ref<Double3x4subDouble3x4>::create(gameArena(), node1, node2);
}

class Double2x4mulDouble3x2 : public Double3x4
{
private:
    weak<const Double2x4> node1;
    weak<const Double3x2> node2;
public:
    Double2x4mulDouble3x2(weak<const Double2x4> node1, weak<const Double3x2> node2)
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
ref<Double3x4> operator *(weak<const Double2x4> node1, weak<const Double3x2> node2)
{
    return ref<Double2x4mulDouble3x2>::create(gameArena(), node1, node2);
}

class Double3x4mulDouble3x3 : public Double3x4
{
private:
    weak<const Double3x4> node1;
    weak<const Double3x3> node2;
public:
    Double3x4mulDouble3x3(weak<const Double3x4> node1, weak<const Double3x3> node2)
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
ref<Double3x4> operator *(weak<const Double3x4> node1, weak<const Double3x3> node2)
{
    return ref<Double3x4mulDouble3x3>::create(gameArena(), node1, node2);
}

class Double4x4mulDouble3x4 : public Double3x4
{
private:
    weak<const Double4x4> node1;
    weak<const Double3x4> node2;
public:
    Double4x4mulDouble3x4(weak<const Double4x4> node1, weak<const Double3x4> node2)
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
ref<Double3x4> operator *(weak<const Double4x4> node1, weak<const Double3x4> node2)
{
    return ref<Double4x4mulDouble3x4>::create(gameArena(), node1, node2);
}

class DoublemulDouble3x4 : public Double3x4
{
private:
    weak<const Double> node1;
    weak<const Double3x4> node2;
public:
    DoublemulDouble3x4(weak<const Double> node1, weak<const Double3x4> node2)
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
ref<Double3x4> operator *(weak<const Double> node1, weak<const Double3x4> node2)
{
    return ref<DoublemulDouble3x4>::create(gameArena(), node1, node2);
}

class Double3x4mulDouble : public Double3x4
{
private:
    weak<const Double3x4> node1;
    weak<const Double> node2;
public:
    Double3x4mulDouble(weak<const Double3x4> node1, weak<const Double> node2)
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
ref<Double3x4> operator *(weak<const Double3x4> node1, weak<const Double> node2)
{
    return ref<Double3x4mulDouble>::create(gameArena(), node1, node2);
}

class DoubledivDouble3x4 : public Double3x4
{
private:
    weak<const Double> node1;
    weak<const Double3x4> node2;
public:
    DoubledivDouble3x4(weak<const Double> node1, weak<const Double3x4> node2)
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
ref<Double3x4> operator /(weak<const Double> node1, weak<const Double3x4> node2)
{
    return ref<DoubledivDouble3x4>::create(gameArena(), node1, node2);
}

class Double3x4divDouble : public Double3x4
{
private:
    weak<const Double3x4> node1;
    weak<const Double> node2;
public:
    Double3x4divDouble(weak<const Double3x4> node1, weak<const Double> node2)
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
ref<Double3x4> operator /(weak<const Double3x4> node1, weak<const Double> node2)
{
    return ref<Double3x4divDouble>::create(gameArena(), node1, node2);
}

class DoubleaddDouble3x4 : public Double3x4
{
private:
    weak<const Double> node1;
    weak<const Double3x4> node2;
public:
    DoubleaddDouble3x4(weak<const Double> node1, weak<const Double3x4> node2)
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
ref<Double3x4> operator +(weak<const Double> node1, weak<const Double3x4> node2)
{
    return ref<DoubleaddDouble3x4>::create(gameArena(), node1, node2);
}

class Double3x4addDouble : public Double3x4
{
private:
    weak<const Double3x4> node1;
    weak<const Double> node2;
public:
    Double3x4addDouble(weak<const Double3x4> node1, weak<const Double> node2)
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
ref<Double3x4> operator +(weak<const Double3x4> node1, weak<const Double> node2)
{
    return ref<Double3x4addDouble>::create(gameArena(), node1, node2);
}

class DoublesubDouble3x4 : public Double3x4
{
private:
    weak<const Double> node1;
    weak<const Double3x4> node2;
public:
    DoublesubDouble3x4(weak<const Double> node1, weak<const Double3x4> node2)
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
ref<Double3x4> operator -(weak<const Double> node1, weak<const Double3x4> node2)
{
    return ref<DoublesubDouble3x4>::create(gameArena(), node1, node2);
}

class Double3x4subDouble : public Double3x4
{
private:
    weak<const Double3x4> node1;
    weak<const Double> node2;
public:
    Double3x4subDouble(weak<const Double3x4> node1, weak<const Double> node2)
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
ref<Double3x4> operator -(weak<const Double3x4> node1, weak<const Double> node2)
{
    return ref<Double3x4subDouble>::create(gameArena(), node1, node2);
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

class Double4x4divDouble4x4 : public Double4x4
{
private:
    weak<const Double4x4> node1;
    weak<const Double4x4> node2;
public:
    Double4x4divDouble4x4(weak<const Double4x4> node1, weak<const Double4x4> node2)
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
    return ref<Double4x4divDouble4x4>::create(gameArena(), node1, node2);
}

class Double4x4addDouble4x4 : public Double4x4
{
private:
    weak<const Double4x4> node1;
    weak<const Double4x4> node2;
public:
    Double4x4addDouble4x4(weak<const Double4x4> node1, weak<const Double4x4> node2)
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
    return ref<Double4x4addDouble4x4>::create(gameArena(), node1, node2);
}

class Double4x4subDouble4x4 : public Double4x4
{
private:
    weak<const Double4x4> node1;
    weak<const Double4x4> node2;
public:
    Double4x4subDouble4x4(weak<const Double4x4> node1, weak<const Double4x4> node2)
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
    return ref<Double4x4subDouble4x4>::create(gameArena(), node1, node2);
}

class Double2x4mulDouble4x2 : public Double4x4
{
private:
    weak<const Double2x4> node1;
    weak<const Double4x2> node2;
public:
    Double2x4mulDouble4x2(weak<const Double2x4> node1, weak<const Double4x2> node2)
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
ref<Double4x4> operator *(weak<const Double2x4> node1, weak<const Double4x2> node2)
{
    return ref<Double2x4mulDouble4x2>::create(gameArena(), node1, node2);
}

class Double3x4mulDouble4x3 : public Double4x4
{
private:
    weak<const Double3x4> node1;
    weak<const Double4x3> node2;
public:
    Double3x4mulDouble4x3(weak<const Double3x4> node1, weak<const Double4x3> node2)
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
ref<Double4x4> operator *(weak<const Double3x4> node1, weak<const Double4x3> node2)
{
    return ref<Double3x4mulDouble4x3>::create(gameArena(), node1, node2);
}

class Double4x4mulDouble4x4 : public Double4x4
{
private:
    weak<const Double4x4> node1;
    weak<const Double4x4> node2;
public:
    Double4x4mulDouble4x4(weak<const Double4x4> node1, weak<const Double4x4> node2)
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
    return ref<Double4x4mulDouble4x4>::create(gameArena(), node1, node2);
}

class DoublemulDouble4x4 : public Double4x4
{
private:
    weak<const Double> node1;
    weak<const Double4x4> node2;
public:
    DoublemulDouble4x4(weak<const Double> node1, weak<const Double4x4> node2)
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
ref<Double4x4> operator *(weak<const Double> node1, weak<const Double4x4> node2)
{
    return ref<DoublemulDouble4x4>::create(gameArena(), node1, node2);
}

class Double4x4mulDouble : public Double4x4
{
private:
    weak<const Double4x4> node1;
    weak<const Double> node2;
public:
    Double4x4mulDouble(weak<const Double4x4> node1, weak<const Double> node2)
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
ref<Double4x4> operator *(weak<const Double4x4> node1, weak<const Double> node2)
{
    return ref<Double4x4mulDouble>::create(gameArena(), node1, node2);
}

class DoubledivDouble4x4 : public Double4x4
{
private:
    weak<const Double> node1;
    weak<const Double4x4> node2;
public:
    DoubledivDouble4x4(weak<const Double> node1, weak<const Double4x4> node2)
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
ref<Double4x4> operator /(weak<const Double> node1, weak<const Double4x4> node2)
{
    return ref<DoubledivDouble4x4>::create(gameArena(), node1, node2);
}

class Double4x4divDouble : public Double4x4
{
private:
    weak<const Double4x4> node1;
    weak<const Double> node2;
public:
    Double4x4divDouble(weak<const Double4x4> node1, weak<const Double> node2)
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
ref<Double4x4> operator /(weak<const Double4x4> node1, weak<const Double> node2)
{
    return ref<Double4x4divDouble>::create(gameArena(), node1, node2);
}

class DoubleaddDouble4x4 : public Double4x4
{
private:
    weak<const Double> node1;
    weak<const Double4x4> node2;
public:
    DoubleaddDouble4x4(weak<const Double> node1, weak<const Double4x4> node2)
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
ref<Double4x4> operator +(weak<const Double> node1, weak<const Double4x4> node2)
{
    return ref<DoubleaddDouble4x4>::create(gameArena(), node1, node2);
}

class Double4x4addDouble : public Double4x4
{
private:
    weak<const Double4x4> node1;
    weak<const Double> node2;
public:
    Double4x4addDouble(weak<const Double4x4> node1, weak<const Double> node2)
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
ref<Double4x4> operator +(weak<const Double4x4> node1, weak<const Double> node2)
{
    return ref<Double4x4addDouble>::create(gameArena(), node1, node2);
}

class DoublesubDouble4x4 : public Double4x4
{
private:
    weak<const Double> node1;
    weak<const Double4x4> node2;
public:
    DoublesubDouble4x4(weak<const Double> node1, weak<const Double4x4> node2)
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
ref<Double4x4> operator -(weak<const Double> node1, weak<const Double4x4> node2)
{
    return ref<DoublesubDouble4x4>::create(gameArena(), node1, node2);
}

class Double4x4subDouble : public Double4x4
{
private:
    weak<const Double4x4> node1;
    weak<const Double> node2;
public:
    Double4x4subDouble(weak<const Double4x4> node1, weak<const Double> node2)
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
ref<Double4x4> operator -(weak<const Double4x4> node1, weak<const Double> node2)
{
    return ref<Double4x4subDouble>::create(gameArena(), node1, node2);
}




}}
