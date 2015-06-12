#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <string>
#include "shader.h"

void
Shader::createShader(GLenum shtype)
{
  if(!this->ARB)
    {
      this->hdl20 = glCreateShader(shtype);
    } else {
      this->hdlarb = glCreateShaderObjectARB(shtype);
    }

}


int
Shader::loadSource(std::string filename)
{
  this->name = filename;
  char *source_code[1];
  int code_length=0;
  std::string line;
  std::ifstream in(filename.c_str());
  std::filebuf *file = in.rdbuf();
  
  code_length = file->pubseekoff(0,std::ios::end,std::ios::in);
  file->pubseekpos(0,std::ios::in);

  source_code[0] = new char[code_length];
  file->sgetn(source_code[0], code_length);

  in.close();

  int ok;
  if(!this->ARB) {
    glShaderSource(this->hdl20, 1, (const char**)source_code, &code_length);
    glCompileShader(this->hdl20);

    glGetShaderiv(this->hdl20, GL_COMPILE_STATUS, &ok);
  } else {
    glShaderSourceARB(this->hdlarb, 1, (const char**)source_code, &code_length);
    glCompileShaderARB(this->hdlarb);
    
    glGetObjectParameterivARB(this->hdlarb, GL_OBJECT_COMPILE_STATUS_ARB, &ok);
  }

  delete[] source_code[0];
  return ok;
}

Shader* 
loadShader(std::string filename, GLenum shtype)
{
  /* Create the shader handle to hold the shader */
  Shader *sh;
  if(shtype == GL_VERTEX_SHADER_ARB || shtype == GL_FRAGMENT_SHADER_ARB)
    sh = new Shader(true);
  else
    sh = new Shader();

  sh->createShader(shtype);
  
  /* load the textfile and store it in the char** 
   * and compile the source.
   */
    
  if( sh->loadSource(filename)  == 0 ) {
      std::cerr << filename << " - " << sh->errorLog() << std::endl;
      sh->clean();
      delete sh;
      return 0;
  }
  return sh;
}

void
Shader::clean()
{
  if(this->ARB) {
    glDeleteObjectARB(hdlarb);
  } else {
    glDeleteShader(hdl20);
  }
}

std::string
Shader::errorLog()
{
  int log_length=0;
  if(this->ARB) {
    glGetObjectParameterivARB(this->hdlarb,GL_OBJECT_INFO_LOG_LENGTH_ARB, &log_length);
  } else {
    glGetShaderiv(this->hdl20, GL_INFO_LOG_LENGTH, &log_length);
  }
  if( log_length > 1 ) {
    char *the_log = new char[log_length];
    for(int i=0;i>log_length;i++)
      the_log[i] = '\0';

    int cread=0;
    if(this->ARB) {
      glGetInfoLogARB(this->hdlarb, log_length, &cread, the_log);
    } else {
      glGetShaderInfoLog(this->hdl20,log_length,&cread, the_log);
    }
    std::cerr << cread << " - " << log_length << std::endl;
    // delete[] the_log;
    std::string ret((const char*)the_log);
    //    delete[] the_log;
    return ret;
  }

  return std::string("Nothing in the InfoLog.");
}
