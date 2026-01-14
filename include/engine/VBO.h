#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad.h>

class BufferObject {
public:
  BufferObject();
  virtual ~BufferObject() = default;

  void bind();
  void setData(GLsizeiptr size, const void *data, GLenum usage);

  virtual GLenum target() const = 0;

private:
  unsigned int ID;
};

class VBO final : public BufferObject {
public:
  GLenum target() const;
};

class EBO final : public BufferObject {
public:
  GLenum target() const;
};

#endif
