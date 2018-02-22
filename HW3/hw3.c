/*
 *  Deekshitha Thumma 9/24/17
 *  Assignment 3: Scene in 3D
 *
 *  Display Scene in 3D: Art Museum columns
 *
 *
 *  Key bindings:
 *  m		   Toggle state (show all columns, or one at a time)
 *  a          Toggle axes
 *  r		   Toggle autorotate on/off
 *  arrows     Change view angle
 *  0          Reset view angle
 *  ESC        Exit
 *
 *  Resources: Used ex8 code as template
 *            
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
double zh=0;      //  Rotation using time
int axes=1;       //  Display axes
int mode=0;       //  What to display
int autorotate=1; //if you want rotation or not

//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Error check function
 *  
 */
void errCheck(char* where){
	int err = glGetError();
	if(err) 
	{
		fprintf(stderr, "ERROR: %s [%s]\n",gluErrorString(err),where);
	}
}
/*
 *  Draw a cube
 *  (bisTop) = 1 if the cube is the top one and 0 if the bottom
 */
static void cube(int bisTop)
{
	//Set purple color
   glColor3f(.75,0,1);

   //If the cube we are drowing is on top
   if(bisTop == 1)
	{
	   glBegin(GL_QUADS);
	   //  Front
	   glVertex3f(-1.5,+3.5, 1.5);
	   glVertex3f(+1.5,+3.5, 1.5);
	   glVertex3f(+1.5,+4, 1.5);
	   glVertex3f(-1.5,+4, 1.5);
	   //  Back
	   glVertex3f(+1.5,+4,-1.5);
	   glVertex3f(-1.5,+4,-1.5);
	   glVertex3f(-1.5,+3.5,-1.5);
	   glVertex3f(+1.5,+3.5,-1.5);
	   //  Right
	   glVertex3f(+1.5,+3.5,+1.5);
	   glVertex3f(+1.5,+3.5,-1.5);
	   glVertex3f(+1.5,+4,-1.5);
	   glVertex3f(+1.5,+4,+1.5);
	   //  Left
	   glVertex3f(-1.5,+3.5,-1.5);
	   glVertex3f(-1.5,+3.5,+1.5);
	   glVertex3f(-1.5,+4,+1.5);
	   glVertex3f(-1.5,+4,-1.5);
	   //  Top
	   glVertex3f(-1.5,+4,+1.5);
	   glVertex3f(+1.5,+4,+1.5);
	   glVertex3f(+1.5,+4,-1.5);
	   glVertex3f(-1.5,+4,-1.5);
	   //  Bottom
	   glVertex3f(-1.5,+3.5,-1.5);
	   glVertex3f(+1.5,+3.5,-1.5);
	   glVertex3f(+1.5,+3.5,+1.5);
	   glVertex3f(-1.5,+3.5,+1.5);

	   //  End
	   glEnd();
	}

	//if the cube we are drawing is on the bottom
	else if(bisTop == 0)
	{
	   glBegin(GL_QUADS);
	   //  Front
	   glVertex3f(-1.5,-1, 1.5);
	   glVertex3f(+1.5,-1, 1.5);
	   glVertex3f(+1.5,-.5, 1.5);
	   glVertex3f(-1.5,-.5, 1.5);
	   //  Back
	   glVertex3f(+1.5,-1,-1.5);
	   glVertex3f(-1.5,-1,-1.5);
	   glVertex3f(-1.5,-.5,-1.5);
	   glVertex3f(+1.5,-.5,-1.5);
	   //  Right
	   glVertex3f(+1.5,-1,+1.5);
	   glVertex3f(+1.5,-1,-1.5);
	   glVertex3f(+1.5,-.5,-1.5);
	   glVertex3f(+1.5,-.5,+1.5);
	   //  Left
	   glVertex3f(-1.5,-1,-1.5);
	   glVertex3f(-1.5,-1,+1.5);
	   glVertex3f(-1.5,-.5,+1.5);
	   glVertex3f(-1.5,-.5,-1.5);
	   //  Top
	   glVertex3f(-1.5,-.5,+1.5);
	   glVertex3f(+1.5,-.5,+1.5);
	   glVertex3f(+1.5,-.5,-1.5);
	   glVertex3f(-1.5,-.5,-1.5);
	   //  Bottom
	   glVertex3f(-1.5,-1,-1.5);
	   glVertex3f(+1.5,-1,-1.5);
	   glVertex3f(+1.5,-1,+1.5);
	   glVertex3f(-1.5,-1,+1.5);

	   //  End
	   glEnd();
	}
	errCheck("Cube");
}

/*
 *  Draw a tall cylinder 
 */
static void cylinder()
{
   //jump in degrees
   const int d=5;
   int th;

    //Form cylinder
    glBegin(GL_QUAD_STRIP);
	//loop through angles until you reach 360 degrees
    for (th=0;th<=360;th+=d)
    {
			//Color to make curved surface look more "3D"
		    glColor3f(1 , Cos(th)*.5, Sin(th)*.5);
            glVertex3f(Cos(th), 3, Sin(th));
            glVertex3f( Cos(th), 0, Sin(th));
    }
    glEnd();
	errCheck("Cylinder");
}

/*
 *  Draw a short fatter cylinder 
 *  (bisTop) = 1 if the cylinder is the top one and 0 if the bottom
 */
static void minicylinder(int bisTop)
{
   //jump in degrees
   const int d=5;
   int th;

   //if top cylinder
   if(bisTop == 1){
		//Form cylinder
		glBegin(GL_QUAD_STRIP);
		//loop through angles until you reach 360 degrees
		for (th=0;th<=360;th+=d)
		{
				//Color to make curved surface look more "3D"
				glColor3f(1 , Cos(th)*.5, Sin(th)*.5);
				glVertex3f(Cos(th)*1.25, 3.5, Sin(th)*1.25);
				glVertex3f( Cos(th)*1.25, 3, Sin(th)*1.25);
		}
		glEnd();
	}
	//if bottom cylinder
	else if(bisTop == 0){
		//Form cylinder
		glBegin(GL_QUAD_STRIP);
		//loop through angles until you reach 360 degrees
		for (th=0;th<=360;th+=d)
		{
				//Color to make curved surface look more "3D"
				 glColor3f(1 , Cos(th)*.5, Sin(th)*.5);
				glVertex3f(Cos(th)*1.25, 0, Sin(th)*1.25);
				glVertex3f( Cos(th)*1.25, -.5, Sin(th)*1.25);
		}
		glEnd();
	}
	errCheck("Minicylinder");
}

/*
 *  Draw a circle 
 *  (bisTop) = 1 if the circle is the top one and 0 if the bottom
 */
static void circle(int bisTop)
{
   //jump in degrees
   const int d=1;
   int th;  

   //Set purple color
   glColor3f(.75,.0,1);

   //if top circle
   if(bisTop == 1)
   {
	    //Draw circle
		glBegin(GL_POLYGON);
		//loop through angles until you reach 360 degrees
		for (th=0;th<=360;th+=d)
		{
				glVertex3f(Cos(th)*1.25, 3, Sin(th)*1.25);
		}
		glEnd();
   }

   //If bottom circle
   if(bisTop == 0)
   {
	    //Draw circle
		glBegin(GL_POLYGON);
		//loop through angles until you reach 360 degrees
		for (th=0;th<=360;th+=d)
		{
				glVertex3f(Cos(th)*1.25, 0, Sin(th)*1.25);
		}
		glEnd();
   }
   errCheck("Circle");
}

/*
 *  Draw a column
 *     at (x,y,z)
 *     radius (r)
 *     rotated (rot) about the z axis
 */
static void column(double x,double y,double z,double r, double rot)
{
   //  Save transformation
   glPushMatrix();

   //  Offset
   glTranslated(x,y,z);
   glRotated(rot,0,0,1);
   if(autorotate ==1)
   {
		glRotatef(zh,0,1,0);
   }
   glScaled(r,r,r);


   //create base cube
   cube(0);
   //create mini embellishment cylinder on top
   circle(0);
   //create top to the mini cylinder
   minicylinder(0);
   //create main column
   cylinder();
   //create bttom to the mini cylinder on top of column
   minicylinder(1);
   //create mini embellishment cylinder on top of column
   circle(1);
   //create top cube
   cube(1);

   //  Undo transformations
   glPopMatrix();
   errCheck("Column");
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Set view angle
   glRotatef(ph,1,0,0);
   glRotatef(th,0,1,0);

   //First find the mode
   switch (mode)
   {
      //  Draw all columns
      case 0:
         column(0.0, 0.0, 0.0, 0.2, 0.0);
		 column(1.0, 0.0, 0.0, 0.2, 0.0);
		 column(0.0, 0.0, 1.0, 0.07, 0.0);
		 column(1.0, 0.0, 0.7, 0.2, 45);
         break;
      //  Draw normal column
      case 1:
         column(.5,0,.5,.2,0);
         break;
      //  Draw translated column
      case 2:
          column(1,0,0,.2,0);
         break;
      //  Draw scaled column
      case 3:
		 column(.5,0,.5,.07,0);
         break;
      //  Draw rotated column
      case 4:
         column(.5,0,.5,.2,45);
         break;
	}

   //  White
   glColor3f(1,1,1);
   //  Draw axes
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Five pixels from the lower left corner of the window
   glWindowPos2i(5,5);

   //  Print the text string
   Print("Angle=%d,%d",th,ph);
   if (!autorotate) {Print("  Autorotate is OFF");}
   else if (autorotate) Print("  Autorotate is ON");
   if (mode ==0) Print("  Mode: All columns");
   else if (mode ==1) Print("  Mode: Original column");
   else if (mode ==2) Print("  Mode: Translated column");
   else if (mode ==3) Print("  Mode: Scaled column");
   else if (mode ==4) Print("  Mode: Rotated (y) column");
   //  Render the scene
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
   errCheck("Display");
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
	// Toggle mode
   else if (ch == 'm')
      mode = (mode+1)%5;
   else if (ch == 'r')
	  autorotate = 1 - autorotate;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   const double dim=2.5;
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection
   glOrtho(-w2h*dim,+w2h*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  GLUT calls this routine when there is nothing else to do
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360);
   glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(600,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Assignment 3: Deekshitha Thumma");
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
