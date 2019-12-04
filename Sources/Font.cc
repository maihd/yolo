#define _CRT_SECURE_NO_WARNINGS

#include <Yolo/Font.h>

#include <Yolo/Array.h>

#include <stdio.h>
#include <assert.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

Font Font::Load(string path, float size)
{
    constexpr int TEXTURE_WIDTH = 1024;
    constexpr int TEXTURE_HEIGHT = 1024;
    constexpr int GLYPHS_COUNT = 256;

    FILE* file = fopen(path, "rb");
    while (file)
    {
        fseek(file, 0, SEEK_END);
        long fileSize = (long)ftell(file);
        fseek(file, 0, SEEK_SET);

        uint8* buffer = (uint8*)malloc(fileSize);
        if (!buffer)
        {
            fclose(file);
            break;
        }

        fread(buffer, fileSize, 1, file);
        fclose(file);

        uint8* pixels = (uint8*)malloc(TEXTURE_WIDTH * TEXTURE_HEIGHT);
        stbtt_bakedchar* bakedChars = (stbtt_bakedchar*)malloc(sizeof(stbtt_bakedchar) * GLYPHS_COUNT);

        stbtt_BakeFontBitmap(buffer, 0, size, pixels, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GLYPHS_COUNT, bakedChars);

        Texture texture = Texture::New(pixels, TEXTURE_WIDTH, TEXTURE_HEIGHT, PixelFormat::Red, PixelFormat::Red);

        float ipw = 1.0f / TEXTURE_WIDTH, iph = 1.0f / TEXTURE_HEIGHT;

        FontGlyph* glyphs = Array::New<FontGlyph>(GLYPHS_COUNT);
        for (int i = 0; i < GLYPHS_COUNT; i++)
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

            Array::Push(&glyphs, fontChar);
        }

        free(bakedChars);
        free(pixels);
        free(buffer);
        
        return {
            size,
            glyphs,
            texture
        };
    }

    return {};
}

void Font::Free(Font* font)
{
    assert(font);

    Array::Free(&font->glyphs);
    Texture::Free(&font->texture);  

    font->size = 0;
}