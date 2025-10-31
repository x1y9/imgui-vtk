#include "VtkViewer2Window.h"

VtkViewer2Window::VtkViewer2Window() : bkgAlpha(0.2f) {
    vtkViewer.getRenderer()->SetBackground(0, 0, 0);
}

void VtkViewer2Window::addActor(vtkSmartPointer<vtkActor> actor) {
    vtkViewer.addActor(actor);
}

void VtkViewer2Window::render(bool& vtk_2_open) {
    ImGui::SetNextWindowSize(ImVec2(720, 480), ImGuiCond_FirstUseEver);
    ImGuiWindowClass vtkViewClass;
    vtkViewClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingOverMe;
    ImGui::SetNextWindowClass(&vtkViewClass);
    
    if (vtk_2_open) {
        ImGui::Begin("Vtk Viewer 2", &vtk_2_open, VtkViewer::NoScrollFlags());

        auto renderer = vtkViewer.getRenderer();
        if (ImGui::Button("VTK Background: Black")) {
            renderer->SetBackground(0, 0, 0);
        }
        ImGui::SameLine();
        if (ImGui::Button("VTK Background: Red")) {
            renderer->SetBackground(1, 0, 0);
        }
        ImGui::SameLine();
        if (ImGui::Button("VTK Background: Green")) {
            renderer->SetBackground(0, 1, 0);
        }
        ImGui::SameLine();
        if (ImGui::Button("VTK Background: Blue")) {
            renderer->SetBackground(0, 0, 1);
        }
        
        ImGui::SliderFloat("Background Alpha", &bkgAlpha, 0.0f, 1.0f);
        renderer->SetBackgroundAlpha(bkgAlpha);

        vtkViewer.render();
        ImGui::End();
    }
}

void VtkViewer2Window::cleanup() {
    vtkViewer.cleanup();
}
