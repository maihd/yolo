#include <Yolo/Time.h>
#include <Yolo/File.h>
#include <Yolo/Window.h>

#include <Yolo/Texture.h>
#include <Yolo/Graphics.h>

int main(void)
{
    Window::Init("Spaneon", 1280, 720);
    Graphics::Init();

#ifndef NDEBUG
    FileOps::AddSearchPath("../Games/Spaneon/Assets");
#endif

    Texture texture = TextureOps::Load("Art/Player.png");

    while (!Window::PollEvents())
    {
        Graphics::Clear();

        Graphics::DrawTexture(texture, { 640, 360 });

        Graphics::Present();

        Time::UpdateAndSleep(60);
    }

    Graphics::Quit();
    Window::Quit();
    return 0;
}