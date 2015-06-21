#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);		
}

// shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 position;\n"
  "layout (location = 1) in vec3 color;\n"
  "out vec3 ourColor;\n"
  "void main()\n"
  "{\n"
  "gl_Position = vec4(position, 1.0);\n"
  "ourColor = color;\n"
  "}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
  "in vec4 vertexColor;\n"
  "in vec3 ourColor;\n"
  "out vec4 color;\n"
  "void main()\n"
  "{\n"
  "color = vec4(ourColor, 1.0f);\n"
  "}\0";

const GLchar* fragmentShaderSourcePink = "#version 330 core\n"
  "out vec4 color;\n"
  "void main()\n"
  "{\n"
  "color = vec4(1.0f, 0.2f, 0.6f, 1.0f);\n"
  "}\0";



int
main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "Getting Context", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "failed to create glfw window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // setup callback
  glfwSetKeyCallback(window, key_callback);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "failed to initialize glew" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);



  // vars for checks
  GLint success;
  GLchar infoLog[512];

  // shaders
  // vertex shader
  GLuint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // Fragment shader
  GLuint fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  GLuint fragmentShaderPink;
  fragmentShaderPink = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderPink, 1, &fragmentShaderSourcePink, NULL);
  glCompileShader(fragmentShaderPink);
  glGetShaderiv(fragmentShaderPink, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShaderPink, 512, NULL, infoLog);
    std::cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }


  GLuint shaderProgram, shaderProgramPink;
  shaderProgram = glCreateProgram();
  shaderProgramPink = glCreateProgram();

  // shaderProgram
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR:SHADERS::LINKING_FAILED\n" << infoLog << std::endl;
  }

  //pink
  glAttachShader(shaderProgramPink, vertexShader);
  glAttachShader(shaderProgramPink, fragmentShaderPink);
  glLinkProgram(shaderProgramPink);
  glGetProgramiv(shaderProgramPink, GL_LINK_STATUS, &success);


  // cleanup
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(fragmentShaderPink);

  // Now that we shadered lets do this
//  GLfloat vertices[] = {
//     0.5f,  0.5f, 0.0f,
//     0.5f, -0.5f, 0.0f,
//    -0.5f, -0.5f, 0.0f,
//    -0.5f,  0.5f, 0.0f,
//  };

  GLfloat vertices0[] = {
    // First triangle
    -0.9f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Left 
    -0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Right
    -0.45f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // Top 
     };  

  GLfloat vertices1[] = {
     // Second triangle
     0.0f, -0.5f, 0.0f,  // Left
     0.9f, -0.5f, 0.0f,  // Right
     0.45f, 0.5f, 0.0f   // Top 
  };

  GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
  };

  // setup element buffer object
//  GLuint EBO;
//  glGenBuffers(1, &EBO);
  
  GLuint VBO0, VAO0;
  glGenBuffers(1, &VBO0);
  glGenVertexArrays(1, &VAO0);

  GLuint VBO1, VAO1;
  glGenBuffers(1, &VBO1);
  glGenVertexArrays(1, &VAO1);

  // setup triangle 0 
  glBindVertexArray(VAO0);
    // copy vertices array to a gl buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);
    // set vertex attr pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat)*3));
    glEnableVertexAttribArray(1);
  // unbind vao
  glBindVertexArray(0);

  // setup triangle 1
  glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // draw pretty color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // render stuff :D
    // triangle 0 

    glUseProgram(shaderProgram);

    glBindVertexArray(VAO0);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glUseProgram(shaderProgramPink);

    glBindVertexArray(VAO1);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);


    glfwSwapBuffers(window);
  }


  // cleanup
  glfwTerminate();
  return 0;
}
