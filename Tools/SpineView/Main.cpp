#include <Yolo/Window.h>
#include <Yolo/Graphics.h>

int main()
{
    OpenWindow("SpineView", 800, 600);

    while (HandleWindowEvents())
    {
        Graphics::NewFrame();

        Graphics::EndFrame();
    }

    CloseWindow();
    return 0;  
}