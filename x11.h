#ifndef __x11_h
#define __x11_h 1

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <GL/glx.h>

extern void X11WindowControl(Display*, Window, GLXContext);
extern Window X11CreateWindow(Display*, int);



#endif /* __x11_h */
