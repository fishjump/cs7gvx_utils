#pragma once

#include "figine/dep.hpp"
#include "figine/logging/log.hpp"

#define SHADER_TYPE_STR(shader_type)                                           \
  ({                                                                           \
    std::string str;                                                           \
    switch (shader_type) {                                                     \
    case GL_VERTEX_SHADER:                                                     \
      str = "GL_VERTEX_SHADER";                                                \
      break;                                                                   \
    case GL_FRAGMENT_SHADER:                                                   \
      str = "GL_FRAGMENT_SHADER";                                              \
    }                                                                          \
    str;                                                                       \
  }).c_str()

namespace {

inline GLuint create_program() {
  GLuint program_id = glCreateProgram();
  if (program_id == 0) {
    LOG_ERR("create program failed");
  }

  return program_id;
}

inline GLuint compile_shader(const u_char *glsl_code,
                             const GLenum shader_type) {

  GLuint shader_id = glCreateShader(shader_type);
  if (shader_id == 0) {
    LOG_ERR("error creating shader type: %s", SHADER_TYPE_STR(shader_type));
    return 0;
  }

  glShaderSource(shader_id, 1, (const GLchar **)&glsl_code, NULL);
  glCompileShader(shader_id);

  GLint success;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLchar log[1024];
    glGetShaderInfoLog(shader_id, 1024, NULL, log);
    LOG_ERR("error compiling shader type: %s, log: %s",
            SHADER_TYPE_STR(shader_type), log);
    return 0;
  }

  return shader_id;
}

inline bool compile(GLuint program_id, const u_char *vert_code,
                    const u_char *frag_code) {
  auto res = compile_shader(vert_code, GL_VERTEX_SHADER);
  if (res == 0) {
    return false;
  }
  GLuint vert_id = res;

  res = compile_shader(frag_code, GL_FRAGMENT_SHADER);
  if (res == 0) {
    return false;
  }
  GLuint frag_id = res;

  glAttachShader(program_id, vert_id);
  glAttachShader(program_id, frag_id);

  glDeleteShader(vert_id);
  glDeleteShader(frag_id);

  return true;
}

inline bool link(GLuint program_id) {
  glLinkProgram(program_id);

  GLint success = 0;
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);
  if (!success) {
    GLchar log[1024] = {'\0'};
    glGetProgramInfoLog(program_id, sizeof(log), NULL, log);
    LOG_ERR("error linking shader program: %s", log);
    return false;
  }

  return true;
}

} // namespace
