set_languages("c17", "cxx17")
set_warnings("all", "error")

add_includedirs(
    "/opt/homebrew/include"
)

add_linkdirs("/opt/homebrew/lib")

add_frameworks("OpenGL", "Cocoa")

target("figine")
  set_kind("static")
  add_includedirs("include")
  add_files("src/**.cpp", "src/**.c")
  add_links("assimp", "glfw", "freetype")

target("test")
  set_kind("binary")
  add_deps("figine");
  add_includedirs("include")
  add_files("test/**.cpp")
  add_links("figine", "assimp", "glfw", "freetype")