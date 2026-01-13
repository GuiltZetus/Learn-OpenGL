#include <engine/VBO.h>

BufferObject::BufferObject() { glGenBuffers(1, &ID); }

void BufferObject::bind() { glBindBuffer(target(), ID); }

void BufferObject::setData(GLsizeiptr size, const void *data, GLenum usage) {
  bind();
  glBufferData(target(), size, data, usage);
}

GLenum VBO::target() const { return GL_ARRAY_BUFFER; };
GLenum EBO::target() const { return GL_ARRAY_BUFFER; };
