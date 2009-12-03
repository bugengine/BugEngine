/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DATA_PARSE_NODE_HH_
#define BE_DATA_PARSE_NODE_HH_
/*****************************************************************************/

extern "C" int yyparse(void* param);

namespace BugEngine { namespace Data { namespace Parse
{

class Context;

class Node : public minitl::refcountable
{
    friend int ::yyparse(void* param);
private:
    mutable Value  m_result;
private:
    virtual void dolink(Context& context) const = 0;
    virtual Value doeval(Context& context) const = 0;
public:
    Node();
    ~Node();

    void link(Context& context) const;
    Value eval(Context& context) const;
public:
    void* operator new(size_t size)                  { return ::operator new(size); }
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
public:
    void  operator delete(void* memory)              { return ::operator delete(memory); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
};

}}}

/*****************************************************************************/
#endif
