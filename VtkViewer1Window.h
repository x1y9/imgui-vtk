#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "VtkViewer.h"

class VtkViewer1Window {
public:
    VtkViewer1Window();
    void addActor(vtkSmartPointer<vtkActor> actor);
    void render(bool& vtk_1_open);
    void cleanup();

private:
    VtkViewer vtkViewer;
};
