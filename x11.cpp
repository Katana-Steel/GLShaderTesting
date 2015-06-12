#include "x11.h"
#include "gl.h"
#include "ani.h"
#include "files.h"
#include "program.h"
#include <cstring>
#include <iostream>

Window
X11CreateWindow(Display* disp, int depth)
{
  XSetWindowAttributes wattrs;
  std::memset(&wattrs, 0, sizeof(wattrs));
  wattrs.event_mask = ExposureMask | KeyPressMask | VisibilityChangeMask | ButtonPressMask | ButtonReleaseMask;
  wattrs.border_pixel = 0;
  wattrs.bit_gravity  = StaticGravity;
  int wmask = CWBitGravity | CWEventMask;

  Window win = XCreateWindow (disp, DefaultRootWindow(disp), 0,0, 550, 300, 0, depth, InputOutput, CopyFromParent, wmask, &wattrs);
  if(win == 0) return 0;
  XChangeProperty(disp, win, XA_WM_NAME, XA_STRING, 8,0, (unsigned char*)"xShader", 7);
  XChangeProperty(disp, win, XA_WM_ICON_NAME, XA_STRING, 8,0, (unsigned char*)"xShader", 7);

  return win;
}


void
X11WindowControl(Display *disp, Window win, GLXContext cx)
{
  XEvent event;
  fd_set input;
  bool   ready = true, mapped = false, end = false, visible = false;
  struct timeval to;
  ani    *a = new ani();
  unsigned int vs = 1;
  unsigned int ps = 1;
          ShaderSourceLoader();
          std::cout << "shaders loaded" << std::endl;

  XMapRaised(disp, win);
  glXMakeCurrent(disp, win, cx);
  GLClear();

  while(!end)
  {
    if(XPending(disp)) {
      XNextEvent( disp, &event);
      switch (event.type) {
      case Expose:
        visible = false;
        if(event.xexpose.count != 0)
            break;
        visible = true;
        break;
      case VisibilityNotify:
        if(event.xvisibility.state == VisibilityFullyObscured) {
          visible = false;
        } else {
          GLDispResize(disp, win);
          std::cout << "atleast a little visible" << std::endl;
          visible = true; mapped = true;
        }
        break;
      case KeyPress:
        switch(event.xkey.keycode) { 
        default:
          std::cout << "keypressed " << event.xkey.keycode << std::endl;
          break;
        case 46: // the 'L' key
          ShaderSourceLoader();
          std::cout << "shaders re-loaded" << std::endl;
          break;
        case 39: // apply selected shaders with the 'S' key.
          if(a->shader) delete a->shader;
          a->shader = new Program(vs,ps);
          if(!a->shader->ApplyShader()) {
            std::cout << a->shader->error() << std::endl;
            delete a->shader;
            a->shader = 0;
          }
          break;
        case 33: // 'V' key selects vertex shader.
          ps++;
          if(ps > fragment_count()) ps = 1;
          std::cout << "fragment selected: " << ps << std::endl;
          break;
        case 55: // 'V' key selects vertex shader.
          vs++;
          if(vs > vertex_count()) vs = 1;
          std::cout << "vertex selected: " << vs << std::endl;
          break;
        case 24:  /* end if the 'Q'(key no24) key is hit in any possible way */
          end = true;
          break;
        case 114: // the right arrow key
          a->xmotion += 0.1f;
          if(a->xmotion > 8.0f)
            a->xmotion = -8.0f;
          break;
        case 113: // the left arrow key
          a->xmotion -= 0.1f;
          if(a->xmotion < -8.0f)
            a->xmotion = 8.0f;
          break;
        }
        break;
      }
    }
    if(visible) {
      GLIdleFunc(a);
      GLDispFunc(a);
      glXSwapBuffers(disp, win);
    }
  }
}
