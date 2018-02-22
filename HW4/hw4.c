/*
 *  Deekshitha Thumma 9/28/17
 *  Assignment 4: Projections and Navigation
 *
 *  Draw Art Museum columns, a table with a cool base, and a vase of flowers to demonstrate orthogonal & prespective projections & first person POV
 *
 *  Key bindings:
 *  m          Toggle between orthogonal, perspective, and first person
 *  +/-        Changes field of view for perspective
 *  a          Toggle axes
  *  r		   Toggle autorotate on/off
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 *
 * Resources: Used ex9 and ex8 code as template, 
 *            https: //github.com/taylorjandrews/CSCI-4229/blob/master/Projections/taan7753hw4.c, 
 *            https: //www.gamedev.net/forums/topic/639798-a-first-person-camera-class-in-opengl-c/,
 *            http: //in2gpu.com/2016/02/26/opengl-fps-camera/
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

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
double zh=0;      //  Rotation using time
int POVth =0;     //  Angle of moving left and right when in First Person
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=8.0;   //  Size of world
double Viewx = 0; //  Where we are pointed x coordinate 
double Viewy = 0; //  Where we are pointed y coordinate
double Viewz = 0; //  Where we are pointed z coordinate
double POVx = 0;  //  Eye x coordinate in First Person (this is where we start off)
double POVy = 1;  //  Eye y coordinate in First Person
double POVz = 5;  //  Eye z coordinate in First Person
double Ex = 0;    //  Eye x coordinate in Perspective
double Ey = 0;    //  Eye y coordinate in Perspective
double Ez = 5;    //  Eye z coordinate in Perspective
double dist=0.05; //  Distance
int autorotate=0; //if you want rotation or not

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))

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
 *  Draw vertex in polar coordinates
 */
static void Vertex(double th,double ph)
{
   glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   glVertex3d(Sin(th)*Cos(ph)*1.5 , Sin(ph)*1.5 , Cos(th)*Cos(ph)*1.5);
}

/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection
   if (mode == 0)
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Perspective transformation or First person
   else if (mode == 1 || mode == 2)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
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
   if(autorotate)
   {
		glRotatef(zh,0,1,0);
   }
   glScaled(r,r,r);

   //put at 0 first
   glPushMatrix();
   glTranslated(0,1,0);
   //create base cube
   cube(0);
   //create mini embellishment cylinder on top
   circle(0);
   //create top to the mini cylinder
   minicylinder(0);
   //create main column
   cylinder();
   //create bottom to the mini cylinder on top of column
   minicylinder(1);
   //create mini embellishment cylinder on top of column
   circle(1);
   //create top cube
   cube(1);
   glPopMatrix();

   //  Undo transformations
   glPopMatrix();
   errCheck("Column");
}

/*
 *  Draw table top
 */
static void tabletop()
{
   glBegin(GL_QUADS);
	//  Front
   glColor3f(1,1,0);
   glVertex3f(-1.5,+1, 1.5);
   glVertex3f(+1.5,+1, 1.5);
   glVertex3f(+1.5,+1.2, 1.5);
   glVertex3f(-1.5,+1.2, 1.5);
   //  Back
   glColor3f(1,1,0);
   glVertex3f(+1.5,+1,-1.5);
   glVertex3f(-1.5,+1,-1.5);
   glVertex3f(-1.5,+1.2,-1.5);
   glVertex3f(+1.5,+1.2,-1.5);
   //  Right
   glColor3f(1,1,0);
   glVertex3f(+1.5,+1,+1.5);
   glVertex3f(+1.5,+1,-1.5);
   glVertex3f(+1.5,+1.2,-1.5);
   glVertex3f(+1.5,+1.2,+1.5);
   //  Left
   glColor3f(1,1,0);
   glVertex3f(-1.5,+1,-1.5);
   glVertex3f(-1.5,+1,+1.5);
   glVertex3f(-1.5,+1.2,+1.5);
   glVertex3f(-1.5,+1.2,-1.5);
   //  Top
   glColor3f(.75,.0,1);
   glVertex3f(-1.5,+1.2,+1.5);
   glVertex3f(+1.5,+1.2,+1.5);
   glVertex3f(+1.5,+1.2,-1.5);
   glVertex3f(-1.5,+1.2,-1.5);
   //  Bottom
   glColor3f(.75,.0,1);
   glVertex3f(-1.5,+1,-1.5);
   glVertex3f(+1.5,+1,-1.5);
   glVertex3f(+1.5,+1,+1.5);
   glVertex3f(-1.5,+1,+1.5);

	//  End
	glEnd();
	errCheck("Tabletop");
}

/*
 *  Draw table bottom, cool design
 */
static void tablebottom()
{
   //jump in degrees
   const int d=90;
   int th;

   //square at the bottom of table bottom structure
   glBegin(GL_QUADS);
   glColor3f(1,1,0);
   glVertex3f(Cos(0), 0, Sin(0));
   glVertex3f(Cos(90), 0, Sin(90));
   glVertex3f(Cos(180), 0, Sin(180));
   glVertex3f(Cos(270), 0, Sin(270));
   glEnd();

   //table bottom cool structure
   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=d)
   {
			//Color to make curved surface look more "3D"
			glColor3f(1 , Cos(th), Sin(th));
			glVertex3f(Cos(th+45), 1, Sin(th+45));
			glVertex3f( Cos(th), 0, Sin(th));
   }
   glEnd();

   errCheck("Tablebottom");
}

/*
 *  Draw a table
 *     at (x,y,z)
 *     radius (r)
 *     rotated (rot) about the z axis
 */
static void table(double x,double y,double z,double r, double rot)
{
   //  Save transformation
   glPushMatrix();

   //  Offset
   glTranslated(x,y,z);
   glRotated(rot,0,0,1);
   if(autorotate)
   {
		glRotatef(zh,0,1,0);
   }
   glScaled(r,r,r);

   //create table top
   tabletop();
   tablebottom();

   //  Undo transformations
   glPopMatrix();
   errCheck("Table");
}

/*
 *  Draw vase (for now its a sphere)
 */
static void vase()
{
      const int d=5;
   int th,ph;

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
}

   errCheck("Vase");
}
/*
 *  Draw flower
 */
static void flower()
{
	
   const int d=5;
   int th,ph;
   glPushMatrix();
   glTranslated(0,3,0);
   //  semi sphere 
   for (ph=0;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
		 glColor3f(Cos(th) , 1 , Sin(th)); 
         glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
		 glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
      }
      glEnd();
   }
   glPopMatrix();

   //top petals  
   glColor3f(1.000, 0.271, 0.000);
   glBegin(GL_TRIANGLES);
    for (th=36;th<=396;th+=72)
    {
		glVertex3f( Cos(th), 3.02, Sin(th));
		glVertex3f( Cos(th+36)*1.5, 3.02, Sin(th+36)*1.5);
		glVertex3f( Cos(th+72), 3.02, Sin(th+72));
    }
	glEnd();

   //bottom petals 
   glColor3f(1,0,1);
   glBegin(GL_TRIANGLES);
    for (th=0;th<=360;th+=72)
    {
		glVertex3f( Cos(th), 3, Sin(th));
		glVertex3f( Cos(th+36)*2, 3, Sin(th+36)*2);
		glVertex3f( Cos(th+72), 3, Sin(th+72));	
    }
	glEnd();
   

    //Draw circle to cover bottom of circle
    glColor3f(0,1,0);
	glBegin(GL_POLYGON);
	for (th=0;th<=360;th+=d)
	{
		glVertex3f(Cos(th), 3, Sin(th));
	}
	glEnd();

   //stem 
   glColor3f(0,1,0);
   glBegin(GL_LINES);
   glVertex3f(0, 0, 0);
   glVertex3f(0, 3, 0);
   glEnd();

   errCheck("Flower");
}

/*
 *  Draw a flower vase
 *     at (x,y,z)
 *     radius (r)
 *     rotated (rot) about the z axis
 */
static void flowervase(double x,double y,double z,double r, double rot)
{
   //  Save transformation
   glPushMatrix();

   //  Offset
   glTranslated(x,y,z);
   glRotated(rot,0,0,1);
   if(autorotate)
   {
		glRotatef(zh,0,1,0);
   }
   glScaled(r,r,r);

   //create vase
   vase();

   //create flowers
   flower();
   glPushMatrix();
   glRotated(60,0,0,1);
   flower();
   glPopMatrix();
   glPushMatrix();
   glRotated(60,1,0,0);
   flower();
   glPopMatrix();
   glPushMatrix();
   glRotated(-60,0,0,1);
   flower();
   glPopMatrix();
   glPushMatrix();
   glRotated(-60,1,0,0);
   flower();
   glPopMatrix();

   //  Undo transformations
   glPopMatrix();
   errCheck("FlowerVase");
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
   //  Orthogonal - set world orientation
   if (mode == 0)
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
   //  Perspective - set eye position
   else if (mode == 1)
   {
      Ex = -2*dim*Sin(th)*Cos(ph);
      Ey = +2*dim        *Sin(ph);
      Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  First person = set "where we are looking" position based on the rotation user made
   else if (mode == 2)
   {
	  Viewx = +2*dim*Sin(POVth); 
      Viewz = -2*dim*Cos(POVth);
      gluLookAt(POVx,POVy,POVz, Viewx+POVx,POVy,Viewz+POVz, 0,1,0); 
   }
   //  Draw columns
   column(0,0,0,.4,0);
   column(5,0,0,.4,0);
   column(0,0,5,.4,0);
   column(5,0,5,.4,0);
   //  Table
   table(2.5,0,2.5,.4,0);
   //  Vase of flowers
   flowervase(2.5,(.4*1.2)+(.15),2.5,.1,0);

   //  Draw axes
   glColor3f(1,1,1);
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
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d",th,ph,dim,fov);
   if (mode == 0) Print(" Projection(m) = Orthoginal");
   else if (mode == 1) Print(" Projection(m) = Perspective");
   else if (mode == 2) Print(" Projection(m) = First Person");
   if (!autorotate) {Print(" Autorotate(r) is OFF");}
   else if (autorotate) Print(" Autorotate(r) is ON");
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
   errCheck("Display");
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
	//if in first person view
	if(mode ==2)
	{
		//  Right arrow key - turn right angle by 5 degrees
	   if (key == GLUT_KEY_RIGHT)
		  POVth += 5;
	   //  Left arrow key - turn left angle by 5 degrees
	   else if (key == GLUT_KEY_LEFT)
		  POVth -= 5;
	   //  Up arrow key - move eye placement vector forward by camera placement 
	   else if (key == GLUT_KEY_UP)
	   {
		  POVx += Viewx*dist; 
          POVz += Viewz*dist;
	   }
	   //  Down arrow key - move eye placement vector back by camera placement 
	   else if (key == GLUT_KEY_DOWN)
	   {
		  POVx -= Viewx*dist;
          POVz -= Viewz*dist;
	   }
	}
	//if not in first person view
	else
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
	   //  PageUp key - increase dim
	   else if (key == GLUT_KEY_PAGE_UP)
		  dim += 0.1;
	   //  PageDown key - decrease dim
	   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
		  dim -= 0.1;
   }
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   POVth %= 360;
   //  Update projection
   Project();
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
   else if (ch == '0'){
      th = ph = 0;
	  dim = 8.0;
	  }
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
      mode = (mode+1)%3;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   else if (ch == 'r')
	  autorotate = 1 - autorotate;
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
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
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Assignment 4: Deekshitha Thumma");
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
