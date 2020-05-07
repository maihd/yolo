#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <assert.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <Yolo/File.h>
#include <Yolo/Font.h>
#include <Yolo/Array.h>
#include <Yolo/Texture.h>

namespace FontOps
{
    Font Load(String path, F32 size)
    {
        constexpr I32 TEXTURE_WIDTH     = 1024;
        constexpr I32 TEXTURE_HEIGHT    = 1024;
        constexpr I32 GLYPHS_COUNT      = 256;
        File file = FileOps::Open(path, FileModes::Read);
        while (file)
        {
            I32 fileSize = FileOps::GetSize(file);

            U8* buffer = (U8*)malloc(fileSize);
            if (!buffer)
            {
                FileOps::Close(file);
                break;
            }

            FileOps::Read(file, buffer, fileSize);
            FileOps::Close(file);

            U8* pixels = (U8*)malloc(TEXTURE_WIDTH * TEXTURE_HEIGHT);
            stbtt_bakedchar* bakedChars = (stbtt_bakedchar*)malloc(sizeof(stbtt_bakedchar) * GLYPHS_COUNT);

            stbtt_BakeFontBitmap(buffer, 0, size, pixels, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GLYPHS_COUNT, bakedChars);

            Texture texture = TextureOps::New(pixels, TEXTURE_WIDTH, TEXTURE_HEIGHT, PixelFormat::Red, PixelFormat::Red);

            F32 ipw = 1.0f / TEXTURE_WIDTH, iph = 1.0f / TEXTURE_HEIGHT;

            Array<FontGlyph> glyphs = ArrayOps::New<FontGlyph>(GLYPHS_COUNT);
            for (I32 i = 0; i < GLYPHS_COUNT; i++)
            {
                const stbtt_bakedchar bakedChar = bakedChars[i];

                F32 xoff = bakedChar.xoff;
                F32 yoff = bakedChar.yoff;

                F32 x0 = xoff;
                F32 y0 = yoff + bakedChar.y1 - bakedChar.y0;
                F32 x1 = xoff + bakedChar.x1 - bakedChar.x0;
                F32 y1 = yoff;

                F32 s0 = bakedChar.x0 * ipw;
                F32 t0 = bakedChar.y1 * iph;
                F32 s1 = bakedChar.x1 * ipw;
                F32 t1 = bakedChar.y0 * iph;

                FontGlyph fontChar = {
                    i,

                    x0,
                    y0,

                    x1,
                    y1,

                    s0,
                    t0,

                    s1,
                    t1,

                    bakedChar.xadvance,
                };

                ArrayOps::Push(&glyphs, fontChar);
            }

            free(bakedChars);
            free(pixels);
            free(buffer);

            return Font{
                size,
                glyphs,
                texture
            };
        }

        return {};
    }

    void Free(Font* font)
    {
        assert(font);

        ArrayOps::Free(&font->glyphs);
        TextureOps::Free(&font->texture);

        font->size = 0;
    }
}
