#pragma once

#include "shader_profile.hpp"

#include <array>
#include <functional>
#include <memory>
#include <vector>

#include <common.hpp>

namespace cs7gvx_utils::gl {

struct shader_id_t final {
  GLuint vert_id;
  GLuint frag_id;
};

class shader_t final {
public:
  shader_t(const std::string &vert_glsl, const std::string &frag_glsl,
           std::shared_ptr<shader_profile_t> profile = nullptr);

  void use();
  void set_profile();

  cs7gvx_utils::common::result_t<GLuint> build();

  std::shared_ptr<shader_profile_t> profile();
  GLuint program_id() const;
  GLuint vao() const;

  template <class T>
  void set_uniform(const std::string &name, const T &t) const;

private:
  cs7gvx_utils::common::result_t<GLuint> create();
  cs7gvx_utils::common::result_t<> compile();
  cs7gvx_utils::common::result_t<> link();
  cs7gvx_utils::common::result_t<> validate() const;

  const std::string _vert_glsl;
  const std::string _frag_glsl;
  std::shared_ptr<shader_profile_t> _profile;
  GLuint _program_id = 0;
};

} // namespace cs7gvx_utils::gl

template <class T>
void cs7gvx_utils::gl::shader_t::set_uniform(const std::string &name, const T &t) const {
  if constexpr (std::is_integral_v<T> || std::is_same_v<T, bool>) {
    glUniform1i(glGetUniformLocation(_program_id, name.c_str()), (int)t);
  } else if constexpr (std::is_same_v<T, float_t>) {
    glUniform1f(glGetUniformLocation(_program_id, name.c_str()), (float_t)t);
  } else if constexpr (cs7gvx_utils::common::is_glm_vec_v<T, 2>) {
    glUniform2fv(glGetUniformLocation(_program_id, name.c_str()), 1, &t[0]);
  } else if constexpr (cs7gvx_utils::common::is_glm_vec_v<T, 3>) {
    glUniform3fv(glGetUniformLocation(_program_id, name.c_str()), 1, &t[0]);
  } else if constexpr (cs7gvx_utils::common::is_glm_vec_v<T, 4>) {
    glUniform4fv(glGetUniformLocation(_program_id, name.c_str()), 1, &t[0]);
  } else if constexpr (cs7gvx_utils::common::is_glm_mat_v<T, 2>) {
    glUniformMatrix2fv(glGetUniformLocation(_program_id, name.c_str()), 1,
                       GL_FALSE, &t[0][0]);
  } else if constexpr (cs7gvx_utils::common::is_glm_mat_v<T, 3>) {
    glUniformMatrix3fv(glGetUniformLocation(_program_id, name.c_str()), 1,
                       GL_FALSE, &t[0][0]);
  } else if constexpr (cs7gvx_utils::common::is_glm_mat_v<T, 4>) {
    glUniformMatrix4fv(glGetUniformLocation(_program_id, name.c_str()), 1,
                       GL_FALSE, &t[0][0]);
  } else {
    static_assert(cs7gvx_utils::common::dependent_false_v<T>,
                  "unsupported type");
  }
}
