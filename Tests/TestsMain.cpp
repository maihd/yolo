#include <stdio.h>
#include <string.h>

#include <Math/Math.h>
#include <Text/String.h>

#include <Graphics/Imgui.h>
#include <Graphics/Window.h>
#include <Graphics/Graphics.h>

#include <Container/Array.h>
#include <Container/HashTable.h>
#include <Container/Sort.h>

#include <Misc/HotDylib.h>

#define TEST_RUNNER
#include <Misc/Testing.h>

#define SHOW_UI 0

int main(void)
{
    MemoryTracker memoryTracker;

    RunAllTestCases();

#if SHOW_UI
    OpenWindow("Yolo Window", 1280, 720);

    Font font = LoadFont("../Assets/OpenSans_Regular.ttf", 32.0f);

    int width = WindowWidth();
    int height = WindowHeight();

    float angle = 0.0f;

    Texture texture = LoadTexture("../Assets/character.png");
    float posX = 0;
    Texture texture2 = LoadTexture("../Assets/character.png");

    SetVSync(true);

    while (!UpdateWindow())
    {
        //Graphics::ClearColor(1, 1, 1);
        if (BeginDrawing())
        {
            Graphics::SetLineWidth(4.0f);
            DrawRectangle(DrawMode::Fill, { 400, 300 }, { 50, 50 }, { 0.5, 0.5, 0.5, 1 });
            DrawCircle(DrawMode::Line, { 400 + 100 * cosf(angle), 300 }, 50, { 1, 1, 1, 1 });

            DrawText("Hello world", font, { posX + width * 0.5f, height * 0.5f });
            DrawTexture(texture, { width * 0.5f, height * 0.5f }, GetTotalTime());
            DrawTexture(texture, { width * 0.25f, height * 0.25f }, GetTotalTime() * 0.5f, Vector2{ 1, 1 }, Vector4{ 0.5f, 0.5f, 0.5f, 1.0f }, Vector2{ 1.0f, 1.0f });
            DrawFramerate(font, { 0.0f, (float)height - font.Size });

            ImGui::Begin("Debug");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();

            ImGui::DumpMemoryAllocs(ImGuiDumpMemoryFlags_OpenWindow);

            EndDrawing();
        }

        angle += GetDeltaTime();
        posX = cosf(angle) * 100;
    }

    FreeFont(&font);
    CloseWindow();
#endif

    MemoryDumpAllocs();
    return 0;
}