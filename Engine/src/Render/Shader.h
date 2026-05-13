#pragma once

#include <cstdint>
#include <glad/glad.h>
#include <string>

struct Shader
{
  uint32_t ShaderId;

  Shader(const std::string &shaderPath);
  virtual ~Shader();

  void Bind() const;
  void Unbind() const;

  void SetUniform4f(const std::string &name, float x, float y, float z, float w);
  void SetUniform2f(const std::string &name, float x, float y);
  void SetUniform1f(const std::string &name, float f);
  void SetUniform1i(const std::string &name, int i);
  void SetUniform1b(const std::string &name, bool value);
private:

  uint32_t Compile(const std::string &shaderSource, GLenum type);
  std::string Parse(const char* filename);
  GLuint CreateShader(const std::string &shaderPath);
};
