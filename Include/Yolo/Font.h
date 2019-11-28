#pragma once

#include <Yolo/Types.h>
#include <Yolo/Texture.h>

struct FontChar
{
    int     value;
    
    float   width;
    float   height;

    float   offsetX;
    float   offsetY;
    float   advanceX;
};

struct Font
{
    float       size;
    FontChar*   chars;
    
    Texture     texture;

    static Font Load(string path, float size);
    static void Free(Font* font);
};