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
    OpenWindow("Spaneon", 1280, 720);
    Graphics::Init();

    Game::Init();

    while (!WindowPollEvents())
    {
        TimeUpdateAndSleep(60);
        Game::Update(GetDeltaTime());

        Graphics::Clear();

        Game::Render();

        Graphics::Present();
    }

    Graphics::Quit();
    CloseWindow();
    return 0;
}