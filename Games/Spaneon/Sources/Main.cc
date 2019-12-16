#include <Yolo/Time.h>
#include <Yolo/Window.h>
#include <Yolo/Graphics.h>

int main(void)
{
    Window::Init("Spaneon", 1280, 720);
    Graphics::Init();

    while (!Window::PollEvents())
    {
        Graphics::Clear();

        Graphics::Present();

        Time::UpdateAndSleep(60);
    }

    Graphics::Quit();
    Window::Quit();
    return 0;
}