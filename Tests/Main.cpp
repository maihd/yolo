#include <Yolo/Math.h>

#include <Yolo/String.h>
#include <Yolo/Window.h>
#include <Yolo/Graphics.h>

#include <Yolo/Array.h>
#include <Yolo/HashTable.h>

#include <Yolo/HotDylib.h>

#include <Yolo/Imgui.h>
#include <Yolo/Sort.h>

#include <stdio.h>
#include <string.h>

int main(void)
{
    String string1 = ConstString("Hello world");
    printf("string 1: %s\n", string1.Buffer);

    String string2 = string1;
    printf("string 2: %s\n", string2.Buffer);

    StringView viewOfHelloWorld = "Hello world";
    printf("viewOfHelloWorld: %s\n", viewOfHelloWorld.Buffer);

    if (string1 == string2)
    {
        printf("string1 and string2 is equal!\n");
    }
    else
    {
        printf("string1 and string2 is not equal!\n");
    }

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
    return 0;
}