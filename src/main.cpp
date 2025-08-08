#include <cmath>
#include <glad/glad.h>

#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "shaderClass.h"
#include "textureClass.h"

// GLfloat vertices[] = {
//     // Positions                               // Colors
//     -0.5f,     -0.5f * float(sqrt(3.0f)) / 3.0f,       0.0f, 0.8f, 0.3f,
//     0.02f, 0.5f,      -0.5f * float(sqrt(3.0f)) / 3.0f,       0.0f, 0.8f,
//     0.3f,  0.02f,
//
//     0.0f,      0.5f * float(sqrt(3.0f)) * 2.0f / 3.0f, 0.0f, 1.0f, 0.6f,
//     0.32f,
//
//     -0.5f / 2, 0.5f * float(sqrt(3.0f)) / 6.0f,        0.0f, 0.9f, 0.45f,
//     0.17f,
//
//     0.5f / 2,  0.5f * float(sqrt(3.0f)) / 6.0f,        0.0f, 0.9f, 0.45f,
//     0.17f,
//
//     0.0f,      -0.5f * float(sqrt(3.0f)) / 3.0f,       0.0f, 0.8f, 0.3f,
//     0.02f,
// };

GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

    0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

    0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

};

GLuint indices[] = {
    0, 2, 1, 0, 3, 2,
};

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

  Shader shaderProgram("../shaders/vertexShader.vert",
                       "../shaders/fragmentShader.frag");

  VAO VAO1;
  VAO1.Bind();

  VBO VBO1(vertices, sizeof(vertices));
  EBO EBO1(indices, sizeof(indices));

  VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
  VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float),
                  (void *)(3 * sizeof(float)));
  VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float),
                  (void *)(6 * sizeof(float)));

  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();

  GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");


  Texture cat("../assets/textures/cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
  cat.texUnit(shaderProgram,"tex0", 0); 

  // Main while loop
  while (!glfwWindowShouldClose(window)) {
    // GLFW events
    glfwPollEvents();

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.Activate();
    glUniform1f(uniID, 0.f);
    cat.Bind();

    VAO1.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
  }

  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();
  cat.Delete();
  shaderProgram.Delete();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
