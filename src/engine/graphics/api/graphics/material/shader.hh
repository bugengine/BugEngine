/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_MATERIAL_SHADER_HH_
#define BE_GRAPHICS_MATERIAL_SHADER_HH_
/*****************************************************************************/
#include    <system/resource/resource.hh>
#include    <graphics/material/shaderpipeline.hh>

namespace BugEngine { namespace Graphics
{

class ShaderParam;

class be_api(GRAPHICS) _Shader : public minitl::pointer
{
public:
    typedef ShaderPipeline  Pipeline;
public:
    _Shader();
    virtual ~_Shader();

    virtual ref<ShaderParam> getParam(const istring& name) const = 0;
public:
    void* operator new(size_t size)                  { return ::operator new(size); }
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
public:
    void  operator delete(void* memory)              { return ::operator delete(memory); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
};

typedef Resource<_Shader>   Shader;

}}

/*****************************************************************************/
#endif
