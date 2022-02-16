#include "figine/core/shader.hpp"
#include "shader_utils.hpp"

namespace figine::core {

std::string load_shader_code(const std::string &file) {
  std::ifstream fs;

  fs.open(file);
  if (!fs.is_open()) {
    LOG_ERR("fail to open file: %s", file.c_str());
  }
  defer(fs.close());

  std::stringstream ss;
  while (!fs.eof()) {
    std::string line;
    std::getline(fs, line);
    ss << line << "\n";
  }

  return ss.str();
};

shader_if::shader_if(const uint8_t *vert_code, const uint8_t *frag_code)
    : _vert_code(vert_code), _frag_code(frag_code) {}

void shader_if::use() const { glUseProgram(_program_id); }

GLuint shader_if::program_id() const { return _program_id; }

void shader_if::build() const {
  _program_id = create_program();
  if (_program_id == 0) {
    LOG_ERR("failed to create program");
    return;
  }

  if (!compile(_program_id, _vert_code, _frag_code)) {
    LOG_ERR("failed to compile shader");
    return;
  }

  if (!link(_program_id)) {
    LOG_ERR("failed to link program");
    return;
  }
}

} // namespace figine::core