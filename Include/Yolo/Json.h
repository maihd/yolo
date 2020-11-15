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
    JsonType type;
    union
    {
        double          number;
        bool            boolean;

        const char*     string;

        Array<Json>     array;

        HashTable<Json> object;
    };
};

namespace JsonOps
{
    Json*       Parse(String content);
    Json*       Parse(String content, I32 contentLength);

    void        Release(Json* rootValue);

    JsonError   GetError(const Json* rootValue);
    const char* GetErrorString(const Json* rootValue);

    bool        Equals(Json a, Json b);

    Json        Find(Json x, String name);
    Json        Find(Json x, U64 hash);
}
