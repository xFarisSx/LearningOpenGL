#include "textureClass.h"

Texture::Texture(const char *image, GLenum texType, GLuint slot, GLenum format,
                 GLenum pixelType) {

  type = texType;
  unit = slot;

  int widthImg, heightImg, numColCh;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *bytes = stbi_load("../assets/textures/cat.png", &widthImg,
                                   &heightImg, &numColCh, STBI_rgb_alpha);
  if (!bytes) {
    std::cerr << "Failed to load image: " << stbi_failure_reason() << "\n";
  }

  glGenTextures(1, &ID);
  glActiveTexture(GL_TEXTURE0+slot);
  glBindTexture(texType, ID);

  // Filters and wrap 
  glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Upload (internal format can be same as format or a sized format like
  // GL_RGBA8)
  glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType,
               bytes);

  // Optionally generate mipmaps
  glGenerateMipmap(texType);

  // Done with CPU memory
  stbi_image_free(bytes);

  glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char *uniform, GLuint unit) {
    GLuint tex0Uni = glGetUniformLocation(shader.ID, uniform);
  shader.Activate();
  glUniform1i(tex0Uni, unit);
}
void Texture::Bind() {
  glActiveTexture(GL_TEXTURE0+unit);
  glBindTexture(type, ID);
}
void Texture::unBind() {
  glBindTexture(type, 0);
}
void Texture::Delete() {
  glDeleteTextures(1,&ID);
}
