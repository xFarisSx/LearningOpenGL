#include "shaderUtils.hpp"

#include <cmath>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>

int main() {

  // Init GLFW
  glfwInit();

  // Tell GLFW what version of OpenGL we are using (3.3)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Tell GLFW we are using the CORE profile (Only modern functions)
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLfloat vertices[] = {-0.5f, -0.5f * float(sqrt(3.0f)) / 3.0f,       0.0f,
                        0.5f,  -0.5f * float(sqrt(3.0f)) / 3.0f,       0.0f,
                        0.0f,  0.5f * float(sqrt(3.0f)) * 2.0f / 3.0f, 0.0f};

  // Create GLFWwindow object of 800x800 pixels
  GLFWwindow *window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
  // Error check if the window fails to create
  if (window == NULL) {
    std::cout << "Failed to create window" << std::endl;
    glfwTerminate();
    return -1;
  }
  // Introduce the window into the current context
  glfwMakeContextCurrent(window);

  // Load GLAD so it configures OpenGL
  gladLoadGL();

  // Specify the viewport of OpenGL in the Window
  glViewport(0, 0, 800, 800);

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  std::string vertexCode = readFile("../shaders/vertexShader.vert");
  const char *vertexSource = vertexCode.c_str();
  glShaderSource(vertexShader, 1, &vertexSource, nullptr);
  glCompileShader(vertexShader);

  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  std::string fragCode = readFile("../shaders/fragmentShader.frag");
  const char *fragSource = fragCode.c_str();
  glShaderSource(fragShader, 1, &fragSource, nullptr);
  glCompileShader(fragShader);

  GLuint shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragShader);

  GLuint VAO, VBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Specify color of background
  glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
  // Clean the back buffer
  glClear(GL_COLOR_BUFFER_BIT);
  // Swap the back buffer with front buffer
  glfwSwapBuffers(window);

  // Main while loop
  while (!glfwWindowShouldClose(window)) {
    // GLFW events
    glfwPollEvents();

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
