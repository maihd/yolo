#include <Yolo/Math.h>
#include <Yolo/Time.h>
#include <Yolo/Font.h>
#include <Yolo/String.h>
#include <Yolo/Window.h>
#include <Yolo/Graphics.h>

#include <Yolo/Array.h>
#include <Yolo/HashTable.h>

#include <stdio.h>
#include <string.h>

int main(void)
{
    String string1 = StringOps::Static("Hello world");
    printf("string 1: %s\n", string1);

    String string2 = StringOps::Intern(string1);
    printf("string 2: %s\n", string2);

    if (string1 == string2)
    {
        printf("string1 and string2 is equal!\n");
    }
    else
    {
        printf("string1 and string2 is not equal!\n");
    }

    Window::Init("Yolo Window", 1280, 720);
    Graphics::Init();

    Font font = FontOps::Load("../Assets/OpenSans_Regular.ttf", 32.0f);

    int width = Window::GetWidth();
    int height = Window::GetHeight();

    Array<int> intArray = {};
    ArrayOps::Push(&intArray, 10);
    ArrayOps::Free(&intArray);

    float angle = 0.0f;

    Texture texture = font.texture;//Texture::Load("../Assets/opengl_logo.png");
    float posX = 0;

    HashTable<int> hashTable = HashTableOps::New<int>();
    HashTableOps::SetValue(&hashTable, CalcHash64("ten"), 10);

    int value = HashTableOps::GetValue(hashTable, CalcHash64("ten"));

    HashTableOps::Free(&hashTable);

    Graphics::SetVSync(true);

    while (!Window::PollEvents())
    {
        //Graphics::ClearColor(1, 1, 1);
        Graphics::Clear();

        //Graphics::SetLineWidth(4.0f);
        //Graphics::DrawRectangle(DrawMode::Fill, { 400, 300 }, { 50, 50 }, { 0.5, 0.5, 0.5, 1 });
        //Graphics::DrawCircle(DrawMode::Line, { 400 + 100 * cosf(angle), 300 }, 50, { 1, 1, 1, 1 });

        //Graphics::DrawText("Hello world", font, { posX + width * 0.5f, height * 0.5f });
        Graphics::DrawFramerate(font, { 0.0f, (float)height - font.size });

        Graphics::Present();

        angle += Time::GetDeltaTime();
        posX = cosf(angle) * 100;
        Time::UpdateAndSleep(60);
    }

    Graphics::Quit();
    Window::Quit();
    return 0;
}