/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shaders/int.script.hh>
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{
/* Type *************************************************************/
Int::Int()
{
}
Int::~Int()
{
}

/* Uniform **********************************************************/
IntUniform::IntUniform(const istring& name)
    :   name(name)
{
}
IntUniform::~IntUniform()
{
}
void IntUniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Int);
}
void IntUniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
IntAttribute::IntAttribute()
{
}
IntAttribute::~IntAttribute()
{
}
void IntAttribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Int);
}
void IntAttribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
IntVarying::IntVarying(weak<const Int> node)
    :   node(node)
{
}
IntVarying::~IntVarying()
{
}
void IntVarying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Int);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void IntVarying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Intmul : public Int
{
private:
    weak<const Int> node1;
    weak<const Int> node2;
public:
    Intmul(weak<const Int> node1, weak<const Int> node2)
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
            stream.addOperator(this, Op_mul, Type_Int, node1, node2);
        }
    }
};
ref<Int> operator *(weak<const Int> node1, weak<const Int> node2)
{
    return ref<Intmul>::create(gameArena(), node1, node2);
}

class Intdiv : public Int
{
private:
    weak<const Int> node1;
    weak<const Int> node2;
public:
    Intdiv(weak<const Int> node1, weak<const Int> node2)
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
            stream.addOperator(this, Op_div, Type_Int, node1, node2);
        }
    }
};
ref<Int> operator /(weak<const Int> node1, weak<const Int> node2)
{
    return ref<Intdiv>::create(gameArena(), node1, node2);
}

class Intadd : public Int
{
private:
    weak<const Int> node1;
    weak<const Int> node2;
public:
    Intadd(weak<const Int> node1, weak<const Int> node2)
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
            stream.addOperator(this, Op_add, Type_Int, node1, node2);
        }
    }
};
ref<Int> operator +(weak<const Int> node1, weak<const Int> node2)
{
    return ref<Intadd>::create(gameArena(), node1, node2);
}

class Intsub : public Int
{
private:
    weak<const Int> node1;
    weak<const Int> node2;
public:
    Intsub(weak<const Int> node1, weak<const Int> node2)
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
            stream.addOperator(this, Op_sub, Type_Int, node1, node2);
        }
    }
};
ref<Int> operator -(weak<const Int> node1, weak<const Int> node2)
{
    return ref<Intsub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Int2::Int2()
{
}
Int2::~Int2()
{
}

/* Uniform **********************************************************/
Int2Uniform::Int2Uniform(const istring& name)
    :   name(name)
{
}
Int2Uniform::~Int2Uniform()
{
}
void Int2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Int2);
}
void Int2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Int2Attribute::Int2Attribute()
{
}
Int2Attribute::~Int2Attribute()
{
}
void Int2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Int2);
}
void Int2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Int2Varying::Int2Varying(weak<const Int2> node)
    :   node(node)
{
}
Int2Varying::~Int2Varying()
{
}
void Int2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Int2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Int2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Int2mul : public Int2
{
private:
    weak<const Int2> node1;
    weak<const Int2> node2;
public:
    Int2mul(weak<const Int2> node1, weak<const Int2> node2)
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
            stream.addOperator(this, Op_mul, Type_Int2, node1, node2);
        }
    }
};
ref<Int2> operator *(weak<const Int2> node1, weak<const Int2> node2)
{
    return ref<Int2mul>::create(gameArena(), node1, node2);
}

class Int2div : public Int2
{
private:
    weak<const Int2> node1;
    weak<const Int2> node2;
public:
    Int2div(weak<const Int2> node1, weak<const Int2> node2)
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
            stream.addOperator(this, Op_div, Type_Int2, node1, node2);
        }
    }
};
ref<Int2> operator /(weak<const Int2> node1, weak<const Int2> node2)
{
    return ref<Int2div>::create(gameArena(), node1, node2);
}

class Int2add : public Int2
{
private:
    weak<const Int2> node1;
    weak<const Int2> node2;
public:
    Int2add(weak<const Int2> node1, weak<const Int2> node2)
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
            stream.addOperator(this, Op_add, Type_Int2, node1, node2);
        }
    }
};
ref<Int2> operator +(weak<const Int2> node1, weak<const Int2> node2)
{
    return ref<Int2add>::create(gameArena(), node1, node2);
}

class Int2sub : public Int2
{
private:
    weak<const Int2> node1;
    weak<const Int2> node2;
public:
    Int2sub(weak<const Int2> node1, weak<const Int2> node2)
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
            stream.addOperator(this, Op_sub, Type_Int2, node1, node2);
        }
    }
};
ref<Int2> operator -(weak<const Int2> node1, weak<const Int2> node2)
{
    return ref<Int2sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Int3::Int3()
{
}
Int3::~Int3()
{
}

/* Uniform **********************************************************/
Int3Uniform::Int3Uniform(const istring& name)
    :   name(name)
{
}
Int3Uniform::~Int3Uniform()
{
}
void Int3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Int3);
}
void Int3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Int3Attribute::Int3Attribute()
{
}
Int3Attribute::~Int3Attribute()
{
}
void Int3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Int3);
}
void Int3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Int3Varying::Int3Varying(weak<const Int3> node)
    :   node(node)
{
}
Int3Varying::~Int3Varying()
{
}
void Int3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Int3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Int3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Int3mul : public Int3
{
private:
    weak<const Int3> node1;
    weak<const Int3> node2;
public:
    Int3mul(weak<const Int3> node1, weak<const Int3> node2)
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
            stream.addOperator(this, Op_mul, Type_Int3, node1, node2);
        }
    }
};
ref<Int3> operator *(weak<const Int3> node1, weak<const Int3> node2)
{
    return ref<Int3mul>::create(gameArena(), node1, node2);
}

class Int3div : public Int3
{
private:
    weak<const Int3> node1;
    weak<const Int3> node2;
public:
    Int3div(weak<const Int3> node1, weak<const Int3> node2)
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
            stream.addOperator(this, Op_div, Type_Int3, node1, node2);
        }
    }
};
ref<Int3> operator /(weak<const Int3> node1, weak<const Int3> node2)
{
    return ref<Int3div>::create(gameArena(), node1, node2);
}

class Int3add : public Int3
{
private:
    weak<const Int3> node1;
    weak<const Int3> node2;
public:
    Int3add(weak<const Int3> node1, weak<const Int3> node2)
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
            stream.addOperator(this, Op_add, Type_Int3, node1, node2);
        }
    }
};
ref<Int3> operator +(weak<const Int3> node1, weak<const Int3> node2)
{
    return ref<Int3add>::create(gameArena(), node1, node2);
}

class Int3sub : public Int3
{
private:
    weak<const Int3> node1;
    weak<const Int3> node2;
public:
    Int3sub(weak<const Int3> node1, weak<const Int3> node2)
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
            stream.addOperator(this, Op_sub, Type_Int3, node1, node2);
        }
    }
};
ref<Int3> operator -(weak<const Int3> node1, weak<const Int3> node2)
{
    return ref<Int3sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Int4::Int4()
{
}
Int4::~Int4()
{
}

/* Uniform **********************************************************/
Int4Uniform::Int4Uniform(const istring& name)
    :   name(name)
{
}
Int4Uniform::~Int4Uniform()
{
}
void Int4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Int4);
}
void Int4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Int4Attribute::Int4Attribute()
{
}
Int4Attribute::~Int4Attribute()
{
}
void Int4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Int4);
}
void Int4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Int4Varying::Int4Varying(weak<const Int4> node)
    :   node(node)
{
}
Int4Varying::~Int4Varying()
{
}
void Int4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Int4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Int4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Int4mul : public Int4
{
private:
    weak<const Int4> node1;
    weak<const Int4> node2;
public:
    Int4mul(weak<const Int4> node1, weak<const Int4> node2)
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
            stream.addOperator(this, Op_mul, Type_Int4, node1, node2);
        }
    }
};
ref<Int4> operator *(weak<const Int4> node1, weak<const Int4> node2)
{
    return ref<Int4mul>::create(gameArena(), node1, node2);
}

class Int4div : public Int4
{
private:
    weak<const Int4> node1;
    weak<const Int4> node2;
public:
    Int4div(weak<const Int4> node1, weak<const Int4> node2)
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
            stream.addOperator(this, Op_div, Type_Int4, node1, node2);
        }
    }
};
ref<Int4> operator /(weak<const Int4> node1, weak<const Int4> node2)
{
    return ref<Int4div>::create(gameArena(), node1, node2);
}

class Int4add : public Int4
{
private:
    weak<const Int4> node1;
    weak<const Int4> node2;
public:
    Int4add(weak<const Int4> node1, weak<const Int4> node2)
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
            stream.addOperator(this, Op_add, Type_Int4, node1, node2);
        }
    }
};
ref<Int4> operator +(weak<const Int4> node1, weak<const Int4> node2)
{
    return ref<Int4add>::create(gameArena(), node1, node2);
}

class Int4sub : public Int4
{
private:
    weak<const Int4> node1;
    weak<const Int4> node2;
public:
    Int4sub(weak<const Int4> node1, weak<const Int4> node2)
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
            stream.addOperator(this, Op_sub, Type_Int4, node1, node2);
        }
    }
};
ref<Int4> operator -(weak<const Int4> node1, weak<const Int4> node2)
{
    return ref<Int4sub>::create(gameArena(), node1, node2);
}




}}}
