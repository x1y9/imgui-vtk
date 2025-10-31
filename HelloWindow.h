#pragma once
#include "imgui.h"

class HelloWindow {
public:
    HelloWindow();
    void render(bool& show_demo_window, bool& show_another_window, bool& vtk_2_open, 
                bool& chart_window_open, bool& table_window_open, ImVec4& clear_color);

private:
    float f;
    int counter;
};
