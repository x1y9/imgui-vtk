#pragma once
#include "imgui.h"
#include "VtkViewer.h"
#include "imgui_internal.h"

class VtkViewer2Window {
public:
    VtkViewer2Window();
    void addActor(vtkSmartPointer<vtkActor> actor);
    void render(bool& vtk_2_open);
    void cleanup();

private:
    VtkViewer vtkViewer;
    float bkgAlpha;
};
