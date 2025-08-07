#include <cmath>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "shaderClass.h"

GLfloat vertices[] = {
    // Positions                               // Colors
    -0.5f,     -0.5f * float(sqrt(3.0f)) / 3.0f,       0.0f, 0.8f, 0.3f,  0.02f,
    0.5f,      -0.5f * float(sqrt(3.0f)) / 3.0f,       0.0f, 0.8f, 0.3f,  0.02f,

    0.0f,      0.5f * float(sqrt(3.0f)) * 2.0f / 3.0f, 0.0f, 1.0f, 0.6f,  0.32f,

    -0.5f / 2, 0.5f * float(sqrt(3.0f)) / 6.0f,        0.0f, 0.9f, 0.45f, 0.17f,

    0.5f / 2,  0.5f * float(sqrt(3.0f)) / 6.0f,        0.0f, 0.9f, 0.45f, 0.17f,

    0.0f,      -0.5f * float(sqrt(3.0f)) / 3.0f,       0.0f, 0.8f, 0.3f,  0.02f,
};

GLuint indices[] = {0, 3, 5, 3, 2, 4, 5, 4, 1};

int main() {

  // Init GLFW
  glfwInit();

  // Tell GLFW what version of OpenGL we are using (3.3)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Tell GLFW we are using the CORE profile (Only modern functions)
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  // Specify the viewport of OpenGL in the Window
  glViewport(0, 0, width, height);

  Shader shaderProgram("../shaders/vertexShader.vert",
                       "../shaders/fragmentShader.frag");

  VAO VAO1;
  VAO1.Bind();

  VBO VBO1(vertices, sizeof(vertices));
  EBO EBO1(indices, sizeof(indices));

  VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void *)0);
  VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float),
                  (void *)(3 * sizeof(float)));

  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();

  GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

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

    shaderProgram.Activate();
    glUniform1f(uniID, 0.f);

    VAO1.Bind();
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
  }

  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();
  shaderProgram.Delete();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
