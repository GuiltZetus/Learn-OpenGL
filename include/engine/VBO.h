#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad.h>
#include <string>
#include <iostream>

class VBO
{

public:
    VBO();
    ~VBO();

    void bind();
    void setData(GLsizeiptr size, const void *data, GLenum usage);

private:
    GLuint ID;
};

#endif