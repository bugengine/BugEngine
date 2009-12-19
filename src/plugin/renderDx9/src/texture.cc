/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <texture.hh>
#include    <renderer.hh>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

Texture::Texture(LPDIRECT3DTEXTURE9 texture)
:   m_texture(texture)
{
}

Texture::~Texture()
{
    m_texture->Release();
}

}}}

