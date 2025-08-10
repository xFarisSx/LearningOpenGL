#include <cmath>
#include <glad/glad.h>

#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "camera.h"
#include "shaderClass.h"
#include "textureClass.h"

int width = 800;
int height = 800;

int swidth, sheight;

// Vertices coordinates
GLfloat vertices[] = {
    //     COORDINATES     /        COLORS          /    TexCoord   / NORMALS //
    -0.5f, 0.0f, 0.5f,  0.83f, 0.70f, 0.44f,
    0.0f,  0.0f, 0.0f,  -1.0f, 0.0f, // Bottom side
    -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f,
    0.0f,  5.0f, 0.0f,  -1.0f, 0.0f, // Bottom side
    0.5f,  0.0f, -0.5f, 0.83f, 0.70f, 0.44f,
    5.0f,  5.0f, 0.0f,  -1.0f, 0.0f, // Bottom side
    0.5f,  0.0f, 0.5f,  0.83f, 0.70f, 0.44f,
    5.0f,  0.0f, 0.0f,  -1.0f, 0.0f, // Bottom side

    -0.5f, 0.0f, 0.5f,  0.83f, 0.70f, 0.44f,
    0.0f,  0.0f, -0.8f, 0.5f,  0.0f, // Left Side
    -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f,
    5.0f,  0.0f, -0.8f, 0.5f,  0.0f, // Left Side
    0.0f,  0.8f, 0.0f,  0.92f, 0.86f, 0.76f,
    2.5f,  5.0f, -0.8f, 0.5f,  0.0f, // Left Side

    -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f,
    5.0f,  0.0f, 0.0f,  0.5f,  -0.8f, // Non-facing side
    0.5f,  0.0f, -0.5f, 0.83f, 0.70f, 0.44f,
    0.0f,  0.0f, 0.0f,  0.5f,  -0.8f, // Non-facing side
    0.0f,  0.8f, 0.0f,  0.92f, 0.86f, 0.76f,
    2.5f,  5.0f, 0.0f,  0.5f,  -0.8f, // Non-facing side

    0.5f,  0.0f, -0.5f, 0.83f, 0.70f, 0.44f,
    0.0f,  0.0f, 0.8f,  0.5f,  0.0f, // Right side
    0.5f,  0.0f, 0.5f,  0.83f, 0.70f, 0.44f,
    5.0f,  0.0f, 0.8f,  0.5f,  0.0f, // Right side
    0.0f,  0.8f, 0.0f,  0.92f, 0.86f, 0.76f,
    2.5f,  5.0f, 0.8f,  0.5f,  0.0f, // Right side

    0.5f,  0.0f, 0.5f,  0.83f, 0.70f, 0.44f,
    5.0f,  0.0f, 0.0f,  0.5f,  0.8f, // Facing side
    -0.5f, 0.0f, 0.5f,  0.83f, 0.70f, 0.44f,
    0.0f,  0.0f, 0.0f,  0.5f,  0.8f, // Facing side
    0.0f,  0.8f, 0.0f,  0.92f, 0.86f, 0.76f,
    2.5f,  5.0f, 0.0f,  0.5f,  0.8f // Facing side
};

// Indices for vertices order
GLuint indices[] = {
    0,  1,  2,  // Bottom side
    0,  2,  3,  // Bottom side
    4,  6,  5,  // Left side
    7,  9,  8,  // Non-facing side
    10, 12, 11, // Right side
    13, 15, 14  // Facing side
};

GLfloat lightVertices[] = { //     COORDINATES     //
    -0.1f, -0.1f, 0.1f,  -0.1f, -0.1f, -0.1f, 0.1f, -0.1f,
    -0.1f, 0.1f,  -0.1f, 0.1f,  -0.1f, 0.1f,  0.1f, -0.1f,
    0.1f,  -0.1f, 0.1f,  0.1f,  -0.1f, 0.1f,  0.1f, 0.1f};

GLuint lightIndices[] = {0, 1, 2, 0, 2, 3, 0, 4, 7, 0, 7, 3, 3, 7, 6, 3, 6, 2,
                         2, 6, 5, 2, 5, 1, 1, 5, 4, 1, 4, 0, 4, 5, 6, 4, 6, 7};

int main() {

  // Init GLFW
  glfwInit();

  // Tell GLFW what version of OpenGL we are using (3.3)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Tell GLFW we are using the CORE profile (Only modern functions)
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create GLFWwindow object of 800x800 pixels
  GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
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

  glfwGetFramebufferSize(window, &swidth, &sheight);
  glViewport(0, 0, swidth, sheight);

  Shader shaderProgram("../shaders/vertexShader.vert",
                       "../shaders/fragmentShader.frag");

  VAO VAO1;
  VAO1.Bind();

  VBO VBO1(vertices, sizeof(vertices));
  EBO EBO1(indices, sizeof(indices));

  VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void *)0);
  VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float),
                  (void *)(3 * sizeof(float)));
  VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float),
                  (void *)(6 * sizeof(float)));
  VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float),
                  (void *)(8 * sizeof(float)));

  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();

  // Shader for light cube
  Shader lightShader("../shaders/light.vert", "../shaders/light.frag");
  // Generates Vertex Array Object and binds it
  VAO lightVAO;
  lightVAO.Bind();
  // Generates Vertex Buffer Object and links it to vertices
  VBO lightVBO(lightVertices, sizeof(lightVertices));
  // Generates Element Buffer Object and links it to indices
  EBO lightEBO(lightIndices, sizeof(lightIndices));
  // Links VBO attributes such as coordinates and colors to VAO
  lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *)0);
  // Unbind all to prevent accidentally modifying them
  lightVAO.Unbind();
  lightVBO.Unbind();
  lightEBO.Unbind();

  glm::vec4 lightColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
  glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, lightPos);

  glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::mat4 pyramidModel = glm::mat4(1.0f);
  pyramidModel = glm::translate(pyramidModel, pyramidPos);

  lightShader.Activate();
  glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE,
                     glm::value_ptr(lightModel));
  glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x,
              lightColor.y, lightColor.z, lightColor.w);
  shaderProgram.Activate();
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1,
                     GL_FALSE, glm::value_ptr(pyramidModel));
  glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"),
              lightColor.x, lightColor.y, lightColor.z, lightColor.w);
  glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x,
              lightPos.y, lightPos.z);

  Texture cat("../assets/textures/cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA,
              GL_UNSIGNED_BYTE);
  cat.texUnit(shaderProgram, "tex0", 0);

  glEnable(GL_DEPTH_TEST);

  Camera camera(swidth, sheight, glm::vec3(0, 0, 2.f));

  // Main while loop
  while (!glfwWindowShouldClose(window)) {
    // GLFW events
    glfwPollEvents();
    camera.Inputs(window);
    camera.UpdateMatrix(45.0f, 0.1f, 100.f);

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Tells OpenGL which Shader Program we want to use
    shaderProgram.Activate();
    // Exports the camera Position to the Fragment Shader for specular lighting
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"),
                camera.Position.x, camera.Position.y, camera.Position.z);
    // Export the camMatrix to the Vertex Shader of the pyramid
    camera.Matrix(shaderProgram, "camMatrix");
    // Binds texture so that is appears in rendering
    cat.Bind();
    // Bind the VAO so OpenGL knows to use it
    VAO1.Bind();
    // Draw primitives, number of indices, datatype of indices, index of indices
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT,
                   0);

    // Tells OpenGL which Shader Program we want to use
    lightShader.Activate();
    // Export the camMatrix to the Vertex Shader of the light cube
    camera.Matrix(lightShader, "camMatrix");
    // Bind the VAO so OpenGL knows to use it
    lightVAO.Bind();
    // Draw primitives, number of indices, datatype of indices, index of indices
    glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int),
                   GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
  }

  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();
  cat.Delete();
  shaderProgram.Delete();

  lightVAO.Delete();
  lightVBO.Delete();
  lightEBO.Delete();
  lightShader.Delete();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
