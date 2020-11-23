#include <Yolo/Math.h>
#include <Yolo/Time.h>
#include <Yolo/Font.h>
#include <Yolo/Texture.h>

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

    int intArray[4];
    intArray[0] = rand();
    intArray[1] = rand();
    intArray[2] = rand();
    intArray[3] = rand();
    InsertSort(intArray, 4);

    if (string1 == string2)
    {
        printf("string1 and string2 is equal!\n");
    }
    else
    {
        printf("string1 and string2 is not equal!\n");
    }

    OpenWindow("Yolo Window", 1280, 720);
    Graphics::Init();

    Font font = FontOps::Load("../Assets/OpenSans_Regular.ttf", 32.0f);

    int width = WindowWidth();
    int height = WindowHeight();

    float angle = 0.0f;

    Texture texture = TextureOps::Load("../Assets/character.png");
    float posX = 0;
    Texture texture2 = TextureOps::Load("../Assets/character.png");

    HashTable<int> hashTable = HashTableOps::New<int>();
    HashTableOps::SetValue(&hashTable, ConstHash64("ten"), 10);

    int value = HashTableOps::GetValue(hashTable, ConstHash64("ten"));

    HashTableOps::Free(&hashTable);

    Graphics::SetVSync(true);

    while (!WindowPollEvents())
    {
        //Graphics::ClearColor(1, 1, 1);
        Graphics::NewFrame();

        //Graphics::SetLineWidth(4.0f);
        //Graphics::DrawRectangle(DrawMode::Fill, { 400, 300 }, { 50, 50 }, { 0.5, 0.5, 0.5, 1 });
        //Graphics::DrawCircle(DrawMode::Line, { 400 + 100 * cosf(angle), 300 }, 50, { 1, 1, 1, 1 });

        //Graphics::DrawText("Hello world", font, { posX + width * 0.5f, height * 0.5f });
        //Graphics::DrawTexture(texture, { width * 0.5f, height * 0.5f }, GetTotalTime());
        //Graphics::DrawTexture(texture, { width * 0.25f, height * 0.25f }, GetTotalTime() * 0.5f, Vector2{ 1, 1 }, Vector4{ 0.5f, 0.5f, 0.5f, 1.0f }, Vector2{ 1.0f, 1.0f });
        //Graphics::DrawFramerate(font, { 0.0f, (float)height - font.size });

        ImGui::Begin("Debug");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        // 4. Tracking your memory allocations
        ImGui::DumpMemoryAllocs(ImGuiDumpMemoryFlags_OpenWindow);

        Graphics::Present();

        angle += GetDeltaTime();
        posX = cosf(angle) * 100;
        TimeUpdateAndSleep(60);
    }

    Graphics::Quit();
    CloseWindow();
    return 0;
}