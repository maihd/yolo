#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <setjmp.h>

#include <Text/Json.h>
#include <Text/String.h>

#include <System/Memory.h>
#include <System/FileSystem.h>

#include <Container/Array.h>
#include <Container/HashTable.h>

#define JSON_SUPEROF(ptr, T, member) (T*)((char*)ptr - offsetof(T, member))

struct JsonState
{
    Json                Root;
    JsonState*          Next;

    int                 Line;
    int                 Column;
    int                 Cursor;
    //JsonType            ParsingType;

    int                 Length;         /* Reference only */
    const char*         Buffer;         /* Reference only */

    JsonError           ErrorCode;
    char*               ErrorMessage;
    jmp_buf             ErrorJump;
};

static void Json_SetErrorArgs(JsonState* state, JsonType type, JsonError code, const char* fmt, va_list valist)
{
    constexpr int ERROR_MESSAGE_SIZE = 1024;

    const char* type_name;
    switch (type)
    {
    case JsonType::Null:
        type_name = "null";
        break;

    case JsonType::Boolean:
        type_name = "boolean";
        break;

    case JsonType::Number:
        type_name = "number";
        break;

    case JsonType::Array:
        type_name = "array";
        break;

    case JsonType::String:
        type_name = "string";
        break;

    case JsonType::Object:
        type_name = "object";
        break;

    default:
        type_name = "unknown";
        break;
    }

    state->ErrorCode = code;
    if (state->ErrorMessage == nullptr)
    {
        state->ErrorMessage = (char*)MemoryAlloc(ERROR_MESSAGE_SIZE);
    }

    char final_format[1024];
    char templ_format[1024] = "%s\n\tAt line %d, column %d. Parsing token: <%s>.";

#if defined(_MSC_VER) && _MSC_VER >= 1200
    sprintf_s(final_format, sizeof(final_format), templ_format, fmt, state->Line, state->Column, type_name);
    sprintf_s(state->ErrorMessage, ERROR_MESSAGE_SIZE, final_format, valist);
#else
    sprintf(final_format, templ_format, fmt, state->line, state->column, type_name);
    sprintf(state->errmsg, final_format, valist);
#endif
}

/* @funcdef: Json_SetError */
static void Json_SetError(JsonState* state, JsonType type, JsonError code, const char* fmt, ...)
{
    va_list varg;
    va_start(varg, fmt);
    Json_SetErrorArgs(state, type, code, fmt, varg);
    va_end(varg);
}

/* funcdef: Json_Panic */
static void Json_Panic(JsonState* state, JsonType type, JsonError code, const char* fmt, ...)
{
    va_list varg;
    va_start(varg, fmt);
    Json_SetErrorArgs(state, type, code, fmt, varg);
    va_end(varg);

    longjmp(state->ErrorJump, (int)code);
}

static void Json_ReleaseMemory(Json* value)
{
    if (value)
    {
        int i, n;
        switch (value->Type)
        {
        case JsonType::Array:
            for (i = 0, n = value->Array.Count; i < n; i++)
            {
                Json_ReleaseMemory(&value->Array.Items[i]);
            }
            FreeArray(&value->Array);
            break;

        case JsonType::Object:
            for (i = 0, n = value->Object.Count; i < n; i++)
            {
                Json_ReleaseMemory(&value->Object.Values[i]);
            }
            FreeHashTable(&value->Object);
            break;

        case JsonType::String:
            FreeString(&value->String);
            break;

        default:
            break;
        }
    }
}

/* @funcdef: JsonState_Make */
static JsonState* JsonState_Make(const char* json, int jsonLength)
{
    JsonState* state = (JsonState*)MemoryAlloc(sizeof(JsonState));
    if (state)
    {
        state->Next = nullptr;

        state->Line = 1;
        state->Column = 1;
        state->Cursor = 0;
        state->Buffer = json;
        state->Length = jsonLength;

        state->ErrorCode = JsonError::None;
        state->ErrorMessage = nullptr;
    }
    return state;
}

/* @funcdef: JsonState_Free */
static void JsonState_Free(JsonState* state)
{
    if (state)
    {
        Json* root = &state->Root;
        Json_ReleaseMemory(root);

        JsonState* next = state->Next;

        MemoryFree(state->ErrorMessage);
        MemoryFree(state);

        JsonState_Free(next);
    }
}

/* @funcdef: Json_IsEOF */
static int Json_IsEOF(JsonState* state)
{
    return state->Cursor >= state->Length || state->Buffer[state->Cursor] <= 0;
}

/* @funcdef: Json_PeekChar */
static int Json_PeekChar(JsonState* state)
{
    return state->Buffer[state->Cursor];
}

/* @funcdef: Json_NextChar */
static int Json_NextChar(JsonState* state)
{
    if (Json_IsEOF(state))
    {
        return -1;
    }
    else
    {
        int c = state->Buffer[++state->Cursor];

        if (c == '\n')
        {
            state->Line++;
            state->Column = 1;
        }
        else
        {
            state->Column = state->Column + 1;
        }

        return c;
    }
}

/* @funcdef: Json_SkipSpace */
static int Json_SkipSpace(JsonState* state)
{
    int c = Json_PeekChar(state);
    while (c > 0 && isspace(c))
    {
        c = Json_NextChar(state);
    }
    return c;
}

/* @funcdef: Json_MatchChar */
static int Json_MatchChar(JsonState* state, JsonType type, int c)
{
    if (Json_PeekChar(state) == c)
    {
        return Json_NextChar(state);
    }
    else
    {
        Json_Panic(state, type, JsonError::UnmatchToken, "Expected '%c'", (char)c);
        return -1;
    }
}

/* All parse functions declaration */

static void Json_ParseArray(JsonState* state, Json* outValue);
static void Json_ParseSingle(JsonState* state, Json* outValue);
static void Json_ParseObject(JsonState* state, Json* outValue);
static void Json_ParseNumber(JsonState* state, Json* outValue);
static void Json_ParseString(JsonState* state, Json* outValue);

/* @funcdef: Json_ParseNumber */
static void Json_ParseNumber(JsonState* state, Json* outValue)
{
    if (Json_SkipSpace(state) > 0)
    {
        int c = Json_PeekChar(state);
        int sign = 1;

        if (c == '+')
        {
            c = Json_NextChar(state);
            Json_Panic(state, JsonType::Number, JsonError::UnexpectedToken, "JSON does not support number start with '+'");
        }
        else if (c == '-')
        {
            sign = -1;
            c = Json_NextChar(state);
        }
        else if (c == '0')
        {
            c = Json_NextChar(state);
            if (!isspace(c) && !ispunct(c))
            {
                Json_Panic(state, JsonType::Number, JsonError::UnexpectedToken, "JSON does not support number start with '0' (only standalone '0' is accepted)");
            }
        }
        else if (!isdigit(c))
        {
            Json_Panic(state, JsonType::Number, JsonError::UnexpectedToken, "Unexpected '%c'", c);
        }

        int    dot = 0;
        int    exp = 0;
        int    expsgn = 0;
        int    exppow = 0;
        int    expchk = 0;
        int    numpow = 1;
        double number = 0;

        while (c > 0)
        {
            if (c == 'e')
            {
                if (exp)
                {
                    Json_Panic(state, JsonType::Number, JsonError::UnexpectedToken, "Too many 'e' are presented in a <number>");
                }
                else if (dot && numpow == 1)
                {
                    Json_Panic(state, JsonType::Number, JsonError::UnexpectedToken,
                        "'.' is presented in number token, but require a digit after '.' ('%c')", c);
                }
                else
                {
                    exp = 1;
                    expchk = 0;
                }
            }
            else if (c == '.')
            {
                if (exp)
                {
                    Json_Panic(state, JsonType::Number, JsonError::UnexpectedToken, "Cannot has '.' after 'e' is presented in a <number>");
                }
                else if (dot)
                {
                    Json_Panic(state, JsonType::Number, JsonError::UnexpectedToken, "Too many '.' are presented in a <number>");
                }
                else
                {
                    dot = 1;
                }
            }
            else if (exp && (c == '-' || c == '+'))
            {
                if (expchk)
                {
                    Json_Panic(state, JsonType::Number, JsonError::UnexpectedToken, "'%c' is presented after digits are presented of exponent part", c);
                }
                else if (expsgn)
                {
                    Json_Panic(state, JsonType::Number, JsonError::UnexpectedToken, "Too many signed characters are presented after 'e'");
                }
                else
                {
                    expsgn = (c == '-' ? -1 : 1);
                }
            }
            else if (!isdigit(c))
            {
                break;
            }
            else
            {
                if (exp)
                {
                    expchk = 1;
                    exppow = exppow * 10 + (c - '0');
                }
                else
                {
                    if (dot)
                    {
                        numpow *= 10;
                        number += (c - '0') / (double)numpow;
                    }
                    else
                    {
                        number = number * 10 + (c - '0');
                    }
                }
            }

            c = Json_NextChar(state);
        }

        if (exp && !expchk)
        {
            Json_Panic(state, JsonType::Number, JsonError::UnexpectedToken, "'e' is presented in number token, but require a digit after 'e' ('%c')", (char)c);
        }
        if (dot && numpow == 1)
        {
            Json_Panic(state, JsonType::Number, JsonError::UnexpectedToken, "'.' is presented in number token, but require a digit after '.' ('%c')", (char)c);
        }
        else
        {
            Json value = { JsonType::Number };
            value.Number = sign * number;

            if (exp)
            {
                int i;
                double tmp = 1;
                for (i = 0; i < exppow; i++)
                {
                    tmp *= 10;
                }

                if (expsgn < 0)
                {
                    value.Number /= tmp;
                }
                else
                {
                    value.Number *= tmp;
                }
            }

            *outValue = value;
        }
    }
}

/* @funcdef: Json_ParseArray */
static void Json_ParseArray(JsonState* state, Json* outValue)
{
    if (Json_SkipSpace(state) > 0)
    {
        Json_MatchChar(state, JsonType::Array, '[');

        Array<Json> values = {};
        while (Json_SkipSpace(state) > 0 && Json_PeekChar(state) != ']')
        {
            if (values.Count > 0)
            {
                Json_MatchChar(state, JsonType::Array, ',');
            }

            Json value;
            Json_ParseSingle(state, &value);

            ArrayPush(&values, value);
        }

        Json_SkipSpace(state);
        Json_MatchChar(state, JsonType::Array, ']');

        outValue->Type = JsonType::Array;
        outValue->Array = values;
    }
}

/* Json_ParseSingle */
static void Json_ParseSingle(JsonState* state, Json* outValue)
{
    if (Json_SkipSpace(state) > 0)
    {
        int c = Json_PeekChar(state);

        switch (c)
        {
        case '[':
            Json_ParseArray(state, outValue);
            break;

        case '{':
            Json_ParseObject(state, outValue);
            break;

        case '"':
            Json_ParseString(state, outValue);
            break;

        case '+': case '-': case '0':
        case '1': case '2': case '3':
        case '4': case '5': case '6':
        case '7': case '8': case '9':
            Json_ParseNumber(state, outValue);
            break;

        default:
        {
            int length = 0;
            while (c > 0 && isalpha(c))
            {
                length++;
                c = Json_NextChar(state);
            }

            const char* token = state->Buffer + state->Cursor - length;
            if (length == 4 && strncmp(token, "true", 4) == 0)
            {
                outValue->Type = JsonType::Boolean;
                outValue->Boolean = true;
            }
            else if (length == 4 && strncmp(token, "null", 4) == 0)
            {
                outValue->Type = JsonType::Null;
            }
            else if (length == 5 && strncmp(token, "false", 5) == 0)
            {
                outValue->Type = JsonType::Boolean;
                outValue->Boolean = false;
            }
            else
            {
                char tmp[256];
                tmp[length] = 0;
                while (length--)
                {
                    tmp[length] = token[length];
                }

                Json_Panic(state, JsonType::Null, JsonError::UnexpectedToken, "Unexpected token '%s'", tmp);
            }
        } break;
        /* END OF SWITCH STATEMENT */
        }
    }
}

static char* Json_ParseStringNoToken(JsonState* state, int* outLength)
{
    Json_MatchChar(state, JsonType::String, '"');

    int i;
    int c0, c1;

    char buffer[2048];
    int length = 0;
    while (!Json_IsEOF(state) && (c0 = Json_PeekChar(state)) != '"')
    {
        if (c0 == '\\')
        {
            c0 = Json_NextChar(state);
            switch (c0)
            {
            case 'n':
                buffer[length++] = '\n';
                break;

            case 't':
                buffer[length++] = '\t';
                break;

            case 'r':
                buffer[length++] = '\r';
                break;

            case 'b':
                buffer[length++] = '\b';
                break;

            case '\\':
                buffer[length++] = '\\';
                break;

            case '"':
                buffer[length++] = '\"';
                break;

            case 'u':
                c1 = 0;
                for (i = 0; i < 4; i++)
                {
                    if (isxdigit((c0 = Json_NextChar(state))))
                    {
                        c1 = c1 * 10 + (isdigit(c0) ? c0 - '0' : c0 < 'a' ? c0 - 'A' : c0 - 'a');
                    }
                    else
                    {
                        Json_Panic(state, JsonType::String, JsonError::UnknownToken, "Expected hexa character in unicode character");
                    }
                }

                if (c1 <= 0x7F)
                {
                    buffer[length++] = c1;
                }
                else if (c1 <= 0x7FF)
                {
                    char c2 = (char)(0xC0 | (c1 >> 6));            /* 110xxxxx */
                    char c3 = (char)(0x80 | (c1 & 0x3F));          /* 10xxxxxx */
                    buffer[length++] = c2;
                    buffer[length++] = c3;
                }
                else if (c1 <= 0xFFFF)
                {
                    char c2 = (char)(0xE0 | (c1 >> 12));           /* 1110xxxx */
                    char c3 = (char)(0x80 | ((c1 >> 6) & 0x3F));   /* 10xxxxxx */
                    char c4 = (char)(0x80 | (c1 & 0x3F));          /* 10xxxxxx */
                    buffer[length++] = c2;
                    buffer[length++] = c3;
                    buffer[length++] = c4;
                }
                else if (c1 <= 0x10FFFF)
                {
                    char c2 = 0xF0 | (c1 >> 18);           /* 11110xxx */
                    char c3 = 0x80 | ((c1 >> 12) & 0x3F);  /* 10xxxxxx */
                    char c4 = 0x80 | ((c1 >> 6) & 0x3F);   /* 10xxxxxx */
                    char c5 = 0x80 | (c1 & 0x3F);          /* 10xxxxxx */
                    buffer[length++] = c2;
                    buffer[length++] = c3;
                    buffer[length++] = c4;
                    buffer[length++] = c5;
                }
                break;

            default:
                Json_Panic(state, JsonType::String, JsonError::UnknownToken, "Unknown escape character");
                break;
            }
        }
        else
        {
            switch (c0)
            {
            case '\r':
            case '\n':
                Json_Panic(state, JsonType::String, JsonError::UnexpectedToken, "Unexpected newline characters '%c'", c0);
                break;

            default:
                buffer[length++] = (char)c0;
                break;
            }
        }

        Json_NextChar(state);
    }

    Json_MatchChar(state, JsonType::String, '"');
    if (length > 0)
    {
        buffer[length] = '\0';
        if (outLength) *outLength = length;

        char* string = (char*)MemoryAlloc(length + 1);
        memcpy(string, buffer, length + 1);

        return string;
    }
    else
    {
        buffer[length] = '\0';
        if (outLength) *outLength = 0;
        return "";
    }
}

/* @funcdef: Json_ParseString */
static void Json_ParseString(JsonState* state, Json* outValue)
{
    if (Json_SkipSpace(state) > 0)
    {
        int length;
        char* string = Json_ParseStringNoToken(state, &length);

        outValue->Type = JsonType::String;
        outValue->String = RefString(string, length, length > 0);
    }
}

/* @funcdef: Json_ParseObject */
static void Json_ParseObject(JsonState* state, Json* outValue)
{
    if (Json_SkipSpace(state) > 0)
    {
        Json_MatchChar(state, JsonType::Object, '{');

        HashTable<Json> values = MakeHashTable<Json>(64);
        while (Json_SkipSpace(state) > 0 && Json_PeekChar(state) != '}')
        {
            if (values.Count > 0)
            {
                Json_MatchChar(state, JsonType::Object, ',');
            }

            if (Json_SkipSpace(state) != '"')
            {
                Json_Panic(state, JsonType::Object, JsonError::UnexpectedToken, "Expected <string> for <member-key> of <object>");
            }

            int   nameLength;
            char* name = Json_ParseStringNoToken(state, &nameLength);

            Json_SkipSpace(state);
            Json_MatchChar(state, JsonType::Object, ':');

            Json value;
            Json_ParseSingle(state, &value);

            HashTableSetValue(&values, CalcHash64(name, nameLength), value);

            MemoryFree(name);
        }

        Json_SkipSpace(state);
        Json_MatchChar(state, JsonType::Object, '}');

        outValue->Type = JsonType::Object;
        outValue->Object = values;
    }
}

/* Internal parsing function
 */
static Json* Json_ParseTopLevel(JsonState* state)
{
    if (!state)
    {
        return NULL;
    }

    if (Json_SkipSpace(state) == '{')
    {
        if (setjmp(state->ErrorJump) == 0)
        {
            Json_ParseObject(state, &state->Root);

            Json_SkipSpace(state);
            if (!Json_IsEOF(state))
            {
                Json_Panic(state, JsonType::Null, JsonError::Format, "JSON is not well-formed. JSON is start with <object>.");
            }

            return &state->Root;
        }
        else
        {
            return NULL;
        }
    }
    else if (Json_SkipSpace(state) == '[')
    {
        if (setjmp(state->ErrorJump) == 0)
        {
            Json_ParseArray(state, &state->Root);

            Json_SkipSpace(state);
            if (!Json_IsEOF(state))
            {
                Json_Panic(state, JsonType::Null, JsonError::Format, "JSON is not well-formed. JSON is start with <array>.");
            }

            return &state->Root;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        Json_SetError(state, JsonType::Null, JsonError::Format, "JSON must be starting with '{' or '[', first character is '%c'", Json_PeekChar(state));
        return NULL;
    }
}

Json* LoadJson(const char* filePath)
{
    return nullptr;
}

Json* ParseJson(const char* json)
{
    return ParseJson(json, (int)strlen(json));
}

Json* ParseJson(const char* jsonContent, int jsonLength)
{
    JsonState* state = JsonState_Make(jsonContent, jsonLength);
    Json* value = Json_ParseTopLevel(state);

    if (!value)
    {
        JsonState_Free(state);
    }

    return value;
}

void FreeJson(Json* rootValue)
{
    if (rootValue)
    {
        JsonState* state = JSON_SUPEROF(rootValue, JsonState, Root);
        JsonState_Free(state);
    }
}

JsonError JsonGetError(const Json* rootValue)
{
    if (rootValue)
    {
        JsonState* state = JSON_SUPEROF(rootValue, JsonState, Root);
        return state->ErrorCode;
    }

    return JsonError::None;
}

const char* JsonGetErrorString(const Json* rootValue)
{
    if (rootValue)
    {
        JsonState* state = JSON_SUPEROF(rootValue, JsonState, Root);
        return state->ErrorMessage;
    }

    return "";
}

bool JsonEquals(Json a, Json b)
{
    int i, n;

    if (a.Type != b.Type)
    {
        return false;
    }

    switch (a.Type)
    {
    case JsonType::Null:
        return true;

    case JsonType::Number:
        return a.Number == b.Number;

    case JsonType::Boolean:
        return a.Boolean == b.Boolean;

    case JsonType::Array:
        if ((n = a.Array.Count) == b.Array.Count)
        {
            for (i = 0; i < n; i++)
            {
                if (!JsonEquals(a.Array.Items[i], b.Array.Items[i]))
                {
                    return false;
                }
            }
        }
        return true;

    case JsonType::Object:
        if ((n = a.Object.Count) == b.Object.Count)
        {
            for (i = 0; i < n; i++)
            {
                if (a.Object.Keys[i] != b.Object.Keys[i])
                {
                    return false;
                }

                if (!JsonEquals(a.Object.Values[i], b.Object.Values[i]))
                {
                    return false;
                }
            }
        }
        return true;

    case JsonType::String:
        return a.String == b.String;
    }

    return false;
}

Json JsonFind(const Json obj, const char* name)
{
    return JsonFind(obj, CalcHash64(name, (int)strlen(name)));
}

Json JsonFind(const Json obj, U64 hash)
{
    if (obj.Type == JsonType::Object)
    {
        int index = HashTableIndexOf(obj.Object, hash);
        if (index > -1)
        {
            return obj.Object.Values[index];
        }
    }

    return {};
}
