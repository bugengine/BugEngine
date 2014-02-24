/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_3D_SHADER_TYPES_SCRIPT_HH_
#define BE_3D_SHADER_TYPES_SCRIPT_HH_
/**************************************************************************************************/
#include    <3d/shader/node.script.hh>

namespace BugEngine { namespace Shaders
{

class be_api(_3D) Output : public Node
{
    BE_NOCOPY(Output);
protected:
    Output();
    ~Output();
};

class be_api(_3D) Float : public Output
{
protected:
    Float();
    ~Float();
};

class be_api(_3D) Float2 : public Output
{
protected:
    Float2();
    ~Float2();
};

class be_api(_3D) Float3 : public Output
{
protected:
    Float3();
    ~Float3();
};

class be_api(_3D) Float4 : public Output
{
protected:
    Float4();
    ~Float4();
};

class be_api(_3D) Int : public Output
{
protected:
    Int();
    ~Int();
};

class be_api(_3D) Int2 : public Output
{
protected:
    Int2();
    ~Int2();
};

class be_api(_3D) Int3 : public Output
{
protected:
    Int3();
    ~Int3();
};

class be_api(_3D) Int4 : public Output
{
protected:
    Int4();
    ~Int4();
};

class be_api(_3D) UInt : public Output
{
protected:
    UInt();
    ~UInt();
};

class be_api(_3D) UInt2 : public Output
{
protected:
    UInt2();
    ~UInt2();
};

class be_api(_3D) UInt3 : public Output
{
protected:
    UInt3();
    ~UInt3();
};

class be_api(_3D) UInt4 : public Output
{
protected:
    UInt4();
    ~UInt4();
};

class be_api(_3D) Bool : public Node
{
protected:
    Bool();
    ~Bool();
};

}}

/**************************************************************************************************/
#endif
