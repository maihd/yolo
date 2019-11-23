#include <Yolo/String.h>
#include <Yolo/Window.h>

#include <stdio.h>

int main(void)
{
    string string1 = String::From("Hello world");
    printf("string 1: %s\n", string1);

    string string2 = String::From(string1);
    printf("string 2: %s\n", string2);

    Window::Init("Yolo Window", 800, 600);

    while (!Window::ShouldQuit())
    {
        Window::PollEvents();

        Window::SwapBuffer();
    }

    Window::Quit();

    String::Free(string1);
    String::Free(string2);
    return 0;
}