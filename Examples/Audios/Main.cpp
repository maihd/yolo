#include <Yolo/Window.h>
#include "Audio.h"

int main()
{
    OpenWindow("Window", 800, 600);
    OpenAudioDevice();

    while (!UpdateWindow())
    {

    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}