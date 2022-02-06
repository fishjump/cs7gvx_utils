#pragma once

#include <map>
#include <memory>

#include <gl.hpp>

namespace cs7gvx_utils::freetype_gl {

struct character_t {
  GLuint texture_id;
  glm::ivec2 size;
  glm::ivec2 bearing;
  GLuint advance;
};

using font_t = std::map<GLchar, character_t>;

void init(gl::shader_t &shader, const size_t win_width,
          const size_t win_height);

std::shared_ptr<font_t> load_font(const std::string &path);
void print(const std::shared_ptr<font_t> font_ptr,cs7gvx_utils::gl::shader_t &shader,
           const std::string &str, float x, float y, float scale,
           const glm::vec3 &color);

} // namespace cs7gvx_utils::freetype_gl
