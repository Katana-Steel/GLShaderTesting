#ifndef __ani_h
#define __ani_h 1

#include <GL/gl.h>

class Program;

struct ani 
{
    ani *next;
    ani *prev;
    Program *shader;
    GLfloat rot;
    GLfloat xmotion;
    GLfloat ymotion;
    GLfloat zmotion;
    ani(ani *a=0);
    void inc_r(GLfloat s, GLfloat min, GLfloat max);
};

#endif

