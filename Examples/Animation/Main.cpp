#include <Yolo/Timer.h>
#include <Yolo/Window.h>
#include <Yolo/Graphics.h>

int main()
{
    OpenWindow("Animation", 1280, 720);

    while (!UpdateWindow())
    {
        UpdateTimerAndSleep(60);
        
        if (BeginDrawing())
        {
            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}