#pragma once

#include <Yolo/Types.h>

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

namespace JsonOps
{
    Json*       Parse(const char* content);
    Json*       Parse(const char* content, int contentLength);

    void        Release(Json* rootValue);

    JsonError   GetError(const Json* rootValue);
    const char* GetErrorString(const Json* rootValue);

    bool        Equals(Json a, Json b);

    Json        Find(Json x, const char* name);
    Json        Find(Json x, U64 hash);
}
