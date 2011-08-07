/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shaders/uint.script.hh>
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{
/* Type *************************************************************/
Uint::Uint()
{
}
Uint::~Uint()
{
}

/* Uniform **********************************************************/
UintUniform::UintUniform(const istring& name)
    :   name(name)
{
}
UintUniform::~UintUniform()
{
}
void UintUniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Uint);
}
void UintUniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
UintAttribute::UintAttribute()
{
}
UintAttribute::~UintAttribute()
{
}
void UintAttribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Uint);
}
void UintAttribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
UintVarying::UintVarying(weak<const Uint> node)
    :   node(node)
{
}
UintVarying::~UintVarying()
{
}
void UintVarying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Uint);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void UintVarying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Uintmul : public Uint
{
private:
    weak<const Uint> node1;
    weak<const Uint> node2;
public:
    Uintmul(weak<const Uint> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Op_mul, Type_Uint, node1, node2);
        }
    }
};
ref<Uint> operator *(weak<const Uint> node1, weak<const Uint> node2)
{
    return ref<Uintmul>::create(gameArena(), node1, node2);
}

class Uintdiv : public Uint
{
private:
    weak<const Uint> node1;
    weak<const Uint> node2;
public:
    Uintdiv(weak<const Uint> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Op_div, Type_Uint, node1, node2);
        }
    }
};
ref<Uint> operator /(weak<const Uint> node1, weak<const Uint> node2)
{
    return ref<Uintdiv>::create(gameArena(), node1, node2);
}

class Uintadd : public Uint
{
private:
    weak<const Uint> node1;
    weak<const Uint> node2;
public:
    Uintadd(weak<const Uint> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Op_add, Type_Uint, node1, node2);
        }
    }
};
ref<Uint> operator +(weak<const Uint> node1, weak<const Uint> node2)
{
    return ref<Uintadd>::create(gameArena(), node1, node2);
}

class Uintsub : public Uint
{
private:
    weak<const Uint> node1;
    weak<const Uint> node2;
public:
    Uintsub(weak<const Uint> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Op_sub, Type_Uint, node1, node2);
        }
    }
};
ref<Uint> operator -(weak<const Uint> node1, weak<const Uint> node2)
{
    return ref<Uintsub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Uint2::Uint2()
{
}
Uint2::~Uint2()
{
}

/* Uniform **********************************************************/
Uint2Uniform::Uint2Uniform(const istring& name)
    :   name(name)
{
}
Uint2Uniform::~Uint2Uniform()
{
}
void Uint2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Uint2);
}
void Uint2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Uint2Attribute::Uint2Attribute()
{
}
Uint2Attribute::~Uint2Attribute()
{
}
void Uint2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Uint2);
}
void Uint2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Uint2Varying::Uint2Varying(weak<const Uint2> node)
    :   node(node)
{
}
Uint2Varying::~Uint2Varying()
{
}
void Uint2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Uint2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Uint2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Uint2mul : public Uint2
{
private:
    weak<const Uint2> node1;
    weak<const Uint2> node2;
public:
    Uint2mul(weak<const Uint2> node1, weak<const Uint2> node2)
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
            stream.addOperator(this, Op_mul, Type_Uint2, node1, node2);
        }
    }
};
ref<Uint2> operator *(weak<const Uint2> node1, weak<const Uint2> node2)
{
    return ref<Uint2mul>::create(gameArena(), node1, node2);
}

class Uint2div : public Uint2
{
private:
    weak<const Uint2> node1;
    weak<const Uint2> node2;
public:
    Uint2div(weak<const Uint2> node1, weak<const Uint2> node2)
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
            stream.addOperator(this, Op_div, Type_Uint2, node1, node2);
        }
    }
};
ref<Uint2> operator /(weak<const Uint2> node1, weak<const Uint2> node2)
{
    return ref<Uint2div>::create(gameArena(), node1, node2);
}

class Uint2add : public Uint2
{
private:
    weak<const Uint2> node1;
    weak<const Uint2> node2;
public:
    Uint2add(weak<const Uint2> node1, weak<const Uint2> node2)
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
            stream.addOperator(this, Op_add, Type_Uint2, node1, node2);
        }
    }
};
ref<Uint2> operator +(weak<const Uint2> node1, weak<const Uint2> node2)
{
    return ref<Uint2add>::create(gameArena(), node1, node2);
}

class Uint2sub : public Uint2
{
private:
    weak<const Uint2> node1;
    weak<const Uint2> node2;
public:
    Uint2sub(weak<const Uint2> node1, weak<const Uint2> node2)
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
            stream.addOperator(this, Op_sub, Type_Uint2, node1, node2);
        }
    }
};
ref<Uint2> operator -(weak<const Uint2> node1, weak<const Uint2> node2)
{
    return ref<Uint2sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Uint3::Uint3()
{
}
Uint3::~Uint3()
{
}

/* Uniform **********************************************************/
Uint3Uniform::Uint3Uniform(const istring& name)
    :   name(name)
{
}
Uint3Uniform::~Uint3Uniform()
{
}
void Uint3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Uint3);
}
void Uint3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Uint3Attribute::Uint3Attribute()
{
}
Uint3Attribute::~Uint3Attribute()
{
}
void Uint3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Uint3);
}
void Uint3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Uint3Varying::Uint3Varying(weak<const Uint3> node)
    :   node(node)
{
}
Uint3Varying::~Uint3Varying()
{
}
void Uint3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Uint3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Uint3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Uint3mul : public Uint3
{
private:
    weak<const Uint3> node1;
    weak<const Uint3> node2;
public:
    Uint3mul(weak<const Uint3> node1, weak<const Uint3> node2)
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
            stream.addOperator(this, Op_mul, Type_Uint3, node1, node2);
        }
    }
};
ref<Uint3> operator *(weak<const Uint3> node1, weak<const Uint3> node2)
{
    return ref<Uint3mul>::create(gameArena(), node1, node2);
}

class Uint3div : public Uint3
{
private:
    weak<const Uint3> node1;
    weak<const Uint3> node2;
public:
    Uint3div(weak<const Uint3> node1, weak<const Uint3> node2)
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
            stream.addOperator(this, Op_div, Type_Uint3, node1, node2);
        }
    }
};
ref<Uint3> operator /(weak<const Uint3> node1, weak<const Uint3> node2)
{
    return ref<Uint3div>::create(gameArena(), node1, node2);
}

class Uint3add : public Uint3
{
private:
    weak<const Uint3> node1;
    weak<const Uint3> node2;
public:
    Uint3add(weak<const Uint3> node1, weak<const Uint3> node2)
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
            stream.addOperator(this, Op_add, Type_Uint3, node1, node2);
        }
    }
};
ref<Uint3> operator +(weak<const Uint3> node1, weak<const Uint3> node2)
{
    return ref<Uint3add>::create(gameArena(), node1, node2);
}

class Uint3sub : public Uint3
{
private:
    weak<const Uint3> node1;
    weak<const Uint3> node2;
public:
    Uint3sub(weak<const Uint3> node1, weak<const Uint3> node2)
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
            stream.addOperator(this, Op_sub, Type_Uint3, node1, node2);
        }
    }
};
ref<Uint3> operator -(weak<const Uint3> node1, weak<const Uint3> node2)
{
    return ref<Uint3sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Uint4::Uint4()
{
}
Uint4::~Uint4()
{
}

/* Uniform **********************************************************/
Uint4Uniform::Uint4Uniform(const istring& name)
    :   name(name)
{
}
Uint4Uniform::~Uint4Uniform()
{
}
void Uint4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Uint4);
}
void Uint4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Uint4Attribute::Uint4Attribute()
{
}
Uint4Attribute::~Uint4Attribute()
{
}
void Uint4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Uint4);
}
void Uint4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Uint4Varying::Uint4Varying(weak<const Uint4> node)
    :   node(node)
{
}
Uint4Varying::~Uint4Varying()
{
}
void Uint4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Uint4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Uint4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Uint4mul : public Uint4
{
private:
    weak<const Uint4> node1;
    weak<const Uint4> node2;
public:
    Uint4mul(weak<const Uint4> node1, weak<const Uint4> node2)
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
            stream.addOperator(this, Op_mul, Type_Uint4, node1, node2);
        }
    }
};
ref<Uint4> operator *(weak<const Uint4> node1, weak<const Uint4> node2)
{
    return ref<Uint4mul>::create(gameArena(), node1, node2);
}

class Uint4div : public Uint4
{
private:
    weak<const Uint4> node1;
    weak<const Uint4> node2;
public:
    Uint4div(weak<const Uint4> node1, weak<const Uint4> node2)
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
            stream.addOperator(this, Op_div, Type_Uint4, node1, node2);
        }
    }
};
ref<Uint4> operator /(weak<const Uint4> node1, weak<const Uint4> node2)
{
    return ref<Uint4div>::create(gameArena(), node1, node2);
}

class Uint4add : public Uint4
{
private:
    weak<const Uint4> node1;
    weak<const Uint4> node2;
public:
    Uint4add(weak<const Uint4> node1, weak<const Uint4> node2)
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
            stream.addOperator(this, Op_add, Type_Uint4, node1, node2);
        }
    }
};
ref<Uint4> operator +(weak<const Uint4> node1, weak<const Uint4> node2)
{
    return ref<Uint4add>::create(gameArena(), node1, node2);
}

class Uint4sub : public Uint4
{
private:
    weak<const Uint4> node1;
    weak<const Uint4> node2;
public:
    Uint4sub(weak<const Uint4> node1, weak<const Uint4> node2)
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
            stream.addOperator(this, Op_sub, Type_Uint4, node1, node2);
        }
    }
};
ref<Uint4> operator -(weak<const Uint4> node1, weak<const Uint4> node2)
{
    return ref<Uint4sub>::create(gameArena(), node1, node2);
}




}}}
