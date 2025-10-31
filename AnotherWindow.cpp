#include "AnotherWindow.h"

void AnotherWindow::render(bool& show_another_window) {
    ImGui::Begin("Another Window", &show_another_window);
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me")) {
        show_another_window = false;
    }
    ImGui::End();
}
