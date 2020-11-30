#pragma once

#include <Yolo/Core.h>

enum struct JsonType
{
    Null,
    Array,
    Object,
    Number,
    String,
    Boolean,
};

enum struct JsonError
{
    None,

    /* Parsing error */

    Format,
    UnmatchToken,
    UnknownToken,
    UnexpectedToken,
    UnsupportedToken,

    /* Runtime error */

    OutOfMemory,
    InternalError,
};

struct Json
{
    JsonType Type;
    union
    {
        double          Number;
        bool            Boolean;

        String          String;

        Array<Json>     Array;

        HashTable<Json> Object;
    };
};


Json*       LoadJson(const char* filePath);

Json*       ParseJson(const char* content);
Json*       ParseJson(const char* content, int contentLength);

void        FreeJson(Json* rootValue);

JsonError   JsonGetError(const Json* rootValue);
const char* JsonGetErrorString(const Json* rootValue);

bool        JsonEquals(Json a, Json b);

Json        JsonFind(Json x, const char* name);
Json        JsonFind(Json x, U64 hash);
