struct UIMainScreen
{
    bool showDebug = false;

    void Render()
    {
        UIButton()
            .Title("Increment")
            .Position(Vector2(0.0f, 0.0f))
            .Color(Vector4(0.0f, 0.0f, 1.0f, 1.0f))
            .Render([]() {

            });

        UICheckBox()
            .Title("Show debug")
            .Position(Vector2(10.0f, 100.0f))
            .Render(&showDebug);

        UIGroup(showDebug)
            .Padding(Vector2(16.0f, 10.0f))
            .Color(0x123456)
            .Render([]() {
                Text("The showDebug is true")
                    .Render();
            });
    }
};

void main()
{
    Sprite* sprite = Sprite::Builder("")
        .position(Vector2(0.0f, 0.0f))
        .parent(this)
        .build();

    return UIApplication()
        .Render(UIMainScreen())
        .Start();
}