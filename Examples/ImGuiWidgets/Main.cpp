#include <System/Memory.h>
#include <Graphics/Window.h>
#include <Graphics/Graphics.h>

static void InitWidgets();
static void FreeWidgets();

static void ImGuiWidgets();

int main()
{
    MemoryTracker memoryTracker; // Tracking memory leaks

    OpenWindow("Window", 1280, 720);
    InitWidgets();

    while (!UpdateWindow())
    {
        if (BeginDrawing())
        {
            ImGuiWidgets();

            EndDrawing();
        }
    }

    FreeWidgets();
    CloseWindow();
    return 0;
}

#include <Graphics/Imgui.h>
#include <Container/Array.h>

#include "DefineWidget.h"

using WidgetFunc = void(*)();

struct Widget
{
    const char*     Name;
    WidgetFunc      Func;
};

static Array<Widget>    gWidgets;
static Widget           gCurrentWidget = { "", nullptr };

DefineWidget::DefineWidget(const char* name, WidgetFunc func)
{
    ArrayPush(&gWidgets, {name, func});
}

void InitWidgets()
{
    if (gWidgets.Count > 0)
    {
        gCurrentWidget = gWidgets.Items[0];
    }
}

void FreeWidgets()
{
    FreeArray(&gWidgets);
}

void ImGuiWidgets()
{
    // Render widgets select window

    ImGui::Begin("Widgets");

    if (ImGui::BeginCombo("Current widget", gCurrentWidget.Name))
    {
        for (const Widget widget : IterateArray(gWidgets))
        {
            bool isSelected = widget.Func == gCurrentWidget.Func;
            if (ImGui::Selectable(widget.Name, isSelected))
            {
                gCurrentWidget = widget;
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }

        if (gWidgets.Count == 0)
        {
            ImGui::Text("NO WIDGETS DEFINED!");
        }

        ImGui::EndCombo();
    }

    ImGui::End();

    // Render the selected widget

    if (gCurrentWidget.Name != "" && gCurrentWidget.Func != nullptr)
    {
        ImGui::Begin(gCurrentWidget.Name);

        gCurrentWidget.Func();

        ImGui::End();
    }
}

DEFINE_WIDGET("Tutorial")
{
    ImGui::Text("Use DEFINE_WIDGET(name) to define a new widget!");
}
