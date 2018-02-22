/*
 *  Deekshitha Thumma 9/14/17
 *  Assignment 2: Lorenz Attractor
 *
 *  Display The Lorenz graph
 *
 *  Key bindings:
 *  s      Increase 's' Lorenz parameter 
 *  a      Decrease 's' Lorenz parameter 
 *  b      Increase 'b' Lorenz parameter
 *  v      Decrease 'b' Lorenz parameter 
 *  r      Increase 'r' Lorenz parameter
 *  e      Decrease 'r' Lorenz parameter 
 *  z      Change Graph color scheme 
 *  arrows Change view angle
 *  0      Reset view angle
 *  x      Reset graph parameters
 *  ESC    Exit
 *
 *  Resources: Used ex6 code as template 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//  Globals
int th=0;          // Azimuth of view angle
int ph=0;          // Elevation of view angle
double dim=2;      // Dimension of orthogonal box
double s  = 10;    //Lorenz parameter s
double b  = 2.6666;//Lorenz parameter b
double r  = 28;    //Lorenz parameter r
double red = 1.0;  //Red color value
double green = 0.0;//Green color value
double blue = 1.0; //Blue color value


#define LEN 8192  // Maximum length of text string
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
 *  Display the scene
 */
void display()
{
   //  Clear the image
   glClear(GL_COLOR_BUFFER_BIT);
   //  Reset previous transforms
   glLoadIdentity();
   //  Set view angle, order of these 2 lines matter
   glRotated(ph,1,0,0); //rotate around x axis
   glRotated(th,0,1,0); //rotate around y axis

   //Set color to the random colors selected
   glColor3f(red,green,blue);
   glPointSize(1);
   glBegin(GL_LINE_STRIP);

   /* Begin calculations */
   int i;
   //Coordinates
   double x = 1;
   double y = 1;
   double z = 1;
   //Time step 
   double dt = 0.001;


   for (i=0;i<50000;i++)
   {
      double dx = s*(y-x);
      double dy = x*(r-z)-y;
      double dz = x*y - b*z;
      x += dt*dx;
      y += dt*dy;
      z += dt*dz;
	  
	  //Change color depending on the number of vertex to look cool
	  if (i > 10000 && i <= 20000) glColor3f(.75,green,.75);
	  else if (i > 20000 && i <= 30000) glColor3f(red,.50,.50);
	  else if (i > 30000 && i <= 40000) glColor3f(.25,.25,blue);
	  else if (i > 40000 && i < 50000) glColor3f(red*.10,green,blue*.60);

	  //Insert vertex but scale down to fit our axis nicely
      glVertex3d(x/50,y/50,z/50);
   }
   /*End calculations*/

   glEnd();
   //  Draw axes in white
   glColor3f(1,1,1);
   glBegin(GL_LINES);//line between 1st and 2nd points, 3rd and 4th point, 5th to 6th etc
   glVertex3d(0,0,0); //x axis drawing
   glVertex3d(1,0,0);
   glVertex3d(0,0,0); //y axis drawing
   glVertex3d(0,1,0);
   glVertex3d(0,0,0);//z axis drawing
   glVertex3d(0,0,1);
   glEnd();
   //  Label axes
   glRasterPos3d(1,0,0);//where you want to write "X"
   Print("X");
   glRasterPos3d(0,1,0);
   Print("Y");
   glRasterPos3d(0,0,1);
   Print("Z");
   //  Display parameters
   glWindowPos2i(5,5); //Where to write the print statemnt
   Print("View Angle=%d,%d  S=%f B=%f R=%f",th,ph,s,b,r);
   //  Flush and swap
   glFlush();
   glutSwapBuffers();
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

   //  Increase s by 1
   else if (ch == 's')
   {
      s = s + 1.0;
   }
    // Decrease s by 1
   else if (ch == 'a')
   {
      s = s - 1.0;
   }
   //  Increase b by 0.1
   else if (ch == 'b')
   {
      b = b + 0.1;
   }
   //  Decrease b by 0.1
   else if (ch == 'v')
   {
      b = b - 0.1;
   }
   //  Increase r by 1
   else if (ch == 'r')
   {
      r = r+ 1.0;
   }
    // Decrease r by 1
   else if (ch == 'e')
   {
		r = r - 1.0;
   }
   //Reset parameters back to default
   else if (ch == 'x')
   {
		s  = 10;
		b  = 2.6666;
		r  = 28;  
   }
   // Select random color
   else if (ch == 'z')
   {
	   red = (rand() % 100);
	   green = (rand() % 100);
	   blue = (rand() % 100);
	   red = red / 100;
	   blue = blue /100;
	   green = green /100;
   }

   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
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
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)//if you resize window, dont distort the object
{
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection box adjusted for the
   //  aspect ratio of the window
   glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
  //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(500,500);
   //  Create the window
   glutCreateWindow("Assignment 2: Deekshitha Thumma");
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
   //  Return code
   return 0;
}
