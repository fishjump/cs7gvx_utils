#include "figine/imnotgui/imnotgui.hpp"

#include <memory>
#include <unordered_set>

#include "imgui-1.86/backends/imgui_impl_glfw.h"
#include "imgui-1.86/backends/imgui_impl_opengl3.h"
#include "imgui-1.86/imgui.h"

namespace {

std::unordered_set<figine::imnotgui::window_t *> windows;

}

figine::imnotgui::window_t::window_t() {}

void figine::imnotgui::init(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
}

void figine::imnotgui::render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  for (window_t *win : windows) {
    win->refresh();
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void figine::imnotgui::register_window(figine::imnotgui::window_t *window) {
  windows.insert(window);
}