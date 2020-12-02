#include <Yolo/Window.h>

int main()
{
    OpenWindow("SpineView", 800, 600);

    while (ShouldWindowQuit())
    {
        ClearWindow();

        SwapWindowBuffers();
    }

    CloseWindow();
    return 0;  
}