#include <Yolo/Array.h>
#include <Yolo/Imgui.h>
#include <Yolo/String.h>
#include <Yolo/Window.h>
#include <Yolo/Graphics.h>

#include <string.h>

struct Todo
{
    String  Content;
    bool    Completed;
};

enum struct Visibility
{
    All,
    Active,
    Completed
};

struct Application
{
    Array<Todo> Todos;
    Visibility  Visibility;
    char        InputBuffer[1024];
};

int main()
{
    Application application = { MakeArray<Todo>(16), Visibility::All, "" };
    ArrayPush(&application.Todos, { ConstString("Learn"), false });
    ArrayPush(&application.Todos, { ConstString("Do work"), false });
    ArrayPush(&application.Todos, { ConstString("Make love"), false });

    OpenWindow("TodoMVC", 1280, 720);

    while (!UpdateWindow())
    {
        ImGui::Begin("TodoMVC");

        ImGui::InputText("New job", application.InputBuffer, CountOf(application.InputBuffer));

        ImGui::SameLine();
        if (ImGui::Button("Add"))
        {
            if (strlen(application.InputBuffer) > 0)
            {
                ArrayPush(&application.Todos, { SaveString(StringView(application.InputBuffer, (int)strlen(application.InputBuffer))), false });
                application.InputBuffer[0] = '\0';
            }
        }
        
        if (ImGui::RadioButton("All", application.Visibility == Visibility::All))
        {
            application.Visibility = Visibility::All;
        }

        ImGui::SameLine();
        if (ImGui::RadioButton("Active", application.Visibility == Visibility::Active))
        {
            application.Visibility = Visibility::Active;
        }

        ImGui::SameLine();
        if (ImGui::RadioButton("Completed", application.Visibility == Visibility::Completed))
        {
            application.Visibility = Visibility::Completed;
        }

        for (int i = 0, n = application.Todos.Count; i < n; i++)
        {
            Todo& todo = application.Todos.Items[i];
            if (application.Visibility == Visibility::Active && todo.Completed
                || (application.Visibility == Visibility::Completed && !todo.Completed))
            {
                continue;
            }

            char buffer[1024];

            if (todo.Completed)
            {
                ImGui::TextDisabled("%d. %s", i, todo.Content.Buffer);
            }
            else
            {
                ImGui::Text("%d. %s", i, todo.Content.Buffer);
            }
            
            ImGui::SameLine(); 
            String checkboxLabel = StringFormat(buffer, CountOf(buffer), "##%d", i);
            ImGui::Checkbox(checkboxLabel.Buffer, &todo.Completed);
            
            ImGui::SameLine();
            String buttonLabel = StringFormat(buffer, CountOf(buffer), "Remove##%d", i);
            if (ImGui::Button(buttonLabel.Buffer))
            {
                ArrayErase(&application.Todos, i);
                break;
            }
        }
        
        ImGui::End();

        ImGui::DumpMemoryAllocs(ImGuiDumpMemoryFlags_OpenWindow);

        if (BeginDrawing())
        {            
            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}
