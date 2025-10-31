#include "TableWindow.h"

TableWindow::TableWindow() : sort_column(0), sort_ascending(true) {
    // Initialize table data - 10000 rows, 4 columns
    table_data.reserve(10000);
    for (int i = 0; i < 10000; i++) {
        TableRow row;
        row.id = i + 1;
        sprintf(row.name, "Project %d", i + 1);
        row.value = (float)(rand() % 10000) / 100.0f;
        row.status = rand() % 5;
        table_data.push_back(row);
    }
}

void TableWindow::render(bool& table_window_open) {
    if (!table_window_open) return;
    
    ImGui::SetNextWindowSize(ImVec2(900, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("Grid", &table_window_open);

    ImGui::Text("Lines: %d", (int)table_data.size());
    ImGui::Separator();

    ImGuiTableFlags flags = ImGuiTableFlags_Sortable |
                            ImGuiTableFlags_Reorderable |
                            ImGuiTableFlags_Resizable |
                            ImGuiTableFlags_ScrollY |
                            ImGuiTableFlags_RowBg |
                            ImGuiTableFlags_BordersOuter |
                            ImGuiTableFlags_BordersV;

    if (ImGui::BeginTable("DataTable", 4, flags, ImVec2(0.0f, 0.0f))) {
        // Setup columns
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 80.0f, 0);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 200.0f, 1);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, 150.0f, 2);
        ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 100.0f, 3);
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        // Handle sorting
        if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs()) {
            if (sort_specs->SpecsDirty) {
                if (sort_specs->SpecsCount > 0) {
                    const ImGuiTableColumnSortSpecs& spec = sort_specs->Specs[0];
                    sort_column = spec.ColumnUserID;
                    sort_ascending = (spec.SortDirection == ImGuiSortDirection_Ascending);

                    // Sort the data
                    std::sort(table_data.begin(), table_data.end(), [this](const TableRow& a, const TableRow& b) {
                        switch (sort_column) {
                            case 0: return sort_ascending ? (a.id < b.id) : (a.id > b.id);
                            case 1: {
                                int cmp = strcmp(a.name, b.name);
                                return sort_ascending ? (cmp < 0) : (cmp > 0);
                            }
                            case 2: return sort_ascending ? (a.value < b.value) : (a.value > b.value);
                            case 3: return sort_ascending ? (a.status < b.status) : (a.status > b.status);
                            default: return false;
                        }
                    });
                }
                sort_specs->SpecsDirty = false;
            }
        }

        // Display rows
        ImGuiListClipper clipper;
        clipper.Begin((int)table_data.size());
        while (clipper.Step()) {
            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
                ImGui::TableNextRow();
                ImGui::PushID(row);

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", table_data[row].id);

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", table_data[row].name);

                ImGui::TableSetColumnIndex(2);
                ImGui::SetNextItemWidth(-FLT_MIN);
                if (ImGui::InputFloat("##value", &table_data[row].value, 0.0f, 0.0f, "%.2f")) {
                    if (table_data[row].value < 0.0f) table_data[row].value = 0.0f;
                    if (table_data[row].value > 100.0f) table_data[row].value = 100.0f;
                }

                ImGui::TableSetColumnIndex(3);
                const char* status_text[] = {"待处理", "进行中", "已完成", "已取消", "错误"};
                ImGui::Text("%s", status_text[table_data[row].status]);

                ImGui::PopID();
            }
        }

        ImGui::EndTable();
    }

    ImGui::End();
}
