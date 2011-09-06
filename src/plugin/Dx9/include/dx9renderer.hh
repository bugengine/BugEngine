/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DX9_RENDERER_HH_
#define BE_DX9_RENDERER_HH_
/*****************************************************************************/
#include    <windowing/renderer.hh>
#include    <system/file/folder.script.hh>

#include    <d3d9.h>

namespace BugEngine { namespace Graphics
{

class RenderTarget;
class RenderWindow;
class Mesh;
class Texture;
class Shader;


namespace DirectX9
{


static inline HRESULT d3d_checkResult(HRESULT r)
{
    if (FAILED(r))
    {
        //be_error("d3d error %s: %s" | DXGetErrorDescription(r) | DXGetErrorString(r));
    }
    return r;
}

class Dx9Window;
class Dx9RenderTarget;

class Dx9Renderer : public Windowing::Renderer
{
    friend class Dx9Window;
private:
    enum DeviceState
    {
        DeviceReady,
        DeviceLost,
        DeviceRestored
    };
private:
    HWND                    m_dummyWindow;
    D3DPRESENT_PARAMETERS   m_dummyParams;
    LPDIRECT3D9             m_directx;
    LPDIRECT3DDEVICE9       m_device;
    D3DCAPS9                m_caps;
    CGcontext               m_context;
    weak<const Folder>      m_dataFolder;
    DeviceState             m_deviceState;
    u64                     m_threadId;
public:
    Dx9Renderer(weak<const Folder> dataFolder);
    ~Dx9Renderer();

    u32                 getMaxSimultaneousRenderTargets() const override { return m_caps.NumSimultaneousRTs; }

    weak<const Folder>  dataFolder() const { return m_dataFolder; }
private:
    void                flush() override;

    ref<IGPUResource>   createRenderTarget(weak<const RenderTarget> rendertarget) override;
    ref<IGPUResource>   createRenderWindow(weak<const RenderWindow> renderwindow) override;
    ref<IGPUResource>   createShaderProgram(weak<const ShaderProgram> shader) override;
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); return ::operator delete(memory); }
};

}}}

/*****************************************************************************/
#endif
