#include <Misc/Testing.h>
#include <Math/Math.h>

DEFINE_TEST_CASE("MakeVector2")
{
    const Vector2 v = MakeVector2(1, 2);
    TestEqual(v, (Vector2 { 1, 2 }));
}
