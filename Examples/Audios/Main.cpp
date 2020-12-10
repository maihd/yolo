#include <Yolo/Window.h>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

int main()
{
    OpenWindow("Window", 800, 600);
    while (!UpdateWindow())
    {

    }

    CloseWindow();
    return 0;
}