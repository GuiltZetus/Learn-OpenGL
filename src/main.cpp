#include <glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <engine/Shader.h>
#include <engine/VAO.h>
#include <engine/VBO.h>

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

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// default yaw/pitch angle for looking around with camera
float yaw = -90.0f;
float pitch = 0.0f;

// delta time default
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// default last X,Y coords for camera rotation (camera start at center)
float lastX = (float)SCR_WIDTH / 2;
float lastY = (float)SCR_HEIGHT / 2;

// check for fixing sudden camera jump from cursor entering Window
bool firstMouse = true;

// callback for changing viewport size after init
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, Shader object);
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
  VAO VAOs[3];
  VBO simpleObject;
  unsigned int EBO;

  glGenBuffers(1, &EBO);

  // 2. copy of vertices array in a buffer
  simpleObject.setData(sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 3. LINKING VERTEX ATTRIBUTE
  // position attribute
  VAOs[0].setAttribute(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
  // texture attribute
  VAOs[0].setAttribute(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

  VAOs[0].bind();

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

  // set world coordiantes for all model
  glm::mat4 base = glm::mat4(1.0f);
  // model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  // view camera translate
  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  // perspectiv projection
  glm::mat4 projection;
  projection =
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

    // bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    basicShaderProgram.use();

    // calculate delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glm::mat4 view;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    VAOs[0].bind();
    int modelLoc = glGetUniformLocation(basicShaderProgram.ID, "model");
    int projectionLoc = glGetUniformLocation(basicShaderProgram.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    int viewLoc = glGetUniformLocation(basicShaderProgram.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // 4. use the shader program and draw the triangle

    // draw the cube
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
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      glDrawArrays(GL_TRIANGLES, 0, 36);
    };

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

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// input cotrol for glfw
void processInput(GLFWwindow *window, Shader object) {
  float cameraSpeed = 2.5f * deltaTime;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    object.setFloat("MixRate", 0.1);
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    object.setFloat("MixRate", 0.4);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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

  const float mouse_sensitivity = 0.1f;
  xoffset *= mouse_sensitivity;
  yoffset *= mouse_sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  // look around matrix
  glm::vec3 lookDirection;
  lookDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  lookDirection.y = sin(glm::radians(pitch));
  lookDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  cameraFront = glm::normalize(lookDirection);
}
