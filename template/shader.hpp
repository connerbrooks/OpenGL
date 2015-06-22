#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
  GLuint Program;
  
  Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
  {
    // get shader source by path
    std::string vertexCode;
    std::string fragmentCode;
    try
    {

    }
    catch (std::exception e)
    {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();

    // compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertex, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER); 
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // print errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertex, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);

    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(this->Program, GL_LINK_STATUS, &success);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // Delete shaders 
    glDeleteShader(vertex);
    glDeleteShader(fragment);

  }

  void
  Use()
  {
    glUseProgram(this->Program);
  }
}

#endif
