#include <Yolo/Window.h>

int main()
{
    OpenWindow("Window", 800, 600);
    while (!UpdateWindow())
    {

    }

    CloseWindow();
    return 0;
}