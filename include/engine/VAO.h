#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad.h>

class VAO {
public:
  VAO();
  ~VAO();

  void bind();
  void unbind();
  void setAttribute(GLint index, GLint size, GLenum type, GLboolean normalized,
                    GLsizei stride, const void *pointerOffset);
  void enable(GLint index);

private:
  GLuint ID;
};

#endif
