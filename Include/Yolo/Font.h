#pragma once

#include <Yolo/Types.h>
#include <Yolo/Texture.h>

struct FontGlyph
{
    int     value;

    float   x0, y0;
    float   x1, y1;

    float   u0, v0;
    float   u1, v1;

    float   advance;
};

struct Font
{
    float       size;
    FontGlyph*  glyphs;
    
    Texture     texture;

    static Font Load(string path, float size);
    static void Free(Font* font);
};