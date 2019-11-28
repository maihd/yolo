#define _CRT_SECURE_NO_WARNINGS

#include <Yolo/Font.h>

#include <Yolo/Array.h>

#include <stdio.h>
#include <assert.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

Font Font::Load(string path, float size)
{
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

        uint8* pixels = (uint8*)malloc(1024 * 1024);
        stbtt_bakedchar* bakedChars = (stbtt_bakedchar*)malloc(sizeof(stbtt_bakedchar) * 256);

        stbtt_BakeFontBitmap(buffer, 0, size, pixels, 1024, 1024, 0, 256, bakedChars);

        Texture texture = Texture::New(pixels, 1024, 1024, PixelFormat::Alpha, PixelFormat::Alpha);

        FontChar* chars = Array::Empty<FontChar>();

        for (int i = 0; i < 256; i++)
        {
            const stbtt_bakedchar bakedChar = bakedChars[i];

            FontChar fontChar = {
                i, 

                (float)(bakedChar.x1 - bakedChar.x0),
                (float)(bakedChar.y1 - bakedChar.y0),

                bakedChar.xoff,
                bakedChar.yoff,

                bakedChar.xadvance,
            };

            Array::Push(&chars, fontChar);
        }

        free(bakedChars);
        free(pixels);
        free(buffer);
        
        return {
            size,
            chars,
            texture
        };
    }

    return {};
}

void Font::Free(Font* font)
{
    assert(font);

    Array::Free(&font->chars);
    Texture::Free(&font->texture);  

    font->size = 0;
}