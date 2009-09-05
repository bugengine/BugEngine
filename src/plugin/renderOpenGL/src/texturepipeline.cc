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

#include    <stdafx.h>
#include    <texturepipeline.hh>
#include    <texture.hh>
#include    <renderer.hh>

#include    <core/memory/streams.hh>
#include    <system/filesystem.hh>

namespace BugEngine { namespace Graphics { namespace OpenGL
{

TexturePipeline::TexturePipeline(Renderer* owner)
    :   m_owner(owner)
{
}

TexturePipeline::~TexturePipeline()
{
}

_Texture* TexturePipeline::load(const ifilename& filename)
{
    refptr<AbstractMemoryStream> file = FileSystem::instance()->open(filename, eReadOnly);

#pragma pack(push)
#pragma pack(1)
    struct TargaHeader
    {
        i8  idLength;
        i8  colormapType;
        i8  imageType;
        struct
        {
            i16 firstEntryIndex;
            i16 length;
            i8  entrySize;
        } colormapSpec;
        struct
        {
            i16 offsetX;
            i16 offsetY;
            i16 width;
            i16 height;
            i8  depth;
            i8  descriptor;
        } imageSpec;
    };
#pragma pack(pop)

    u8* data = (u8*)file->memory();
    TargaHeader* header = (TargaHeader*)data;

    switch(header->imageType)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 5:
    case 10:
    case 11:
        break;
    default:
        be_notreached();
    }

/*    u8* imageData = data+sizeof(TargaHeader)+header->idLength+header->colormapSpec.length*header->colormapSpec.entrySize;
    bool raw = false;
    u8 repetition = 0;
    u8 color = 0;


    LPDIRECT3DTEXTURE9 dx9texture;
    D3D_CHECKRESULT(m_owner->m_device->CreateTexture(header->imageSpec.width, header->imageSpec.height, 1, D3DUSAGE_DYNAMIC, D3DFMT_R32F, D3DPOOL_DEFAULT, &dx9texture, 0));
    D3DLOCKED_RECT r;
    D3D_CHECKRESULT(dx9texture->LockRect(0, &r, 0, D3DLOCK_DISCARD));
    float* bits = (float*)r.pBits;
    for(i16 line = 0; line < header->imageSpec.height; ++line, bits+=r.Pitch/4)
    {
        for(i16 column = 0; column < header->imageSpec.width;++column)
        {
            if(!repetition)
            {
                raw = !(*(imageData) >> 7);
                repetition = (*(imageData++) & 0x7f)+1;
                color = *(imageData++);
            }
            else if (raw)
            {
                color = *(imageData++);
            }
            if(color)
            {
                bits[column] = 1.0f;
            }
            else
            {
                bits[column] = 0.0f;
            }
            repetition--;
        }
    }
    dx9texture->UnlockRect(0);*/
    return new Texture();
}

void TexturePipeline::unload(_Texture* s)
{
    delete s;
}

}}}
