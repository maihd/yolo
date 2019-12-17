#include <Yolo/File.h>

namespace Game
{
    void Init(void)
    {
#ifndef NDEBUG
        FileOps::AddSearchPath("../Games/Spaneon/Assets");
#endif
    }

    void Update(float dt)
    {

    }

    void Render(void)
    {
        
    }
}