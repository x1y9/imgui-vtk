#include "ChartWindow.h"

ChartWindow::ChartWindow() {
    // Initialize chart data with sine waves
    for (int chart = 0; chart < num_charts; chart++) {
        for (int i = 0; i < data_points; i++) {
            float phase = (float)chart * 0.5f;
            chart_data[chart][i] = sinf(i * 0.05f + phase) * (chart + 1) * 10.0f;
        }
    }
}

void ChartWindow::render(bool& chart_window_open, int count) {
    if (!chart_window_open) return;
    
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("chart", &chart_window_open);
    ImGui::Text("Lines:%d,%d points per line", num_charts, data_points);
    ImGui::Separator();

    ImVec2 chart_size = ImVec2(ImGui::GetContentRegionAvail().x, 150);
    for (int chart = 0; chart < num_charts; chart++) {
        char label[64];
        sprintf(label, "chart #%d", chart + 1);

        float min_val = -(chart + 1) * 10.0f - 5.0f;
        float max_val = (chart + 1) * 10.0f + 5.0f;

        ImGui::PlotLines(label, chart_data[chart], data_points, count, NULL,
                        min_val, max_val, chart_size);
    }

    ImGui::End();
}
