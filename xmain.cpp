#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>

#include "keys.h"

/*
 *   Globals
 */



using namespace std;

class GLPoint {
	GLfloat _vec[4];
public:
	GLPoint(GLfloat ix=0.0f,GLfloat iy=0.0f,GLfloat iz=0.0f,GLfloat iw=1.0f)
	{
		_vec[0] = ix;
		_vec[1] = iy;
		_vec[2] = iz;
		_vec[3] = iw;
	};

/*	const GLfloat* operator=(const GLfloat *v)  {
		return &this->_vec[0];
	} */
	GLPoint& operator=(const GLPoint &old) {
		this->set(old._vec[0],old._vec[1],old._vec[2],old._vec[3]);
		return *this;
	};

	GLfloat x() { return _vec[0]; };
	GLfloat y() { return _vec[1]; };
	GLfloat z() { return _vec[2]; };
	GLfloat w() { return _vec[3]; };
	const GLfloat* vec() { return &_vec[0]; };

	void setx(GLfloat ix) { _vec[0] = ix; };
	void sety(GLfloat iy) { _vec[1] = iy; };
	void setz(GLfloat iz) { _vec[2] = iz; };
	void setw(GLfloat iw) { _vec[3] = iw; };
	void set(GLfloat ix=0.0f,GLfloat iy=0.0f,GLfloat iz=0.0f,GLfloat iw=1.0f) {
		_vec[0] = ix;
		_vec[1] = iy;
		_vec[2] = iz;
		_vec[3] = iw;
	};
};

int     CubeMouseButton,
        CubeMouseX,
        CubeMouseY,
        CubeWidth,
        CubeHeight;
bool    key[256], shape, fill;
int		yrot, ztrans, rot;
GLuint  plate;
GLuint  fontlist;

/*
 *   Functions
 */

void   DisplayFunc(void);
void   InitModelLists(Display *disp);
void   IdleFunc(void);
// void   MotionFunc(int x, int y);
void   MouseFunc(int button, int state, int x, int y);
void   KeybFunc(int button, int state);
void   ReshapeFunc(int width, int height);

/*
 * 'main()'
 */

int main(int argc, char *argv[])
{
	Bool                  mapped;
	GLXContext            cx;
	Display              *disp;
	Window                win;
	XVisualInfo          *vinfo;
	XSetWindowAttributes  winattrs;
	int                   winmask;
	XEvent                event;
	XWindowAttributes     windata;
	struct timeval        timeout;
	fd_set                input;
	int                   ready;
	static int            attrs[] = {
		                          GLX_RGBA,
					  GLX_RED_SIZE, 8,
					  GLX_GREEN_SIZE, 8,
					  GLX_BLUE_SIZE, 8,
					  GLX_DEPTH_SIZE, 16,
					  GLX_DOUBLEBUFFER,
					  0
					};
	if(argc < 0) return argc;
	for(int i=0;i<256;i++) key[i] = false;
	yrot = 0;
	rot = false;
	ztrans = -24;

	disp  = XOpenDisplay( getenv("DISPLAY") );
    glXQueryVersion(disp, &CubeHeight, &CubeWidth);
    printf("glx version: %d.%d\n", CubeHeight, CubeWidth);

	vinfo = glXChooseVisual( disp, DefaultScreen(disp), attrs );
	if(vinfo == NULL) { printf("No Visual\n"); return 0; }

	winattrs.event_mask   = ExposureMask | VisibilityChangeMask |
						    StructureNotifyMask | ButtonPressMask |
						    ButtonReleaseMask | KeyPressMask | KeyReleaseMask;
	winattrs.border_pixel = 0;
	winattrs.bit_gravity  = StaticGravity;
	winmask               = CWBorderPixel | CWBitGravity | CWEventMask;

	win = XCreateWindow( disp, DefaultRootWindow(disp), 0, 0, 400, 400,
						 0, vinfo->depth, InputOutput, vinfo->visual,
						 winmask, &winattrs );
	if(win == 0) { printf("Failed to Create Window\n"); return 0; }
	XChangeProperty( disp, win, XA_WM_NAME, XA_STRING, 8, 0,
					 (unsigned char*)argv[0], strlen(argv[0]) );
	XChangeProperty( disp, win, XA_WM_ICON_NAME, XA_STRING, 8, 0,
					 (unsigned char*)argv[0], strlen(argv[0]) );

       	XMapWindow( disp, win );

	cx = glXCreateContext( disp, vinfo, 0, True );
	glXMakeCurrent( disp, win, cx );

	CubeWidth        = 400;
	CubeHeight       = 400;

    InitModelLists(disp);

/*	CubeRotation[0]  = 45.0f;
	CubeRotation[1]  = 45.0f;
	CubeRotation[2]  = 45.0f;
	CubeRate[0]      = 1.0f;
	CubeRate[1]      = 1.0f;
	CubeRate[2]      = 1.0f; */

	mapped = False;

	for(;;)
	{
		if(mapped) {
			FD_ZERO(&input);
			FD_SET( ConnectionNumber(disp), &input );

			timeout.tv_sec  = 0;
			timeout.tv_usec = 20000;

			ready = select( ConnectionNumber(disp) + 1, &input, NULL, NULL, &timeout );
		}
		else
			ready = 1;

		if (ready) {

			XNextEvent( disp, &event );

			switch(event.type) {
			case MapNotify :
				mapped = True;

			case ConfigureNotify :
				XGetWindowAttributes( disp, win, &windata );

				ReshapeFunc(windata.width, windata.height);
				break;

			case UnmapNotify :
				mapped = False;
				break;

			case ButtonPress :
				MouseFunc(event.xbutton.button, 0, event.xbutton.x, event.xbutton.y);
				break;

			case ButtonRelease :
				MouseFunc(event.xbutton.button, 1, event.xbutton.x, event.xbutton.y);
				break;

			case KeyPress :
				KeybFunc(event.xkey.keycode, 0);
				break;

			case KeyRelease :
				printf("key %d\n",event.xkey.keycode);
				KeybFunc(event.xkey.keycode, 1);
				break;
			}
		}

		if(mapped) {
			IdleFunc();
			DisplayFunc();
			glXSwapBuffers(disp, win);
		}
	}
}

/*
 * DisplayFunc()
 */

void DisplayFunc(void) {

	static const GLfloat colors[7][3] = {
		{  1.0f,  0.0f,  0.0f },
		{  0.0f,  1.0f,  0.0f },
		{  1.0f,  1.0f,  0.0f },
		{  0.0f,  0.0f,  1.0f },
		{  1.0f,  0.0f,  1.0f },
		{  0.0f,  1.0f,  1.0f },
		{  1.0f,  1.0f,  1.0f }
	};

	glViewport( 0, 0, CubeWidth, CubeHeight );
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glPushAttrib(GL_LIST_BIT);
	glListBase(fontlist - ' ');

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	GLfloat ratio = (GLfloat)CubeHeight / CubeWidth;
//	glOrtho( -2.0f, 2.0f, -2.0f * ratio, 2.0f * ratio, -4.0f, 4.0f );
	gluPerspective( 60.0f, (GLfloat)CubeWidth/CubeHeight, 1.0f, 100.0f );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(yrot < 90 || yrot > 270) glFrontFace(GL_CCW);
	else glFrontFace(GL_CW);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glTranslatef( -9.0f, 9.0f,(GLfloat)ztrans );

	if(fill) glPolygonMode(GL_FRONT, GL_LINE); else glPolygonMode(GL_FRONT, GL_FILL);
	glRotatef((GLfloat)yrot, 0.0f,1.0f,0.0f);
	for(int p =0;p<4;p++) {
		glRotatef(p*90.0f,0.0f,0.0f,1.0f);
        glColor3fv(colors[p]);
        if(shape) glCallList(plate); else glCallList(plate+1);
	}

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glFrontFace(GL_CW);
	glTranslatef( 2.5f, 0.0f,(GLfloat)ztrans+0.2f );
	glScalef(2.0f,1.0f,1.0f);
    glCallList(plate+2); // inputB
    glTranslatef(-0.2f, 0.2f,0.0f);
    glCallList(plate+3); // inputT

    glPopMatrix();

	glPushMatrix();

	glLoadIdentity();

	//glTranslatef( 0.0f, 0.0f,(GLfloat)ztrans+0.5 );
	glTranslatef( -0.2f, 0.0f, -0.5f );
	char *s = "Alchemiestick IS";

    glColor3fv(colors[6]);
	glRasterPos3f(0.0f, 0.0f, -0.5f );
	//glWindowPos3f(0.0f, 0.0f, 0.5f);

	glCallLists(strlen(s), GL_BYTE, s);
	glPopMatrix();
	glPopAttrib();

}

/*
 * InitModelLists()
 */

void InitModelLists(Display *disp)
{
	GLPoint lg1[17] = {
		GLPoint( 0.0f, 0.0f, 0.0f), // 0
		GLPoint( 1.0f, 0.0f, 0.0f), // 1
		GLPoint( 2.0f,-1.0f, 0.0f), // 2
		GLPoint( 3.0f, 0.0f, 0.0f), // 3
		GLPoint( 3.0f, 1.0f, 0.0f), // 4
		GLPoint( 4.0f, 1.0f, 0.0f), // 5
		GLPoint( 5.0f, 2.0f, 0.0f), // 6
		GLPoint( 6.0f, 2.0f, 0.0f), // 7
		GLPoint( 5.0f, 3.0f, 0.0f), // 8
		GLPoint( 4.0f, 3.0f, 0.0f), // 9
		GLPoint( 4.0f, 4.0f, 0.0f), // 10
		GLPoint( 3.0f, 3.0f, 0.0f), // 11
		GLPoint( 3.0f, 2.0f, 0.0f), // 12
		GLPoint( 2.0f, 1.0f, 0.0f), // 13
		GLPoint( 2.0f, 2.0f, 0.0f), // 14
		GLPoint( 1.0f, 2.0f, 0.0f), // 15
		GLPoint( 0.0f, 1.0f, 0.0f)  // 16
	};

	GLPoint lg2[17] = {
		GLPoint( 0.0f, 0.0f, 0.0f),
		GLPoint( 1.0f, 1.0f, 0.0f),
		GLPoint( 1.0f, 2.0f, 0.0f),
		GLPoint( 2.0f, 2.0f, 0.0f),
		GLPoint( 3.0f, 3.0f, 0.0f),
		GLPoint( 4.0f, 3.0f, 0.0f),
		GLPoint( 3.0f, 4.0f, 0.0f),
		GLPoint( 2.0f, 4.0f, 0.0f),
		GLPoint( 2.0f, 5.0f, 0.0f),
		GLPoint( 1.0f, 4.0f, 0.0f),
		GLPoint( 1.0f, 3.0f, 0.0f),
		GLPoint( 0.0f, 2.0f, 0.0f),
		GLPoint( 0.0f, 3.0f, 0.0f),
		GLPoint(-1.0f, 3.0f, 0.0f),
		GLPoint(-2.0f, 2.0f, 0.0f),
		GLPoint(-2.0f, 1.0f, 0.0f),
		GLPoint(-1.0f, 1.0f, 0.0f)
	};
	const int sides1[6][4] = {
		{0,1,15,16},
		{1,2,14,15},
		{2,3,12,13},
		{4,5,6,12},
		{7,8,9,12},
		{9,10,11,12}
	};
	const int sides2[6][4] = {
		{15,16,13,14},
		{16,0,12,13},
		{0,1,10,11},
		{2,3,4,10},
		{5,6,7,10},
		{7,8,9,10}
	};

    plate = glGenLists(100);
    fontlist = plate+4;

    glNewList(plate, GL_COMPILE);

        glBegin(GL_QUADS);
			for(int i=0;i<6;i++)
				for(int j=0;j<4;j++)
					glVertex3fv(lg1[sides1[i][j]].vec());
        glEnd();
    glEndList();

    glNewList(plate+1, GL_COMPILE);

        glBegin(GL_QUADS);
			for(int i=0;i<6;i++)
				for(int j=0;j<4;j++)
					glVertex3fv(lg2[sides2[i][j]].vec());
        glEnd();
    glEndList();

    GLPoint inputV[] = {
        GLPoint(-5.0f, -1.0f, 0.0), // 0
        GLPoint(-4.0f,  0.0f, 0.0),
        GLPoint( 3.5f,  0.0f, 0.0),
        GLPoint( 3.0f,  1.0f, 0.0),
        GLPoint( 5.0f, -1.0f, 0.0),
        GLPoint( 4.0f, -1.0f, 0.0),
        GLPoint( 4.2f,  2.0f, 0.0),
        GLPoint(-4.8f,  2.0f, 0.0), // 7
        GLPoint(-3.8f,  1.0f, 0.0), // 8
        GLPoint( 2.0f,  1.0f, 0.0), // 9
        GLPoint( 5.2f, -1.0f, 0.0)
    };
    int inputB [3][3] = {
        {0,1,2},
        {0,2,5},
        {3,4,5}
    };
    int inputT [4][3] = {
        {3,6,10},
        {3,7,6},
        {3,8,7},
        {9,3,2}
    };

    glNewList(plate+2, GL_COMPILE);

        glColor3f(0.9f,0.8f,1.0f);
        glBegin(GL_TRIANGLES);
            for(int i=0;i<3;i++)
                for(int j=0;j<3;j++)
                    glVertex3fv(inputV[inputB[i][j]].vec());
        glEnd();

    glEndList();

    glNewList(plate+3, GL_COMPILE);

        glColor3f(0.9f,0.5f,1.0f);
        glBegin(GL_TRIANGLES);
            for(int i=0;i<4;i++)
                for(int j=0;j<3;j++)
                    glVertex3fv(inputV[inputT[i][j]].vec());
        glEnd();

    glEndList();

    XFontStruct *font = XLoadQueryFont(disp, "-*-georgia-bold-r-normal--18-*-*-*-*-*-*-*");
    if(!font) {
        printf("Failed to get font, using fallback \"Courier\"\n");
        font = XLoadQueryFont(disp, "-*-courier-bold-r-normal--18-*-*-*-*-*-*-*");
    }
    glXUseXFont(font->fid, ' ', 96, fontlist);
/*
    glNewList(fontlist - ' ' + 'A', GL_COMPILE);

        glBegin(GL_TRIANGLES);
            glVertex3f(-0.1f, -0.1f, 0.0f);
            glVertex3f(0.1f, -0.1f, 0.0f);
            glVertex3f(0.0f, 0.1f, 0.0f);
        glEnd();

    glEndList();
*/
}

/*
 * IdleFunc()
 */

void IdleFunc(void) {
	if(key[KEY_Shift]) {
		if(key[KEY_Right]) yrot += 15;
		if(key[KEY_Left])  yrot -= 15;
	} else {
		if(key[KEY_Right]) yrot += 5;
		if(key[KEY_Left])  yrot -= 5;
	}
	if(key[KEY_LCtrl])     rot = !rot;
	if(rot)                yrot += 5;
	if(key[KEY_Up])        ztrans += 1;
	if(key[KEY_Down])      ztrans -= 1;
	if(yrot < 0)           yrot = 360 + yrot;
	if(yrot > 360)         yrot = yrot - 360;
	if(key[KEY_Space])     shape = !shape;
	if(key[KEY_Alt] && !key[KEY_LCtrl] && !key[KEY_RCtrl])   fill  = !fill;
	if(key[KEY_Escape]) exit(EXIT_SUCCESS);
}

/*
 * MotionFunc()
 *

void MotionFunc(int x, int y) {
	x -= CubeMouseX;
	y -= CubeMouseY;

	switch(CubeMouseButton) {
	case 0 :
		CubeRate[0] = 0.01f * x;
		CubeRate[1] = 0.01f * y;
		CubeRate[2] = 0.0f;
		break;

	case 1 :
		CubeRate[0] = 0.0f;
		CubeRate[1] = 0.01f * y;
		CubeRate[2] = 0.01f * x;
		break;

	default :
		CubeRate[0] = 0.01f * y;
		CubeRate[1] = 0.0f;
		CubeRate[2] = 0.01f * x;
		break;

	}
}  */

/*
 * KeybFunc()
 */

void KeybFunc(int button, int state) {
	if(state == 1) {
		key[button] = false;
	} else {
		key[button] = true;
	}
}

/*
 * MouseFunc()
 */

void MouseFunc(int button, int state, int x, int y) {
	if(state) return;

	CubeMouseButton = button;
	CubeMouseX      = x;
	CubeMouseY      = y;

/*	for(int i=0;i<3;i++)
		CubeRate[i] = 0.0f; */
}

/*
 * ReshapeFunc
 */

void ReshapeFunc(int width, int height) {
	CubeWidth  = width;
	CubeHeight = height;
}
