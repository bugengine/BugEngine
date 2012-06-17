/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */
/* GENERATED FILE! do not edit; see generateShaderTypes.py */

#include    <shadermodel1/stdafx.h>
#include    <shadermodel1/shaders/uint.script.hh>
#include    <3d/shader/ishaderbuilder.hh>

namespace BugEngine { namespace Uint
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
void UintUniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Uint);
}
void UintUniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
UintAttribute::UintAttribute()
{
}
UintAttribute::~UintAttribute()
{
}
void UintAttribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Uint);
}
void UintAttribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
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
void UintVarying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Uint);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void UintVarying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class UintdivUint : public Uint
{
private:
    weak<const Uint> node1;
    weak<const Uint> node2;
public:
    UintdivUint(weak<const Uint> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Uint, node1, node2);
        }
    }
};
ref<Uint> operator /(weak<const Uint> node1, weak<const Uint> node2)
{
    return ref<UintdivUint>::create(Arena::script(), node1, node2);
}

class UintaddUint : public Uint
{
private:
    weak<const Uint> node1;
    weak<const Uint> node2;
public:
    UintaddUint(weak<const Uint> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Uint, node1, node2);
        }
    }
};
ref<Uint> operator +(weak<const Uint> node1, weak<const Uint> node2)
{
    return ref<UintaddUint>::create(Arena::script(), node1, node2);
}

class UintsubUint : public Uint
{
private:
    weak<const Uint> node1;
    weak<const Uint> node2;
public:
    UintsubUint(weak<const Uint> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Uint, node1, node2);
        }
    }
};
ref<Uint> operator -(weak<const Uint> node1, weak<const Uint> node2)
{
    return ref<UintsubUint>::create(Arena::script(), node1, node2);
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
void Uint2Uniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Uint2);
}
void Uint2Uniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Uint2Attribute::Uint2Attribute()
{
}
Uint2Attribute::~Uint2Attribute()
{
}
void Uint2Attribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Uint2);
}
void Uint2Attribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
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
void Uint2Varying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Uint2);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void Uint2Varying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class Uint2divUint2 : public Uint2
{
private:
    weak<const Uint2> node1;
    weak<const Uint2> node2;
public:
    Uint2divUint2(weak<const Uint2> node1, weak<const Uint2> node2)
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
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Uint2, node1, node2);
        }
    }
};
ref<Uint2> operator /(weak<const Uint2> node1, weak<const Uint2> node2)
{
    return ref<Uint2divUint2>::create(Arena::script(), node1, node2);
}

class Uint2addUint2 : public Uint2
{
private:
    weak<const Uint2> node1;
    weak<const Uint2> node2;
public:
    Uint2addUint2(weak<const Uint2> node1, weak<const Uint2> node2)
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
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Uint2, node1, node2);
        }
    }
};
ref<Uint2> operator +(weak<const Uint2> node1, weak<const Uint2> node2)
{
    return ref<Uint2addUint2>::create(Arena::script(), node1, node2);
}

class Uint2subUint2 : public Uint2
{
private:
    weak<const Uint2> node1;
    weak<const Uint2> node2;
public:
    Uint2subUint2(weak<const Uint2> node1, weak<const Uint2> node2)
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
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Uint2, node1, node2);
        }
    }
};
ref<Uint2> operator -(weak<const Uint2> node1, weak<const Uint2> node2)
{
    return ref<Uint2subUint2>::create(Arena::script(), node1, node2);
}

class UintmulUint2 : public Uint2
{
private:
    weak<const Uint> node1;
    weak<const Uint2> node2;
public:
    UintmulUint2(weak<const Uint> node1, weak<const Uint2> node2)
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
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Uint2, node1, node2);
        }
    }
};
ref<Uint2> operator *(weak<const Uint> node1, weak<const Uint2> node2)
{
    return ref<UintmulUint2>::create(Arena::script(), node1, node2);
}

class Uint2mulUint : public Uint2
{
private:
    weak<const Uint2> node1;
    weak<const Uint> node2;
public:
    Uint2mulUint(weak<const Uint2> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Uint2, node1, node2);
        }
    }
};
ref<Uint2> operator *(weak<const Uint2> node1, weak<const Uint> node2)
{
    return ref<Uint2mulUint>::create(Arena::script(), node1, node2);
}

class UintdivUint2 : public Uint2
{
private:
    weak<const Uint> node1;
    weak<const Uint2> node2;
public:
    UintdivUint2(weak<const Uint> node1, weak<const Uint2> node2)
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
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Uint2, node1, node2);
        }
    }
};
ref<Uint2> operator /(weak<const Uint> node1, weak<const Uint2> node2)
{
    return ref<UintdivUint2>::create(Arena::script(), node1, node2);
}

class Uint2divUint : public Uint2
{
private:
    weak<const Uint2> node1;
    weak<const Uint> node2;
public:
    Uint2divUint(weak<const Uint2> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Uint2, node1, node2);
        }
    }
};
ref<Uint2> operator /(weak<const Uint2> node1, weak<const Uint> node2)
{
    return ref<Uint2divUint>::create(Arena::script(), node1, node2);
}

class UintaddUint2 : public Uint2
{
private:
    weak<const Uint> node1;
    weak<const Uint2> node2;
public:
    UintaddUint2(weak<const Uint> node1, weak<const Uint2> node2)
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
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Uint2, node1, node2);
        }
    }
};
ref<Uint2> operator +(weak<const Uint> node1, weak<const Uint2> node2)
{
    return ref<UintaddUint2>::create(Arena::script(), node1, node2);
}

class Uint2addUint : public Uint2
{
private:
    weak<const Uint2> node1;
    weak<const Uint> node2;
public:
    Uint2addUint(weak<const Uint2> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Uint2, node1, node2);
        }
    }
};
ref<Uint2> operator +(weak<const Uint2> node1, weak<const Uint> node2)
{
    return ref<Uint2addUint>::create(Arena::script(), node1, node2);
}

class UintsubUint2 : public Uint2
{
private:
    weak<const Uint> node1;
    weak<const Uint2> node2;
public:
    UintsubUint2(weak<const Uint> node1, weak<const Uint2> node2)
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
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Uint2, node1, node2);
        }
    }
};
ref<Uint2> operator -(weak<const Uint> node1, weak<const Uint2> node2)
{
    return ref<UintsubUint2>::create(Arena::script(), node1, node2);
}

class Uint2subUint : public Uint2
{
private:
    weak<const Uint2> node1;
    weak<const Uint> node2;
public:
    Uint2subUint(weak<const Uint2> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Uint2, node1, node2);
        }
    }
};
ref<Uint2> operator -(weak<const Uint2> node1, weak<const Uint> node2)
{
    return ref<Uint2subUint>::create(Arena::script(), node1, node2);
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
void Uint3Uniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Uint3);
}
void Uint3Uniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Uint3Attribute::Uint3Attribute()
{
}
Uint3Attribute::~Uint3Attribute()
{
}
void Uint3Attribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Uint3);
}
void Uint3Attribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
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
void Uint3Varying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Uint3);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void Uint3Varying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class Uint3divUint3 : public Uint3
{
private:
    weak<const Uint3> node1;
    weak<const Uint3> node2;
public:
    Uint3divUint3(weak<const Uint3> node1, weak<const Uint3> node2)
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
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Uint3, node1, node2);
        }
    }
};
ref<Uint3> operator /(weak<const Uint3> node1, weak<const Uint3> node2)
{
    return ref<Uint3divUint3>::create(Arena::script(), node1, node2);
}

class Uint3addUint3 : public Uint3
{
private:
    weak<const Uint3> node1;
    weak<const Uint3> node2;
public:
    Uint3addUint3(weak<const Uint3> node1, weak<const Uint3> node2)
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
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Uint3, node1, node2);
        }
    }
};
ref<Uint3> operator +(weak<const Uint3> node1, weak<const Uint3> node2)
{
    return ref<Uint3addUint3>::create(Arena::script(), node1, node2);
}

class Uint3subUint3 : public Uint3
{
private:
    weak<const Uint3> node1;
    weak<const Uint3> node2;
public:
    Uint3subUint3(weak<const Uint3> node1, weak<const Uint3> node2)
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
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Uint3, node1, node2);
        }
    }
};
ref<Uint3> operator -(weak<const Uint3> node1, weak<const Uint3> node2)
{
    return ref<Uint3subUint3>::create(Arena::script(), node1, node2);
}

class UintmulUint3 : public Uint3
{
private:
    weak<const Uint> node1;
    weak<const Uint3> node2;
public:
    UintmulUint3(weak<const Uint> node1, weak<const Uint3> node2)
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
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Uint3, node1, node2);
        }
    }
};
ref<Uint3> operator *(weak<const Uint> node1, weak<const Uint3> node2)
{
    return ref<UintmulUint3>::create(Arena::script(), node1, node2);
}

class Uint3mulUint : public Uint3
{
private:
    weak<const Uint3> node1;
    weak<const Uint> node2;
public:
    Uint3mulUint(weak<const Uint3> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Uint3, node1, node2);
        }
    }
};
ref<Uint3> operator *(weak<const Uint3> node1, weak<const Uint> node2)
{
    return ref<Uint3mulUint>::create(Arena::script(), node1, node2);
}

class UintdivUint3 : public Uint3
{
private:
    weak<const Uint> node1;
    weak<const Uint3> node2;
public:
    UintdivUint3(weak<const Uint> node1, weak<const Uint3> node2)
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
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Uint3, node1, node2);
        }
    }
};
ref<Uint3> operator /(weak<const Uint> node1, weak<const Uint3> node2)
{
    return ref<UintdivUint3>::create(Arena::script(), node1, node2);
}

class Uint3divUint : public Uint3
{
private:
    weak<const Uint3> node1;
    weak<const Uint> node2;
public:
    Uint3divUint(weak<const Uint3> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Uint3, node1, node2);
        }
    }
};
ref<Uint3> operator /(weak<const Uint3> node1, weak<const Uint> node2)
{
    return ref<Uint3divUint>::create(Arena::script(), node1, node2);
}

class UintaddUint3 : public Uint3
{
private:
    weak<const Uint> node1;
    weak<const Uint3> node2;
public:
    UintaddUint3(weak<const Uint> node1, weak<const Uint3> node2)
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
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Uint3, node1, node2);
        }
    }
};
ref<Uint3> operator +(weak<const Uint> node1, weak<const Uint3> node2)
{
    return ref<UintaddUint3>::create(Arena::script(), node1, node2);
}

class Uint3addUint : public Uint3
{
private:
    weak<const Uint3> node1;
    weak<const Uint> node2;
public:
    Uint3addUint(weak<const Uint3> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Uint3, node1, node2);
        }
    }
};
ref<Uint3> operator +(weak<const Uint3> node1, weak<const Uint> node2)
{
    return ref<Uint3addUint>::create(Arena::script(), node1, node2);
}

class UintsubUint3 : public Uint3
{
private:
    weak<const Uint> node1;
    weak<const Uint3> node2;
public:
    UintsubUint3(weak<const Uint> node1, weak<const Uint3> node2)
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
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Uint3, node1, node2);
        }
    }
};
ref<Uint3> operator -(weak<const Uint> node1, weak<const Uint3> node2)
{
    return ref<UintsubUint3>::create(Arena::script(), node1, node2);
}

class Uint3subUint : public Uint3
{
private:
    weak<const Uint3> node1;
    weak<const Uint> node2;
public:
    Uint3subUint(weak<const Uint3> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Uint3, node1, node2);
        }
    }
};
ref<Uint3> operator -(weak<const Uint3> node1, weak<const Uint> node2)
{
    return ref<Uint3subUint>::create(Arena::script(), node1, node2);
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
void Uint4Uniform::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addUniform(this, currentStage, name, Shaders::Type_Uint4);
}
void Uint4Uniform::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

/* Attribute ********************************************************/
Uint4Attribute::Uint4Attribute()
{
}
Uint4Attribute::~Uint4Attribute()
{
}
void Uint4Attribute::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    stream.addAttribute(this, currentStage, targetStage, Shaders::Type_Uint4);
}
void Uint4Attribute::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
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
void Uint4Varying::buildDeclarations(Shaders::IShaderBuilder& stream, Shaders::Stage currentStage, Shaders::Stage targetStage) const
{
    if (currentStage == targetStage)
        stream.addVarying(this, currentStage, targetStage, Shaders::Type_Uint4);
    node->buildDeclarations(stream, Shaders::VertexStage, targetStage);
}
void Uint4Varying::buildDefinitions(Shaders::IShaderBuilder& /*stream*/, Shaders::Stage /*currentStage*/, Shaders::Stage /*targetStage*/) const
{
}

class Uint4divUint4 : public Uint4
{
private:
    weak<const Uint4> node1;
    weak<const Uint4> node2;
public:
    Uint4divUint4(weak<const Uint4> node1, weak<const Uint4> node2)
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
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Uint4, node1, node2);
        }
    }
};
ref<Uint4> operator /(weak<const Uint4> node1, weak<const Uint4> node2)
{
    return ref<Uint4divUint4>::create(Arena::script(), node1, node2);
}

class Uint4addUint4 : public Uint4
{
private:
    weak<const Uint4> node1;
    weak<const Uint4> node2;
public:
    Uint4addUint4(weak<const Uint4> node1, weak<const Uint4> node2)
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
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Uint4, node1, node2);
        }
    }
};
ref<Uint4> operator +(weak<const Uint4> node1, weak<const Uint4> node2)
{
    return ref<Uint4addUint4>::create(Arena::script(), node1, node2);
}

class Uint4subUint4 : public Uint4
{
private:
    weak<const Uint4> node1;
    weak<const Uint4> node2;
public:
    Uint4subUint4(weak<const Uint4> node1, weak<const Uint4> node2)
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
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Uint4, node1, node2);
        }
    }
};
ref<Uint4> operator -(weak<const Uint4> node1, weak<const Uint4> node2)
{
    return ref<Uint4subUint4>::create(Arena::script(), node1, node2);
}

class UintmulUint4 : public Uint4
{
private:
    weak<const Uint> node1;
    weak<const Uint4> node2;
public:
    UintmulUint4(weak<const Uint> node1, weak<const Uint4> node2)
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
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Uint4, node1, node2);
        }
    }
};
ref<Uint4> operator *(weak<const Uint> node1, weak<const Uint4> node2)
{
    return ref<UintmulUint4>::create(Arena::script(), node1, node2);
}

class Uint4mulUint : public Uint4
{
private:
    weak<const Uint4> node1;
    weak<const Uint> node2;
public:
    Uint4mulUint(weak<const Uint4> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Shaders::Op_mul, Shaders::Type_Uint4, node1, node2);
        }
    }
};
ref<Uint4> operator *(weak<const Uint4> node1, weak<const Uint> node2)
{
    return ref<Uint4mulUint>::create(Arena::script(), node1, node2);
}

class UintdivUint4 : public Uint4
{
private:
    weak<const Uint> node1;
    weak<const Uint4> node2;
public:
    UintdivUint4(weak<const Uint> node1, weak<const Uint4> node2)
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
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Uint4, node1, node2);
        }
    }
};
ref<Uint4> operator /(weak<const Uint> node1, weak<const Uint4> node2)
{
    return ref<UintdivUint4>::create(Arena::script(), node1, node2);
}

class Uint4divUint : public Uint4
{
private:
    weak<const Uint4> node1;
    weak<const Uint> node2;
public:
    Uint4divUint(weak<const Uint4> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Shaders::Op_div, Shaders::Type_Uint4, node1, node2);
        }
    }
};
ref<Uint4> operator /(weak<const Uint4> node1, weak<const Uint> node2)
{
    return ref<Uint4divUint>::create(Arena::script(), node1, node2);
}

class UintaddUint4 : public Uint4
{
private:
    weak<const Uint> node1;
    weak<const Uint4> node2;
public:
    UintaddUint4(weak<const Uint> node1, weak<const Uint4> node2)
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
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Uint4, node1, node2);
        }
    }
};
ref<Uint4> operator +(weak<const Uint> node1, weak<const Uint4> node2)
{
    return ref<UintaddUint4>::create(Arena::script(), node1, node2);
}

class Uint4addUint : public Uint4
{
private:
    weak<const Uint4> node1;
    weak<const Uint> node2;
public:
    Uint4addUint(weak<const Uint4> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Shaders::Op_add, Shaders::Type_Uint4, node1, node2);
        }
    }
};
ref<Uint4> operator +(weak<const Uint4> node1, weak<const Uint> node2)
{
    return ref<Uint4addUint>::create(Arena::script(), node1, node2);
}

class UintsubUint4 : public Uint4
{
private:
    weak<const Uint> node1;
    weak<const Uint4> node2;
public:
    UintsubUint4(weak<const Uint> node1, weak<const Uint4> node2)
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
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Uint4, node1, node2);
        }
    }
};
ref<Uint4> operator -(weak<const Uint> node1, weak<const Uint4> node2)
{
    return ref<UintsubUint4>::create(Arena::script(), node1, node2);
}

class Uint4subUint : public Uint4
{
private:
    weak<const Uint4> node1;
    weak<const Uint> node2;
public:
    Uint4subUint(weak<const Uint4> node1, weak<const Uint> node2)
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
            stream.addOperator(this, Shaders::Op_sub, Shaders::Type_Uint4, node1, node2);
        }
    }
};
ref<Uint4> operator -(weak<const Uint4> node1, weak<const Uint> node2)
{
    return ref<Uint4subUint>::create(Arena::script(), node1, node2);
}





}}
