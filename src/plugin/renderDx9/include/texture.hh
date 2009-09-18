/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DX9_TEXTURE_HH_
#define BE_DX9_TEXTURE_HH_
/*****************************************************************************/
#include    <graphics/renderer/renderbackend.hh>
#include    <graphics/material/texture.hh>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

class Texture : public _Texture
{
private:
    LPDIRECT3DTEXTURE9 m_texture;
public:
    Texture(LPDIRECT3DTEXTURE9 texture);
    ~Texture();

    LPDIRECT3DTEXTURE9 textureObject() const { return m_texture; }
};


}}}

/*****************************************************************************/
#endif
