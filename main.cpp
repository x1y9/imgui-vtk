// Standard Library
#include <iostream>

// OpenGL Loader
#include <GL/gl3w.h>

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

// ImGui + imgui-vtk
#include "imgui.h"
#include "imgui_internal.h"  
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Window classes
#include "HelloWindow.h"
#include "AnotherWindow.h"
#include "VtkViewer1Window.h"
#include "VtkViewer2Window.h"
#include "ChartWindow.h"
#include "TableWindow.h"

// VTK
#include <vtkSmartPointer.h>
#include <vtkActor.h>

// File-Specific Includes
#include "imgui_vtk_demo.h"

// remove console for release build
#if defined(_WIN32) && defined(NDEBUG)
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

static void glfw_error_callback(int error, const char* description)
{
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char* argv[])
{
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()){
    return 1;
  }

  // Use GL 3.2 (All Platforms)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Decide GLSL version
#ifdef __APPLE__
  const char* glsl_version = "#version 150";
#else
  const char* glsl_version = "#version 130";
#endif

  // Create window with graphics context
  GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui VTKViewer Example", NULL, NULL);
  if (window == NULL){
    return 1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0); // Disable vsync

  // Initialize OpenGL loader
  if (gl3wInit() != 0){
    fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    return 1;
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 22.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // 完全独立的pipeline，避免共享导致的性能劣化
  auto actor1 = SetupDemoPipeline();
  auto actor2 = SetupDemoPipeline();

  // Initialize window objects
  HelloWindow helloWindow;
  AnotherWindow anotherWindow;
  VtkViewer1Window vtkViewer1Window;
  vtkViewer1Window.addActor(actor1);
  VtkViewer2Window vtkViewer2Window;
  vtkViewer2Window.addActor(actor2);
  ChartWindow chartWindow;
  TableWindow tableWindow;

  // Window state
  bool show_demo_window = true;
  bool show_another_window = false;
  bool vtk_1_open = true;
  bool vtk_2_open = true;
  bool chart_window_open = true;
  bool table_window_open = true;
  bool enable_vsync = false; // VSync开关
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Main loop
  int count = 0;
  while (!glfwWindowShouldClose(window) && ++count)
  {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create a fullscreen dockspace
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | 
                                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | 
                                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | 
                                     ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
    
    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(3);
    
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();
    
    // Render windows
    if (show_demo_window){
      ImGui::ShowDemoWindow(&show_demo_window);
    }

    helloWindow.render(show_demo_window, show_another_window, vtk_2_open, 
                       chart_window_open, table_window_open, clear_color);
    
    // VSync控制面板
    ImGui::Begin("Performance Settings");
    if (ImGui::Checkbox("Enable VSync", &enable_vsync)) {
      glfwSwapInterval(enable_vsync ? 1 : 0);
    }
    ImGui::Text("FPS: %.1f", io.Framerate);
    ImGui::Text("Frame Time: %.3f ms", 1000.0f / io.Framerate);
    ImGui::End();

    if (show_another_window){
      anotherWindow.render(show_another_window);
    }

    vtkViewer1Window.render(vtk_1_open);
    vtkViewer2Window.render(vtk_2_open);
    chartWindow.render(chart_window_open, count);
    tableWindow.render(table_window_open);

    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  // Cleanup
  vtkViewer1Window.cleanup();
  vtkViewer2Window.cleanup();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
