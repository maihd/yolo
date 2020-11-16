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

#include <stdio.h>
#include <string.h>

int main(void)
{
    String string1 = StringOps::Static("Hello world");
    printf("string 1: %s\n", string1.Buffer);

    String string2 = StringOps::Intern(string1);
    printf("string 2: %s\n", string2.Buffer);

    if (string1.Buffer == string2.Buffer)
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

    Array<int> intArray = {};
    ArrayPush(&intArray, 10);
    ArrayFree(&intArray);

    float angle = 0.0f;

    Texture texture = TextureOps::Load("../Assets/character.png");
    float posX = 0;
    Texture texture2 = TextureOps::Load("../Assets/character.png");

    HashTable<int> hashTable = HashTableOps::New<int>();
    HashTableOps::SetValue(&hashTable, CalcHash64("ten"), 10);

    int value = HashTableOps::GetValue(hashTable, CalcHash64("ten"));

    HashTableOps::Free(&hashTable);

    Graphics::SetVSync(true);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        Graphics::Present();

        angle += GetDeltaTime();
        posX = cosf(angle) * 100;
        TimeUpdateAndSleep(60);
    }

    Graphics::Quit();
    CloseWindow();
    return 0;
}