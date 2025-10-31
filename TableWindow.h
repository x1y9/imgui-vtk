#pragma once
#include "imgui.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cstdio>

class TableWindow {
public:
    TableWindow();
    void render(bool& table_window_open);

private:
    struct TableRow {
        int id;
        char name[32];
        float value;
        int status;
    };

    std::vector<TableRow> table_data;
    int sort_column;
    bool sort_ascending;
};
