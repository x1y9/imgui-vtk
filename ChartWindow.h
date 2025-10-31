#pragma once
#include "imgui.h"
#include <cmath>
#include <cstdio>

class ChartWindow {
public:
    ChartWindow();
    void render(bool& chart_window_open, int count);

private:
    static const int num_charts = 10;
    static const int data_points = 10000;
    float chart_data[num_charts][data_points];
};
