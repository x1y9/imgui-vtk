#include "VtkViewer1Window.h"

VtkViewer1Window::VtkViewer1Window() {}

void VtkViewer1Window::addActor(vtkSmartPointer<vtkActor> actor) {
    vtkViewer.addActor(actor);
}

void VtkViewer1Window::render(bool& vtk_1_open) {
    ImGui::SetNextWindowSize(ImVec2(360, 240), ImGuiCond_FirstUseEver);
    ImGuiWindowClass vtkViewClass;
    vtkViewClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingOverMe;
    ImGui::SetNextWindowClass(&vtkViewClass);
    
    if (vtk_1_open) {
        ImGui::Begin("Vtk Viewer 1", &vtk_1_open, VtkViewer::NoScrollFlags());
        vtkViewer.render();
        ImGui::End();
    }
}

void VtkViewer1Window::cleanup() {
    vtkViewer.cleanup();
}
