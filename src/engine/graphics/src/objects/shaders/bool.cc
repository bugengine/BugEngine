/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <graphics/stdafx.h>
#include    <graphics/objects/shaders/bool.script.hh>
#include    <graphics/tools/ishaderbuilder.hh>

namespace BugEngine { namespace Graphics { namespace Shaders
{
/* Type *************************************************************/
Bool::Bool()
{
}
Bool::~Bool()
{
}

/* Uniform **********************************************************/
BoolUniform::BoolUniform(const istring& name)
    :   name(name)
{
}
BoolUniform::~BoolUniform()
{
}
void BoolUniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Bool);
}
void BoolUniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
BoolAttribute::BoolAttribute()
{
}
BoolAttribute::~BoolAttribute()
{
}
void BoolAttribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Bool);
}
void BoolAttribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
BoolVarying::BoolVarying(weak<const Bool> node)
    :   node(node)
{
}
BoolVarying::~BoolVarying()
{
}
void BoolVarying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Bool);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void BoolVarying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Boolmul : public Bool
{
private:
    weak<const Bool> node1;
    weak<const Bool> node2;
public:
    Boolmul(weak<const Bool> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Bool, node1, node2);
        }
    }
};
ref<Bool> operator *(weak<const Bool> node1, weak<const Bool> node2)
{
    return ref<Boolmul>::create(gameArena(), node1, node2);
}

class Booldiv : public Bool
{
private:
    weak<const Bool> node1;
    weak<const Bool> node2;
public:
    Booldiv(weak<const Bool> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Bool, node1, node2);
        }
    }
};
ref<Bool> operator /(weak<const Bool> node1, weak<const Bool> node2)
{
    return ref<Booldiv>::create(gameArena(), node1, node2);
}

class Booladd : public Bool
{
private:
    weak<const Bool> node1;
    weak<const Bool> node2;
public:
    Booladd(weak<const Bool> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Bool, node1, node2);
        }
    }
};
ref<Bool> operator +(weak<const Bool> node1, weak<const Bool> node2)
{
    return ref<Booladd>::create(gameArena(), node1, node2);
}

class Boolsub : public Bool
{
private:
    weak<const Bool> node1;
    weak<const Bool> node2;
public:
    Boolsub(weak<const Bool> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Bool, node1, node2);
        }
    }
};
ref<Bool> operator -(weak<const Bool> node1, weak<const Bool> node2)
{
    return ref<Boolsub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Bool2::Bool2()
{
}
Bool2::~Bool2()
{
}

/* Uniform **********************************************************/
Bool2Uniform::Bool2Uniform(const istring& name)
    :   name(name)
{
}
Bool2Uniform::~Bool2Uniform()
{
}
void Bool2Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Bool2);
}
void Bool2Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Bool2Attribute::Bool2Attribute()
{
}
Bool2Attribute::~Bool2Attribute()
{
}
void Bool2Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Bool2);
}
void Bool2Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Bool2Varying::Bool2Varying(weak<const Bool2> node)
    :   node(node)
{
}
Bool2Varying::~Bool2Varying()
{
}
void Bool2Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Bool2);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Bool2Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Bool2mul : public Bool2
{
private:
    weak<const Bool2> node1;
    weak<const Bool2> node2;
public:
    Bool2mul(weak<const Bool2> node1, weak<const Bool2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Bool2, node1, node2);
        }
    }
};
ref<Bool2> operator *(weak<const Bool2> node1, weak<const Bool2> node2)
{
    return ref<Bool2mul>::create(gameArena(), node1, node2);
}

class Bool2div : public Bool2
{
private:
    weak<const Bool2> node1;
    weak<const Bool2> node2;
public:
    Bool2div(weak<const Bool2> node1, weak<const Bool2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Bool2, node1, node2);
        }
    }
};
ref<Bool2> operator /(weak<const Bool2> node1, weak<const Bool2> node2)
{
    return ref<Bool2div>::create(gameArena(), node1, node2);
}

class Bool2add : public Bool2
{
private:
    weak<const Bool2> node1;
    weak<const Bool2> node2;
public:
    Bool2add(weak<const Bool2> node1, weak<const Bool2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Bool2, node1, node2);
        }
    }
};
ref<Bool2> operator +(weak<const Bool2> node1, weak<const Bool2> node2)
{
    return ref<Bool2add>::create(gameArena(), node1, node2);
}

class Bool2sub : public Bool2
{
private:
    weak<const Bool2> node1;
    weak<const Bool2> node2;
public:
    Bool2sub(weak<const Bool2> node1, weak<const Bool2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Bool2, node1, node2);
        }
    }
};
ref<Bool2> operator -(weak<const Bool2> node1, weak<const Bool2> node2)
{
    return ref<Bool2sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Bool3::Bool3()
{
}
Bool3::~Bool3()
{
}

/* Uniform **********************************************************/
Bool3Uniform::Bool3Uniform(const istring& name)
    :   name(name)
{
}
Bool3Uniform::~Bool3Uniform()
{
}
void Bool3Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Bool3);
}
void Bool3Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Bool3Attribute::Bool3Attribute()
{
}
Bool3Attribute::~Bool3Attribute()
{
}
void Bool3Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Bool3);
}
void Bool3Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Bool3Varying::Bool3Varying(weak<const Bool3> node)
    :   node(node)
{
}
Bool3Varying::~Bool3Varying()
{
}
void Bool3Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Bool3);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Bool3Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Bool3mul : public Bool3
{
private:
    weak<const Bool3> node1;
    weak<const Bool3> node2;
public:
    Bool3mul(weak<const Bool3> node1, weak<const Bool3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Bool3, node1, node2);
        }
    }
};
ref<Bool3> operator *(weak<const Bool3> node1, weak<const Bool3> node2)
{
    return ref<Bool3mul>::create(gameArena(), node1, node2);
}

class Bool3div : public Bool3
{
private:
    weak<const Bool3> node1;
    weak<const Bool3> node2;
public:
    Bool3div(weak<const Bool3> node1, weak<const Bool3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Bool3, node1, node2);
        }
    }
};
ref<Bool3> operator /(weak<const Bool3> node1, weak<const Bool3> node2)
{
    return ref<Bool3div>::create(gameArena(), node1, node2);
}

class Bool3add : public Bool3
{
private:
    weak<const Bool3> node1;
    weak<const Bool3> node2;
public:
    Bool3add(weak<const Bool3> node1, weak<const Bool3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Bool3, node1, node2);
        }
    }
};
ref<Bool3> operator +(weak<const Bool3> node1, weak<const Bool3> node2)
{
    return ref<Bool3add>::create(gameArena(), node1, node2);
}

class Bool3sub : public Bool3
{
private:
    weak<const Bool3> node1;
    weak<const Bool3> node2;
public:
    Bool3sub(weak<const Bool3> node1, weak<const Bool3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Bool3, node1, node2);
        }
    }
};
ref<Bool3> operator -(weak<const Bool3> node1, weak<const Bool3> node2)
{
    return ref<Bool3sub>::create(gameArena(), node1, node2);
}




/* Type *************************************************************/
Bool4::Bool4()
{
}
Bool4::~Bool4()
{
}

/* Uniform **********************************************************/
Bool4Uniform::Bool4Uniform(const istring& name)
    :   name(name)
{
}
Bool4Uniform::~Bool4Uniform()
{
}
void Bool4Uniform::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Type_Bool4);
}
void Bool4Uniform::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Bool4Attribute::Bool4Attribute()
{
}
Bool4Attribute::~Bool4Attribute()
{
}
void Bool4Attribute::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Type_Bool4);
}
void Bool4Attribute::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

/* Varying **********************************************************/
Bool4Varying::Bool4Varying(weak<const Bool4> node)
    :   node(node)
{
}
Bool4Varying::~Bool4Varying()
{
}
void Bool4Varying::buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Type_Bool4);
    node->buildDeclarations(stream, VertexStage, targetStage);
}
void Bool4Varying::buildDefinitions(IShaderBuilder& /*stream*/, Stage /*currentStage*/, Stage /*targetStage*/) const
{
}

class Bool4mul : public Bool4
{
private:
    weak<const Bool4> node1;
    weak<const Bool4> node2;
public:
    Bool4mul(weak<const Bool4> node1, weak<const Bool4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_mul, Type_Bool4, node1, node2);
        }
    }
};
ref<Bool4> operator *(weak<const Bool4> node1, weak<const Bool4> node2)
{
    return ref<Bool4mul>::create(gameArena(), node1, node2);
}

class Bool4div : public Bool4
{
private:
    weak<const Bool4> node1;
    weak<const Bool4> node2;
public:
    Bool4div(weak<const Bool4> node1, weak<const Bool4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_div, Type_Bool4, node1, node2);
        }
    }
};
ref<Bool4> operator /(weak<const Bool4> node1, weak<const Bool4> node2)
{
    return ref<Bool4div>::create(gameArena(), node1, node2);
}

class Bool4add : public Bool4
{
private:
    weak<const Bool4> node1;
    weak<const Bool4> node2;
public:
    Bool4add(weak<const Bool4> node1, weak<const Bool4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_add, Type_Bool4, node1, node2);
        }
    }
};
ref<Bool4> operator +(weak<const Bool4> node1, weak<const Bool4> node2)
{
    return ref<Bool4add>::create(gameArena(), node1, node2);
}

class Bool4sub : public Bool4
{
private:
    weak<const Bool4> node1;
    weak<const Bool4> node2;
public:
    Bool4sub(weak<const Bool4> node1, weak<const Bool4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDeclarations(stream, currentStage, targetStage);
        node2->buildDeclarations(stream, currentStage, targetStage);
    }
    void buildDefinitions(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
        node1->buildDefinitions(stream, currentStage, targetStage);
        node2->buildDefinitions(stream, currentStage, targetStage);
        if (targetStage == currentStage)
        {
            stream.addOperator(this, Op_sub, Type_Bool4, node1, node2);
        }
    }
};
ref<Bool4> operator -(weak<const Bool4> node1, weak<const Bool4> node2)
{
    return ref<Bool4sub>::create(gameArena(), node1, node2);
}




}}}
