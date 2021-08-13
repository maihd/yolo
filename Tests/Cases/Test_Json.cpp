#include <Misc/Testing.h>

#include <Text/Json.h>
#include <Text/String.h>

DEFINE_TEST_CASE("Basics json")
{
    Json* json = ParseJson("{\"greeting\":\"Hello World\"}");
    Test(json->Type == JsonType::Object 
        && JsonFind(*json, "greeting").Type == JsonType::String
        && StringCompare(JsonFind(*json, "greeting").String, "Hello World") == 0
    );

    FreeJson(json);
}