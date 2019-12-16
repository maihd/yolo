#include <Yolo/Math.h>
#include <Yolo/Time.h>
#include <Yolo/File.h>
#include <Yolo/Input.h>
#include <Yolo/Window.h>

#include <Yolo/Texture.h>
#include <Yolo/Graphics.h>

namespace Game
{
    void Init(void);
    void Update(float dt);
    void Render(void);
}

int main(void)
{
    Window::Init("Spaneon", 1280, 720);
    Graphics::Init();

    Game::Init();

    while (!Window::PollEvents())
    {
        Time::UpdateAndSleep(60);
        Game::Update(Time::GetDeltaTime());

        Graphics::Clear();

        Game::Render();

        Graphics::Present();
    }

    Graphics::Quit();
    Window::Quit();
    return 0;
}