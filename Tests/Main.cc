#include <Yolo/Math.h>
#include <Yolo/Time.h>
#include <Yolo/Array.h>
#include <Yolo/String.h>
#include <Yolo/Window.h>
#include <Yolo/Graphics.h>

#include <stdio.h>

int main(void)
{
    string string1 = String::From("Hello world");
    printf("string 1: %s\n", string1);

    string string2 = String::From(string1);
    printf("string 2: %s\n", string2);

    Window::Init("Yolo Window", 1280, 720);
    Graphics::Init();

    int width = Window::GetWidth();
    int height = Window::GetHeight();

    int* intArray = Array::Empty<int>();
    Array::Push(&intArray, 10);
    Array::Free(&intArray);

    float angle = 0.0f;

    Texture texture = Texture::Load("../Assets/opengl_logo.png");

    while (!Window::PollEvents())
    {
        //Graphics::ClearColor(0, 1, 1);
        Graphics::Clear();

        //Graphics::DrawCircle({ 400, 300 }, 10, { 0.5, 0.5, 0.5, 1 });
        //Graphics::DrawCircle({ 400 + 100 * cosf(angle), 300 }, 50, { 1, 1, 1, 1 });

        Graphics::DrawTexture(texture, { width * 0.5f, height * 0.5f }, { (float)texture.width, (float)texture.height });

        Graphics::Present();

        angle += Time::GetDeltaTime();
        Time::UpdateAndSleep(60);
    }

    Graphics::Quit();
    Window::Quit();

    String::Free(string1);
    String::Free(string2);
    return 0;
}