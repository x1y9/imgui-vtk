#include "VtkViewer2Window.h"
#include "Localization.h"

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
        ImGui::Begin(TR("window.vtk2"), &vtk_2_open, VtkViewer::NoScrollFlags());

        auto renderer = vtkViewer.getRenderer();
        if (ImGui::Button(TR("vtk.bg.black"))) {
            renderer->SetBackground(0, 0, 0);
            vtkViewer.markDirty();
        }
        ImGui::SameLine();
        if (ImGui::Button(TR("vtk.bg.red"))) {
            renderer->SetBackground(1, 0, 0);
            vtkViewer.markDirty();
        }
        ImGui::SameLine();
        if (ImGui::Button(TR("vtk.bg.green"))) {
            renderer->SetBackground(0, 1, 0);
            vtkViewer.markDirty();
        }
        ImGui::SameLine();
        if (ImGui::Button(TR("vtk.bg.blue"))) {
            renderer->SetBackground(0, 0, 1);
            vtkViewer.markDirty();
        }
        
        if (ImGui::SliderFloat(TR("vtk.bg.alpha"), &bkgAlpha, 0.0f, 1.0f)) {
            renderer->SetBackgroundAlpha(bkgAlpha);
            vtkViewer.markDirty();
        }

        vtkViewer.render();
        ImGui::End();
    }
}

void VtkViewer2Window::cleanup() {
    vtkViewer.cleanup();
}
