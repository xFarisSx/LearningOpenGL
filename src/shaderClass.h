#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

std::string getFileContents(const char *path);

class Shader {
public:
  GLuint ID;
  Shader(const char *vertexFile, const char *fragmentFile);

  void Activate();
  void Delete();
};

#endif
