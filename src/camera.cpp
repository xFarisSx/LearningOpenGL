#include "camera.h"

Camera::Camera(int width, int height, glm::vec3 position) {
  Camera::width = width;
  Camera::height = height;
  Position = position;
}
void Camera::Matrix(Shader &shader, const char *uniform){
  glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE,
                     glm::value_ptr(cameraMatrix));

}
void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane) {
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 proj = glm::mat4(1.0f);

  view = glm::lookAt(Position, Position + Orientation, Up);
  proj = glm::perspective(glm::radians(FOVdeg), (float)(width / height),
                          nearPlane, farPlane);

  cameraMatrix = proj*view;

}
void Camera::Inputs(GLFWwindow *window) {
  glm::vec3 Right = glm::normalize(glm::cross(Orientation, Up));
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    Position += speed * Orientation;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    Position -= speed * Orientation;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    Position += speed * Right;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    Position -= speed * Right;
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    Position += speed * Up;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    Position -= speed * Up;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    speed = 0.1f;
  } else {
    speed = 0.01f;
  }

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

    // Get current mouse position
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (firstMouse) {
      lastX = (float)mouseX;
      lastY = (float)mouseY;
      firstMouse = false;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    // Calculate offset since last frame
    float offsetX = (float)(mouseX - lastX);
    float offsetY = (float)(mouseY - lastY);

    lastX = (float)mouseX;
    lastY = (float)mouseY;

    // Apply sensitivity
    offsetX *= sensitivity;
    offsetY *= sensitivity;

    yaw += offsetX;
    pitch -= offsetY;

    // Clamp pitch between -85 and 85 degrees
    if (pitch > 85.0f)
      pitch = 85.0f;
    if (pitch < -85.0f)
      pitch = -85.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    Orientation = glm::normalize(direction);
  } else {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    firstMouse = true; // Reset for next click
  }
}
