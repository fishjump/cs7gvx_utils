#include <imnotgui/imnotgui.hpp>

#include <memory>
#include <unordered_set>

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

namespace {

std::unordered_set<cs7gvx_utils::imnotgui::window_t *> windows;

}

cs7gvx_utils::imnotgui::window_t::window_t() { register_window(this); }

void cs7gvx_utils::imnotgui::init(GLFWwindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
}

void cs7gvx_utils::imnotgui::render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  for (window_t *win : windows) {
    win->refresh();
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void cs7gvx_utils::imnotgui::register_window(
    cs7gvx_utils::imnotgui::window_t *window) {
  windows.insert(window);
}