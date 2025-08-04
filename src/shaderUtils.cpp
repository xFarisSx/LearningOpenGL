#include "shaderUtils.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

std::string readFile(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Failed to open shader file: " << path << std::endl;
    return "";
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}
