#ifndef __shader_h
#define __shader_h 1

#include <string>

struct Shader
{
  bool               ARB;
  GLuint             hdl20;
  GLhandleARB        hdlarb;
  GLenum             type;
  std::string        name;

  Shader(bool arb=false) : ARB(arb) {};

  void createShader(GLenum shtype);
  int loadSource(std::string filename);
  void clean();

  std::string errorLog();
};

extern Shader* loadShader(std::string filename, GLenum shtype);

#endif
