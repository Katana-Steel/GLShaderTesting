#include "gl.h"
#include <string>

#include "shader.h"
#include "x11.h"
/* coming soon near you:
#include <GL/gl.h>
all the shader fun and openGL rendering happens there
*/
#include <iostream>
#include <GL/glx.h>
#include <GL/glu.h>
#include <cstdlib>

static int attrs[] = {
  GLX_USE_GL,
  GLX_RGBA,
  GLX_RED_SIZE, 8,
  GLX_GREEN_SIZE, 8,
  GLX_BLUE_SIZE, 8,
  //  GLX_ALPHA_SIZE, 8,
  GLX_DEPTH_SIZE, 16,
  GLX_DOUBLEBUFFER,
  None
};

int main()
{
  Display *disp = XOpenDisplay( std::getenv("DISPLAY") );
  XVisualInfo *vinfo = glXChooseVisual( disp, DefaultScreen(disp), attrs );
  if(vinfo == (0)) { 
    std::cout << "no visuals found, >_< bailing out." << std::endl;
    return 0;
  }
  std::cout << "visual:" << vinfo->visual << std::endl;
  Window win = X11CreateWindow(disp, vinfo->depth);
  if (win == 0) {
    std::cout << "Window Not created, >_< bailing out." << std::endl;
    return 0;
  } 
  GLXContext cx = glXCreateContext( disp, vinfo, 0, True);
  
  glXMakeCurrent(disp, win, cx);  

  Shader *sh = loadShader("test.vs", GL_VERTEX_SHADER);
  if(sh) {
    std::cout << "vertex shader loaded" << std::endl;
    std::cout << sh->errorLog() << std::endl; 
    sh->clean();
    delete sh;
  } else {
    std::cout << "could not load the vertex shader" << std::endl;
  }
  sh = loadShader("test.ps", GL_FRAGMENT_SHADER);
  if(sh) {
    std::cout << "pixel shader loaded" << std::endl;
    std::cout << sh->errorLog() << std::endl;
    sh->clean();
    delete sh;
  } else {
    std::cout << "could not load the vertex shader" << std::endl;
  }
  X11WindowControl(disp, win, cx);
  glXMakeCurrent( disp, None, 0 );
  glXDestroyContext(disp, cx);
  return 0;
}
