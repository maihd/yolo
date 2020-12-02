#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <assert.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <Yolo/File.h>
#include <Yolo/Array.h>
#include <Yolo/Memory.h>
#include <Yolo/Graphics.h>

Font LoadFont(StringView path, float size)
{
    constexpr I32 TEXTURE_WIDTH = 1024;
    constexpr I32 TEXTURE_HEIGHT = 1024;
    constexpr I32 GLYPHS_COUNT = 256;
    File file = OpenFile(path, FileMode::Read);
    while (file)
    {
        I32 fileSize = GetFileSize(file);

        U8* buffer = (U8*)MemoryAlloc(fileSize);
        if (!buffer)
        {
            CloseFile(file);
            break;
        }

        FileRead(file, buffer, fileSize);
        CloseFile(file);

        U8* pixels = (U8*)MemoryAlloc(TEXTURE_WIDTH * TEXTURE_HEIGHT);
        stbtt_bakedchar* bakedChars = (stbtt_bakedchar*)MemoryAlloc(sizeof(stbtt_bakedchar) * GLYPHS_COUNT);

        stbtt_BakeFontBitmap(buffer, 0, size, pixels, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GLYPHS_COUNT, bakedChars);

        Texture texture = MakeTexture(pixels, TEXTURE_WIDTH, TEXTURE_HEIGHT, PixelFormat::Red, PixelFormat::Red);

        float ipw = 1.0f / TEXTURE_WIDTH, iph = 1.0f / TEXTURE_HEIGHT;

        Array<FontGlyph> glyphs = MakeArray<FontGlyph>(GLYPHS_COUNT);
        for (I32 i = 0; i < GLYPHS_COUNT; i++)
        {
            const stbtt_bakedchar bakedChar = bakedChars[i];

            float xoff = bakedChar.xoff;
            float yoff = bakedChar.yoff;

            float x0 = xoff;
            float y0 = yoff + bakedChar.y1 - bakedChar.y0;
            float x1 = xoff + bakedChar.x1 - bakedChar.x0;
            float y1 = yoff;

            float s0 = bakedChar.x0 * ipw;
            float t0 = bakedChar.y1 * iph;
            float s1 = bakedChar.x1 * ipw;
            float t1 = bakedChar.y0 * iph;

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

            ArrayPush(&glyphs, fontChar);
        }

        MemoryFree(bakedChars);
        MemoryFree(pixels);
        MemoryFree(buffer);

        return Font{
            size,
            glyphs,
            texture
        };
    }

    return {};
}

void FreeFont(Font* font)
{
    assert(font);

    FreeArray(&font->Glyphs);
    FreeTexture(&font->Texture);

    font->Size = 0;
}
