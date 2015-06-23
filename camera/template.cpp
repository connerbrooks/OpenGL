#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "camera.hpp"

// Prototype
void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

void
mouse_callback(GLFWwindow *window, double xpos, double ypos);

void
do_movement();

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 400;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Dimens
const GLfloat WIDTH = 800.0f, HEIGHT = 600.0f;


int
main()
{
  // Setup
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Getting Context", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "failed to create glfw window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // setup callback
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "failed to initialize glew" << std::endl;
    return -1;
  }

  glViewport(0, 0, WIDTH, HEIGHT);
  // Setup end

  glEnable(GL_DEPTH_TEST);

  // Setup shaders
  Shader shader("default.vs", "default.fs");

  GLfloat vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  glm::vec3 cubePositions[] = {
          glm::vec3( 0.0f,  0.0f,  0.0f),
          glm::vec3( 2.0f,  5.0f, -15.0f),
          glm::vec3(-1.5f, -2.2f, -2.5f),
          glm::vec3(-3.8f, -2.0f, -12.3f),
          glm::vec3( 2.4f, -0.4f, -3.5f),
          glm::vec3(-1.7f,  3.0f, -7.5f),
          glm::vec3( 1.3f, -2.0f, -2.5f),
          glm::vec3( 1.5f,  2.0f, -2.5f),
          glm::vec3( 1.5f,  0.2f, -1.5f),
          glm::vec3(-1.3f,  1.0f, -1.5f)
  };


  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  // Game loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    do_movement();

    // draw clear color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader.Use();

    glm::mat4 view, projection;

    view = camera.GetViewMatrix();
    projection = glm::perspective(45.0f, WIDTH / HEIGHT, 0.1f, 100.0f);

    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
    GLint viewLoc = glGetUniformLocation(shader.Program, "view");
    GLint projectionLoc = glGetUniformLocation(shader.Program, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    for (GLuint i = 0; i < 10; i++) {
      glm::mat4 model;
      model = glm::translate(model, cubePositions[i]);
      GLfloat angle = 20.0f * i;
      model = glm::rotate(model, (GLfloat)glfwGetTime() * angle / 20.0f, glm::vec3(1.0f, 0.3f, 0.5f));
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  // cleanup
  glfwTerminate();

  return 0;
}


void do_movement()
{
  if (keys[GLFW_KEY_W])
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (keys[GLFW_KEY_S])
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (keys[GLFW_KEY_A])
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (keys[GLFW_KEY_D])
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  //GLfloat cameraSpeed = 0.05f;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
  }
}

bool firstMouse = true;

void
mouse_callback(GLFWwindow *window, double xpos, double ypos)
{

  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  GLfloat xoffset = xpos - lastX;
  GLfloat yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}
