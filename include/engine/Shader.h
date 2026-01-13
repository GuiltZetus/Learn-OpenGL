#include <glm/fwd.hpp>
#ifndef SHADER_CLASS_H

#include <glad.h>

#include <string>
#include <glm/glm.hpp>

class Shader {
public:
  GLuint ID;

  Shader(const char *vertexPath, const char *fragmentPath);

  void use();
  void setBool(const std::string &name, GLboolean value) const;
  void setInt(const std::string &name, GLint value) const;
  void setFloat(const std::string &name, GLfloat value) const;
  void setUniform4fv(const std::string &name, glm::mat4 vectorMatrix) const;
  void setVec3(const std::string &name, glm::vec3 vectorValue) const;

private:
  void checkCompileError(GLuint shader, std::string type);
};

#endif
