#include <Yolo/File.h>

#include "World.h"

namespace Game
{
    static World world;

    void Init(void)
    {
#ifndef NDEBUG
        FileOps::AddSearchPath("../Games/Spaneon/Assets");
#endif

        world = WorldOps::New();
    }

    void Quit(void)
    {
        WorldOps::Free(&world);
    }

    void Update(float dt)
    {
        WorldOps::Update(&world, 0, 0, Vector2{}, false, dt);
    }

    void Render(void)
    {
        WorldOps::Render(world);
    }
}