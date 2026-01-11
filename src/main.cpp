#include <glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <engine/Shader.h>
#include <engine/VAO.h>
#include <engine/VBO.h>

// gl math include
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/trigonometric.hpp>
#include <iostream>

#include <tools/ImageLoader.h>

// settings for window size
const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;

// callback for changing viewport size after init
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// input cotrol for glfw
void processInput(GLFWwindow *window, Shader object) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    object.setFloat("MixRate", 0.1);
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    object.setFloat("MixRate", 0.4);
  }
}

int main() {
  // init the glfw window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // GLFW window creation
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGLWindow", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  // make glfw window the main context of the current thread
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

  // start root location debug
  // std::cout << std::filesystem::current_path() << std::endl;

  // build and compile our shader program
  Shader basicShaderProgram("../assets/shaders/basicShader.vert",
                            "../assets/shaders/orangeShader.frag");

  // SETUP VERTICES AND BUFFER
  //  vertex data
  float vertices[] = {
      // positions          // colors           // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };

  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };
  // 0. vertex buffer object and vertex array object creation
  VAO VAOs[3];
  VBO simpleObject;
  unsigned int EBO;

  glGenBuffers(1, &EBO);

  // 2. copy of vertices array in a buffer
  simpleObject.setData(sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 3. LINKING VERTEX ATTRIBUTE
  // position attribute
  VAOs[0].setAttribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)0);
  // color attribute
  VAOs[0].setAttribute(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  // texture attribute
  VAOs[0].setAttribute(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

  VAOs[0].bind();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // generating a texture
  unsigned int texture1, texture2;
  glGenTextures(1, &texture1);
  // bind texture (work like any other object)
  glBindTexture(GL_TEXTURE_2D, texture1);
  // 1t. set texture wrapping
  // in texture coords x,y,z get replace by s,t,r
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // 2t. set texture filtering options
  // can be set for magnifying and minifying operations (scaling up or down)
  // using nearest neighbor filtering when texture are downscaled
  // and linear filtering for upscaled textured
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // downscaled

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // upscaled

  // set mipmap options
  // set it to linear mipmap linear filterring
  // this will override the first min filter call
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

  // load texture image with stb image
  stbi_set_flip_vertically_on_load(true); // reverse all the image
  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("../assets/textures/container.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "failed to load texture" << std::endl;
  }

  // good practices to free the image memory after we are done
  stbi_image_free(data);

  // 2nd texture
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

  data = stbi_load("../assets/textures/awesomeface.png", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "failed to load texture" << std::endl;
  }

  stbi_image_free(data);

  basicShaderProgram.use();
  basicShaderProgram.setInt("texture1", 0);
  basicShaderProgram.setInt("texture2", 1);

  // vec = trans * vec;
  // std::cout << vec.x << vec.y << vec.z << std::endl;

  // RENDERER LOOP
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window, basicShaderProgram);

    // rendering command
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // transformation
    glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

    basicShaderProgram.use();
    unsigned int tranformLoc = glGetUniformLocation(basicShaderProgram.ID, "transform");
    glUniformMatrix4fv(tranformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    // 4. use the shader program and draw the triangle

    VAOs[0].bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    trans = glm::mat4(1.0f);
    float ScaleAmount = (float)sin(glfwGetTime());
    trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
    trans = glm::scale(trans, glm::vec3(ScaleAmount, ScaleAmount, ScaleAmount));

    glUniformMatrix4fv(tranformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // glBindVertexArray(VAOs[1]);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // check and call events and swap the buffer
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // terminate the window
  std::cout << "Window is closing" << std::endl;
  glfwTerminate();
  return 0;
}
