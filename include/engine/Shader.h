#ifndef SHADER_CLASS_H

#include <glad.h>

#include <string>

class Shader {
public:
  GLuint ID;

  Shader(const char *vertexPath, const char *fragmentPath);

  void use();
  void setBool(const std::string &name, GLboolean value) const;
  void setInt(const std::string &name, GLint value) const;
  void setFloat(const std::string &name, GLfloat value) const;

private:
  void checkCompileError(GLuint shader, std::string type);
};

#endif
