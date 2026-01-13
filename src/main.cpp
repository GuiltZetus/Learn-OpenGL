#include <glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <engine/Shader.h>
#include <engine/VAO.h>
#include <engine/VBO.h>
#include "engine/Camera.h"

// gl math include
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/trigonometric.hpp>
#include <iostream>
#include <cstdlib>

#include <tools/ImageLoader.h>

// GLOBAL VARIABLES
// settings for window size
const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;

// default last X,Y coords for camera rotation (camera start at center)
float lastX = (float)800 / 2;
float lastY = (float)600 / 2;

// Camera obj
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// delta time default
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// check for fixing sudden camera jump from cursor entering Window
bool firstMouse = true;

// callback for changing viewport size after init
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, Shader shader);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);

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
  // set mouse center locked and get mouse input
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);

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

  // shader setup
  basicShaderProgram.use();
  basicShaderProgram.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
  basicShaderProgram.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

  // SETUP VERTICES AND BUFFER
  //  vertex data
  float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
                      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

                      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
                      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

                      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
                      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
                      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                               glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                               glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                               glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                               glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
  // 0. vertex buffer object and vertex array object creation
  VAO cubeVAO, lightVAO;
  VBO simpleObject;

  simpleObject.setData(sizeof(vertices), vertices, GL_STATIC_DRAW);

  cubeVAO.bind();
  simpleObject.bind();
  cubeVAO.setAttribute(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);

  lightVAO.bind();
  simpleObject.bind();
  lightVAO.setAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);

  // perspectiv projection
  glm::mat4 projection =
      glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

  // enalbe depth test
  glEnable(GL_DEPTH_TEST);

  // RENDERER LOOP
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window, basicShaderProgram);

    // rendering command
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glm::mat4 view;
    view = glm::lookAt(camera.Pos, camera.Pos + camera.Front, camera.Up);

    basicShaderProgram.setUniform4fv("projection", projection);
    basicShaderProgram.setUniform4fv("view", view);

    // 4. use the shader program and draw the triangle

    // draw the cube

    cubeVAO.bind();

    for (unsigned int i = 0; i < 10; i++) {
      // set world coordiantes for all model
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);

      float angle = 20.0f * (i + 1);
      // make the cube rotate
      if (i == 1 || i % 3 == 0) {
        angle *= glfwGetTime();
      }
      model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
      basicShaderProgram.setUniform4fv("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    };

    // glBindVertexArray(cubeVAO[1]);
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// input cotrol for glfw
void processInput(GLFWwindow *window, Shader shader) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    shader.setFloat("MixRate", 0.1);
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    shader.setFloat("MixRate", 0.4);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.handleKeyboardInput(FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.handleKeyboardInput(BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.handleKeyboardInput(LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.handleKeyboardInput(RIGHT, deltaTime);
  }
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  if (firstMouse) // initially set to true
  {
    lastX = xposIn;
    lastY = yposIn;
    firstMouse = false;
  }
  float xoffset = xposIn - lastX;
  float yoffset = lastY - yposIn;

  lastX = xposIn;
  lastY = yposIn;
  camera.handleMouseInput(xoffset, yoffset);
}
