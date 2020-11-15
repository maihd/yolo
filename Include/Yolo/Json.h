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
        F64             number;
        bool            boolean;

        String          string;

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
    String      GetErrorString(const Json* rootValue);

    bool        Equals(Json a, Json b);

    Json        Find(Json x, String name);
    Json        Find(Json x, U64 hash);
}
