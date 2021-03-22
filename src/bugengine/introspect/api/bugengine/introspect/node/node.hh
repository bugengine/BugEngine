/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_INTROSPECT_NODE_NODE_HH_
#define BE_INTROSPECT_NODE_NODE_HH_
/**************************************************************************************************/
#include <bugengine/introspect/stdafx.h>
#include <bugengine/meta/value.hh>

namespace BugEngine { namespace Meta { namespace AST {

class Array;
class Bool;
class FileName;
class Float;
class Float2;
class Float3;
class Float4;
class Integer;
class Int2;
class Int3;
class Int4;
class Object;
class Parameter;
class Property;
class Reference;
class String;

struct DbContext;

class be_api(INTROSPECT) Node : public minitl::refcountable
{
    friend class Array;

public:
    struct Visitor;

private:
    enum State
    {
        Parsed,
        InResolution,
        ResolvedError,
        Resolved,
        Evaluated
    };
    minitl::vector< minitl::tuple< const istring, Meta::Value > > m_metadata;
    mutable Value                                                 m_cache;
    mutable State                                                 m_state;

protected:
    Node() : m_metadata(Arena::meta()), m_cache(), m_state(Parsed)
    {
    }
    virtual void doEval(const Type& expectedType, Value& result) const = 0;
    virtual bool doResolve(DbContext & context);
    virtual void doVisit(Node::Visitor & visitor) const = 0;

public:
    struct Visitor
    {
        virtual void accept(weak< const Array > arrayValue);
        virtual void accept(weak< const Bool > boolValue);
        virtual void accept(weak< const FileName > filenameValue);
        virtual void accept(weak< const Float > floatValue);
        virtual void accept(weak< const Float2 > float2Value);
        virtual void accept(weak< const Float3 > float3Value);
        virtual void accept(weak< const Float4 > float4Value);
        virtual void accept(weak< const Integer > integerValue);
        virtual void accept(weak< const Int2 > int2Value);
        virtual void accept(weak< const Int3 > int3Value);
        virtual void accept(weak< const Int4 > int4Value);
        virtual void accept(weak< const Object > objectValue);
        virtual void accept(weak< const Parameter > parameter, istring name,
                            weak< const Node > value);
        virtual void accept(weak< const Property > propertyValue);
        virtual void accept(weak< const Reference > referenceValue, const Value& referencedValue);
        virtual void accept(weak< const Reference > referenceValue,
                            weak< const Node >      referencedNode);
        virtual void accept(weak< const String > stringValue);
    };

    virtual ConversionCost distance(const Type& type) const = 0;
    virtual ref< Node >    getProperty(DbContext & context, const inamespace& name) const;
    virtual minitl::tuple< raw< const Meta::Method >, bool > getCall(DbContext & context) const;
    bool                                                     resolve(DbContext & context);
    void  eval(const Type& expectedType, Value& result) const;
    Value eval(const Type& expectedType) const;

    template < typename T >
    void setMetadata(const istring name, T value)
    {
        for(minitl::vector< minitl::tuple< const istring, Meta::Value > >::iterator it
            = m_metadata.begin();
            it != m_metadata.end(); ++it)
        {
            if(it->first == name)
            {
                it->second = value;
                return;
            }
        }
        m_metadata.push_back(minitl::make_tuple(name, Meta::Value()));
        m_metadata.rbegin()->second = value;
    }
    const Value& getMetadata(const istring name) const;

    void visit(Visitor & visitor) const;
};

}}}  // namespace BugEngine::Meta::AST

/**************************************************************************************************/
#endif
