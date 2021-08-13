#pragma once

#include <System/Core.h>

/// JsonType
/// Json data type
enum struct JsonType
{
    Null,
    Array,
    Object,
    Number,
    String,
    Boolean,
};

/// JsonError
/// Json error type when working with json
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

/// Json
/// Data structure from the result when parsing a json document
/// Note: 
///     This data structure is used to parsing json only
///     Please, donot store Json data structure and its memory in your system
///     Use the right data structure instead.
struct Json
{
    JsonType    Type;
    //char        Data[];
    union
    {
        double          Number;
        bool            Boolean;

        String          String;

        Array<Json>     Array;

        HashTable<Json> Object;
    };
};

Json*           LoadJson(const char* filePath);

Json*           ParseJson(const char* content);
Json*           ParseJson(const char* content, int contentLength);

void            FreeJson(Json* rootValue);

JsonError       JsonGetError(const Json* rootValue);
const char*     JsonGetErrorString(const Json* rootValue);

bool            JsonEquals(const Json a, const Json b);

Json            JsonFind(const Json value, const char* name);
Json            JsonFind(const Json value, U64 hash);
