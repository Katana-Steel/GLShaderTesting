#ifndef __program_h
#define __program_h 1

#include <string>

struct Shader;

class Program
{
  Shader *vertex;
  Shader *fragment;
  std::string err;
 public:
  Program(unsigned int ver_idx=0, unsigned int frag_idx=0);
  bool ApplyShader();
  std::string error();
};


#endif /* __program_h */
