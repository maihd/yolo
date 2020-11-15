#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <setjmp.h>

#include <Yolo/Json.h>
#include <Yolo/Array.h>
#include <Yolo/String.h>
#include <Yolo/HashTable.h>

#define JSON_SUPEROF(ptr, T, member) (T*)((char*)ptr - offsetof(T, member))

namespace JsonOps
{
    struct JsonState
    {
        Json                root;
        JsonState*          next;

        int                 line;
        int                 column;
        int                 cursor;
        //JsonType            parsingType;

        int                 length;         /* Reference only */
        const char*         buffer;         /* Reference only */

        JsonError           errnum;
        char*               errmsg;
        jmp_buf             errjmp;
    };

    static void Json_SetErrorArgs(JsonState* state, JsonType type, JsonError code, const char* fmt, va_list valist)
    {
        const int errmsg_size = 1024;

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

        state->errnum = code;
        if (state->errmsg == NULL)
        {
            state->errmsg = (char*)malloc(errmsg_size);
        }

        char final_format[1024];
        char templ_format[1024] = "%s\n\tAt line %d, column %d. Parsing token: <%s>.";

#if defined(_MSC_VER) && _MSC_VER >= 1200
        sprintf_s(final_format, sizeof(final_format), templ_format, fmt, state->line, state->column, type_name);
        sprintf_s(state->errmsg, errmsg_size, final_format, valist);
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

        longjmp(state->errjmp, (int)code);
    }

    static void Json_ReleaseMemory(Json* value)
    {
        if (value)
        {
            int i, n;
            switch (value->type)
            {
            case JsonType::Array:
                for (i = 0, n = value->array.count; i < n; i++)
                {
                    Json_ReleaseMemory(&value->array.elements[i]);
                }
                ArrayFree(&value->array);
                break;

            case JsonType::Object:
                for (i = 0, n = value->object.count; i < n; i++)
                {
                    Json_ReleaseMemory(&value->object.values[i]);
                }
                HashTableOps::Free(&value->object);
                break;

            case JsonType::String:
                //free((void*)value->string);
                break;

            default:
                break;
            }
        }
    }

    /* @funcdef: JsonState_Make */
    static JsonState* JsonState_Make(const char* json, int jsonLength)
    {
        JsonState* state = (JsonState*)malloc(sizeof(JsonState));
        if (state)
        {
            state->next = NULL;

            state->line = 1;
            state->column = 1;
            state->cursor = 0;
            state->buffer = json;
            state->length = jsonLength;

            state->errmsg = NULL;
            state->errnum = JsonError::None;
        }
        return state;
    }

    /* @funcdef: JsonState_Free */
    static void JsonState_Free(JsonState* state)
    {
        if (state)
        {
            Json* root = &state->root;
            Json_ReleaseMemory(root);

            JsonState* next = state->next;

            free(state->errmsg);
            free(state);

            JsonState_Free(next);
        }
    }

    /* @funcdef: Json_IsEOF */
    static int Json_IsEOF(JsonState* state)
    {
        return state->cursor >= state->length || state->buffer[state->cursor] <= 0;
    }

    /* @funcdef: Json_PeekChar */
    static int Json_PeekChar(JsonState* state)
    {
        return state->buffer[state->cursor];
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
            int c = state->buffer[++state->cursor];

            if (c == '\n')
            {
                state->line++;
                state->column = 1;
            }
            else
            {
                state->column = state->column + 1;
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
                value.number = sign * number;

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
                        value.number /= tmp;
                    }
                    else
                    {
                        value.number *= tmp;
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
                if (values.count > 0)
                {
                    Json_MatchChar(state, JsonType::Array, ',');
                }

                Json value;
                Json_ParseSingle(state, &value);

                ArrayPush(&values, value);
            }

            Json_SkipSpace(state);
            Json_MatchChar(state, JsonType::Array, ']');

            outValue->type  = JsonType::Array;
            outValue->array = values;
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

                const char* token = state->buffer + state->cursor - length;
                if (length == 4 && strncmp(token, "true", 4) == 0)
                {
                    outValue->type = JsonType::Boolean;
                    outValue->boolean = true;
                }
                else if (length == 4 && strncmp(token, "null", 4) == 0)
                {
                    outValue->type = JsonType::Null;
                }
                else if (length == 5 && strncmp(token, "false", 5) == 0)
                {
                    outValue->type = JsonType::Boolean;
                    outValue->boolean = false;
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

        //JsonTempArray(char, 2048) buffer = JsonTempArray_Init(NULL);
        while (!Json_IsEOF(state) && (c0 = Json_PeekChar(state)) != '"')
        {
            if (c0 == '\\')
            {
                c0 = Json_NextChar(state);
                switch (c0)
                {
                case 'n':
                    //JsonTempArray_Push(&buffer, '\n', &state->allocator);
                    break;

                case 't':
                    //JsonTempArray_Push(&buffer, '\t', &state->allocator);
                    break;

                case 'r':
                    //JsonTempArray_Push(&buffer, '\r', &state->allocator);
                    break;

                case 'b':
                    //JsonTempArray_Push(&buffer, '\b', &state->allocator);
                    break;

                case '\\':
                    //JsonTempArray_Push(&buffer, '\\', &state->allocator);
                    break;

                case '"':
                    //JsonTempArray_Push(&buffer, '\"', &state->allocator);
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
                            //Json_Panic(state, JsonType::String, JSON_ERROR_UNKNOWN, "Expected hexa character in unicode character");
                        }
                    }

                    if (c1 <= 0x7F)
                    {
                        //JsonTempArray_Push(&buffer, (char)c1, &state->allocator);
                    }
                    else if (c1 <= 0x7FF)
                    {
                        char c2 = (char)(0xC0 | (c1 >> 6));            /* 110xxxxx */
                        char c3 = (char)(0x80 | (c1 & 0x3F));          /* 10xxxxxx */
                        //JsonTempArray_Push(&buffer, c2, &state->allocator);
                        //JsonTempArray_Push(&buffer, c3, &state->allocator);
                    }
                    else if (c1 <= 0xFFFF)
                    {
                        char c2 = (char)(0xE0 | (c1 >> 12));           /* 1110xxxx */
                        char c3 = (char)(0x80 | ((c1 >> 6) & 0x3F));   /* 10xxxxxx */
                        char c4 = (char)(0x80 | (c1 & 0x3F));          /* 10xxxxxx */
                        //JsonTempArray_Push(&buffer, c2, &state->allocator);
                        //JsonTempArray_Push(&buffer, c3, &state->allocator);
                        //JsonTempArray_Push(&buffer, c4, &state->allocator);
                    }
                    else if (c1 <= 0x10FFFF)
                    {
                        char c2 = 0xF0 | (c1 >> 18);           /* 11110xxx */
                        char c3 = 0x80 | ((c1 >> 12) & 0x3F);  /* 10xxxxxx */
                        char c4 = 0x80 | ((c1 >> 6) & 0x3F);   /* 10xxxxxx */
                        char c5 = 0x80 | (c1 & 0x3F);          /* 10xxxxxx */
                        //JsonTempArray_Push(&buffer, c2, &state->allocator);
                        //JsonTempArray_Push(&buffer, c3, &state->allocator);
                        //JsonTempArray_Push(&buffer, c4, &state->allocator);
                        //JsonTempArray_Push(&buffer, c5, &state->allocator);
                    }
                    break;

                default:
                    //Json_Panic(state, JsonType::String, JSON_ERROR_UNKNOWN, "Unknown escape character");
                    break;
                }
            }
            else
            {
                switch (c0)
                {
                case '\r':
                case '\n':
                    //Json_Panic(state, JsonType::String, JsonError::UnexpectedToken, "Unexpected newline characters '%c'", c0);
                    break;

                default:
                    //JsonTempArray_Push(&buffer, (char)c0, &state->allocator);
                    break;
                }
            }

            Json_NextChar(state);
        }

        Json_MatchChar(state, JsonType::String, '"');
        //if (buffer.count > 0)
        //{
        //    if (outLength) *outLength = JsonTempArray_GetCount(&buffer);
        //    JsonTempArray_Push(&buffer, 0, &state->allocator);
        //
        //    char* string = (char*)JsonTempArray_ToBuffer(&buffer, &state->allocator);
        //    JsonTempArray_Free(&buffer, &state->allocator);
        //
        //    return string;
        //}
        //else
        //{
        //    if (outLength) *outLength = 0;
        //    return NULL;
        //}
    }

    /* @funcdef: Json_ParseString */
    static void Json_ParseString(JsonState* state, Json* outValue)
    {
        if (Json_SkipSpace(state) > 0)
        {
            int length;
            const char* string = Json_ParseStringNoToken(state, &length);

            outValue->type = JsonType::String;
            outValue->string = string;
        }
    }

    /* @funcdef: Json_ParseObject */
    static void Json_ParseObject(JsonState* state, Json* outValue)
    {
        if (Json_SkipSpace(state) > 0)
        {
            Json_MatchChar(state, JsonType::Object, '{');

            HashTable<Json> values = HashTableOps::New<Json>(64);
            while (Json_SkipSpace(state) > 0 && Json_PeekChar(state) != '}')
            {
                if (values.count > 0)
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

                HashTableOps::SetValue(&values, CalcHash64(name, nameLength), value);
            }

            Json_SkipSpace(state);
            Json_MatchChar(state, JsonType::Object, '}');

            outValue->type   = JsonType::Object;
            outValue->object = values;
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
            if (setjmp(state->errjmp) == 0)
            {
                Json_ParseObject(state, &state->root);

                Json_SkipSpace(state);
                if (!Json_IsEOF(state))
                {
                    Json_Panic(state, JsonType::Null, JsonError::Format, "JSON is not well-formed. JSON is start with <object>.");
                }

                return &state->root;
            }
            else
            {
                return NULL;
            }
        }
        else if (Json_SkipSpace(state) == '[')
        {
            if (setjmp(state->errjmp) == 0)
            {
                Json_ParseArray(state, &state->root);

                Json_SkipSpace(state);
                if (!Json_IsEOF(state))
                {
                    Json_Panic(state, JsonType::Null, JsonError::Format, "JSON is not well-formed. JSON is start with <array>.");
                }

                return &state->root;
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

    Json* Parse(String json)
    {
        return Parse(json, StringOps::Length(json));
    }

    Json* Parse(String json, int jsonLength)
    {
        JsonState* state = JsonState_Make(json.buffer, jsonLength);
        Json* value = Json_ParseTopLevel(state);

        if (!value)
        {
            JsonState_Free(state);
        }

        return value;
    }

    void Release(Json* rootValue)
    {
        if (rootValue)
        {
            JsonState* state = JSON_SUPEROF(rootValue, JsonState, root);
            JsonState_Free(state);
        }
    }

    JsonError GetError(const Json* rootValue)
    {
        if (rootValue)
        {
            JsonState* state = JSON_SUPEROF(rootValue, JsonState, root);
            return state->errnum;
        }

        return JsonError::None;
    }

    const char* GetErrorString(const Json* rootValue)
    {
        if (rootValue)
        {
            JsonState* state = JSON_SUPEROF(rootValue, JsonState, root);
            return state->errmsg;
        }

        return "";
    }

    bool Equals(Json a, Json b)
    {
        int i, n;

        if (a.type != b.type)
        {
            return false;
        }

        switch (a.type)
        {
        case JsonType::Null:
            return true;

        case JsonType::Number:
            return a.number == b.number;

        case JsonType::Boolean:
            return a.boolean == b.boolean;

        case JsonType::Array:
            if ((n = a.array.count) == b.array.count)
            {
                for (i = 0; i < n; i++)
                {
                    if (!Equals(a.array.elements[i], b.array.elements[i]))
                    {
                        return false;
                    }
                }
            }
            return true;

        case JsonType::Object:
            if ((n = a.object.count) == b.object.count)
            {
                for (i = 0; i < n; i++)
                {
                    if (a.object.keys[i] != b.object.keys[i])
                    {
                        return false;
                    }

                    if (!Equals(a.object.values[i], b.object.values[i]))
                    {
                        return false;
                    }
                }
            }
            return true;

        case JsonType::String:
            return a.string == b.string;
        }

        return false;
    }

    Json Find(Json obj, String name)
    {
        return Find(obj, CalcHash64(name));
    }

    Json Find(Json obj, U64 hash)
    {
        if (obj.type == JsonType::Object)
        {
            int index = HashTableOps::IndexOf(obj.object, hash);
            if (index > -1)
            {
                return obj.object.values[index];
            }
        }

        return {};
    }
}
