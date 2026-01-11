#include <engine/VBO.h>

#include <iostream>

VBO::VBO()
{
    glGenBuffers(1, &ID);
}

VBO::~VBO()
{
    glDeleteBuffers(1, &ID);
}

void VBO::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::setData(GLsizeiptr size, const void *data, GLenum usage)
{
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}
