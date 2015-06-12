#ifndef __local_gl_h
#define __local_gl_h 1

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

struct ani;

extern void GLClear();
extern void GLIdleFunc(ani *);
extern void GLDispFunc(ani *);
extern void GLDispResize(Display*, Window);

#endif /* __local_gl_h */
