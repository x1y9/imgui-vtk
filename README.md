# imgui-vtk
- Incorporate VTK's dataset visualization capabilities with versatility of Dear ImGui
  - Multiple independent `VtkViewer` instances can be used in the same program to display multiple VTK scenes simultaneously
- Uses VTK's OpenGLRenderWindow class
  - Renders to texture and displays the texture natively with ImGui
- Compatible with both ImGui's `main` and `docking` branches

## Usage:
- Build VTK 9.3.1, if static, set VTK_SMP_ENABLE_STDTHREAD=OFF



