#include <engine/VAO.h>

VAO::VAO() { glGenVertexArrays(1, &ID); }

VAO::~VAO() { glDeleteVertexArrays(1, &ID); }

void VAO::bind() { glBindVertexArray(ID); }

void VAO::unbind() { glBindVertexArray(0); }

void VAO::setAttribute(GLint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                       const void *pointerOffset) {
  bind();
  glVertexAttribPointer(index, size, type, normalized, stride, pointerOffset);
  glEnableVertexAttribArray(index);
}

void VAO::enable(GLint index) { glEnableVertexAttribArray(index); }
