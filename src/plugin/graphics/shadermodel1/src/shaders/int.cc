/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <shadermodel1/stdafx.h>
#include    <shadermodel1/shaders/int.script.hh>
#include    <3d/shader/ishaderbuilder.hh>

namespace BugEngine { namespace Shaders {namespace Int
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

class IntdivInt : public Int
{
private:
    weak<const Int> node1;
    weak<const Int> node2;
public:
    IntdivInt(weak<const Int> node1, weak<const Int> node2)
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
    return ref<IntdivInt>::create(Arena::script(), node1, node2);
}

class IntaddInt : public Int
{
private:
    weak<const Int> node1;
    weak<const Int> node2;
public:
    IntaddInt(weak<const Int> node1, weak<const Int> node2)
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
    return ref<IntaddInt>::create(Arena::script(), node1, node2);
}

class IntsubInt : public Int
{
private:
    weak<const Int> node1;
    weak<const Int> node2;
public:
    IntsubInt(weak<const Int> node1, weak<const Int> node2)
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
    return ref<IntsubInt>::create(Arena::script(), node1, node2);
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

class Int2divInt2 : public Int2
{
private:
    weak<const Int2> node1;
    weak<const Int2> node2;
public:
    Int2divInt2(weak<const Int2> node1, weak<const Int2> node2)
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
    return ref<Int2divInt2>::create(Arena::script(), node1, node2);
}

class Int2addInt2 : public Int2
{
private:
    weak<const Int2> node1;
    weak<const Int2> node2;
public:
    Int2addInt2(weak<const Int2> node1, weak<const Int2> node2)
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
    return ref<Int2addInt2>::create(Arena::script(), node1, node2);
}

class Int2subInt2 : public Int2
{
private:
    weak<const Int2> node1;
    weak<const Int2> node2;
public:
    Int2subInt2(weak<const Int2> node1, weak<const Int2> node2)
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
    return ref<Int2subInt2>::create(Arena::script(), node1, node2);
}

class IntmulInt2 : public Int2
{
private:
    weak<const Int> node1;
    weak<const Int2> node2;
public:
    IntmulInt2(weak<const Int> node1, weak<const Int2> node2)
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
ref<Int2> operator *(weak<const Int> node1, weak<const Int2> node2)
{
    return ref<IntmulInt2>::create(Arena::script(), node1, node2);
}

class Int2mulInt : public Int2
{
private:
    weak<const Int2> node1;
    weak<const Int> node2;
public:
    Int2mulInt(weak<const Int2> node1, weak<const Int> node2)
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
ref<Int2> operator *(weak<const Int2> node1, weak<const Int> node2)
{
    return ref<Int2mulInt>::create(Arena::script(), node1, node2);
}

class IntdivInt2 : public Int2
{
private:
    weak<const Int> node1;
    weak<const Int2> node2;
public:
    IntdivInt2(weak<const Int> node1, weak<const Int2> node2)
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
ref<Int2> operator /(weak<const Int> node1, weak<const Int2> node2)
{
    return ref<IntdivInt2>::create(Arena::script(), node1, node2);
}

class Int2divInt : public Int2
{
private:
    weak<const Int2> node1;
    weak<const Int> node2;
public:
    Int2divInt(weak<const Int2> node1, weak<const Int> node2)
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
ref<Int2> operator /(weak<const Int2> node1, weak<const Int> node2)
{
    return ref<Int2divInt>::create(Arena::script(), node1, node2);
}

class IntaddInt2 : public Int2
{
private:
    weak<const Int> node1;
    weak<const Int2> node2;
public:
    IntaddInt2(weak<const Int> node1, weak<const Int2> node2)
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
ref<Int2> operator +(weak<const Int> node1, weak<const Int2> node2)
{
    return ref<IntaddInt2>::create(Arena::script(), node1, node2);
}

class Int2addInt : public Int2
{
private:
    weak<const Int2> node1;
    weak<const Int> node2;
public:
    Int2addInt(weak<const Int2> node1, weak<const Int> node2)
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
ref<Int2> operator +(weak<const Int2> node1, weak<const Int> node2)
{
    return ref<Int2addInt>::create(Arena::script(), node1, node2);
}

class IntsubInt2 : public Int2
{
private:
    weak<const Int> node1;
    weak<const Int2> node2;
public:
    IntsubInt2(weak<const Int> node1, weak<const Int2> node2)
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
ref<Int2> operator -(weak<const Int> node1, weak<const Int2> node2)
{
    return ref<IntsubInt2>::create(Arena::script(), node1, node2);
}

class Int2subInt : public Int2
{
private:
    weak<const Int2> node1;
    weak<const Int> node2;
public:
    Int2subInt(weak<const Int2> node1, weak<const Int> node2)
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
ref<Int2> operator -(weak<const Int2> node1, weak<const Int> node2)
{
    return ref<Int2subInt>::create(Arena::script(), node1, node2);
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

class Int3divInt3 : public Int3
{
private:
    weak<const Int3> node1;
    weak<const Int3> node2;
public:
    Int3divInt3(weak<const Int3> node1, weak<const Int3> node2)
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
    return ref<Int3divInt3>::create(Arena::script(), node1, node2);
}

class Int3addInt3 : public Int3
{
private:
    weak<const Int3> node1;
    weak<const Int3> node2;
public:
    Int3addInt3(weak<const Int3> node1, weak<const Int3> node2)
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
    return ref<Int3addInt3>::create(Arena::script(), node1, node2);
}

class Int3subInt3 : public Int3
{
private:
    weak<const Int3> node1;
    weak<const Int3> node2;
public:
    Int3subInt3(weak<const Int3> node1, weak<const Int3> node2)
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
    return ref<Int3subInt3>::create(Arena::script(), node1, node2);
}

class IntmulInt3 : public Int3
{
private:
    weak<const Int> node1;
    weak<const Int3> node2;
public:
    IntmulInt3(weak<const Int> node1, weak<const Int3> node2)
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
ref<Int3> operator *(weak<const Int> node1, weak<const Int3> node2)
{
    return ref<IntmulInt3>::create(Arena::script(), node1, node2);
}

class Int3mulInt : public Int3
{
private:
    weak<const Int3> node1;
    weak<const Int> node2;
public:
    Int3mulInt(weak<const Int3> node1, weak<const Int> node2)
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
ref<Int3> operator *(weak<const Int3> node1, weak<const Int> node2)
{
    return ref<Int3mulInt>::create(Arena::script(), node1, node2);
}

class IntdivInt3 : public Int3
{
private:
    weak<const Int> node1;
    weak<const Int3> node2;
public:
    IntdivInt3(weak<const Int> node1, weak<const Int3> node2)
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
ref<Int3> operator /(weak<const Int> node1, weak<const Int3> node2)
{
    return ref<IntdivInt3>::create(Arena::script(), node1, node2);
}

class Int3divInt : public Int3
{
private:
    weak<const Int3> node1;
    weak<const Int> node2;
public:
    Int3divInt(weak<const Int3> node1, weak<const Int> node2)
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
ref<Int3> operator /(weak<const Int3> node1, weak<const Int> node2)
{
    return ref<Int3divInt>::create(Arena::script(), node1, node2);
}

class IntaddInt3 : public Int3
{
private:
    weak<const Int> node1;
    weak<const Int3> node2;
public:
    IntaddInt3(weak<const Int> node1, weak<const Int3> node2)
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
ref<Int3> operator +(weak<const Int> node1, weak<const Int3> node2)
{
    return ref<IntaddInt3>::create(Arena::script(), node1, node2);
}

class Int3addInt : public Int3
{
private:
    weak<const Int3> node1;
    weak<const Int> node2;
public:
    Int3addInt(weak<const Int3> node1, weak<const Int> node2)
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
ref<Int3> operator +(weak<const Int3> node1, weak<const Int> node2)
{
    return ref<Int3addInt>::create(Arena::script(), node1, node2);
}

class IntsubInt3 : public Int3
{
private:
    weak<const Int> node1;
    weak<const Int3> node2;
public:
    IntsubInt3(weak<const Int> node1, weak<const Int3> node2)
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
ref<Int3> operator -(weak<const Int> node1, weak<const Int3> node2)
{
    return ref<IntsubInt3>::create(Arena::script(), node1, node2);
}

class Int3subInt : public Int3
{
private:
    weak<const Int3> node1;
    weak<const Int> node2;
public:
    Int3subInt(weak<const Int3> node1, weak<const Int> node2)
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
ref<Int3> operator -(weak<const Int3> node1, weak<const Int> node2)
{
    return ref<Int3subInt>::create(Arena::script(), node1, node2);
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

class Int4divInt4 : public Int4
{
private:
    weak<const Int4> node1;
    weak<const Int4> node2;
public:
    Int4divInt4(weak<const Int4> node1, weak<const Int4> node2)
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
    return ref<Int4divInt4>::create(Arena::script(), node1, node2);
}

class Int4addInt4 : public Int4
{
private:
    weak<const Int4> node1;
    weak<const Int4> node2;
public:
    Int4addInt4(weak<const Int4> node1, weak<const Int4> node2)
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
    return ref<Int4addInt4>::create(Arena::script(), node1, node2);
}

class Int4subInt4 : public Int4
{
private:
    weak<const Int4> node1;
    weak<const Int4> node2;
public:
    Int4subInt4(weak<const Int4> node1, weak<const Int4> node2)
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
    return ref<Int4subInt4>::create(Arena::script(), node1, node2);
}

class IntmulInt4 : public Int4
{
private:
    weak<const Int> node1;
    weak<const Int4> node2;
public:
    IntmulInt4(weak<const Int> node1, weak<const Int4> node2)
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
ref<Int4> operator *(weak<const Int> node1, weak<const Int4> node2)
{
    return ref<IntmulInt4>::create(Arena::script(), node1, node2);
}

class Int4mulInt : public Int4
{
private:
    weak<const Int4> node1;
    weak<const Int> node2;
public:
    Int4mulInt(weak<const Int4> node1, weak<const Int> node2)
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
ref<Int4> operator *(weak<const Int4> node1, weak<const Int> node2)
{
    return ref<Int4mulInt>::create(Arena::script(), node1, node2);
}

class IntdivInt4 : public Int4
{
private:
    weak<const Int> node1;
    weak<const Int4> node2;
public:
    IntdivInt4(weak<const Int> node1, weak<const Int4> node2)
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
ref<Int4> operator /(weak<const Int> node1, weak<const Int4> node2)
{
    return ref<IntdivInt4>::create(Arena::script(), node1, node2);
}

class Int4divInt : public Int4
{
private:
    weak<const Int4> node1;
    weak<const Int> node2;
public:
    Int4divInt(weak<const Int4> node1, weak<const Int> node2)
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
ref<Int4> operator /(weak<const Int4> node1, weak<const Int> node2)
{
    return ref<Int4divInt>::create(Arena::script(), node1, node2);
}

class IntaddInt4 : public Int4
{
private:
    weak<const Int> node1;
    weak<const Int4> node2;
public:
    IntaddInt4(weak<const Int> node1, weak<const Int4> node2)
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
ref<Int4> operator +(weak<const Int> node1, weak<const Int4> node2)
{
    return ref<IntaddInt4>::create(Arena::script(), node1, node2);
}

class Int4addInt : public Int4
{
private:
    weak<const Int4> node1;
    weak<const Int> node2;
public:
    Int4addInt(weak<const Int4> node1, weak<const Int> node2)
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
ref<Int4> operator +(weak<const Int4> node1, weak<const Int> node2)
{
    return ref<Int4addInt>::create(Arena::script(), node1, node2);
}

class IntsubInt4 : public Int4
{
private:
    weak<const Int> node1;
    weak<const Int4> node2;
public:
    IntsubInt4(weak<const Int> node1, weak<const Int4> node2)
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
ref<Int4> operator -(weak<const Int> node1, weak<const Int4> node2)
{
    return ref<IntsubInt4>::create(Arena::script(), node1, node2);
}

class Int4subInt : public Int4
{
private:
    weak<const Int4> node1;
    weak<const Int> node2;
public:
    Int4subInt(weak<const Int4> node1, weak<const Int> node2)
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
ref<Int4> operator -(weak<const Int4> node1, weak<const Int> node2)
{
    return ref<Int4subInt>::create(Arena::script(), node1, node2);
}




}}}
