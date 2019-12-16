#include <Yolo/Time.h>
#include <Yolo/File.h>
#include <Yolo/Input.h>
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
    vec2 position = { 640, 360 };
    while (!Window::PollEvents())
    {
        Graphics::Clear();

        Graphics::DrawTexture(texture, position);

        Graphics::Present();

        float vertical = 0.0f;
        float horizontal = 0.0f;
        if (Input::GetKey(KeyCode::LeftArrow))
        {
            horizontal = -1.0f;
        }
        else if (Input::GetKey(KeyCode::RightArrow))
        {
            horizontal = 1.0f;
        }

        if (Input::GetKey(KeyCode::DownArrow))
        {
            vertical = -1.0f;
        }
        else if (Input::GetKey(KeyCode::UpArrow))
        {
            vertical = 1.0f;
        }
        position.x += 100 * Time::GetDeltaTime() * horizontal;
        position.y += 100 * Time::GetDeltaTime() * vertical;

        Time::UpdateAndSleep(60);
    }

    Graphics::Quit();
    Window::Quit();
    return 0;
}