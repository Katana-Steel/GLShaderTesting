#include "ani.h"
#include <GL/gl.h>

ani::ani(ani *a)
{
    this->next = 0;
    this->prev = a;
    this->shader = 0;
    if(a) a->next = this;
    this->rot = 0.0f;
    this->xmotion = 0.0f;
    this->ymotion = 0.0f;
    this->zmotion = 0.0f;
}

void
ani::inc_r(GLfloat s, GLfloat min, GLfloat max)
{
   this->rot += s;
   if(this->rot > max)
     this->rot = min;
   if(this->rot < min)
     this->rot = max;
}

