#include <Graphics/Window.h>
#include <Graphics/Graphics.h>

#define TEST_RUNNER
#include <Misc/Testing.h>

#include "Framework/ECSCoordinator.h"
#include "Components/Types.h"
#include "Systems/RenderSystem.h"

#include <stdint.h>

float RandomMinMax(float min, float max)
{
    float value = (float)((double)RandomNextU32() / UINT32_MAX);
    return min + (max - min) * value;
}

float RandomPosition()
{
    return RandomMinMax(-100.0f, 100.0f);
}

float RandomRotation()
{
    return RandomMinMax(0.0f, 3.0f);
}

float RandomColor()
{
    return RandomMinMax(0.0f, 1.0f);
}

int main()
{
    RunAllTestCases();

    OpenWindow("ECS", 1280, 720);

    ECSCoordinator coordinator;
    ECSCoordinator_Init(&coordinator);
    
    ECSCoordinator_RegisterComponent<Camera>(&coordinator);
    ECSCoordinator_RegisterComponent<Gravity>(&coordinator);
    ECSCoordinator_RegisterComponent<Player>(&coordinator);
    ECSCoordinator_RegisterComponent<Renderable>(&coordinator);
    ECSCoordinator_RegisterComponent<RigidBody>(&coordinator);
    ECSCoordinator_RegisterComponent<Thrust>(&coordinator);
    ECSCoordinator_RegisterComponent<Transform>(&coordinator);

    RenderSystem* renderSystem = ECSCoordinator_RegisterSystem<RenderSystem>(&coordinator);
    {
        ECSSignature signature = 0
            | (1 << ECSCoordinator_GetComponentType<Renderable>(&coordinator))
            | (1 << ECSCoordinator_GetComponentType<Transform>(&coordinator))
            ;
        ECSCoordinator_SetSystemSignature<RenderSystem>(&coordinator, signature);
    }
    renderSystem->Init(&coordinator);

    for (I32 i = 0; i < MAX_ENTITIES - 10; i++)
    {
        ECSEntity entity = ECSCoordinator_CreateEntity(&coordinator);

        ECSCoordinator_AddComponent(
            &coordinator,
            entity,
            Transform {
                Vector3 { RandomPosition(), RandomPosition(), RandomPosition() },
                Vector3 { RandomRotation(), RandomRotation(), RandomRotation() },
                Vector3 { 1.0f, 1.0f, 1.0f }
            });

        ECSCoordinator_AddComponent(
            &coordinator,
            entity,
            Renderable{
                Vector3 { RandomColor(), RandomColor(), RandomColor() }
            });
    }

    while (!UpdateWindow())
    {
        const float deltaTime = GetDeltaTime();

        if (BeginDrawing())
        {
            renderSystem->Render(&coordinator, deltaTime);
            EndDrawing();
        }
    }

    renderSystem->Free(&coordinator);
    ECSCoordinator_Free(&coordinator);
    CloseWindow();
    return 0;
}