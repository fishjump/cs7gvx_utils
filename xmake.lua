set_languages("c17", "cxx17")
set_warnings("all", "error")

add_includedirs(
  "/opt/homebrew/Cellar/glm/0.9.9.8/include",
  "/opt/homebrew/Cellar/glfw/3.3.6/include",
  "/opt/homebrew/Cellar/boost/1.76.0/include",
  "/opt/homebrew/Cellar/assimp/5.2.0/include",
  "/opt/homebrew/Cellar/freetype/2.11.1/include/freetype2"
)

add_linkdirs(
  "/opt/homebrew/Cellar/glm/0.9.9.8/lib",
  "/opt/homebrew/Cellar/glfw/3.3.6/lib",
  "/opt/homebrew/Cellar/boost/1.76.0/lib",
  "/opt/homebrew/Cellar/assimp/5.2.0/lib",
  "/opt/homebrew/Cellar/freetype/2.11.1/lib"
)

add_frameworks("OpenGL", "GLUT", "Cocoa")

target("cs7gvx-utils")
  set_kind("static")
  add_includedirs("include")
  add_files("src/**.cpp")
  add_links("assimp", "glfw", "freetype")