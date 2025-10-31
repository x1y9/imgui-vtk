// Standard Library
#include <iostream>
#include <vector>
#include <algorithm>

// OpenGL Loader
// This can be replaced with another loader, e.g. glad, but
// remember to also change the corresponding initialize call!
#include <GL/gl3w.h>            // GL3w, initialized with gl3wInit() below

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

// ImGui + imgui-vtk, some flags need internal.h
#include "imgui.h"
#include "imgui_internal.h"  
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "VtkViewer.h"

// remove console for release build
#if defined(_WIN32) && defined(NDEBUG)
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

// VTK
#include <vtkSmartPointer.h>
#include <vtkActor.h>

// File-Specific Includes
#include "imgui_vtk_demo.h" // Actor generator for this demo

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
  // GLSL 150
  const char* glsl_version = "#version 150";
#else
  // GLSL 130
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
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 22.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Initialize VtkViewer objects
  auto actor = SetupDemoPipeline();
  VtkViewer vtkViewer1;
  vtkViewer1.addActor(actor);

  VtkViewer vtkViewer2;
  vtkViewer2.getRenderer()->SetBackground(0, 0, 0); // Black background
  vtkViewer2.addActor(actor);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  bool vtk_1_open = true;
  bool vtk_2_open = true;
  bool chart_window_open = true;
  bool table_window_open = true;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Chart data for performance testing
  const int num_charts = 10;
  const int data_points = 10000;
  static float chart_data[num_charts][data_points];
  
  // Initialize chart data with sine waves
  for (int chart = 0; chart < num_charts; chart++) {
    for (int i = 0; i < data_points; i++) {
      float phase = (float)chart * 0.5f;
      chart_data[chart][i] = sinf(i * 0.05f + phase) * (chart + 1) * 10.0f;
    }
  }

  // Table data - 10000 rows, 4 columns
  struct TableRow {
    int id;
    char name[32];
    float value;
    int status;
  };
  
  static std::vector<TableRow> table_data;
  table_data.reserve(10000);
  for (int i = 0; i < 10000; i++) {
    TableRow row;
    row.id = i + 1;
    sprintf(row.name, "Project %d", i + 1);
    row.value = (float)(rand() % 10000) / 100.0f;
    row.status = rand() % 5;
    table_data.push_back(row);
  }
  
  // Sorting state
  static int sort_column = 0;
  static bool sort_ascending = true;

  // Main loop
  int count = 0;
  while (!glfwWindowShouldClose(window) && ++count)
  {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
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
    
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window){
      ImGui::ShowDemoWindow(&show_demo_window);
    }

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
      ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
      ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
      ImGui::Checkbox("Another Window", &show_another_window);
      ImGui::Checkbox("VTK Viewer #2", &vtk_2_open);
      ImGui::Checkbox("Chart Window", &chart_window_open);
      ImGui::Checkbox("Table Window", &table_window_open);

      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

      if (ImGui::Button("Button")){                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
      }
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::End();

    // 3. Show another simple window.
    if (show_another_window){
      ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me")){
        show_another_window = false;
      }
      ImGui::End();
    }

    // Show a simple VtkViewer Instance and disable docking to this window
    ImGui::SetNextWindowSize(ImVec2(360, 240), ImGuiCond_FirstUseEver);
    ImGuiWindowClass vtkViewClass;
    vtkViewClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingOverMe;
    ImGui::SetNextWindowClass(&vtkViewClass);
    if (vtk_1_open) {
        ImGui::Begin("Vtk Viewer 1", &vtk_1_open, VtkViewer::NoScrollFlags());
        vtkViewer1.render(); // default render size = ImGui::GetContentRegionAvail()
        ImGui::End();
    }
    
    // 5. Show a more complex VtkViewer Instance (Closable, Widgets in Window)
    ImGui::SetNextWindowSize(ImVec2(720, 480), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowClass(&vtkViewClass);
    if (vtk_2_open){
      ImGui::Begin("Vtk Viewer 2", &vtk_2_open, VtkViewer::NoScrollFlags());

      // Other widgets can be placed in the same window as the VTKViewer
      // However, since the VTKViewer is rendered to size ImGui::GetContentRegionAvail(),
      // it is best to put all widgets first (i.e., render the VTKViewer last).
      // If you want the VTKViewer to be at the top of a window, you can manually calculate
      // and define its size, accounting for the space taken up by other widgets

      auto renderer = vtkViewer2.getRenderer();
      if (ImGui::Button("VTK Background: Black")){
        renderer->SetBackground(0, 0, 0);
      }
      ImGui::SameLine();
      if (ImGui::Button("VTK Background: Red")){
        renderer->SetBackground(1, 0, 0);
      }
      ImGui::SameLine();
      if (ImGui::Button("VTK Background: Green")){
        renderer->SetBackground(0, 1, 0);
      }
      ImGui::SameLine();
      if (ImGui::Button("VTK Background: Blue")){
        renderer->SetBackground(0, 0, 1);
      }
      static float vtk2BkgAlpha = 0.2f;
      ImGui::SliderFloat("Background Alpha", &vtk2BkgAlpha, 0.0f, 1.0f);
      renderer->SetBackgroundAlpha(vtk2BkgAlpha);

      vtkViewer2.render();
      ImGui::End();
    }

    // 6. Show Chart Window for Performance Testing
    if (chart_window_open) {
      ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
      ImGui::Begin("chart", &chart_window_open);      
      ImGui::Text("Lines:%d,%d points per line", num_charts, data_points);
      ImGui::Separator();
      
      // Draw multiple charts
      ImVec2 chart_size = ImVec2(ImGui::GetContentRegionAvail().x, 150);
      for (int chart = 0; chart < num_charts; chart++) {
        char label[64];
        sprintf(label, "chart #%d", chart + 1);
        
        float min_val = -(chart + 1) * 10.0f - 5.0f;
        float max_val = (chart + 1) * 10.0f + 5.0f;
        
        ImGui::PlotLines(label, chart_data[chart], data_points, count, NULL, 
                        min_val, max_val, chart_size);
      }
      
      ImGui::End();
    }

    // 7. Show Table Window with 10000 rows and 4 columns
    if (table_window_open) {
      ImGui::SetNextWindowSize(ImVec2(900, 600), ImGuiCond_FirstUseEver);
      ImGui::Begin("Grid", &table_window_open);
      
      ImGui::Text("Lines: %d", (int)table_data.size());
      ImGui::Separator();
      
      // Table with sorting and reorderable columns
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
        ImGui::TableSetupScrollFreeze(0, 1); // Freeze header row
        ImGui::TableHeadersRow();
        
        // Handle sorting
        if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs()) {
          if (sort_specs->SpecsDirty) {
            if (sort_specs->SpecsCount > 0) {
              const ImGuiTableColumnSortSpecs& spec = sort_specs->Specs[0];
              sort_column = spec.ColumnUserID;
              sort_ascending = (spec.SortDirection == ImGuiSortDirection_Ascending);
              
              // Sort the data
              std::sort(table_data.begin(), table_data.end(), [](const TableRow& a, const TableRow& b) {
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
            
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", table_data[row].id);
            
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", table_data[row].name);
            
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.2f", table_data[row].value);
            
            ImGui::TableSetColumnIndex(3);
            const char* status_text[] = {"待处理", "进行中", "已完成", "已取消", "错误"};
            ImGui::Text("%s", status_text[table_data[row].status]);
          }
        }
        
        ImGui::EndTable();
      }
      
      ImGui::End();
    }

    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  // Cleanup VTK viewers first
  vtkViewer1.cleanup();
  vtkViewer2.cleanup();

  // Then cleanup ImGui and GLFW
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
