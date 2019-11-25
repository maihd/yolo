#include <Yolo/Math.h>
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

    Window::Init("Yolo Window", 800, 600);
    Graphics::Init();

    vec3 vec3 = { 1, 2, 3 };
    float vals[] = { 1, 2 };
    vec2 vec2 = vec2::From(vals);

    int* intArray = Array::Empty<int>();
    Array::Push(&intArray, 10);
    Array::Free(&intArray);

    while (!Window::PollEvents())
    {
        //Graphics::ClearColor(0, 1, 1);
        Graphics::Clear();

        Graphics::Present();
    }

    Graphics::Quit();
    Window::Quit();

    String::Free(string1);
    String::Free(string2);
    return 0;
}