#pragma once

#include <Graphics/Graphics.h>
#include "../Framework/ECSTypes.h"

struct RenderSystem : public ECSSystem
{
    ECSEntity       CameraEntity;

    Shader          Shader;
    VertexArray     VertexArray;

    void            Init(ECSCoordinator* coordinator);
    void            Free(ECSCoordinator* coordinator);

    void            Render(ECSCoordinator* coordinator, const float dt);
};
