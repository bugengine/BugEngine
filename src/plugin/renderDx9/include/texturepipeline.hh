/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#ifndef BE_DX9_TEXTUREPIPELINE_HH_
#define BE_DX9_TEXTUREPIPELINE_HH_
/*****************************************************************************/
#include    <graphics/material/texturepipeline.hh>

namespace BugEngine { namespace Graphics { namespace DirectX9
{

class Renderer;

class TexturePipeline : public Graphics::TexturePipeline
{
private:
    Renderer*   m_owner;
public:
    TexturePipeline(Renderer* owner);
    ~TexturePipeline();

    _Texture*               load(const ifilename& file) override;
    void                    unload(_Texture* s) override;
};

}}}

/*****************************************************************************/
#endif
