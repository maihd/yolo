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

        FontGlyph* glyphs = Array::New<FontGlyph>(GLYPHS_COUNT);
        for (int i = 0; i < GLYPHS_COUNT; i++)
        {
            const stbtt_bakedchar bakedChar = bakedChars[i];

            float tmpX = 0;
            float tmpY = 0;
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(bakedChars, TEXTURE_WIDTH, TEXTURE_HEIGHT, i, &tmpX, &tmpY, &q, 1);//1=opengl & d3d10+,0=d3d9

            FontGlyph fontChar = {
                i, 

                q.x0,
                q.y0,
                
                q.x1,
                q.y1,

                q.s0,
                q.t1,

                q.s1,
                q.t0,

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