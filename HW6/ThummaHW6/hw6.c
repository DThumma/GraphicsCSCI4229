/*
 *  Deekshitha Thumma 10/19/17
 *  Assignment 6: Lighting & Textures
 *
 *  Draw Art Museum columns, a table with a cool base, and a vase of flowers to demonstrate textures
 *
 *  Key bindings:
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode
 *  F3         Toggle light distance (1/5)
 *  F8         Change ball increment
 *  F9         Invert bottom normal
 *  m          Toggles light movement
 *  </>        Move light manually
 *  []         Lower/rise light
 *  p          Toggles ortogonal/perspective projection
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 *
 *
 * Resources: Used ex15 & ex16 code as template and used HW5 objects & lighting
 */
#include "CSCIx229.h"

int axes=0;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int POVth =0;     //  Angle of moving left and right when in First Person
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world

//Projection values
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

// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   1;  // Elevation of light (how high the light is)

//texture
unsigned int texture[10]; // Texture names

/*
 *  Draw a cube
 *  (bisTop) = 1 if the cube is the top one and 0 if the bottom
 */
static void cube(int bisTop)
{
   //Set purple color
   glColor3f(.75,0,1);

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[0]); //marble texture, never do this between a glbegin and an glend 

   if(bisTop == 1)
	{
	   glBegin(GL_QUADS);
	   //  Front
	   glNormal3f( 0, 0, 1);
	   glTexCoord2f(0,0); glVertex3f(-1.5,+3.5, 1.5);
	   glTexCoord2f(1,0); glVertex3f(+1.5,+3.5, 1.5);
	   glTexCoord2f(1,.17); glVertex3f(+1.5,+4, 1.5);
	   glTexCoord2f(0,.17); glVertex3f(-1.5,+4, 1.5);
	   //  Back
	   glNormal3f( 0, 0,-1);
	   glTexCoord2f(0,0); glVertex3f(+1.5,+4,-1.5);
	   glTexCoord2f(1,0); glVertex3f(-1.5,+4,-1.5);
	   glTexCoord2f(1,.17); glVertex3f(-1.5,+3.5,-1.5);
	   glTexCoord2f(0,.17); glVertex3f(+1.5,+3.5,-1.5);
	   //  Right
	   glNormal3f(+1, 0, 0);
	   glTexCoord2f(0,0); glVertex3f(+1.5,+3.5,+1.5);
	   glTexCoord2f(1,0); glVertex3f(+1.5,+3.5,-1.5);
	   glTexCoord2f(1,.17); glVertex3f(+1.5,+4,-1.5);
	   glTexCoord2f(0,.17); glVertex3f(+1.5,+4,+1.5);
	   //  Left
	   glNormal3f(-1, 0, 0);
	   glTexCoord2f(0,0); glVertex3f(-1.5,+3.5,-1.5);
	   glTexCoord2f(1,0); glVertex3f(-1.5,+3.5,+1.5);
	   glTexCoord2f(1,.17); glVertex3f(-1.5,+4,+1.5);
	   glTexCoord2f(0,.17); glVertex3f(-1.5,+4,-1.5);
	   //  Top
	   glNormal3f( 0,+1, 0);
	   glTexCoord2f(0,0);glVertex3f(-1.5,+4,+1.5);
	   glTexCoord2f(1,0);glVertex3f(+1.5,+4,+1.5);
	   glTexCoord2f(1,1);glVertex3f(+1.5,+4,-1.5);
	   glTexCoord2f(0,1);glVertex3f(-1.5,+4,-1.5);
	   //  Bottom
	   glNormal3f( 0,-1, 0);
	   glTexCoord2f(0,0);glVertex3f(-1.5,+3.5,-1.5);
	   glTexCoord2f(1,0);glVertex3f(+1.5,+3.5,-1.5);
	   glTexCoord2f(1,1);glVertex3f(+1.5,+3.5,+1.5);
	   glTexCoord2f(0,1);glVertex3f(-1.5,+3.5,+1.5);

	   //  End
	   glEnd();
	}

	//if the cube we are drawing is on the bottom
	else if(bisTop == 0)
	{
	   glBegin(GL_QUADS);
	   //  Front
	   glNormal3f( 0, 0, 1);
	   glTexCoord2f(0,0); glVertex3f(-1.5,-1, 1.5);
	   glTexCoord2f(1,0); glVertex3f(+1.5,-1, 1.5);
	   glTexCoord2f(1,.17); glVertex3f(+1.5,-.5, 1.5);
	   glTexCoord2f(0,.17); glVertex3f(-1.5,-.5, 1.5);
	   //  Back
	   glNormal3f( 0, 0,-1);
	   glTexCoord2f(0,0); glVertex3f(+1.5,-1,-1.5);
	   glTexCoord2f(1,0); glVertex3f(-1.5,-1,-1.5);
	   glTexCoord2f(1,.17); glVertex3f(-1.5,-.5,-1.5);
	   glTexCoord2f(0,.17); glVertex3f(+1.5,-.5,-1.5);
	   //  Right
	   glNormal3f(+1, 0, 0);
	   glTexCoord2f(0,0);glVertex3f(+1.5,-1,+1.5);
	   glTexCoord2f(1,0);glVertex3f(+1.5,-1,-1.5);
	   glTexCoord2f(1,.17);glVertex3f(+1.5,-.5,-1.5);
	   glTexCoord2f(0,.17);glVertex3f(+1.5,-.5,+1.5);
	   //  Left
	   glNormal3f(-1, 0, 0);
	   glTexCoord2f(0,0);glVertex3f(-1.5,-1,-1.5);
	   glTexCoord2f(1,0);glVertex3f(-1.5,-1,+1.5);
	   glTexCoord2f(1,.17);glVertex3f(-1.5,-.5,+1.5);
	   glTexCoord2f(0,.17);glVertex3f(-1.5,-.5,-1.5);
	   //  Top
	   glNormal3f( 0,+1, 0);
	   glTexCoord2f(0,0);glVertex3f(-1.5,-.5,+1.5);
	   glTexCoord2f(1,0);glVertex3f(+1.5,-.5,+1.5);
	   glTexCoord2f(1,1);glVertex3f(+1.5,-.5,-1.5);
	   glTexCoord2f(0,1);glVertex3f(-1.5,-.5,-1.5);
	   //  Bottom
	   glNormal3f( 0,-1, 0);
	   glTexCoord2f(0,0);glVertex3f(-1.5,-1,-1.5);
	   glTexCoord2f(1,0);glVertex3f(+1.5,-1,-1.5);
	   glTexCoord2f(1,1);glVertex3f(+1.5,-1,+1.5);
	   glTexCoord2f(0,1);glVertex3f(-1.5,-1,+1.5);

	   //  End
	   glEnd();
	}

	glDisable(GL_TEXTURE_2D);
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
   
   //set color
      glColor3f(1,1,1);

   //set ligthing values
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[0]); 

   //if top circle
   if(bisTop == 1)
   {
	    //Draw circle
		glBegin(GL_POLYGON);
		//loop through angles until you reach 360 degrees
		for (th=0;th<=360;th+=d)
		{
				glNormal3f( 0,+1, 0);
				glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
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
				glNormal3f( 0,+1, 0);
				glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
				glVertex3f(Cos(th)*1.25, 0, Sin(th)*1.25);
		}
		glEnd();
   }
   glDisable(GL_TEXTURE_2D);
   ErrCheck("Circle");
}

/*
 *  Draw a tall cylinder 
 */
static void cylinder()
{
   //jump in degrees
   const int d=5;
   int th;

   //set color
   glColor3f(.75,0,1);

   //set ligthing values
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[0]); //marble texture, never do this between a glbegin and an glend 
   
    //Form cylinder
    glBegin(GL_QUAD_STRIP);
	//loop through angles until you reach 360 degrees
    for (th=0;th<=360;th+=d)
    {
			glNormal3d(Cos(th), 0, Sin(th));
			glTexCoord2f(th/360.0,0);
            glVertex3f(Cos(th), 3, Sin(th));
			glTexCoord2f(th/360.0,1);
            glVertex3f( Cos(th), 0, Sin(th));
    }
    glEnd();
	glDisable(GL_TEXTURE_2D);
	ErrCheck("Cylinder");
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

   //set color
   glColor3f(1, 1, 1);

   //set ligthing values
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[7]); 

   //if top cylinder
   if(bisTop == 1){
		//Form cylinder
		glBegin(GL_QUAD_STRIP);
		//loop through angles until you reach 360 degrees
		for (th=0;th<=360;th+=d)
		{
				glNormal3d(Cos(th)*1.25, 0, Sin(th)*1.25);
				glTexCoord2f(th/360.0,0);
				glVertex3f(Cos(th)*1.25, 3.5, Sin(th)*1.25);
				glTexCoord2f(th/360.0,.125);
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
				glNormal3d(Cos(th)*1.25, 0, Sin(th)*1.25);
				glTexCoord2f(th/360.0,0);
				glVertex3f(Cos(th)*1.25, 0, Sin(th)*1.25);
				glTexCoord2f(th/360.0,.125);
				glVertex3f( Cos(th)*1.25, -.5, Sin(th)*1.25);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	ErrCheck("Minicylinder");
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
   ErrCheck("Column");
}

/*
 *  Draw table top
 */
static void tabletop()
{
   //set ligthing values
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);


   //set color
   glColor3f(1,1,1);

    //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[7]); 

   glBegin(GL_QUADS);
	//  Front
   glNormal3f( 0, 0, 1); 
   glTexCoord2f(0,0); glVertex3f(-1.5,+1, 1.5);
   glTexCoord2f(1,0); glVertex3f(+1.5,+1, 1.5);
   glTexCoord2f(1,.17); glVertex3f(+1.5,+1.2, 1.5);
   glTexCoord2f(0,.17); glVertex3f(-1.5,+1.2, 1.5);
   //  Back
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1.5,+1,-1.5);
   glTexCoord2f(1,0); glVertex3f(-1.5,+1,-1.5);
   glTexCoord2f(1,.17); glVertex3f(-1.5,+1.2,-1.5);
   glTexCoord2f(0,.17); glVertex3f(+1.5,+1.2,-1.5);
   //  Right
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1.5,+1,+1.5);
   glTexCoord2f(1,0); glVertex3f(+1.5,+1,-1.5);
   glTexCoord2f(1,.17); glVertex3f(+1.5,+1.2,-1.5);
   glTexCoord2f(0,.17); glVertex3f(+1.5,+1.2,+1.5);
   //  Left
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1.5,+1,-1.5);
   glTexCoord2f(1,0); glVertex3f(-1.5,+1,+1.5);
   glTexCoord2f(1,.17); glVertex3f(-1.5,+1.2,+1.5);
   glTexCoord2f(0,.17); glVertex3f(-1.5,+1.2,-1.5);

   //  End
	glEnd();
	glDisable(GL_TEXTURE_2D);

    //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[2]); 

   glBegin(GL_QUADS);

   //  Top
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0);glVertex3f(-1.5,+1.2,+1.5);
   glTexCoord2f(1,0);glVertex3f(+1.5,+1.2,+1.5);
   glTexCoord2f(1,1);glVertex3f(+1.5,+1.2,-1.5);
   glTexCoord2f(0,1);glVertex3f(-1.5,+1.2,-1.5);
   //  Bottom
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0);glVertex3f(-1.5,+1,-1.5);
   glTexCoord2f(1,0);glVertex3f(+1.5,+1,-1.5);
   glTexCoord2f(1,1);glVertex3f(+1.5,+1,+1.5);
   glTexCoord2f(0,1);glVertex3f(-1.5,+1,+1.5);

	//  End
	glEnd();
	glDisable(GL_TEXTURE_2D);
	ErrCheck("Tabletop");
}

/*
 *  Draw table bottom, cool design
 */
static void tablebottom()
{
   //jump in degrees
   const int d=90;
   int th;


   //set ligthing values
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Enable textures for bottom slab of bottom of table
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[8]);

   //square at the bottom of table bottom structure
   glBegin(GL_QUADS);
   //glColor3f(0.098, 0.098, 0.439);
   glColor3f(1,1,1);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,0); glVertex3f(Cos(0), 0, Sin(0));
   glTexCoord2f(0,1); glVertex3f(Cos(90), 0, Sin(90));
   glTexCoord2f(1,0);glVertex3f(Cos(180), 0, Sin(180));
   glTexCoord2f(1,1);glVertex3f(Cos(270), 0, Sin(270));
   glEnd();

   //Disbale textures
   glDisable(GL_TEXTURE_2D);

   //set color
   glColor3f(1,.5,1);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[3]);

   //table bottom cool structure
   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=d)
   {
		glNormal3d(Cos(th+45), 0, Sin(th+45));
		glTexCoord2f((th+45)/360.0,1);
		glVertex3f(Cos(th+45), 1, Sin(th+45));
		glNormal3d(Cos(th), 0, Sin(th));
		glTexCoord2f((th)/360.0,0);
		glVertex3f( Cos(th), 0, Sin(th));
   }
   glEnd();
   glDisable(GL_TEXTURE_2D);
   ErrCheck("Tablebottom");
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
   glScaled(r,r,r);

   //create table top
   tabletop();
   tablebottom();

   //  Undo transformations
   glPopMatrix();
   ErrCheck("Table");
}

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph, int bisSemi)
{
   if(!bisSemi)
   {
	   double x = -Sin(th)*Cos(ph);
	   double y = Cos(th)*Cos(ph);
	   double z =         Sin(ph);
	   //  For a sphere at the origin, the position
	   //  and normal vectors are the same
	   glNormal3d(x,y,z);
	   glVertex3d(x,y,z);
   }
   else
   {
	   double x = -Sin(th)*Cos(ph);
	   double z = Cos(th)*Cos(ph);
	   double y =         Sin(ph);
	   //  For a sphere at the origin, the position
	   //  and normal vectors are the same
	   glNormal3d(x,y,z);
	   glVertex3d(x,y,z);
   }
   
}

/*
 *  Draw vase (for now its a sphere)
 */
static void vase()
{
   const int d=5;
   int th,ph;

   //set ligthing values
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //set color
   glColor3f(1,1,1);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[4]); 

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
	     glTexCoord2f(th/360.0,ph/180.0);
         Vertex(th,ph,0);
		 glTexCoord2f(th/360.0,ph/180.0+0.05);
         Vertex(th,ph+d,0);
	  }
	  glEnd();
   }
   glDisable(GL_TEXTURE_2D);
   ErrCheck("Vase");
}

/*
 *  Draw flower
 */
static void flower()
{
	
   const int d=5;
   int th,ph;

   //set ligthing values
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[1]); 
   //  semi sphere 
   glPushMatrix();
   glTranslated(0,3,0);
   glColor3f(1 , 1 , 1); 
   for (ph=0;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
	     glTexCoord2f(th/360.0,ph/360.0);
         Vertex(th,ph,1);
		 glTexCoord2f(th/360.0,(ph+d)/360.0);
         Vertex(th,ph+d,1);
      }
      glEnd();
   }
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[5]); 
   //top petals  
   glColor3f(1,1,1);
   glBegin(GL_TRIANGLES);
    for (th=36;th<=396;th+=72)
    {
		glTexCoord2f(0,0);
		glVertex3f( Cos(th), 3.02, Sin(th));
		glNormal3f(0,1,0);
		glTexCoord2f(.5,.5);
		glVertex3f( Cos(th+36)*1.5, 3.02, Sin(th+36)*1.5);
		glTexCoord2f(1,0);
		glVertex3f( Cos(th+72), 3.02, Sin(th+72));
    }
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//  Enable textures
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[6]); 
   //bottom petals 
   glColor3f(1,1,1);
   glBegin(GL_TRIANGLES);
    for (th=0;th<=360;th+=72)
    {
		glTexCoord2f(0,.5);
		glVertex3f( Cos(th), 3, Sin(th));
		glNormal3f(0,1,0);
		glTexCoord2f(.5,1);
		glVertex3f( Cos(th+36)*2, 3.01, Sin(th+36)*2);
		glTexCoord2f(1,.5);
		glVertex3f( Cos(th+72), 3.01, Sin(th+72));	
    }
	glEnd();
    glDisable(GL_TEXTURE_2D);


	//  Enable textures
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[9]); 

    //Draw circle to cover bottom of circle
    glColor3f(0,0.392,0);
	glBegin(GL_POLYGON);
	for (th=0;th<=360;th+=d)
	{
	    glNormal3f(0,-1,0);
		glTexCoord2f(.5 + Cos(th)*.2,.5 +Sin(th)*.2);
		glVertex3f(Cos(th), 3, Sin(th));
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
   //stem 
   glColor3f(0,0.392,0);
   glBegin(GL_LINES);
   glVertex3f(0, 0, 0);
   glVertex3f(0, 3, 0);
   glEnd();

   ErrCheck("Flower");
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
   glScaled(r,r,r);

   //create vase
   glPushMatrix();
   glScaled(1.5, 1.5, 1.5);
   glRotated(90,1,0,0);
   vase();
   glPopMatrix();

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
   ErrCheck("FlowerVase");
}


/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph,0);
         Vertex(th,ph+inc,0);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2.0;  //  Length of axes
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


   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);

   //  Draw columns
   column(-2.5,0,-2.5,.4,0);
   column(2.5,0,-2.5,.4,0);
   column(-2.5,0,2.5,.4,0);
   column(2.5,0,2.5,.4,0);
   //  Table
   table(0,0,0,.4,0);
   //  Vase of flowers
   flowervase(0,(.4*1.2)+(.15),0,.1,0);

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
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
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Light=%s",
     th,ph,dim,fov,light?"On":"Off");
   if (mode == 0) Print(" Projection(m) = Orthoginal");
   else if (mode == 1) Print(" Projection(m) = Perspective");
   else if (mode == 2) Print(" Projection(m) = First Person");
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
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
	   //  Smooth color model
	   else if (key == GLUT_KEY_F1)
		  smooth = 1-smooth;
	   //  Local Viewer
	   else if (key == GLUT_KEY_F2)
		  local = 1-local;
	   else if (key == GLUT_KEY_F3)
		  distance = (distance==1) ? 5 : 1;
	   //  Toggle ball increment
	   else if (key == GLUT_KEY_F8)
		  inc = (inc==10)?3:10;
	   //  Flip sign
	   else if (key == GLUT_KEY_F9)
		  one = -one;
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
	   else if (key == GLUT_KEY_PAGE_DOWN)
		  dim += 0.1;
	   //  PageDown key - decrease dim
	   else if (key == GLUT_KEY_PAGE_UP && dim>1)
		  dim -= 0.1;
	   //  Smooth color model
	   else if (key == GLUT_KEY_F1)
		  smooth = 1-smooth;
	   //  Local Viewer
	   else if (key == GLUT_KEY_F2)
		  local = 1-local;
	   else if (key == GLUT_KEY_F3)
		  distance = (distance==1) ? 5 : 1;
	   //  Toggle ball increment
	   else if (key == GLUT_KEY_F8)
		  inc = (inc==10)?3:10;
	   //  Flip sign
	   else if (key == GLUT_KEY_F9)
		  one = -one;  
   }
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   POVth %= 360;
   //  Update projection
   Project(mode, fov,asp,dim);
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
   {
		th = ph = 0;
		dim = 5;
   }
      
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = (mode+1)%3;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode,fov,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
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
   Project(mode,fov,asp,dim);
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
   glutCreateWindow("Assignment 6: Deekshitha Thumma");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
    //  Load textures
   texture[0] = LoadTexBMP("marble.bmp");
   texture[1] = LoadTexBMP("flowercenter.bmp");
   texture[2] = LoadTexBMP("tabletop.bmp");
   texture[3] = LoadTexBMP("tablebottom.bmp");
   texture[4] = LoadTexBMP("vase.bmp");
   texture[5] = LoadTexBMP("petal1.bmp");
   texture[6] = LoadTexBMP("petal2.bmp");
   texture[7] = LoadTexBMP("velvet.bmp");
   texture[8] = LoadTexBMP("bluemarble.bmp");
   texture[9] = LoadTexBMP("flowerbottom.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
