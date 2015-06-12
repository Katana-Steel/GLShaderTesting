#include <GL/gl.h>
#include <GL/glx.h>
#include "gl.h"
#include "shader.h"
#include "program.h"
#include "ani.h"
#include <vector>
#include <iostream>

std::vector<Shader*> vertex_list;
std::vector<Shader*> fragment_list;

void
GLIdleFunc(ani *a)
{
    a->inc_r(0.25f, 0.0f, 360.0f);
}

void
AddUnitCube()
{
    GLfloat mat_spec[] = {1.0,1.0,1.0,1.0};
    GLfloat mat_shine[] = {0.0,0.0,0.0,1.0};
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
//    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shine);
    glBegin(GL_QUADS);                                 
        /* top of cube */                              
        glColor3f(0.0f, 1.0f, 0.0f);
        glNormal3f(0.0f, 1.0f, 0.0f);                   
        glVertex3f(1.0f, 1.0f, -1.0f);                 
        glVertex3f(-1.0f, 1.0f, -1.0f);                
        glVertex3f(-1.0f, 1.0f, 1.0f);                 
        glVertex3f(1.0f, 1.0f, 1.0f);
                                  
        /* bottom of cube */                           
        glColor3f(1.0f, 0.5f, 0.0f);
        glNormal3f(0.0f, -1.0f, 0.0f);                   

        glVertex3f(1.0f, -1.0f, 1.0f);                 
        glVertex3f(-1.0f, -1.0f, 1.0f);                
        glVertex3f(-1.0f, -1.0f, -1.0f);               
        glVertex3f(1.0f, -1.0f, -1.0f);                

        /* front of cube */                            
        glColor3f(1.0f, 0.0f, 0.0f);
        glNormal3f(0.0f, 0.0f, 1.0f);

        glVertex3f(1.0f, 1.0f, 1.0f);                  
        glVertex3f(-1.0f, 1.0f, 1.0f);                 
        glVertex3f(-1.0f, -1.0f, 1.0f);                
        glVertex3f(1.0f, -1.0f, 1.0f);                 

        /* back of cube */                             
        glColor3f(1.0f, 1.0f, 0.0f);                   
        glNormal3f(0.0f, 0.0f, -1.0f);

        glVertex3f(-1.0f, 1.0f, -1.0f);                
        glVertex3f(1.0f, 1.0f, -1.0f);                 
        glVertex3f(1.0f, -1.0f, -1.0f);                
        glVertex3f(-1.0f, -1.0f, -1.0f);               

        /* right side of cube */                       
        glColor3f(1.0f, 0.0f, 1.0f);                   
        glNormal3f(1.0f, 0.0f, 0.0f);

        glVertex3f(1.0f, 1.0f, -1.0f);                 
        glVertex3f(1.0f, 1.0f, 1.0f);                  
        glVertex3f(1.0f, -1.0f, 1.0f);                 
        glVertex3f(1.0f, -1.0f, -1.0f);                

        /* left side of cube */                        
        glColor3f(0.0f, 1.0f, 1.0f);
        glNormal3f(-1.0f, 0.0f, 0.0f);

        glVertex3f(-1.0f, 1.0f, 1.0f);                 
        glVertex3f(-1.0f, 1.0f, -1.0f);                
        glVertex3f(-1.0f, -1.0f, -1.0f);               
        glVertex3f(-1.0f, -1.0f, 1.0f);                
    glEnd();                                           
    glNormal3f(0.0f,0.0f,0.0f);
}

void
GLDispFunc(ani *a)
{
    GLfloat lightpos[] = {1.0f,1.0f,0.0f,0.0f};
    GLfloat amb [] = {0.0f,0.0f,0.0f,1.0f};
    GLfloat spec_diff[] = {1.0f,1.0f,1.0f,1.0f}; 
    glMatrixMode(GL_MODELVIEW); 

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLightfv(GL_LIGHT0,GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0,GL_SPECULAR, lightpos);
    glLightfv(GL_LIGHT0,GL_DIFFUSE, spec_diff);

    glLoadIdentity();


    glTranslatef(0.0f, 0.0f, -7.0f);
    glTranslatef(a->xmotion, a->ymotion, a->zmotion);

    glRotatef(a->rot, 1.0f, 0.5f, 0.25f); 

    AddUnitCube();
    
    glLoadIdentity();

    glLightfv(GL_LIGHT0,GL_POSITION, lightpos);

    glFlush();
}

void
GLClear()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glDepthFunc(GL_LEQUAL);              
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glFlush();
}

#include <GL/glu.h>

void
GLDispResize(Display *disp, Window win)
{
    
    XWindowAttributes wattr;
    XGetWindowAttributes(disp, win, &wattr);

    GLfloat hw=(GLfloat)wattr.width/2, hh=(GLfloat)wattr.height/2;

   // GLClear();
    glViewport(0, 0, wattr.width, wattr.height);
    glMatrixMode(GL_PROJECTION);                      
    glLoadIdentity();
//    glFrustum(-(hw),hw,-(hh),hh, 0.0f, 100.0f);                                 
    gluPerspective(45.0f, (GLfloat)wattr.width / (GLfloat)wattr.height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);     
    glFlush();
}

