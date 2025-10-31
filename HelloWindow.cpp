#include "HelloWindow.h"

HelloWindow::HelloWindow() : f(0.0f), counter(0) {}

void HelloWindow::render(bool& show_demo_window, bool& show_another_window, bool& vtk_2_open,
                         bool& chart_window_open, bool& table_window_open, ImVec4& clear_color) {
    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");
    ImGui::Checkbox("Demo Window", &show_demo_window);
    ImGui::Checkbox("Another Window", &show_another_window);
    ImGui::Checkbox("VTK Viewer #2", &vtk_2_open);
    ImGui::Checkbox("Chart Window", &chart_window_open);
    ImGui::Checkbox("Table Window", &table_window_open);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", (float*)&clear_color);

    if (ImGui::Button("Button")) {
        counter++;
    }
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::End();
}
