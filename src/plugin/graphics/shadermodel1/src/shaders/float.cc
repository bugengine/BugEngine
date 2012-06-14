/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <shadermodel1/stdafx.h>
#include    <shadermodel1/shaders/float.script.hh>
#include    <3d/shader/ishaderbuilder.hh>

namespace BugEngine { namespace FloatVariable
{
/* Type *************************************************************/
Float::Float()
{
}
Float::~Float()
{
}

/* Uniform **********************************************************/
FloatUniform::FloatUniform(const istring& name)
    :   name(name)
{
}
FloatUniform::~FloatUniform()
{
}
void FloatUniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Float);
}
void FloatUniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
FloatAttribute::FloatAttribute()
{
}
FloatAttribute::~FloatAttribute()
{
}
void FloatAttribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Float);
}
void FloatAttribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
FloatVarying::FloatVarying(weak<const Float> node)
    :   node(node)
{
}
FloatVarying::~FloatVarying()
{
}
void FloatVarying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Float);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void FloatVarying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class FloatdivFloat : public Float
{
private:
    weak<const Float> node1;
    weak<const Float> node2;
public:
    FloatdivFloat(weak<const Float> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float, node1, node2);
        }
    }
};
ref<Float> operator /(weak<const Float> node1, weak<const Float> node2)
{
    return ref<FloatdivFloat>::create(Arena::script(), node1, node2);
}

class FloataddFloat : public Float
{
private:
    weak<const Float> node1;
    weak<const Float> node2;
public:
    FloataddFloat(weak<const Float> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float, node1, node2);
        }
    }
};
ref<Float> operator +(weak<const Float> node1, weak<const Float> node2)
{
    return ref<FloataddFloat>::create(Arena::script(), node1, node2);
}

class FloatsubFloat : public Float
{
private:
    weak<const Float> node1;
    weak<const Float> node2;
public:
    FloatsubFloat(weak<const Float> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float, node1, node2);
        }
    }
};
ref<Float> operator -(weak<const Float> node1, weak<const Float> node2)
{
    return ref<FloatsubFloat>::create(Arena::script(), node1, node2);
}




/* Type *************************************************************/
Float2::Float2()
{
}
Float2::~Float2()
{
}

/* Uniform **********************************************************/
Float2Uniform::Float2Uniform(const istring& name)
    :   name(name)
{
}
Float2Uniform::~Float2Uniform()
{
}
void Float2Uniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Float2);
}
void Float2Uniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float2Attribute::Float2Attribute()
{
}
Float2Attribute::~Float2Attribute()
{
}
void Float2Attribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Float2);
}
void Float2Attribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float2Varying::Float2Varying(weak<const Float2> node)
    :   node(node)
{
}
Float2Varying::~Float2Varying()
{
}
void Float2Varying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Float2);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void Float2Varying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class Float2divFloat2 : public Float2
{
private:
    weak<const Float2> node1;
    weak<const Float2> node2;
public:
    Float2divFloat2(weak<const Float2> node1, weak<const Float2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float2, node1, node2);
        }
    }
};
ref<Float2> operator /(weak<const Float2> node1, weak<const Float2> node2)
{
    return ref<Float2divFloat2>::create(Arena::script(), node1, node2);
}

class Float2addFloat2 : public Float2
{
private:
    weak<const Float2> node1;
    weak<const Float2> node2;
public:
    Float2addFloat2(weak<const Float2> node1, weak<const Float2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float2, node1, node2);
        }
    }
};
ref<Float2> operator +(weak<const Float2> node1, weak<const Float2> node2)
{
    return ref<Float2addFloat2>::create(Arena::script(), node1, node2);
}

class Float2subFloat2 : public Float2
{
private:
    weak<const Float2> node1;
    weak<const Float2> node2;
public:
    Float2subFloat2(weak<const Float2> node1, weak<const Float2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float2, node1, node2);
        }
    }
};
ref<Float2> operator -(weak<const Float2> node1, weak<const Float2> node2)
{
    return ref<Float2subFloat2>::create(Arena::script(), node1, node2);
}

class Float2mulFloat2x2 : public Float2
{
private:
    weak<const Float2> node1;
    weak<const Float2x2> node2;
public:
    Float2mulFloat2x2(weak<const Float2> node1, weak<const Float2x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2, node1, node2);
        }
    }
};
ref<Float2> operator *(weak<const Float2> node1, weak<const Float2x2> node2)
{
    return ref<Float2mulFloat2x2>::create(Arena::script(), node1, node2);
}

class Float2x2mulFloat2 : public Float2
{
private:
    weak<const Float2x2> node1;
    weak<const Float2> node2;
public:
    Float2x2mulFloat2(weak<const Float2x2> node1, weak<const Float2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2, node1, node2);
        }
    }
};
ref<Float2> operator *(weak<const Float2x2> node1, weak<const Float2> node2)
{
    return ref<Float2x2mulFloat2>::create(Arena::script(), node1, node2);
}

class FloatmulFloat2 : public Float2
{
private:
    weak<const Float> node1;
    weak<const Float2> node2;
public:
    FloatmulFloat2(weak<const Float> node1, weak<const Float2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2, node1, node2);
        }
    }
};
ref<Float2> operator *(weak<const Float> node1, weak<const Float2> node2)
{
    return ref<FloatmulFloat2>::create(Arena::script(), node1, node2);
}

class Float2mulFloat : public Float2
{
private:
    weak<const Float2> node1;
    weak<const Float> node2;
public:
    Float2mulFloat(weak<const Float2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2, node1, node2);
        }
    }
};
ref<Float2> operator *(weak<const Float2> node1, weak<const Float> node2)
{
    return ref<Float2mulFloat>::create(Arena::script(), node1, node2);
}

class FloatdivFloat2 : public Float2
{
private:
    weak<const Float> node1;
    weak<const Float2> node2;
public:
    FloatdivFloat2(weak<const Float> node1, weak<const Float2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float2, node1, node2);
        }
    }
};
ref<Float2> operator /(weak<const Float> node1, weak<const Float2> node2)
{
    return ref<FloatdivFloat2>::create(Arena::script(), node1, node2);
}

class Float2divFloat : public Float2
{
private:
    weak<const Float2> node1;
    weak<const Float> node2;
public:
    Float2divFloat(weak<const Float2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float2, node1, node2);
        }
    }
};
ref<Float2> operator /(weak<const Float2> node1, weak<const Float> node2)
{
    return ref<Float2divFloat>::create(Arena::script(), node1, node2);
}

class FloataddFloat2 : public Float2
{
private:
    weak<const Float> node1;
    weak<const Float2> node2;
public:
    FloataddFloat2(weak<const Float> node1, weak<const Float2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float2, node1, node2);
        }
    }
};
ref<Float2> operator +(weak<const Float> node1, weak<const Float2> node2)
{
    return ref<FloataddFloat2>::create(Arena::script(), node1, node2);
}

class Float2addFloat : public Float2
{
private:
    weak<const Float2> node1;
    weak<const Float> node2;
public:
    Float2addFloat(weak<const Float2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float2, node1, node2);
        }
    }
};
ref<Float2> operator +(weak<const Float2> node1, weak<const Float> node2)
{
    return ref<Float2addFloat>::create(Arena::script(), node1, node2);
}

class FloatsubFloat2 : public Float2
{
private:
    weak<const Float> node1;
    weak<const Float2> node2;
public:
    FloatsubFloat2(weak<const Float> node1, weak<const Float2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float2, node1, node2);
        }
    }
};
ref<Float2> operator -(weak<const Float> node1, weak<const Float2> node2)
{
    return ref<FloatsubFloat2>::create(Arena::script(), node1, node2);
}

class Float2subFloat : public Float2
{
private:
    weak<const Float2> node1;
    weak<const Float> node2;
public:
    Float2subFloat(weak<const Float2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float2, node1, node2);
        }
    }
};
ref<Float2> operator -(weak<const Float2> node1, weak<const Float> node2)
{
    return ref<Float2subFloat>::create(Arena::script(), node1, node2);
}




/* Type *************************************************************/
Float3::Float3()
{
}
Float3::~Float3()
{
}

/* Uniform **********************************************************/
Float3Uniform::Float3Uniform(const istring& name)
    :   name(name)
{
}
Float3Uniform::~Float3Uniform()
{
}
void Float3Uniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Float3);
}
void Float3Uniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float3Attribute::Float3Attribute()
{
}
Float3Attribute::~Float3Attribute()
{
}
void Float3Attribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Float3);
}
void Float3Attribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float3Varying::Float3Varying(weak<const Float3> node)
    :   node(node)
{
}
Float3Varying::~Float3Varying()
{
}
void Float3Varying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Float3);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void Float3Varying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class Float3divFloat3 : public Float3
{
private:
    weak<const Float3> node1;
    weak<const Float3> node2;
public:
    Float3divFloat3(weak<const Float3> node1, weak<const Float3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float3, node1, node2);
        }
    }
};
ref<Float3> operator /(weak<const Float3> node1, weak<const Float3> node2)
{
    return ref<Float3divFloat3>::create(Arena::script(), node1, node2);
}

class Float3addFloat3 : public Float3
{
private:
    weak<const Float3> node1;
    weak<const Float3> node2;
public:
    Float3addFloat3(weak<const Float3> node1, weak<const Float3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float3, node1, node2);
        }
    }
};
ref<Float3> operator +(weak<const Float3> node1, weak<const Float3> node2)
{
    return ref<Float3addFloat3>::create(Arena::script(), node1, node2);
}

class Float3subFloat3 : public Float3
{
private:
    weak<const Float3> node1;
    weak<const Float3> node2;
public:
    Float3subFloat3(weak<const Float3> node1, weak<const Float3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float3, node1, node2);
        }
    }
};
ref<Float3> operator -(weak<const Float3> node1, weak<const Float3> node2)
{
    return ref<Float3subFloat3>::create(Arena::script(), node1, node2);
}

class Float3mulFloat3x3 : public Float3
{
private:
    weak<const Float3> node1;
    weak<const Float3x3> node2;
public:
    Float3mulFloat3x3(weak<const Float3> node1, weak<const Float3x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3, node1, node2);
        }
    }
};
ref<Float3> operator *(weak<const Float3> node1, weak<const Float3x3> node2)
{
    return ref<Float3mulFloat3x3>::create(Arena::script(), node1, node2);
}

class Float3x3mulFloat3 : public Float3
{
private:
    weak<const Float3x3> node1;
    weak<const Float3> node2;
public:
    Float3x3mulFloat3(weak<const Float3x3> node1, weak<const Float3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3, node1, node2);
        }
    }
};
ref<Float3> operator *(weak<const Float3x3> node1, weak<const Float3> node2)
{
    return ref<Float3x3mulFloat3>::create(Arena::script(), node1, node2);
}

class FloatmulFloat3 : public Float3
{
private:
    weak<const Float> node1;
    weak<const Float3> node2;
public:
    FloatmulFloat3(weak<const Float> node1, weak<const Float3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3, node1, node2);
        }
    }
};
ref<Float3> operator *(weak<const Float> node1, weak<const Float3> node2)
{
    return ref<FloatmulFloat3>::create(Arena::script(), node1, node2);
}

class Float3mulFloat : public Float3
{
private:
    weak<const Float3> node1;
    weak<const Float> node2;
public:
    Float3mulFloat(weak<const Float3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3, node1, node2);
        }
    }
};
ref<Float3> operator *(weak<const Float3> node1, weak<const Float> node2)
{
    return ref<Float3mulFloat>::create(Arena::script(), node1, node2);
}

class FloatdivFloat3 : public Float3
{
private:
    weak<const Float> node1;
    weak<const Float3> node2;
public:
    FloatdivFloat3(weak<const Float> node1, weak<const Float3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float3, node1, node2);
        }
    }
};
ref<Float3> operator /(weak<const Float> node1, weak<const Float3> node2)
{
    return ref<FloatdivFloat3>::create(Arena::script(), node1, node2);
}

class Float3divFloat : public Float3
{
private:
    weak<const Float3> node1;
    weak<const Float> node2;
public:
    Float3divFloat(weak<const Float3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float3, node1, node2);
        }
    }
};
ref<Float3> operator /(weak<const Float3> node1, weak<const Float> node2)
{
    return ref<Float3divFloat>::create(Arena::script(), node1, node2);
}

class FloataddFloat3 : public Float3
{
private:
    weak<const Float> node1;
    weak<const Float3> node2;
public:
    FloataddFloat3(weak<const Float> node1, weak<const Float3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float3, node1, node2);
        }
    }
};
ref<Float3> operator +(weak<const Float> node1, weak<const Float3> node2)
{
    return ref<FloataddFloat3>::create(Arena::script(), node1, node2);
}

class Float3addFloat : public Float3
{
private:
    weak<const Float3> node1;
    weak<const Float> node2;
public:
    Float3addFloat(weak<const Float3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float3, node1, node2);
        }
    }
};
ref<Float3> operator +(weak<const Float3> node1, weak<const Float> node2)
{
    return ref<Float3addFloat>::create(Arena::script(), node1, node2);
}

class FloatsubFloat3 : public Float3
{
private:
    weak<const Float> node1;
    weak<const Float3> node2;
public:
    FloatsubFloat3(weak<const Float> node1, weak<const Float3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float3, node1, node2);
        }
    }
};
ref<Float3> operator -(weak<const Float> node1, weak<const Float3> node2)
{
    return ref<FloatsubFloat3>::create(Arena::script(), node1, node2);
}

class Float3subFloat : public Float3
{
private:
    weak<const Float3> node1;
    weak<const Float> node2;
public:
    Float3subFloat(weak<const Float3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float3, node1, node2);
        }
    }
};
ref<Float3> operator -(weak<const Float3> node1, weak<const Float> node2)
{
    return ref<Float3subFloat>::create(Arena::script(), node1, node2);
}




/* Type *************************************************************/
Float4::Float4()
{
}
Float4::~Float4()
{
}

/* Uniform **********************************************************/
Float4Uniform::Float4Uniform(const istring& name)
    :   name(name)
{
}
Float4Uniform::~Float4Uniform()
{
}
void Float4Uniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Float4);
}
void Float4Uniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float4Attribute::Float4Attribute()
{
}
Float4Attribute::~Float4Attribute()
{
}
void Float4Attribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Float4);
}
void Float4Attribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float4Varying::Float4Varying(weak<const Float4> node)
    :   node(node)
{
}
Float4Varying::~Float4Varying()
{
}
void Float4Varying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Float4);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void Float4Varying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class Float4divFloat4 : public Float4
{
private:
    weak<const Float4> node1;
    weak<const Float4> node2;
public:
    Float4divFloat4(weak<const Float4> node1, weak<const Float4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float4, node1, node2);
        }
    }
};
ref<Float4> operator /(weak<const Float4> node1, weak<const Float4> node2)
{
    return ref<Float4divFloat4>::create(Arena::script(), node1, node2);
}

class Float4addFloat4 : public Float4
{
private:
    weak<const Float4> node1;
    weak<const Float4> node2;
public:
    Float4addFloat4(weak<const Float4> node1, weak<const Float4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float4, node1, node2);
        }
    }
};
ref<Float4> operator +(weak<const Float4> node1, weak<const Float4> node2)
{
    return ref<Float4addFloat4>::create(Arena::script(), node1, node2);
}

class Float4subFloat4 : public Float4
{
private:
    weak<const Float4> node1;
    weak<const Float4> node2;
public:
    Float4subFloat4(weak<const Float4> node1, weak<const Float4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float4, node1, node2);
        }
    }
};
ref<Float4> operator -(weak<const Float4> node1, weak<const Float4> node2)
{
    return ref<Float4subFloat4>::create(Arena::script(), node1, node2);
}

class Float4mulFloat4x4 : public Float4
{
private:
    weak<const Float4> node1;
    weak<const Float4x4> node2;
public:
    Float4mulFloat4x4(weak<const Float4> node1, weak<const Float4x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4, node1, node2);
        }
    }
};
ref<Float4> operator *(weak<const Float4> node1, weak<const Float4x4> node2)
{
    return ref<Float4mulFloat4x4>::create(Arena::script(), node1, node2);
}

class Float4x4mulFloat4 : public Float4
{
private:
    weak<const Float4x4> node1;
    weak<const Float4> node2;
public:
    Float4x4mulFloat4(weak<const Float4x4> node1, weak<const Float4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4, node1, node2);
        }
    }
};
ref<Float4> operator *(weak<const Float4x4> node1, weak<const Float4> node2)
{
    return ref<Float4x4mulFloat4>::create(Arena::script(), node1, node2);
}

class FloatmulFloat4 : public Float4
{
private:
    weak<const Float> node1;
    weak<const Float4> node2;
public:
    FloatmulFloat4(weak<const Float> node1, weak<const Float4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4, node1, node2);
        }
    }
};
ref<Float4> operator *(weak<const Float> node1, weak<const Float4> node2)
{
    return ref<FloatmulFloat4>::create(Arena::script(), node1, node2);
}

class Float4mulFloat : public Float4
{
private:
    weak<const Float4> node1;
    weak<const Float> node2;
public:
    Float4mulFloat(weak<const Float4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4, node1, node2);
        }
    }
};
ref<Float4> operator *(weak<const Float4> node1, weak<const Float> node2)
{
    return ref<Float4mulFloat>::create(Arena::script(), node1, node2);
}

class FloatdivFloat4 : public Float4
{
private:
    weak<const Float> node1;
    weak<const Float4> node2;
public:
    FloatdivFloat4(weak<const Float> node1, weak<const Float4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float4, node1, node2);
        }
    }
};
ref<Float4> operator /(weak<const Float> node1, weak<const Float4> node2)
{
    return ref<FloatdivFloat4>::create(Arena::script(), node1, node2);
}

class Float4divFloat : public Float4
{
private:
    weak<const Float4> node1;
    weak<const Float> node2;
public:
    Float4divFloat(weak<const Float4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float4, node1, node2);
        }
    }
};
ref<Float4> operator /(weak<const Float4> node1, weak<const Float> node2)
{
    return ref<Float4divFloat>::create(Arena::script(), node1, node2);
}

class FloataddFloat4 : public Float4
{
private:
    weak<const Float> node1;
    weak<const Float4> node2;
public:
    FloataddFloat4(weak<const Float> node1, weak<const Float4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float4, node1, node2);
        }
    }
};
ref<Float4> operator +(weak<const Float> node1, weak<const Float4> node2)
{
    return ref<FloataddFloat4>::create(Arena::script(), node1, node2);
}

class Float4addFloat : public Float4
{
private:
    weak<const Float4> node1;
    weak<const Float> node2;
public:
    Float4addFloat(weak<const Float4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float4, node1, node2);
        }
    }
};
ref<Float4> operator +(weak<const Float4> node1, weak<const Float> node2)
{
    return ref<Float4addFloat>::create(Arena::script(), node1, node2);
}

class FloatsubFloat4 : public Float4
{
private:
    weak<const Float> node1;
    weak<const Float4> node2;
public:
    FloatsubFloat4(weak<const Float> node1, weak<const Float4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float4, node1, node2);
        }
    }
};
ref<Float4> operator -(weak<const Float> node1, weak<const Float4> node2)
{
    return ref<FloatsubFloat4>::create(Arena::script(), node1, node2);
}

class Float4subFloat : public Float4
{
private:
    weak<const Float4> node1;
    weak<const Float> node2;
public:
    Float4subFloat(weak<const Float4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float4, node1, node2);
        }
    }
};
ref<Float4> operator -(weak<const Float4> node1, weak<const Float> node2)
{
    return ref<Float4subFloat>::create(Arena::script(), node1, node2);
}




/* Type *************************************************************/
Float2x2::Float2x2()
{
}
Float2x2::~Float2x2()
{
}

/* Uniform **********************************************************/
Float2x2Uniform::Float2x2Uniform(const istring& name)
    :   name(name)
{
}
Float2x2Uniform::~Float2x2Uniform()
{
}
void Float2x2Uniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Float2x2);
}
void Float2x2Uniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float2x2Attribute::Float2x2Attribute()
{
}
Float2x2Attribute::~Float2x2Attribute()
{
}
void Float2x2Attribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Float2x2);
}
void Float2x2Attribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float2x2Varying::Float2x2Varying(weak<const Float2x2> node)
    :   node(node)
{
}
Float2x2Varying::~Float2x2Varying()
{
}
void Float2x2Varying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Float2x2);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void Float2x2Varying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class Float2x2divFloat2x2 : public Float2x2
{
private:
    weak<const Float2x2> node1;
    weak<const Float2x2> node2;
public:
    Float2x2divFloat2x2(weak<const Float2x2> node1, weak<const Float2x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float2x2, node1, node2);
        }
    }
};
ref<Float2x2> operator /(weak<const Float2x2> node1, weak<const Float2x2> node2)
{
    return ref<Float2x2divFloat2x2>::create(Arena::script(), node1, node2);
}

class Float2x2addFloat2x2 : public Float2x2
{
private:
    weak<const Float2x2> node1;
    weak<const Float2x2> node2;
public:
    Float2x2addFloat2x2(weak<const Float2x2> node1, weak<const Float2x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float2x2, node1, node2);
        }
    }
};
ref<Float2x2> operator +(weak<const Float2x2> node1, weak<const Float2x2> node2)
{
    return ref<Float2x2addFloat2x2>::create(Arena::script(), node1, node2);
}

class Float2x2subFloat2x2 : public Float2x2
{
private:
    weak<const Float2x2> node1;
    weak<const Float2x2> node2;
public:
    Float2x2subFloat2x2(weak<const Float2x2> node1, weak<const Float2x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float2x2, node1, node2);
        }
    }
};
ref<Float2x2> operator -(weak<const Float2x2> node1, weak<const Float2x2> node2)
{
    return ref<Float2x2subFloat2x2>::create(Arena::script(), node1, node2);
}

class Float2x2mulFloat2x2 : public Float2x2
{
private:
    weak<const Float2x2> node1;
    weak<const Float2x2> node2;
public:
    Float2x2mulFloat2x2(weak<const Float2x2> node1, weak<const Float2x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2x2, node1, node2);
        }
    }
};
ref<Float2x2> operator *(weak<const Float2x2> node1, weak<const Float2x2> node2)
{
    return ref<Float2x2mulFloat2x2>::create(Arena::script(), node1, node2);
}

class Float3x2mulFloat2x3 : public Float2x2
{
private:
    weak<const Float3x2> node1;
    weak<const Float2x3> node2;
public:
    Float3x2mulFloat2x3(weak<const Float3x2> node1, weak<const Float2x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2x2, node1, node2);
        }
    }
};
ref<Float2x2> operator *(weak<const Float3x2> node1, weak<const Float2x3> node2)
{
    return ref<Float3x2mulFloat2x3>::create(Arena::script(), node1, node2);
}

class Float4x2mulFloat2x4 : public Float2x2
{
private:
    weak<const Float4x2> node1;
    weak<const Float2x4> node2;
public:
    Float4x2mulFloat2x4(weak<const Float4x2> node1, weak<const Float2x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2x2, node1, node2);
        }
    }
};
ref<Float2x2> operator *(weak<const Float4x2> node1, weak<const Float2x4> node2)
{
    return ref<Float4x2mulFloat2x4>::create(Arena::script(), node1, node2);
}

class FloatmulFloat2x2 : public Float2x2
{
private:
    weak<const Float> node1;
    weak<const Float2x2> node2;
public:
    FloatmulFloat2x2(weak<const Float> node1, weak<const Float2x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2x2, node1, node2);
        }
    }
};
ref<Float2x2> operator *(weak<const Float> node1, weak<const Float2x2> node2)
{
    return ref<FloatmulFloat2x2>::create(Arena::script(), node1, node2);
}

class Float2x2mulFloat : public Float2x2
{
private:
    weak<const Float2x2> node1;
    weak<const Float> node2;
public:
    Float2x2mulFloat(weak<const Float2x2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2x2, node1, node2);
        }
    }
};
ref<Float2x2> operator *(weak<const Float2x2> node1, weak<const Float> node2)
{
    return ref<Float2x2mulFloat>::create(Arena::script(), node1, node2);
}

class FloatdivFloat2x2 : public Float2x2
{
private:
    weak<const Float> node1;
    weak<const Float2x2> node2;
public:
    FloatdivFloat2x2(weak<const Float> node1, weak<const Float2x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float2x2, node1, node2);
        }
    }
};
ref<Float2x2> operator /(weak<const Float> node1, weak<const Float2x2> node2)
{
    return ref<FloatdivFloat2x2>::create(Arena::script(), node1, node2);
}

class Float2x2divFloat : public Float2x2
{
private:
    weak<const Float2x2> node1;
    weak<const Float> node2;
public:
    Float2x2divFloat(weak<const Float2x2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float2x2, node1, node2);
        }
    }
};
ref<Float2x2> operator /(weak<const Float2x2> node1, weak<const Float> node2)
{
    return ref<Float2x2divFloat>::create(Arena::script(), node1, node2);
}

class FloataddFloat2x2 : public Float2x2
{
private:
    weak<const Float> node1;
    weak<const Float2x2> node2;
public:
    FloataddFloat2x2(weak<const Float> node1, weak<const Float2x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float2x2, node1, node2);
        }
    }
};
ref<Float2x2> operator +(weak<const Float> node1, weak<const Float2x2> node2)
{
    return ref<FloataddFloat2x2>::create(Arena::script(), node1, node2);
}

class Float2x2addFloat : public Float2x2
{
private:
    weak<const Float2x2> node1;
    weak<const Float> node2;
public:
    Float2x2addFloat(weak<const Float2x2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float2x2, node1, node2);
        }
    }
};
ref<Float2x2> operator +(weak<const Float2x2> node1, weak<const Float> node2)
{
    return ref<Float2x2addFloat>::create(Arena::script(), node1, node2);
}

class FloatsubFloat2x2 : public Float2x2
{
private:
    weak<const Float> node1;
    weak<const Float2x2> node2;
public:
    FloatsubFloat2x2(weak<const Float> node1, weak<const Float2x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float2x2, node1, node2);
        }
    }
};
ref<Float2x2> operator -(weak<const Float> node1, weak<const Float2x2> node2)
{
    return ref<FloatsubFloat2x2>::create(Arena::script(), node1, node2);
}

class Float2x2subFloat : public Float2x2
{
private:
    weak<const Float2x2> node1;
    weak<const Float> node2;
public:
    Float2x2subFloat(weak<const Float2x2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float2x2, node1, node2);
        }
    }
};
ref<Float2x2> operator -(weak<const Float2x2> node1, weak<const Float> node2)
{
    return ref<Float2x2subFloat>::create(Arena::script(), node1, node2);
}




/* Type *************************************************************/
Float3x2::Float3x2()
{
}
Float3x2::~Float3x2()
{
}

/* Uniform **********************************************************/
Float3x2Uniform::Float3x2Uniform(const istring& name)
    :   name(name)
{
}
Float3x2Uniform::~Float3x2Uniform()
{
}
void Float3x2Uniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Float3x2);
}
void Float3x2Uniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float3x2Attribute::Float3x2Attribute()
{
}
Float3x2Attribute::~Float3x2Attribute()
{
}
void Float3x2Attribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Float3x2);
}
void Float3x2Attribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float3x2Varying::Float3x2Varying(weak<const Float3x2> node)
    :   node(node)
{
}
Float3x2Varying::~Float3x2Varying()
{
}
void Float3x2Varying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Float3x2);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void Float3x2Varying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class Float3x2divFloat3x2 : public Float3x2
{
private:
    weak<const Float3x2> node1;
    weak<const Float3x2> node2;
public:
    Float3x2divFloat3x2(weak<const Float3x2> node1, weak<const Float3x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float3x2, node1, node2);
        }
    }
};
ref<Float3x2> operator /(weak<const Float3x2> node1, weak<const Float3x2> node2)
{
    return ref<Float3x2divFloat3x2>::create(Arena::script(), node1, node2);
}

class Float3x2addFloat3x2 : public Float3x2
{
private:
    weak<const Float3x2> node1;
    weak<const Float3x2> node2;
public:
    Float3x2addFloat3x2(weak<const Float3x2> node1, weak<const Float3x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float3x2, node1, node2);
        }
    }
};
ref<Float3x2> operator +(weak<const Float3x2> node1, weak<const Float3x2> node2)
{
    return ref<Float3x2addFloat3x2>::create(Arena::script(), node1, node2);
}

class Float3x2subFloat3x2 : public Float3x2
{
private:
    weak<const Float3x2> node1;
    weak<const Float3x2> node2;
public:
    Float3x2subFloat3x2(weak<const Float3x2> node1, weak<const Float3x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float3x2, node1, node2);
        }
    }
};
ref<Float3x2> operator -(weak<const Float3x2> node1, weak<const Float3x2> node2)
{
    return ref<Float3x2subFloat3x2>::create(Arena::script(), node1, node2);
}

class Float2x2mulFloat3x2 : public Float3x2
{
private:
    weak<const Float2x2> node1;
    weak<const Float3x2> node2;
public:
    Float2x2mulFloat3x2(weak<const Float2x2> node1, weak<const Float3x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3x2, node1, node2);
        }
    }
};
ref<Float3x2> operator *(weak<const Float2x2> node1, weak<const Float3x2> node2)
{
    return ref<Float2x2mulFloat3x2>::create(Arena::script(), node1, node2);
}

class Float3x2mulFloat3x3 : public Float3x2
{
private:
    weak<const Float3x2> node1;
    weak<const Float3x3> node2;
public:
    Float3x2mulFloat3x3(weak<const Float3x2> node1, weak<const Float3x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3x2, node1, node2);
        }
    }
};
ref<Float3x2> operator *(weak<const Float3x2> node1, weak<const Float3x3> node2)
{
    return ref<Float3x2mulFloat3x3>::create(Arena::script(), node1, node2);
}

class Float4x2mulFloat3x4 : public Float3x2
{
private:
    weak<const Float4x2> node1;
    weak<const Float3x4> node2;
public:
    Float4x2mulFloat3x4(weak<const Float4x2> node1, weak<const Float3x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3x2, node1, node2);
        }
    }
};
ref<Float3x2> operator *(weak<const Float4x2> node1, weak<const Float3x4> node2)
{
    return ref<Float4x2mulFloat3x4>::create(Arena::script(), node1, node2);
}

class FloatmulFloat3x2 : public Float3x2
{
private:
    weak<const Float> node1;
    weak<const Float3x2> node2;
public:
    FloatmulFloat3x2(weak<const Float> node1, weak<const Float3x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3x2, node1, node2);
        }
    }
};
ref<Float3x2> operator *(weak<const Float> node1, weak<const Float3x2> node2)
{
    return ref<FloatmulFloat3x2>::create(Arena::script(), node1, node2);
}

class Float3x2mulFloat : public Float3x2
{
private:
    weak<const Float3x2> node1;
    weak<const Float> node2;
public:
    Float3x2mulFloat(weak<const Float3x2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3x2, node1, node2);
        }
    }
};
ref<Float3x2> operator *(weak<const Float3x2> node1, weak<const Float> node2)
{
    return ref<Float3x2mulFloat>::create(Arena::script(), node1, node2);
}

class FloatdivFloat3x2 : public Float3x2
{
private:
    weak<const Float> node1;
    weak<const Float3x2> node2;
public:
    FloatdivFloat3x2(weak<const Float> node1, weak<const Float3x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float3x2, node1, node2);
        }
    }
};
ref<Float3x2> operator /(weak<const Float> node1, weak<const Float3x2> node2)
{
    return ref<FloatdivFloat3x2>::create(Arena::script(), node1, node2);
}

class Float3x2divFloat : public Float3x2
{
private:
    weak<const Float3x2> node1;
    weak<const Float> node2;
public:
    Float3x2divFloat(weak<const Float3x2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float3x2, node1, node2);
        }
    }
};
ref<Float3x2> operator /(weak<const Float3x2> node1, weak<const Float> node2)
{
    return ref<Float3x2divFloat>::create(Arena::script(), node1, node2);
}

class FloataddFloat3x2 : public Float3x2
{
private:
    weak<const Float> node1;
    weak<const Float3x2> node2;
public:
    FloataddFloat3x2(weak<const Float> node1, weak<const Float3x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float3x2, node1, node2);
        }
    }
};
ref<Float3x2> operator +(weak<const Float> node1, weak<const Float3x2> node2)
{
    return ref<FloataddFloat3x2>::create(Arena::script(), node1, node2);
}

class Float3x2addFloat : public Float3x2
{
private:
    weak<const Float3x2> node1;
    weak<const Float> node2;
public:
    Float3x2addFloat(weak<const Float3x2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float3x2, node1, node2);
        }
    }
};
ref<Float3x2> operator +(weak<const Float3x2> node1, weak<const Float> node2)
{
    return ref<Float3x2addFloat>::create(Arena::script(), node1, node2);
}

class FloatsubFloat3x2 : public Float3x2
{
private:
    weak<const Float> node1;
    weak<const Float3x2> node2;
public:
    FloatsubFloat3x2(weak<const Float> node1, weak<const Float3x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float3x2, node1, node2);
        }
    }
};
ref<Float3x2> operator -(weak<const Float> node1, weak<const Float3x2> node2)
{
    return ref<FloatsubFloat3x2>::create(Arena::script(), node1, node2);
}

class Float3x2subFloat : public Float3x2
{
private:
    weak<const Float3x2> node1;
    weak<const Float> node2;
public:
    Float3x2subFloat(weak<const Float3x2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float3x2, node1, node2);
        }
    }
};
ref<Float3x2> operator -(weak<const Float3x2> node1, weak<const Float> node2)
{
    return ref<Float3x2subFloat>::create(Arena::script(), node1, node2);
}




/* Type *************************************************************/
Float4x2::Float4x2()
{
}
Float4x2::~Float4x2()
{
}

/* Uniform **********************************************************/
Float4x2Uniform::Float4x2Uniform(const istring& name)
    :   name(name)
{
}
Float4x2Uniform::~Float4x2Uniform()
{
}
void Float4x2Uniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Float4x2);
}
void Float4x2Uniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float4x2Attribute::Float4x2Attribute()
{
}
Float4x2Attribute::~Float4x2Attribute()
{
}
void Float4x2Attribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Float4x2);
}
void Float4x2Attribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float4x2Varying::Float4x2Varying(weak<const Float4x2> node)
    :   node(node)
{
}
Float4x2Varying::~Float4x2Varying()
{
}
void Float4x2Varying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Float4x2);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void Float4x2Varying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class Float4x2divFloat4x2 : public Float4x2
{
private:
    weak<const Float4x2> node1;
    weak<const Float4x2> node2;
public:
    Float4x2divFloat4x2(weak<const Float4x2> node1, weak<const Float4x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float4x2, node1, node2);
        }
    }
};
ref<Float4x2> operator /(weak<const Float4x2> node1, weak<const Float4x2> node2)
{
    return ref<Float4x2divFloat4x2>::create(Arena::script(), node1, node2);
}

class Float4x2addFloat4x2 : public Float4x2
{
private:
    weak<const Float4x2> node1;
    weak<const Float4x2> node2;
public:
    Float4x2addFloat4x2(weak<const Float4x2> node1, weak<const Float4x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float4x2, node1, node2);
        }
    }
};
ref<Float4x2> operator +(weak<const Float4x2> node1, weak<const Float4x2> node2)
{
    return ref<Float4x2addFloat4x2>::create(Arena::script(), node1, node2);
}

class Float4x2subFloat4x2 : public Float4x2
{
private:
    weak<const Float4x2> node1;
    weak<const Float4x2> node2;
public:
    Float4x2subFloat4x2(weak<const Float4x2> node1, weak<const Float4x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float4x2, node1, node2);
        }
    }
};
ref<Float4x2> operator -(weak<const Float4x2> node1, weak<const Float4x2> node2)
{
    return ref<Float4x2subFloat4x2>::create(Arena::script(), node1, node2);
}

class Float2x2mulFloat4x2 : public Float4x2
{
private:
    weak<const Float2x2> node1;
    weak<const Float4x2> node2;
public:
    Float2x2mulFloat4x2(weak<const Float2x2> node1, weak<const Float4x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4x2, node1, node2);
        }
    }
};
ref<Float4x2> operator *(weak<const Float2x2> node1, weak<const Float4x2> node2)
{
    return ref<Float2x2mulFloat4x2>::create(Arena::script(), node1, node2);
}

class Float3x2mulFloat4x3 : public Float4x2
{
private:
    weak<const Float3x2> node1;
    weak<const Float4x3> node2;
public:
    Float3x2mulFloat4x3(weak<const Float3x2> node1, weak<const Float4x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4x2, node1, node2);
        }
    }
};
ref<Float4x2> operator *(weak<const Float3x2> node1, weak<const Float4x3> node2)
{
    return ref<Float3x2mulFloat4x3>::create(Arena::script(), node1, node2);
}

class Float4x2mulFloat4x4 : public Float4x2
{
private:
    weak<const Float4x2> node1;
    weak<const Float4x4> node2;
public:
    Float4x2mulFloat4x4(weak<const Float4x2> node1, weak<const Float4x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4x2, node1, node2);
        }
    }
};
ref<Float4x2> operator *(weak<const Float4x2> node1, weak<const Float4x4> node2)
{
    return ref<Float4x2mulFloat4x4>::create(Arena::script(), node1, node2);
}

class FloatmulFloat4x2 : public Float4x2
{
private:
    weak<const Float> node1;
    weak<const Float4x2> node2;
public:
    FloatmulFloat4x2(weak<const Float> node1, weak<const Float4x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4x2, node1, node2);
        }
    }
};
ref<Float4x2> operator *(weak<const Float> node1, weak<const Float4x2> node2)
{
    return ref<FloatmulFloat4x2>::create(Arena::script(), node1, node2);
}

class Float4x2mulFloat : public Float4x2
{
private:
    weak<const Float4x2> node1;
    weak<const Float> node2;
public:
    Float4x2mulFloat(weak<const Float4x2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4x2, node1, node2);
        }
    }
};
ref<Float4x2> operator *(weak<const Float4x2> node1, weak<const Float> node2)
{
    return ref<Float4x2mulFloat>::create(Arena::script(), node1, node2);
}

class FloatdivFloat4x2 : public Float4x2
{
private:
    weak<const Float> node1;
    weak<const Float4x2> node2;
public:
    FloatdivFloat4x2(weak<const Float> node1, weak<const Float4x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float4x2, node1, node2);
        }
    }
};
ref<Float4x2> operator /(weak<const Float> node1, weak<const Float4x2> node2)
{
    return ref<FloatdivFloat4x2>::create(Arena::script(), node1, node2);
}

class Float4x2divFloat : public Float4x2
{
private:
    weak<const Float4x2> node1;
    weak<const Float> node2;
public:
    Float4x2divFloat(weak<const Float4x2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float4x2, node1, node2);
        }
    }
};
ref<Float4x2> operator /(weak<const Float4x2> node1, weak<const Float> node2)
{
    return ref<Float4x2divFloat>::create(Arena::script(), node1, node2);
}

class FloataddFloat4x2 : public Float4x2
{
private:
    weak<const Float> node1;
    weak<const Float4x2> node2;
public:
    FloataddFloat4x2(weak<const Float> node1, weak<const Float4x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float4x2, node1, node2);
        }
    }
};
ref<Float4x2> operator +(weak<const Float> node1, weak<const Float4x2> node2)
{
    return ref<FloataddFloat4x2>::create(Arena::script(), node1, node2);
}

class Float4x2addFloat : public Float4x2
{
private:
    weak<const Float4x2> node1;
    weak<const Float> node2;
public:
    Float4x2addFloat(weak<const Float4x2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float4x2, node1, node2);
        }
    }
};
ref<Float4x2> operator +(weak<const Float4x2> node1, weak<const Float> node2)
{
    return ref<Float4x2addFloat>::create(Arena::script(), node1, node2);
}

class FloatsubFloat4x2 : public Float4x2
{
private:
    weak<const Float> node1;
    weak<const Float4x2> node2;
public:
    FloatsubFloat4x2(weak<const Float> node1, weak<const Float4x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float4x2, node1, node2);
        }
    }
};
ref<Float4x2> operator -(weak<const Float> node1, weak<const Float4x2> node2)
{
    return ref<FloatsubFloat4x2>::create(Arena::script(), node1, node2);
}

class Float4x2subFloat : public Float4x2
{
private:
    weak<const Float4x2> node1;
    weak<const Float> node2;
public:
    Float4x2subFloat(weak<const Float4x2> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float4x2, node1, node2);
        }
    }
};
ref<Float4x2> operator -(weak<const Float4x2> node1, weak<const Float> node2)
{
    return ref<Float4x2subFloat>::create(Arena::script(), node1, node2);
}




/* Type *************************************************************/
Float2x3::Float2x3()
{
}
Float2x3::~Float2x3()
{
}

/* Uniform **********************************************************/
Float2x3Uniform::Float2x3Uniform(const istring& name)
    :   name(name)
{
}
Float2x3Uniform::~Float2x3Uniform()
{
}
void Float2x3Uniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Float2x3);
}
void Float2x3Uniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float2x3Attribute::Float2x3Attribute()
{
}
Float2x3Attribute::~Float2x3Attribute()
{
}
void Float2x3Attribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Float2x3);
}
void Float2x3Attribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float2x3Varying::Float2x3Varying(weak<const Float2x3> node)
    :   node(node)
{
}
Float2x3Varying::~Float2x3Varying()
{
}
void Float2x3Varying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Float2x3);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void Float2x3Varying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class Float2x3divFloat2x3 : public Float2x3
{
private:
    weak<const Float2x3> node1;
    weak<const Float2x3> node2;
public:
    Float2x3divFloat2x3(weak<const Float2x3> node1, weak<const Float2x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float2x3, node1, node2);
        }
    }
};
ref<Float2x3> operator /(weak<const Float2x3> node1, weak<const Float2x3> node2)
{
    return ref<Float2x3divFloat2x3>::create(Arena::script(), node1, node2);
}

class Float2x3addFloat2x3 : public Float2x3
{
private:
    weak<const Float2x3> node1;
    weak<const Float2x3> node2;
public:
    Float2x3addFloat2x3(weak<const Float2x3> node1, weak<const Float2x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float2x3, node1, node2);
        }
    }
};
ref<Float2x3> operator +(weak<const Float2x3> node1, weak<const Float2x3> node2)
{
    return ref<Float2x3addFloat2x3>::create(Arena::script(), node1, node2);
}

class Float2x3subFloat2x3 : public Float2x3
{
private:
    weak<const Float2x3> node1;
    weak<const Float2x3> node2;
public:
    Float2x3subFloat2x3(weak<const Float2x3> node1, weak<const Float2x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float2x3, node1, node2);
        }
    }
};
ref<Float2x3> operator -(weak<const Float2x3> node1, weak<const Float2x3> node2)
{
    return ref<Float2x3subFloat2x3>::create(Arena::script(), node1, node2);
}

class Float2x3mulFloat2x2 : public Float2x3
{
private:
    weak<const Float2x3> node1;
    weak<const Float2x2> node2;
public:
    Float2x3mulFloat2x2(weak<const Float2x3> node1, weak<const Float2x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2x3, node1, node2);
        }
    }
};
ref<Float2x3> operator *(weak<const Float2x3> node1, weak<const Float2x2> node2)
{
    return ref<Float2x3mulFloat2x2>::create(Arena::script(), node1, node2);
}

class Float3x3mulFloat2x3 : public Float2x3
{
private:
    weak<const Float3x3> node1;
    weak<const Float2x3> node2;
public:
    Float3x3mulFloat2x3(weak<const Float3x3> node1, weak<const Float2x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2x3, node1, node2);
        }
    }
};
ref<Float2x3> operator *(weak<const Float3x3> node1, weak<const Float2x3> node2)
{
    return ref<Float3x3mulFloat2x3>::create(Arena::script(), node1, node2);
}

class Float4x3mulFloat2x4 : public Float2x3
{
private:
    weak<const Float4x3> node1;
    weak<const Float2x4> node2;
public:
    Float4x3mulFloat2x4(weak<const Float4x3> node1, weak<const Float2x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2x3, node1, node2);
        }
    }
};
ref<Float2x3> operator *(weak<const Float4x3> node1, weak<const Float2x4> node2)
{
    return ref<Float4x3mulFloat2x4>::create(Arena::script(), node1, node2);
}

class FloatmulFloat2x3 : public Float2x3
{
private:
    weak<const Float> node1;
    weak<const Float2x3> node2;
public:
    FloatmulFloat2x3(weak<const Float> node1, weak<const Float2x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2x3, node1, node2);
        }
    }
};
ref<Float2x3> operator *(weak<const Float> node1, weak<const Float2x3> node2)
{
    return ref<FloatmulFloat2x3>::create(Arena::script(), node1, node2);
}

class Float2x3mulFloat : public Float2x3
{
private:
    weak<const Float2x3> node1;
    weak<const Float> node2;
public:
    Float2x3mulFloat(weak<const Float2x3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2x3, node1, node2);
        }
    }
};
ref<Float2x3> operator *(weak<const Float2x3> node1, weak<const Float> node2)
{
    return ref<Float2x3mulFloat>::create(Arena::script(), node1, node2);
}

class FloatdivFloat2x3 : public Float2x3
{
private:
    weak<const Float> node1;
    weak<const Float2x3> node2;
public:
    FloatdivFloat2x3(weak<const Float> node1, weak<const Float2x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float2x3, node1, node2);
        }
    }
};
ref<Float2x3> operator /(weak<const Float> node1, weak<const Float2x3> node2)
{
    return ref<FloatdivFloat2x3>::create(Arena::script(), node1, node2);
}

class Float2x3divFloat : public Float2x3
{
private:
    weak<const Float2x3> node1;
    weak<const Float> node2;
public:
    Float2x3divFloat(weak<const Float2x3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float2x3, node1, node2);
        }
    }
};
ref<Float2x3> operator /(weak<const Float2x3> node1, weak<const Float> node2)
{
    return ref<Float2x3divFloat>::create(Arena::script(), node1, node2);
}

class FloataddFloat2x3 : public Float2x3
{
private:
    weak<const Float> node1;
    weak<const Float2x3> node2;
public:
    FloataddFloat2x3(weak<const Float> node1, weak<const Float2x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float2x3, node1, node2);
        }
    }
};
ref<Float2x3> operator +(weak<const Float> node1, weak<const Float2x3> node2)
{
    return ref<FloataddFloat2x3>::create(Arena::script(), node1, node2);
}

class Float2x3addFloat : public Float2x3
{
private:
    weak<const Float2x3> node1;
    weak<const Float> node2;
public:
    Float2x3addFloat(weak<const Float2x3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float2x3, node1, node2);
        }
    }
};
ref<Float2x3> operator +(weak<const Float2x3> node1, weak<const Float> node2)
{
    return ref<Float2x3addFloat>::create(Arena::script(), node1, node2);
}

class FloatsubFloat2x3 : public Float2x3
{
private:
    weak<const Float> node1;
    weak<const Float2x3> node2;
public:
    FloatsubFloat2x3(weak<const Float> node1, weak<const Float2x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float2x3, node1, node2);
        }
    }
};
ref<Float2x3> operator -(weak<const Float> node1, weak<const Float2x3> node2)
{
    return ref<FloatsubFloat2x3>::create(Arena::script(), node1, node2);
}

class Float2x3subFloat : public Float2x3
{
private:
    weak<const Float2x3> node1;
    weak<const Float> node2;
public:
    Float2x3subFloat(weak<const Float2x3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float2x3, node1, node2);
        }
    }
};
ref<Float2x3> operator -(weak<const Float2x3> node1, weak<const Float> node2)
{
    return ref<Float2x3subFloat>::create(Arena::script(), node1, node2);
}




/* Type *************************************************************/
Float3x3::Float3x3()
{
}
Float3x3::~Float3x3()
{
}

/* Uniform **********************************************************/
Float3x3Uniform::Float3x3Uniform(const istring& name)
    :   name(name)
{
}
Float3x3Uniform::~Float3x3Uniform()
{
}
void Float3x3Uniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Float3x3);
}
void Float3x3Uniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float3x3Attribute::Float3x3Attribute()
{
}
Float3x3Attribute::~Float3x3Attribute()
{
}
void Float3x3Attribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Float3x3);
}
void Float3x3Attribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float3x3Varying::Float3x3Varying(weak<const Float3x3> node)
    :   node(node)
{
}
Float3x3Varying::~Float3x3Varying()
{
}
void Float3x3Varying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Float3x3);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void Float3x3Varying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class Float3x3divFloat3x3 : public Float3x3
{
private:
    weak<const Float3x3> node1;
    weak<const Float3x3> node2;
public:
    Float3x3divFloat3x3(weak<const Float3x3> node1, weak<const Float3x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float3x3, node1, node2);
        }
    }
};
ref<Float3x3> operator /(weak<const Float3x3> node1, weak<const Float3x3> node2)
{
    return ref<Float3x3divFloat3x3>::create(Arena::script(), node1, node2);
}

class Float3x3addFloat3x3 : public Float3x3
{
private:
    weak<const Float3x3> node1;
    weak<const Float3x3> node2;
public:
    Float3x3addFloat3x3(weak<const Float3x3> node1, weak<const Float3x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float3x3, node1, node2);
        }
    }
};
ref<Float3x3> operator +(weak<const Float3x3> node1, weak<const Float3x3> node2)
{
    return ref<Float3x3addFloat3x3>::create(Arena::script(), node1, node2);
}

class Float3x3subFloat3x3 : public Float3x3
{
private:
    weak<const Float3x3> node1;
    weak<const Float3x3> node2;
public:
    Float3x3subFloat3x3(weak<const Float3x3> node1, weak<const Float3x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float3x3, node1, node2);
        }
    }
};
ref<Float3x3> operator -(weak<const Float3x3> node1, weak<const Float3x3> node2)
{
    return ref<Float3x3subFloat3x3>::create(Arena::script(), node1, node2);
}

class Float2x3mulFloat3x2 : public Float3x3
{
private:
    weak<const Float2x3> node1;
    weak<const Float3x2> node2;
public:
    Float2x3mulFloat3x2(weak<const Float2x3> node1, weak<const Float3x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3x3, node1, node2);
        }
    }
};
ref<Float3x3> operator *(weak<const Float2x3> node1, weak<const Float3x2> node2)
{
    return ref<Float2x3mulFloat3x2>::create(Arena::script(), node1, node2);
}

class Float3x3mulFloat3x3 : public Float3x3
{
private:
    weak<const Float3x3> node1;
    weak<const Float3x3> node2;
public:
    Float3x3mulFloat3x3(weak<const Float3x3> node1, weak<const Float3x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3x3, node1, node2);
        }
    }
};
ref<Float3x3> operator *(weak<const Float3x3> node1, weak<const Float3x3> node2)
{
    return ref<Float3x3mulFloat3x3>::create(Arena::script(), node1, node2);
}

class Float4x3mulFloat3x4 : public Float3x3
{
private:
    weak<const Float4x3> node1;
    weak<const Float3x4> node2;
public:
    Float4x3mulFloat3x4(weak<const Float4x3> node1, weak<const Float3x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3x3, node1, node2);
        }
    }
};
ref<Float3x3> operator *(weak<const Float4x3> node1, weak<const Float3x4> node2)
{
    return ref<Float4x3mulFloat3x4>::create(Arena::script(), node1, node2);
}

class FloatmulFloat3x3 : public Float3x3
{
private:
    weak<const Float> node1;
    weak<const Float3x3> node2;
public:
    FloatmulFloat3x3(weak<const Float> node1, weak<const Float3x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3x3, node1, node2);
        }
    }
};
ref<Float3x3> operator *(weak<const Float> node1, weak<const Float3x3> node2)
{
    return ref<FloatmulFloat3x3>::create(Arena::script(), node1, node2);
}

class Float3x3mulFloat : public Float3x3
{
private:
    weak<const Float3x3> node1;
    weak<const Float> node2;
public:
    Float3x3mulFloat(weak<const Float3x3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3x3, node1, node2);
        }
    }
};
ref<Float3x3> operator *(weak<const Float3x3> node1, weak<const Float> node2)
{
    return ref<Float3x3mulFloat>::create(Arena::script(), node1, node2);
}

class FloatdivFloat3x3 : public Float3x3
{
private:
    weak<const Float> node1;
    weak<const Float3x3> node2;
public:
    FloatdivFloat3x3(weak<const Float> node1, weak<const Float3x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float3x3, node1, node2);
        }
    }
};
ref<Float3x3> operator /(weak<const Float> node1, weak<const Float3x3> node2)
{
    return ref<FloatdivFloat3x3>::create(Arena::script(), node1, node2);
}

class Float3x3divFloat : public Float3x3
{
private:
    weak<const Float3x3> node1;
    weak<const Float> node2;
public:
    Float3x3divFloat(weak<const Float3x3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float3x3, node1, node2);
        }
    }
};
ref<Float3x3> operator /(weak<const Float3x3> node1, weak<const Float> node2)
{
    return ref<Float3x3divFloat>::create(Arena::script(), node1, node2);
}

class FloataddFloat3x3 : public Float3x3
{
private:
    weak<const Float> node1;
    weak<const Float3x3> node2;
public:
    FloataddFloat3x3(weak<const Float> node1, weak<const Float3x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float3x3, node1, node2);
        }
    }
};
ref<Float3x3> operator +(weak<const Float> node1, weak<const Float3x3> node2)
{
    return ref<FloataddFloat3x3>::create(Arena::script(), node1, node2);
}

class Float3x3addFloat : public Float3x3
{
private:
    weak<const Float3x3> node1;
    weak<const Float> node2;
public:
    Float3x3addFloat(weak<const Float3x3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float3x3, node1, node2);
        }
    }
};
ref<Float3x3> operator +(weak<const Float3x3> node1, weak<const Float> node2)
{
    return ref<Float3x3addFloat>::create(Arena::script(), node1, node2);
}

class FloatsubFloat3x3 : public Float3x3
{
private:
    weak<const Float> node1;
    weak<const Float3x3> node2;
public:
    FloatsubFloat3x3(weak<const Float> node1, weak<const Float3x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float3x3, node1, node2);
        }
    }
};
ref<Float3x3> operator -(weak<const Float> node1, weak<const Float3x3> node2)
{
    return ref<FloatsubFloat3x3>::create(Arena::script(), node1, node2);
}

class Float3x3subFloat : public Float3x3
{
private:
    weak<const Float3x3> node1;
    weak<const Float> node2;
public:
    Float3x3subFloat(weak<const Float3x3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float3x3, node1, node2);
        }
    }
};
ref<Float3x3> operator -(weak<const Float3x3> node1, weak<const Float> node2)
{
    return ref<Float3x3subFloat>::create(Arena::script(), node1, node2);
}




/* Type *************************************************************/
Float4x3::Float4x3()
{
}
Float4x3::~Float4x3()
{
}

/* Uniform **********************************************************/
Float4x3Uniform::Float4x3Uniform(const istring& name)
    :   name(name)
{
}
Float4x3Uniform::~Float4x3Uniform()
{
}
void Float4x3Uniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Float4x3);
}
void Float4x3Uniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float4x3Attribute::Float4x3Attribute()
{
}
Float4x3Attribute::~Float4x3Attribute()
{
}
void Float4x3Attribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Float4x3);
}
void Float4x3Attribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float4x3Varying::Float4x3Varying(weak<const Float4x3> node)
    :   node(node)
{
}
Float4x3Varying::~Float4x3Varying()
{
}
void Float4x3Varying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Float4x3);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void Float4x3Varying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class Float4x3divFloat4x3 : public Float4x3
{
private:
    weak<const Float4x3> node1;
    weak<const Float4x3> node2;
public:
    Float4x3divFloat4x3(weak<const Float4x3> node1, weak<const Float4x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float4x3, node1, node2);
        }
    }
};
ref<Float4x3> operator /(weak<const Float4x3> node1, weak<const Float4x3> node2)
{
    return ref<Float4x3divFloat4x3>::create(Arena::script(), node1, node2);
}

class Float4x3addFloat4x3 : public Float4x3
{
private:
    weak<const Float4x3> node1;
    weak<const Float4x3> node2;
public:
    Float4x3addFloat4x3(weak<const Float4x3> node1, weak<const Float4x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float4x3, node1, node2);
        }
    }
};
ref<Float4x3> operator +(weak<const Float4x3> node1, weak<const Float4x3> node2)
{
    return ref<Float4x3addFloat4x3>::create(Arena::script(), node1, node2);
}

class Float4x3subFloat4x3 : public Float4x3
{
private:
    weak<const Float4x3> node1;
    weak<const Float4x3> node2;
public:
    Float4x3subFloat4x3(weak<const Float4x3> node1, weak<const Float4x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float4x3, node1, node2);
        }
    }
};
ref<Float4x3> operator -(weak<const Float4x3> node1, weak<const Float4x3> node2)
{
    return ref<Float4x3subFloat4x3>::create(Arena::script(), node1, node2);
}

class Float2x3mulFloat4x2 : public Float4x3
{
private:
    weak<const Float2x3> node1;
    weak<const Float4x2> node2;
public:
    Float2x3mulFloat4x2(weak<const Float2x3> node1, weak<const Float4x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4x3, node1, node2);
        }
    }
};
ref<Float4x3> operator *(weak<const Float2x3> node1, weak<const Float4x2> node2)
{
    return ref<Float2x3mulFloat4x2>::create(Arena::script(), node1, node2);
}

class Float3x3mulFloat4x3 : public Float4x3
{
private:
    weak<const Float3x3> node1;
    weak<const Float4x3> node2;
public:
    Float3x3mulFloat4x3(weak<const Float3x3> node1, weak<const Float4x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4x3, node1, node2);
        }
    }
};
ref<Float4x3> operator *(weak<const Float3x3> node1, weak<const Float4x3> node2)
{
    return ref<Float3x3mulFloat4x3>::create(Arena::script(), node1, node2);
}

class Float4x3mulFloat4x4 : public Float4x3
{
private:
    weak<const Float4x3> node1;
    weak<const Float4x4> node2;
public:
    Float4x3mulFloat4x4(weak<const Float4x3> node1, weak<const Float4x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4x3, node1, node2);
        }
    }
};
ref<Float4x3> operator *(weak<const Float4x3> node1, weak<const Float4x4> node2)
{
    return ref<Float4x3mulFloat4x4>::create(Arena::script(), node1, node2);
}

class FloatmulFloat4x3 : public Float4x3
{
private:
    weak<const Float> node1;
    weak<const Float4x3> node2;
public:
    FloatmulFloat4x3(weak<const Float> node1, weak<const Float4x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4x3, node1, node2);
        }
    }
};
ref<Float4x3> operator *(weak<const Float> node1, weak<const Float4x3> node2)
{
    return ref<FloatmulFloat4x3>::create(Arena::script(), node1, node2);
}

class Float4x3mulFloat : public Float4x3
{
private:
    weak<const Float4x3> node1;
    weak<const Float> node2;
public:
    Float4x3mulFloat(weak<const Float4x3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4x3, node1, node2);
        }
    }
};
ref<Float4x3> operator *(weak<const Float4x3> node1, weak<const Float> node2)
{
    return ref<Float4x3mulFloat>::create(Arena::script(), node1, node2);
}

class FloatdivFloat4x3 : public Float4x3
{
private:
    weak<const Float> node1;
    weak<const Float4x3> node2;
public:
    FloatdivFloat4x3(weak<const Float> node1, weak<const Float4x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float4x3, node1, node2);
        }
    }
};
ref<Float4x3> operator /(weak<const Float> node1, weak<const Float4x3> node2)
{
    return ref<FloatdivFloat4x3>::create(Arena::script(), node1, node2);
}

class Float4x3divFloat : public Float4x3
{
private:
    weak<const Float4x3> node1;
    weak<const Float> node2;
public:
    Float4x3divFloat(weak<const Float4x3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float4x3, node1, node2);
        }
    }
};
ref<Float4x3> operator /(weak<const Float4x3> node1, weak<const Float> node2)
{
    return ref<Float4x3divFloat>::create(Arena::script(), node1, node2);
}

class FloataddFloat4x3 : public Float4x3
{
private:
    weak<const Float> node1;
    weak<const Float4x3> node2;
public:
    FloataddFloat4x3(weak<const Float> node1, weak<const Float4x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float4x3, node1, node2);
        }
    }
};
ref<Float4x3> operator +(weak<const Float> node1, weak<const Float4x3> node2)
{
    return ref<FloataddFloat4x3>::create(Arena::script(), node1, node2);
}

class Float4x3addFloat : public Float4x3
{
private:
    weak<const Float4x3> node1;
    weak<const Float> node2;
public:
    Float4x3addFloat(weak<const Float4x3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float4x3, node1, node2);
        }
    }
};
ref<Float4x3> operator +(weak<const Float4x3> node1, weak<const Float> node2)
{
    return ref<Float4x3addFloat>::create(Arena::script(), node1, node2);
}

class FloatsubFloat4x3 : public Float4x3
{
private:
    weak<const Float> node1;
    weak<const Float4x3> node2;
public:
    FloatsubFloat4x3(weak<const Float> node1, weak<const Float4x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float4x3, node1, node2);
        }
    }
};
ref<Float4x3> operator -(weak<const Float> node1, weak<const Float4x3> node2)
{
    return ref<FloatsubFloat4x3>::create(Arena::script(), node1, node2);
}

class Float4x3subFloat : public Float4x3
{
private:
    weak<const Float4x3> node1;
    weak<const Float> node2;
public:
    Float4x3subFloat(weak<const Float4x3> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float4x3, node1, node2);
        }
    }
};
ref<Float4x3> operator -(weak<const Float4x3> node1, weak<const Float> node2)
{
    return ref<Float4x3subFloat>::create(Arena::script(), node1, node2);
}




/* Type *************************************************************/
Float2x4::Float2x4()
{
}
Float2x4::~Float2x4()
{
}

/* Uniform **********************************************************/
Float2x4Uniform::Float2x4Uniform(const istring& name)
    :   name(name)
{
}
Float2x4Uniform::~Float2x4Uniform()
{
}
void Float2x4Uniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Float2x4);
}
void Float2x4Uniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float2x4Attribute::Float2x4Attribute()
{
}
Float2x4Attribute::~Float2x4Attribute()
{
}
void Float2x4Attribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Float2x4);
}
void Float2x4Attribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float2x4Varying::Float2x4Varying(weak<const Float2x4> node)
    :   node(node)
{
}
Float2x4Varying::~Float2x4Varying()
{
}
void Float2x4Varying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Float2x4);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void Float2x4Varying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class Float2x4divFloat2x4 : public Float2x4
{
private:
    weak<const Float2x4> node1;
    weak<const Float2x4> node2;
public:
    Float2x4divFloat2x4(weak<const Float2x4> node1, weak<const Float2x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float2x4, node1, node2);
        }
    }
};
ref<Float2x4> operator /(weak<const Float2x4> node1, weak<const Float2x4> node2)
{
    return ref<Float2x4divFloat2x4>::create(Arena::script(), node1, node2);
}

class Float2x4addFloat2x4 : public Float2x4
{
private:
    weak<const Float2x4> node1;
    weak<const Float2x4> node2;
public:
    Float2x4addFloat2x4(weak<const Float2x4> node1, weak<const Float2x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float2x4, node1, node2);
        }
    }
};
ref<Float2x4> operator +(weak<const Float2x4> node1, weak<const Float2x4> node2)
{
    return ref<Float2x4addFloat2x4>::create(Arena::script(), node1, node2);
}

class Float2x4subFloat2x4 : public Float2x4
{
private:
    weak<const Float2x4> node1;
    weak<const Float2x4> node2;
public:
    Float2x4subFloat2x4(weak<const Float2x4> node1, weak<const Float2x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float2x4, node1, node2);
        }
    }
};
ref<Float2x4> operator -(weak<const Float2x4> node1, weak<const Float2x4> node2)
{
    return ref<Float2x4subFloat2x4>::create(Arena::script(), node1, node2);
}

class Float2x4mulFloat2x2 : public Float2x4
{
private:
    weak<const Float2x4> node1;
    weak<const Float2x2> node2;
public:
    Float2x4mulFloat2x2(weak<const Float2x4> node1, weak<const Float2x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2x4, node1, node2);
        }
    }
};
ref<Float2x4> operator *(weak<const Float2x4> node1, weak<const Float2x2> node2)
{
    return ref<Float2x4mulFloat2x2>::create(Arena::script(), node1, node2);
}

class Float3x4mulFloat2x3 : public Float2x4
{
private:
    weak<const Float3x4> node1;
    weak<const Float2x3> node2;
public:
    Float3x4mulFloat2x3(weak<const Float3x4> node1, weak<const Float2x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2x4, node1, node2);
        }
    }
};
ref<Float2x4> operator *(weak<const Float3x4> node1, weak<const Float2x3> node2)
{
    return ref<Float3x4mulFloat2x3>::create(Arena::script(), node1, node2);
}

class Float4x4mulFloat2x4 : public Float2x4
{
private:
    weak<const Float4x4> node1;
    weak<const Float2x4> node2;
public:
    Float4x4mulFloat2x4(weak<const Float4x4> node1, weak<const Float2x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2x4, node1, node2);
        }
    }
};
ref<Float2x4> operator *(weak<const Float4x4> node1, weak<const Float2x4> node2)
{
    return ref<Float4x4mulFloat2x4>::create(Arena::script(), node1, node2);
}

class FloatmulFloat2x4 : public Float2x4
{
private:
    weak<const Float> node1;
    weak<const Float2x4> node2;
public:
    FloatmulFloat2x4(weak<const Float> node1, weak<const Float2x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2x4, node1, node2);
        }
    }
};
ref<Float2x4> operator *(weak<const Float> node1, weak<const Float2x4> node2)
{
    return ref<FloatmulFloat2x4>::create(Arena::script(), node1, node2);
}

class Float2x4mulFloat : public Float2x4
{
private:
    weak<const Float2x4> node1;
    weak<const Float> node2;
public:
    Float2x4mulFloat(weak<const Float2x4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float2x4, node1, node2);
        }
    }
};
ref<Float2x4> operator *(weak<const Float2x4> node1, weak<const Float> node2)
{
    return ref<Float2x4mulFloat>::create(Arena::script(), node1, node2);
}

class FloatdivFloat2x4 : public Float2x4
{
private:
    weak<const Float> node1;
    weak<const Float2x4> node2;
public:
    FloatdivFloat2x4(weak<const Float> node1, weak<const Float2x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float2x4, node1, node2);
        }
    }
};
ref<Float2x4> operator /(weak<const Float> node1, weak<const Float2x4> node2)
{
    return ref<FloatdivFloat2x4>::create(Arena::script(), node1, node2);
}

class Float2x4divFloat : public Float2x4
{
private:
    weak<const Float2x4> node1;
    weak<const Float> node2;
public:
    Float2x4divFloat(weak<const Float2x4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float2x4, node1, node2);
        }
    }
};
ref<Float2x4> operator /(weak<const Float2x4> node1, weak<const Float> node2)
{
    return ref<Float2x4divFloat>::create(Arena::script(), node1, node2);
}

class FloataddFloat2x4 : public Float2x4
{
private:
    weak<const Float> node1;
    weak<const Float2x4> node2;
public:
    FloataddFloat2x4(weak<const Float> node1, weak<const Float2x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float2x4, node1, node2);
        }
    }
};
ref<Float2x4> operator +(weak<const Float> node1, weak<const Float2x4> node2)
{
    return ref<FloataddFloat2x4>::create(Arena::script(), node1, node2);
}

class Float2x4addFloat : public Float2x4
{
private:
    weak<const Float2x4> node1;
    weak<const Float> node2;
public:
    Float2x4addFloat(weak<const Float2x4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float2x4, node1, node2);
        }
    }
};
ref<Float2x4> operator +(weak<const Float2x4> node1, weak<const Float> node2)
{
    return ref<Float2x4addFloat>::create(Arena::script(), node1, node2);
}

class FloatsubFloat2x4 : public Float2x4
{
private:
    weak<const Float> node1;
    weak<const Float2x4> node2;
public:
    FloatsubFloat2x4(weak<const Float> node1, weak<const Float2x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float2x4, node1, node2);
        }
    }
};
ref<Float2x4> operator -(weak<const Float> node1, weak<const Float2x4> node2)
{
    return ref<FloatsubFloat2x4>::create(Arena::script(), node1, node2);
}

class Float2x4subFloat : public Float2x4
{
private:
    weak<const Float2x4> node1;
    weak<const Float> node2;
public:
    Float2x4subFloat(weak<const Float2x4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float2x4, node1, node2);
        }
    }
};
ref<Float2x4> operator -(weak<const Float2x4> node1, weak<const Float> node2)
{
    return ref<Float2x4subFloat>::create(Arena::script(), node1, node2);
}




/* Type *************************************************************/
Float3x4::Float3x4()
{
}
Float3x4::~Float3x4()
{
}

/* Uniform **********************************************************/
Float3x4Uniform::Float3x4Uniform(const istring& name)
    :   name(name)
{
}
Float3x4Uniform::~Float3x4Uniform()
{
}
void Float3x4Uniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Float3x4);
}
void Float3x4Uniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float3x4Attribute::Float3x4Attribute()
{
}
Float3x4Attribute::~Float3x4Attribute()
{
}
void Float3x4Attribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Float3x4);
}
void Float3x4Attribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float3x4Varying::Float3x4Varying(weak<const Float3x4> node)
    :   node(node)
{
}
Float3x4Varying::~Float3x4Varying()
{
}
void Float3x4Varying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Float3x4);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void Float3x4Varying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class Float3x4divFloat3x4 : public Float3x4
{
private:
    weak<const Float3x4> node1;
    weak<const Float3x4> node2;
public:
    Float3x4divFloat3x4(weak<const Float3x4> node1, weak<const Float3x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float3x4, node1, node2);
        }
    }
};
ref<Float3x4> operator /(weak<const Float3x4> node1, weak<const Float3x4> node2)
{
    return ref<Float3x4divFloat3x4>::create(Arena::script(), node1, node2);
}

class Float3x4addFloat3x4 : public Float3x4
{
private:
    weak<const Float3x4> node1;
    weak<const Float3x4> node2;
public:
    Float3x4addFloat3x4(weak<const Float3x4> node1, weak<const Float3x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float3x4, node1, node2);
        }
    }
};
ref<Float3x4> operator +(weak<const Float3x4> node1, weak<const Float3x4> node2)
{
    return ref<Float3x4addFloat3x4>::create(Arena::script(), node1, node2);
}

class Float3x4subFloat3x4 : public Float3x4
{
private:
    weak<const Float3x4> node1;
    weak<const Float3x4> node2;
public:
    Float3x4subFloat3x4(weak<const Float3x4> node1, weak<const Float3x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float3x4, node1, node2);
        }
    }
};
ref<Float3x4> operator -(weak<const Float3x4> node1, weak<const Float3x4> node2)
{
    return ref<Float3x4subFloat3x4>::create(Arena::script(), node1, node2);
}

class Float2x4mulFloat3x2 : public Float3x4
{
private:
    weak<const Float2x4> node1;
    weak<const Float3x2> node2;
public:
    Float2x4mulFloat3x2(weak<const Float2x4> node1, weak<const Float3x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3x4, node1, node2);
        }
    }
};
ref<Float3x4> operator *(weak<const Float2x4> node1, weak<const Float3x2> node2)
{
    return ref<Float2x4mulFloat3x2>::create(Arena::script(), node1, node2);
}

class Float3x4mulFloat3x3 : public Float3x4
{
private:
    weak<const Float3x4> node1;
    weak<const Float3x3> node2;
public:
    Float3x4mulFloat3x3(weak<const Float3x4> node1, weak<const Float3x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3x4, node1, node2);
        }
    }
};
ref<Float3x4> operator *(weak<const Float3x4> node1, weak<const Float3x3> node2)
{
    return ref<Float3x4mulFloat3x3>::create(Arena::script(), node1, node2);
}

class Float4x4mulFloat3x4 : public Float3x4
{
private:
    weak<const Float4x4> node1;
    weak<const Float3x4> node2;
public:
    Float4x4mulFloat3x4(weak<const Float4x4> node1, weak<const Float3x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3x4, node1, node2);
        }
    }
};
ref<Float3x4> operator *(weak<const Float4x4> node1, weak<const Float3x4> node2)
{
    return ref<Float4x4mulFloat3x4>::create(Arena::script(), node1, node2);
}

class FloatmulFloat3x4 : public Float3x4
{
private:
    weak<const Float> node1;
    weak<const Float3x4> node2;
public:
    FloatmulFloat3x4(weak<const Float> node1, weak<const Float3x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3x4, node1, node2);
        }
    }
};
ref<Float3x4> operator *(weak<const Float> node1, weak<const Float3x4> node2)
{
    return ref<FloatmulFloat3x4>::create(Arena::script(), node1, node2);
}

class Float3x4mulFloat : public Float3x4
{
private:
    weak<const Float3x4> node1;
    weak<const Float> node2;
public:
    Float3x4mulFloat(weak<const Float3x4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float3x4, node1, node2);
        }
    }
};
ref<Float3x4> operator *(weak<const Float3x4> node1, weak<const Float> node2)
{
    return ref<Float3x4mulFloat>::create(Arena::script(), node1, node2);
}

class FloatdivFloat3x4 : public Float3x4
{
private:
    weak<const Float> node1;
    weak<const Float3x4> node2;
public:
    FloatdivFloat3x4(weak<const Float> node1, weak<const Float3x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float3x4, node1, node2);
        }
    }
};
ref<Float3x4> operator /(weak<const Float> node1, weak<const Float3x4> node2)
{
    return ref<FloatdivFloat3x4>::create(Arena::script(), node1, node2);
}

class Float3x4divFloat : public Float3x4
{
private:
    weak<const Float3x4> node1;
    weak<const Float> node2;
public:
    Float3x4divFloat(weak<const Float3x4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float3x4, node1, node2);
        }
    }
};
ref<Float3x4> operator /(weak<const Float3x4> node1, weak<const Float> node2)
{
    return ref<Float3x4divFloat>::create(Arena::script(), node1, node2);
}

class FloataddFloat3x4 : public Float3x4
{
private:
    weak<const Float> node1;
    weak<const Float3x4> node2;
public:
    FloataddFloat3x4(weak<const Float> node1, weak<const Float3x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float3x4, node1, node2);
        }
    }
};
ref<Float3x4> operator +(weak<const Float> node1, weak<const Float3x4> node2)
{
    return ref<FloataddFloat3x4>::create(Arena::script(), node1, node2);
}

class Float3x4addFloat : public Float3x4
{
private:
    weak<const Float3x4> node1;
    weak<const Float> node2;
public:
    Float3x4addFloat(weak<const Float3x4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float3x4, node1, node2);
        }
    }
};
ref<Float3x4> operator +(weak<const Float3x4> node1, weak<const Float> node2)
{
    return ref<Float3x4addFloat>::create(Arena::script(), node1, node2);
}

class FloatsubFloat3x4 : public Float3x4
{
private:
    weak<const Float> node1;
    weak<const Float3x4> node2;
public:
    FloatsubFloat3x4(weak<const Float> node1, weak<const Float3x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float3x4, node1, node2);
        }
    }
};
ref<Float3x4> operator -(weak<const Float> node1, weak<const Float3x4> node2)
{
    return ref<FloatsubFloat3x4>::create(Arena::script(), node1, node2);
}

class Float3x4subFloat : public Float3x4
{
private:
    weak<const Float3x4> node1;
    weak<const Float> node2;
public:
    Float3x4subFloat(weak<const Float3x4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float3x4, node1, node2);
        }
    }
};
ref<Float3x4> operator -(weak<const Float3x4> node1, weak<const Float> node2)
{
    return ref<Float3x4subFloat>::create(Arena::script(), node1, node2);
}




/* Type *************************************************************/
Float4x4::Float4x4()
{
}
Float4x4::~Float4x4()
{
}

/* Uniform **********************************************************/
Float4x4Uniform::Float4x4Uniform(const istring& name)
    :   name(name)
{
}
Float4x4Uniform::~Float4x4Uniform()
{
}
void Float4x4Uniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Float4x4);
}
void Float4x4Uniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Float4x4Attribute::Float4x4Attribute()
{
}
Float4x4Attribute::~Float4x4Attribute()
{
}
void Float4x4Attribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Float4x4);
}
void Float4x4Attribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Float4x4Varying::Float4x4Varying(weak<const Float4x4> node)
    :   node(node)
{
}
Float4x4Varying::~Float4x4Varying()
{
}
void Float4x4Varying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Float4x4);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void Float4x4Varying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class Float4x4divFloat4x4 : public Float4x4
{
private:
    weak<const Float4x4> node1;
    weak<const Float4x4> node2;
public:
    Float4x4divFloat4x4(weak<const Float4x4> node1, weak<const Float4x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float4x4, node1, node2);
        }
    }
};
ref<Float4x4> operator /(weak<const Float4x4> node1, weak<const Float4x4> node2)
{
    return ref<Float4x4divFloat4x4>::create(Arena::script(), node1, node2);
}

class Float4x4addFloat4x4 : public Float4x4
{
private:
    weak<const Float4x4> node1;
    weak<const Float4x4> node2;
public:
    Float4x4addFloat4x4(weak<const Float4x4> node1, weak<const Float4x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float4x4, node1, node2);
        }
    }
};
ref<Float4x4> operator +(weak<const Float4x4> node1, weak<const Float4x4> node2)
{
    return ref<Float4x4addFloat4x4>::create(Arena::script(), node1, node2);
}

class Float4x4subFloat4x4 : public Float4x4
{
private:
    weak<const Float4x4> node1;
    weak<const Float4x4> node2;
public:
    Float4x4subFloat4x4(weak<const Float4x4> node1, weak<const Float4x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float4x4, node1, node2);
        }
    }
};
ref<Float4x4> operator -(weak<const Float4x4> node1, weak<const Float4x4> node2)
{
    return ref<Float4x4subFloat4x4>::create(Arena::script(), node1, node2);
}

class Float2x4mulFloat4x2 : public Float4x4
{
private:
    weak<const Float2x4> node1;
    weak<const Float4x2> node2;
public:
    Float2x4mulFloat4x2(weak<const Float2x4> node1, weak<const Float4x2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4x4, node1, node2);
        }
    }
};
ref<Float4x4> operator *(weak<const Float2x4> node1, weak<const Float4x2> node2)
{
    return ref<Float2x4mulFloat4x2>::create(Arena::script(), node1, node2);
}

class Float3x4mulFloat4x3 : public Float4x4
{
private:
    weak<const Float3x4> node1;
    weak<const Float4x3> node2;
public:
    Float3x4mulFloat4x3(weak<const Float3x4> node1, weak<const Float4x3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4x4, node1, node2);
        }
    }
};
ref<Float4x4> operator *(weak<const Float3x4> node1, weak<const Float4x3> node2)
{
    return ref<Float3x4mulFloat4x3>::create(Arena::script(), node1, node2);
}

class Float4x4mulFloat4x4 : public Float4x4
{
private:
    weak<const Float4x4> node1;
    weak<const Float4x4> node2;
public:
    Float4x4mulFloat4x4(weak<const Float4x4> node1, weak<const Float4x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4x4, node1, node2);
        }
    }
};
ref<Float4x4> operator *(weak<const Float4x4> node1, weak<const Float4x4> node2)
{
    return ref<Float4x4mulFloat4x4>::create(Arena::script(), node1, node2);
}

class FloatmulFloat4x4 : public Float4x4
{
private:
    weak<const Float> node1;
    weak<const Float4x4> node2;
public:
    FloatmulFloat4x4(weak<const Float> node1, weak<const Float4x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4x4, node1, node2);
        }
    }
};
ref<Float4x4> operator *(weak<const Float> node1, weak<const Float4x4> node2)
{
    return ref<FloatmulFloat4x4>::create(Arena::script(), node1, node2);
}

class Float4x4mulFloat : public Float4x4
{
private:
    weak<const Float4x4> node1;
    weak<const Float> node2;
public:
    Float4x4mulFloat(weak<const Float4x4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Float4x4, node1, node2);
        }
    }
};
ref<Float4x4> operator *(weak<const Float4x4> node1, weak<const Float> node2)
{
    return ref<Float4x4mulFloat>::create(Arena::script(), node1, node2);
}

class FloatdivFloat4x4 : public Float4x4
{
private:
    weak<const Float> node1;
    weak<const Float4x4> node2;
public:
    FloatdivFloat4x4(weak<const Float> node1, weak<const Float4x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float4x4, node1, node2);
        }
    }
};
ref<Float4x4> operator /(weak<const Float> node1, weak<const Float4x4> node2)
{
    return ref<FloatdivFloat4x4>::create(Arena::script(), node1, node2);
}

class Float4x4divFloat : public Float4x4
{
private:
    weak<const Float4x4> node1;
    weak<const Float> node2;
public:
    Float4x4divFloat(weak<const Float4x4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Float4x4, node1, node2);
        }
    }
};
ref<Float4x4> operator /(weak<const Float4x4> node1, weak<const Float> node2)
{
    return ref<Float4x4divFloat>::create(Arena::script(), node1, node2);
}

class FloataddFloat4x4 : public Float4x4
{
private:
    weak<const Float> node1;
    weak<const Float4x4> node2;
public:
    FloataddFloat4x4(weak<const Float> node1, weak<const Float4x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float4x4, node1, node2);
        }
    }
};
ref<Float4x4> operator +(weak<const Float> node1, weak<const Float4x4> node2)
{
    return ref<FloataddFloat4x4>::create(Arena::script(), node1, node2);
}

class Float4x4addFloat : public Float4x4
{
private:
    weak<const Float4x4> node1;
    weak<const Float> node2;
public:
    Float4x4addFloat(weak<const Float4x4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Float4x4, node1, node2);
        }
    }
};
ref<Float4x4> operator +(weak<const Float4x4> node1, weak<const Float> node2)
{
    return ref<Float4x4addFloat>::create(Arena::script(), node1, node2);
}

class FloatsubFloat4x4 : public Float4x4
{
private:
    weak<const Float> node1;
    weak<const Float4x4> node2;
public:
    FloatsubFloat4x4(weak<const Float> node1, weak<const Float4x4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float4x4, node1, node2);
        }
    }
};
ref<Float4x4> operator -(weak<const Float> node1, weak<const Float4x4> node2)
{
    return ref<FloatsubFloat4x4>::create(Arena::script(), node1, node2);
}

class Float4x4subFloat : public Float4x4
{
private:
    weak<const Float4x4> node1;
    weak<const Float> node2;
public:
    Float4x4subFloat(weak<const Float4x4> node1, weak<const Float> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Float4x4, node1, node2);
        }
    }
};
ref<Float4x4> operator -(weak<const Float4x4> node1, weak<const Float> node2)
{
    return ref<Float4x4subFloat>::create(Arena::script(), node1, node2);
}





}}
