#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glu.h>
#include "program.h"
#include "shader.h"
#include <vector>

extern std::vector<Shader*> vertex_list;
extern std::vector<Shader*> fragment_list;

Program::Program(unsigned int ver_idx,unsigned int frag_idx)
{
  this->vertex = 0;
  this->fragment = 0;
  if(ver_idx != 0 && ver_idx <= vertex_list.size()) {
    this->vertex = vertex_list.at(ver_idx-1);
  }
  if(frag_idx != 0 && frag_idx <= fragment_list.size()) {
    this->fragment = fragment_list.at(frag_idx-1);
  }
}

bool
Program::ApplyShader()
{
    if(!this->vertex) {
        this->err = "vertex shader idx out of range";
        return false;
    }
    if(!this->fragment) {
        this->err = "pixel shader idx out of range";
        return false;
    }
    if(this->vertex->ARB != this->fragment->ARB) {
        this->err = "incompatable shaders";
        return false;
    }
    if(this->vertex->ARB) {
        GLhandleARB hdl = glCreateProgramObjectARB();
        glAttachObjectARB(hdl, this->vertex->hdlarb);
        glAttachObjectARB(hdl, this->fragment->hdlarb);
        glLinkProgramARB(hdl);
        glUseProgramObjectARB(hdl);
    } else {
        GLuint hdl = glCreateProgram();
        glAttachShader(hdl, this->vertex->hdl20);
        glAttachShader(hdl, this->fragment->hdl20);
        glLinkProgram(hdl);
        glUseProgram(hdl);
    }
    return true;
}

std::string
Program::error()
{
    std::string ret = this->err;
    this->err = "";
    return ret;
}
    


