/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <shadermodel1/stdafx.h>
#include    <shadermodel1/shaders/bool.script.hh>
#include    <3d/shader/ishaderbuilder.hh>

namespace BugEngine { namespace Shaders {namespace Bool
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

class BooldivBool : public Bool
{
private:
    weak<const Bool> node1;
    weak<const Bool> node2;
public:
    BooldivBool(weak<const Bool> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
    return ref<BooldivBool>::create(Arena::script(), node1, node2);
}

class BooladdBool : public Bool
{
private:
    weak<const Bool> node1;
    weak<const Bool> node2;
public:
    BooladdBool(weak<const Bool> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
    return ref<BooladdBool>::create(Arena::script(), node1, node2);
}

class BoolsubBool : public Bool
{
private:
    weak<const Bool> node1;
    weak<const Bool> node2;
public:
    BoolsubBool(weak<const Bool> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
    return ref<BoolsubBool>::create(Arena::script(), node1, node2);
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

class Bool2divBool2 : public Bool2
{
private:
    weak<const Bool2> node1;
    weak<const Bool2> node2;
public:
    Bool2divBool2(weak<const Bool2> node1, weak<const Bool2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
    return ref<Bool2divBool2>::create(Arena::script(), node1, node2);
}

class Bool2addBool2 : public Bool2
{
private:
    weak<const Bool2> node1;
    weak<const Bool2> node2;
public:
    Bool2addBool2(weak<const Bool2> node1, weak<const Bool2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
    return ref<Bool2addBool2>::create(Arena::script(), node1, node2);
}

class Bool2subBool2 : public Bool2
{
private:
    weak<const Bool2> node1;
    weak<const Bool2> node2;
public:
    Bool2subBool2(weak<const Bool2> node1, weak<const Bool2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
    return ref<Bool2subBool2>::create(Arena::script(), node1, node2);
}

class BoolmulBool2 : public Bool2
{
private:
    weak<const Bool> node1;
    weak<const Bool2> node2;
public:
    BoolmulBool2(weak<const Bool> node1, weak<const Bool2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool2> operator *(weak<const Bool> node1, weak<const Bool2> node2)
{
    return ref<BoolmulBool2>::create(Arena::script(), node1, node2);
}

class Bool2mulBool : public Bool2
{
private:
    weak<const Bool2> node1;
    weak<const Bool> node2;
public:
    Bool2mulBool(weak<const Bool2> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool2> operator *(weak<const Bool2> node1, weak<const Bool> node2)
{
    return ref<Bool2mulBool>::create(Arena::script(), node1, node2);
}

class BooldivBool2 : public Bool2
{
private:
    weak<const Bool> node1;
    weak<const Bool2> node2;
public:
    BooldivBool2(weak<const Bool> node1, weak<const Bool2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool2> operator /(weak<const Bool> node1, weak<const Bool2> node2)
{
    return ref<BooldivBool2>::create(Arena::script(), node1, node2);
}

class Bool2divBool : public Bool2
{
private:
    weak<const Bool2> node1;
    weak<const Bool> node2;
public:
    Bool2divBool(weak<const Bool2> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool2> operator /(weak<const Bool2> node1, weak<const Bool> node2)
{
    return ref<Bool2divBool>::create(Arena::script(), node1, node2);
}

class BooladdBool2 : public Bool2
{
private:
    weak<const Bool> node1;
    weak<const Bool2> node2;
public:
    BooladdBool2(weak<const Bool> node1, weak<const Bool2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool2> operator +(weak<const Bool> node1, weak<const Bool2> node2)
{
    return ref<BooladdBool2>::create(Arena::script(), node1, node2);
}

class Bool2addBool : public Bool2
{
private:
    weak<const Bool2> node1;
    weak<const Bool> node2;
public:
    Bool2addBool(weak<const Bool2> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool2> operator +(weak<const Bool2> node1, weak<const Bool> node2)
{
    return ref<Bool2addBool>::create(Arena::script(), node1, node2);
}

class BoolsubBool2 : public Bool2
{
private:
    weak<const Bool> node1;
    weak<const Bool2> node2;
public:
    BoolsubBool2(weak<const Bool> node1, weak<const Bool2> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool2> operator -(weak<const Bool> node1, weak<const Bool2> node2)
{
    return ref<BoolsubBool2>::create(Arena::script(), node1, node2);
}

class Bool2subBool : public Bool2
{
private:
    weak<const Bool2> node1;
    weak<const Bool> node2;
public:
    Bool2subBool(weak<const Bool2> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool2> operator -(weak<const Bool2> node1, weak<const Bool> node2)
{
    return ref<Bool2subBool>::create(Arena::script(), node1, node2);
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

class Bool3divBool3 : public Bool3
{
private:
    weak<const Bool3> node1;
    weak<const Bool3> node2;
public:
    Bool3divBool3(weak<const Bool3> node1, weak<const Bool3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
    return ref<Bool3divBool3>::create(Arena::script(), node1, node2);
}

class Bool3addBool3 : public Bool3
{
private:
    weak<const Bool3> node1;
    weak<const Bool3> node2;
public:
    Bool3addBool3(weak<const Bool3> node1, weak<const Bool3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
    return ref<Bool3addBool3>::create(Arena::script(), node1, node2);
}

class Bool3subBool3 : public Bool3
{
private:
    weak<const Bool3> node1;
    weak<const Bool3> node2;
public:
    Bool3subBool3(weak<const Bool3> node1, weak<const Bool3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
    return ref<Bool3subBool3>::create(Arena::script(), node1, node2);
}

class BoolmulBool3 : public Bool3
{
private:
    weak<const Bool> node1;
    weak<const Bool3> node2;
public:
    BoolmulBool3(weak<const Bool> node1, weak<const Bool3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool3> operator *(weak<const Bool> node1, weak<const Bool3> node2)
{
    return ref<BoolmulBool3>::create(Arena::script(), node1, node2);
}

class Bool3mulBool : public Bool3
{
private:
    weak<const Bool3> node1;
    weak<const Bool> node2;
public:
    Bool3mulBool(weak<const Bool3> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool3> operator *(weak<const Bool3> node1, weak<const Bool> node2)
{
    return ref<Bool3mulBool>::create(Arena::script(), node1, node2);
}

class BooldivBool3 : public Bool3
{
private:
    weak<const Bool> node1;
    weak<const Bool3> node2;
public:
    BooldivBool3(weak<const Bool> node1, weak<const Bool3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool3> operator /(weak<const Bool> node1, weak<const Bool3> node2)
{
    return ref<BooldivBool3>::create(Arena::script(), node1, node2);
}

class Bool3divBool : public Bool3
{
private:
    weak<const Bool3> node1;
    weak<const Bool> node2;
public:
    Bool3divBool(weak<const Bool3> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool3> operator /(weak<const Bool3> node1, weak<const Bool> node2)
{
    return ref<Bool3divBool>::create(Arena::script(), node1, node2);
}

class BooladdBool3 : public Bool3
{
private:
    weak<const Bool> node1;
    weak<const Bool3> node2;
public:
    BooladdBool3(weak<const Bool> node1, weak<const Bool3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool3> operator +(weak<const Bool> node1, weak<const Bool3> node2)
{
    return ref<BooladdBool3>::create(Arena::script(), node1, node2);
}

class Bool3addBool : public Bool3
{
private:
    weak<const Bool3> node1;
    weak<const Bool> node2;
public:
    Bool3addBool(weak<const Bool3> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool3> operator +(weak<const Bool3> node1, weak<const Bool> node2)
{
    return ref<Bool3addBool>::create(Arena::script(), node1, node2);
}

class BoolsubBool3 : public Bool3
{
private:
    weak<const Bool> node1;
    weak<const Bool3> node2;
public:
    BoolsubBool3(weak<const Bool> node1, weak<const Bool3> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool3> operator -(weak<const Bool> node1, weak<const Bool3> node2)
{
    return ref<BoolsubBool3>::create(Arena::script(), node1, node2);
}

class Bool3subBool : public Bool3
{
private:
    weak<const Bool3> node1;
    weak<const Bool> node2;
public:
    Bool3subBool(weak<const Bool3> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool3> operator -(weak<const Bool3> node1, weak<const Bool> node2)
{
    return ref<Bool3subBool>::create(Arena::script(), node1, node2);
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

class Bool4divBool4 : public Bool4
{
private:
    weak<const Bool4> node1;
    weak<const Bool4> node2;
public:
    Bool4divBool4(weak<const Bool4> node1, weak<const Bool4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
    return ref<Bool4divBool4>::create(Arena::script(), node1, node2);
}

class Bool4addBool4 : public Bool4
{
private:
    weak<const Bool4> node1;
    weak<const Bool4> node2;
public:
    Bool4addBool4(weak<const Bool4> node1, weak<const Bool4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
    return ref<Bool4addBool4>::create(Arena::script(), node1, node2);
}

class Bool4subBool4 : public Bool4
{
private:
    weak<const Bool4> node1;
    weak<const Bool4> node2;
public:
    Bool4subBool4(weak<const Bool4> node1, weak<const Bool4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
    return ref<Bool4subBool4>::create(Arena::script(), node1, node2);
}

class BoolmulBool4 : public Bool4
{
private:
    weak<const Bool> node1;
    weak<const Bool4> node2;
public:
    BoolmulBool4(weak<const Bool> node1, weak<const Bool4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool4> operator *(weak<const Bool> node1, weak<const Bool4> node2)
{
    return ref<BoolmulBool4>::create(Arena::script(), node1, node2);
}

class Bool4mulBool : public Bool4
{
private:
    weak<const Bool4> node1;
    weak<const Bool> node2;
public:
    Bool4mulBool(weak<const Bool4> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool4> operator *(weak<const Bool4> node1, weak<const Bool> node2)
{
    return ref<Bool4mulBool>::create(Arena::script(), node1, node2);
}

class BooldivBool4 : public Bool4
{
private:
    weak<const Bool> node1;
    weak<const Bool4> node2;
public:
    BooldivBool4(weak<const Bool> node1, weak<const Bool4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool4> operator /(weak<const Bool> node1, weak<const Bool4> node2)
{
    return ref<BooldivBool4>::create(Arena::script(), node1, node2);
}

class Bool4divBool : public Bool4
{
private:
    weak<const Bool4> node1;
    weak<const Bool> node2;
public:
    Bool4divBool(weak<const Bool4> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool4> operator /(weak<const Bool4> node1, weak<const Bool> node2)
{
    return ref<Bool4divBool>::create(Arena::script(), node1, node2);
}

class BooladdBool4 : public Bool4
{
private:
    weak<const Bool> node1;
    weak<const Bool4> node2;
public:
    BooladdBool4(weak<const Bool> node1, weak<const Bool4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool4> operator +(weak<const Bool> node1, weak<const Bool4> node2)
{
    return ref<BooladdBool4>::create(Arena::script(), node1, node2);
}

class Bool4addBool : public Bool4
{
private:
    weak<const Bool4> node1;
    weak<const Bool> node2;
public:
    Bool4addBool(weak<const Bool4> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool4> operator +(weak<const Bool4> node1, weak<const Bool> node2)
{
    return ref<Bool4addBool>::create(Arena::script(), node1, node2);
}

class BoolsubBool4 : public Bool4
{
private:
    weak<const Bool> node1;
    weak<const Bool4> node2;
public:
    BoolsubBool4(weak<const Bool> node1, weak<const Bool4> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool4> operator -(weak<const Bool> node1, weak<const Bool4> node2)
{
    return ref<BoolsubBool4>::create(Arena::script(), node1, node2);
}

class Bool4subBool : public Bool4
{
private:
    weak<const Bool4> node1;
    weak<const Bool> node2;
public:
    Bool4subBool(weak<const Bool4> node1, weak<const Bool> node2)
        :   node1(node1)
        ,   node2(node2)
    {
    }
private:
    void buildDeclarations(IShaderBuilder& stream, Stage currentStage, Stage targetStage) const override
    {
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
ref<Bool4> operator -(weak<const Bool4> node1, weak<const Bool> node2)
{
    return ref<Bool4subBool>::create(Arena::script(), node1, node2);
}




}}}
