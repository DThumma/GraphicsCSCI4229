/*
 *  Deekshitha Thumma 10/26/17
 *  Final Project
 *
 *  
 *
 *  Key bindings:
 *  p          Toggles ortogonal/perspective projection
 *  arrows     Change view angle, walk in First Person
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  m          Toggles light movement
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 *
 *
 * Resources: Used Homeworks and examples
 */
#include "CSCIx229.h"

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int POVth =0;     //  Angle of moving left and right when in First Person
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
int d =5;         //  Degree of chenge in cylinder
double asp=1;     //  Aspect ratio
double dim=16.0;   //  Size of world

//Projection values
double Viewx = 0; //  Where we are pointed x coordinate 
double Viewy = 0; //  Where we are pointed y coordinate
double Viewz = 0; //  Where we are pointed z coordinate
double POVx = 3;  //  Eye x coordinate in First Person (this is where we start off)
double POVy = 1;  //  Eye y coordinate in First Person
double POVz = 0;  //  Eye z coordinate in First Person
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
int ambient   =  25;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   1;  // Elevation of light (how high the light is)

//textures
unsigned int texture[54]; // Texture names

//array of different shades of purple
float rcolor[5][3] = {
    {0.729, 0.333, 0.827},
    {0.576, 0.439, 0.859},
    {0.541, 0.169, 0.886},
    {0.580, 0.000, 0.827},
    {0.294, 0.000, 0.510}
};

//array of rainbow colors
float rainbow[15][3] = {
    {0.000, 1.000, 1.000},
    {0.498, 1.000, 0.831},
    {0.000, 0.808, 0.820},
    {0.000, 0.749, 1.000},
    {0.482, 0.408, 0.933},
    {0.416, 0.353, 0.804},
    {0.600, 0.196, 0.800},
    {0.541, 0.169, 0.886},
    {0.729, 0.333, 0.827},
    {0.867, 0.627, 0.867},
    {1.000, 0.000, 1.000},
    {1.000, 0.412, 0.706},
    {1.000, 0.412, 0.706},
    {0.863, 0.078, 0.235},
    {0.804, 0.361, 0.361}
};

//shader
int transparent_shader = 0;


/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = -Sin(th)*Cos(ph);
   double y =  Sin(ph);
   double z =          Cos(th)*Cos(ph);
   glNormal3d(x,y,z);
   glTexCoord2d(th/360.0,ph/180.0+0.5);
   glVertex3d(x,y,z);
}

/*
 *  Draw a circle disk
 *  texturenum is the texture name (integer) for the circle
 *  bisHalf is bool to say if we want just a semi circle or full
 */
static void circle(int texturenum, bool bisHalf, bool bisTransparent)
{

   //jump in degrees
   const int d=1;
   int th;  
   int degrees = 360;
   
   //set color
   glColor3f(1,1,1);

   //set ligthing values
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Enable textures
   if(bisTransparent) glUseProgram(transparent_shader);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[texturenum]); 

   //if just half, make degrees half
   if(bisHalf) degrees = 180;

   //Draw circle
   glBegin(GL_POLYGON);
    //loop through angles until you reach 360 degrees
	for (th=0;th<=degrees;th+=d)
	{
			glNormal3f( 0,+1, 0);
			glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
			glVertex3f(Cos(th), 0, Sin(th));
	}

   glEnd();
   glDisable(GL_TEXTURE_2D);
   if(bisTransparent) glUseProgram(0);
   ErrCheck("Circle");
}

/*
 *  Draw a tall cylinder 
 *  texturenum is the texture name (integer) for the column
 *  bisHalf is bool to say if we want just a semi column or full 
 */
static void cylinder(int texturenum, bool bisHalf)
{
   //jump in degrees
   const int d=5;
   int th;
   int degrees = 360;

   //set ligthing values
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[texturenum]); 
      
    if (bisHalf) degrees = 180;

    //Form cylinder
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=degrees;th+=d)
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
 *  hallway
 * *  texturenum is the texture name (integer) for the hallway
 */
static void hallway(int texturenum)
{
   //  Create hallway floor
   glPushMatrix();
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[12]);

	glBegin(GL_QUADS);
	//grey tiles
	glNormal3f( 0, 1, 0);

	for(int i = 0; i<= 6; i++)
	{
		for(int j = -12; j <0; j++)
		{
			if(i == 2 || i == 4 )
			{
				if(abs(j )% 2 ==0)
				{
					glTexCoord2f(0,0); glVertex3f(i,.001, j);
					glTexCoord2f(1,0); glVertex3f(i+1,.001, j);
					glTexCoord2f(1,1); glVertex3f(i+1,.001, j+1);
					glTexCoord2f(0,1); glVertex3f(i,.001, j+1);
					
				}
			}
			else if(i == 3)
			{
				if(abs(j) % 2 ==1)
				{
					glTexCoord2f(0,0); glVertex3f(i,.001, j);
					glTexCoord2f(1,0); glVertex3f(i+1,.001, j);
					glTexCoord2f(1,1); glVertex3f(i+1,.001, j+1);
					glTexCoord2f(0,1); glVertex3f(i,.001, j+1);
				}
			}
			else
			{
				glTexCoord2f(0,0); glVertex3f(i,.001, j);
				glTexCoord2f(1,0); glVertex3f(i+1,.001, j);
				glTexCoord2f(1,1); glVertex3f(i+1,.001, j+1);
				glTexCoord2f(0,1); glVertex3f(i,.001, j+1);
			}
		} 
	}
	//  End
	glEnd();
	glDisable(GL_TEXTURE_2D);

   //  Create hallway floor
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[11]);

	glBegin(GL_QUADS);
	//purple tiles
	glNormal3f( 0, 1, 0);

	for(int i = 0; i<= 6; i++)
	{
		for(int j = -12; j <0; j++)
		{
			if(i == 2 || i == 4 )
			{
				if(abs(j )% 2)
				{
					glTexCoord2f(0,0); glVertex3f(i,.001, j);
					glTexCoord2f(1,0); glVertex3f(i+1,.001, j);
					glTexCoord2f(1,1); glVertex3f(i+1,.001, j+1);
					glTexCoord2f(0,1); glVertex3f(i,.001, j+1);
					
				}
			}
			if(i == 3)
			{
				if(abs(j) % 2 == 0)
				{
					glTexCoord2f(0,0); glVertex3f(i,.001, j);
					glTexCoord2f(1,0); glVertex3f(i+1,.001, j);
					glTexCoord2f(1,1); glVertex3f(i+1,.001, j+1);
					glTexCoord2f(0,1); glVertex3f(i,.001, j+1);
				}
			}
		} 
	}
	//  End
	glEnd();
	glDisable(GL_TEXTURE_2D);

   //  Create hallway ceiling
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[texturenum]);

	glBegin(GL_QUADS);
	//ceiling
	glNormal3f( 0, -1, 0);

	for(int i = 0; i<= 6; i++)
	{
		for(int j = -12; j <0; j++)
		{
			glTexCoord2f(0,0); glVertex3f(i,4.001, j);
			glTexCoord2f(1,0); glVertex3f(i+1,4.001, j);
			glTexCoord2f(1,1); glVertex3f(i+1,4.001, j+1);
			glTexCoord2f(0,1); glVertex3f(i,4.001, j+1);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//hallway roof
	glEnable(GL_TEXTURE_2D);
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,texture[texturenum]);

	//left
	glBegin(GL_QUADS);
	glNormal3f( -1, 0, 0); //TODO - change normal 
	glTexCoord2f(0,0); glVertex3f(0,4.001, -12);
	glTexCoord2f(1,0); glVertex3f(0,4.001, 0);
	glTexCoord2f(1,1); glVertex3f(3.5,5,0);
	glTexCoord2f(0,1); glVertex3f(3.5,5, -12);
	glEnd();

	//right
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0); //TODO - change normal

	glTexCoord2f(0,0); glVertex3f(7,4.001, 0);
	glTexCoord2f(1,0); glVertex3f(7,4.001, -12);
	glTexCoord2f(1,1); glVertex3f(3.5,5,-12);
	glTexCoord2f(0,1); glVertex3f(3.5,5, 0);
	glEnd();

	//front - triangle
	glBegin(GL_TRIANGLES);
	glNormal3f( 0, 0, 1);

	glTexCoord2f(0,0); glVertex3f(0,4.001, 0);
	glTexCoord2f(1,0); glVertex3f(7,4.001, 0);
	glTexCoord2f(.5,.25); glVertex3f(3.5,5, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	ErrCheck("Hallway");
}


/*
 *  main room
 *  texturenum is the texture name (integer) for the main room walls/floor
 */
static void mainroom(int texturenum)
{  
   glPushMatrix();
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);


   //  Create main room floor
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[13]);

   	//floor
	glBegin(GL_QUADS);
	glNormal3f( 0, 1, 0);
	for(int i = -9; i<= 15; i++)
	{
		for(int j = -27; j <-12; j++)
		{
			glTexCoord2f(0,0); glVertex3f(i,0, j);
			glTexCoord2f(1,0); glVertex3f(i+1,0, j);
			glTexCoord2f(1,1); glVertex3f(i+1,0, j+1);
		    glTexCoord2f(0,1); glVertex3f(i,0, j+1);
			
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

   //  Create main room ceiling
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[13]);

	glBegin(GL_QUADS);
	//ceiling
	glNormal3f( 0, -1, 0);

	for(int i = -10; i<= 16; i++)
	{
		for(int j = -28; j <-11; j++)
		{
			glTexCoord2f(0,0); glVertex3f(i,8, j);
			glTexCoord2f(1,0); glVertex3f(i+1,8, j);
			glTexCoord2f(1,1); glVertex3f(i+1,8, j+1);
			glTexCoord2f(0,1); glVertex3f(i,8, j+1);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);


	//  Create main room roof
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(0.7, 0.902, 0.980);
   glBindTexture(GL_TEXTURE_2D,texture[14]);
	glBegin(GL_QUADS);
	//  Front
	glNormal3f( 0, 0, 1); //TODO - fix normals
	glTexCoord2f(0,0); glVertex3f(-10,8, -11);
	glTexCoord2f(5,0); glVertex3f(17,8, -11);
	glTexCoord2f(5,.5); glVertex3f(16,9, -12);
	glTexCoord2f(0,.5); glVertex3f(-9,9, -12);
	//  Back
	glNormal3f( 0, 0,-1); //TODO - fix normals
	glTexCoord2f(0,0); glVertex3f(17,8,-28);
	glTexCoord2f(5,0); glVertex3f(-10,8,-28);
	glTexCoord2f(5,.5); glVertex3f(-9,9,-27);
	glTexCoord2f(0,.5); glVertex3f(16,9,-27);
	//  Right
	glNormal3f(1, 0, 0); //TODO - fix normals
	glTexCoord2f(0,0);glVertex3f(17,8,-11);
	glTexCoord2f(3,0);glVertex3f(17,8,-28);
	glTexCoord2f(3,.5);glVertex3f(16,9,-27);
	glTexCoord2f(0,.5);glVertex3f(16, 9,-12);	
	//  Left
	glNormal3f(-1, 0, 0); //TODO - fix normals
	glTexCoord2f(0,0);glVertex3f(-10,8,-28);
	glTexCoord2f(3,0);glVertex3f(-10,8,-11);
	glTexCoord2f(3,.5);glVertex3f(-9,9,-12);
	glTexCoord2f(0,.5);glVertex3f(-9, 9,-27);
	//  Top
	glNormal3f( 0,1, 0);
	glTexCoord2f(0,0);glVertex3f(-9,9,-12);
	glTexCoord2f(5,0);glVertex3f(16,9,-12);
	glTexCoord2f(5,5);glVertex3f(16,9,-27);
	glTexCoord2f(0,5);glVertex3f(-9,9,-27);

	//  End
	glEnd();
	glDisable(GL_TEXTURE_2D);
   
	//  Create left wall
	//start 2 sided lighting
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[13]);

	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);

	for(int i = 0; i<8; i++)
	{
		for(int j = -27; j <-12; j++)
		{
			glTexCoord2f(0,0); glVertex3f(-9,i, j);
			glTexCoord2f(1,0); glVertex3f(-9,i, j+1);
			glTexCoord2f(1,1); glVertex3f(-9,i+1, j+1);
			glTexCoord2f(0,1); glVertex3f(-9,i+1, j);
		}
	}
	//  End
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//  Create right wall
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[13]);

	glBegin(GL_QUADS);
	glNormal3f( -1, 0, 0);

	for(int i = 0; i<8; i++)
	{
		for(int j = -27; j <-12; j++)
		{
			glTexCoord2f(0,0); glVertex3f(16,i, j);
			glTexCoord2f(1,0); glVertex3f(16,i, j+1);
			glTexCoord2f(1,1); glVertex3f(16,i+1, j+1);
			glTexCoord2f(0,1); glVertex3f(16,i+1, j);
		}
	}
	//  End
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//  Create front wall (left and right cutouts then center)
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[13]);

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	//left
	for(int i = 0; i<8; i++)
	{
		for(int j = -9; j <0; j++)
		{
			glTexCoord2f(0,0); glVertex3f(j,i, -12);
			glTexCoord2f(1,0); glVertex3f(j+1,i, -12);
			glTexCoord2f(1,1); glVertex3f(j+1,i+1, -12);
			glTexCoord2f(0,1); glVertex3f(j,i+1, -12);
		}
	}
	//  End
	glEnd();


	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	//right
	for(int i = 0; i<8; i++)
	{
		for(int j = 7; j <16; j++)
		{
			glTexCoord2f(0,0); glVertex3f(j,i, -12);
			glTexCoord2f(1,0); glVertex3f(j+1,i, -12);
			glTexCoord2f(1,1); glVertex3f(j+1,i+1, -12);
			glTexCoord2f(0,1); glVertex3f(j,i+1, -12);
		}
	}
	//  End
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	//center
	for(int i = 4; i<8; i++)
	{
		for(int j = 0; j <7; j++)
		{
			glTexCoord2f(0,0); glVertex3f(j,i, -12);
			glTexCoord2f(1,0); glVertex3f(j+1,i, -12);
			glTexCoord2f(1,1); glVertex3f(j+1,i+1, -12);
			glTexCoord2f(0,1); glVertex3f(j,i+1, -12);
		}
	}
	//  End
	glEnd();
	glDisable(GL_TEXTURE_2D);


	//Create backwall
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[13]);

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	for(int i = 0; i<8; i++)
	{
		for(int j = -9; j <=15; j++)
		{
			if(j < 1|| j >5)
			{
                if(j >=-8 && j<0 && i >= 1 && i <4)
                {
                
                }
                else if (j >=7 && j<15 && i >= 1 && i <4)
                {
                
                }
                else
                {
				    glTexCoord2f(0,0); glVertex3f(j,i, -27);
				    glTexCoord2f(1,0); glVertex3f(j+1,i, -27);
				    glTexCoord2f(1,1); glVertex3f(j+1,i+1, -27);
				    glTexCoord2f(0,1); glVertex3f(j,i+1, -27);
                }
			}
			else
			{
				if(i > 3)
				{
					glTexCoord2f(0,0); glVertex3f(j,i, -27);
					glTexCoord2f(1,0); glVertex3f(j+1,i, -27);
					glTexCoord2f(1,1); glVertex3f(j+1,i+1, -27);
					glTexCoord2f(0,1); glVertex3f(j,i+1, -27);
					
				}
			}
		}
	}
	//  End
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//turn off two sided lighting 
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);
	


    //Create half columns on front wall (exterior of wall)
    glPushMatrix();
    glTranslated(-3,0,-12);
    glScaled(.5,2.67,.5);
    circle(12,true, false);
    cylinder(16,true);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-6,0,-12);
    glScaled(.5,2.67,.5);
    circle(12,true, false);
    cylinder(16,true);
    glPopMatrix();

    glPushMatrix();
    glTranslated(10,0,-12);
    glScaled(.5,2.67,.5);
    circle(12,true, false);
    cylinder(16,true);
    glPopMatrix();

    glPushMatrix();
    glTranslated(13,0,-12);
    glScaled(.5,2.67,.5);
    circle(12,true, false);
    cylinder(16,true);
    glPopMatrix();

    glPopMatrix();
	ErrCheck("Main Room");
}

/*
* stained glass
*  texturenum is the texture name (integer)
*/
static void stainedglass(int texturenum)
{
   //lighting
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Enable textures
   glColor4f(1,1,1,.1);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[texturenum]);
   glEnable(GL_BLEND);
   glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_COLOR); 
   glDepthMask(0);
	
    //left
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,1);
   glTexCoord2f(0,0); glVertex3f(-8,1,-27);
   glTexCoord2f(1,0); glVertex3f(0,1,-27);
   glTexCoord2f(1,1); glVertex3f(0,4,-27);
   glTexCoord2f(0,1); glVertex3f(-8,4,-27);
   glEnd();

   //right - opposite direction of texture
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,1);
   glTexCoord2f(1,0); glVertex3f(7,1,-27);
   glTexCoord2f(0,0); glVertex3f(15,1,-27);
   glTexCoord2f(0,1); glVertex3f(15,4,-27);
   glTexCoord2f(1,1); glVertex3f(7,4,-27);
   glEnd();

   glDisable(GL_BLEND);
   glDepthMask(1);
   glDisable(GL_TEXTURE_2D);
   ErrCheck("Stained Glass");
}

/*
 *  dome room - TODO two sided lighting
 *  texturenum is the texture name (integer) for small halway walls/floor
 */
static void domeroom(int texturenum)
{
   
   glPushMatrix();
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);


   //  Create small hallway left wall
   //turn on two sided lighting 
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[13]);

	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);

	for(int i = 0; i<4; i++)
	{
		for(int j = -33; j < -27; j++)
		{
			glTexCoord2f(0,0); glVertex3f(1,i, j);
			glTexCoord2f(1,0); glVertex3f(1,i, j+1);
			glTexCoord2f(1,1); glVertex3f(1,i+1, j+1);
			glTexCoord2f(0,1); glVertex3f(1,i+1, j);
		}
	}
	//  End
	glEnd();

	//  Create small hallway right wall
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);

	for(int i = 0; i<4; i++)
	{
		for(int j = -33; j < -27; j++)
		{
			glTexCoord2f(0,0); glVertex3f(6,i, j);
			glTexCoord2f(1,0); glVertex3f(6,i, j+1);
			glTexCoord2f(1,1); glVertex3f(6,i+1, j+1);
			glTexCoord2f(0,1); glVertex3f(6,i+1, j);
		}
	}
	//  End
	glEnd();
	glDisable(GL_TEXTURE_2D);


	//Create small hallway top celing
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[13]);

   glBegin(GL_QUADS);
	glNormal3f( 0, -1, 0);

	for(int i = 1; i<6; i++)
	{
		for(int j = -33; j <-27; j++)
		{
			glTexCoord2f(0,0); glVertex3f(i,4, j);
			glTexCoord2f(1,0); glVertex3f(i+1,4, j);
			glTexCoord2f(1,1); glVertex3f(i+1,4, j+1);
			glTexCoord2f(0,1); glVertex3f(i,4, j+1);
		}
	}
	//  End
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//turn off two sided lighting 
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);

	//Create small hallway floor
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[13]);

   glBegin(GL_QUADS);
   glNormal3f( 0, 1, 0);

	for(int i = 1; i<6; i++)
	{
		for(int j = -33; j < -27; j++)
		{
			glTexCoord2f(0,0); glVertex3f(i,0, j);
			glTexCoord2f(0,1); glVertex3f(i,0, j+1);
			glTexCoord2f(1,1); glVertex3f(i+1,0, j+1);
			glTexCoord2f(1,0); glVertex3f(i+1,0, j);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	
	//create disk floor
	glPushMatrix();
	glTranslated(3.5,0,-42.5);
	glScaled(10,0,10);
	circle(13, false, false);
	glPopMatrix();

    //create disk transparent ceiling
    glPushMatrix();
	glTranslated(3.5,4.05,-42.5);
	glScaled(10,0,10);
	circle(53, false, true);
	glPopMatrix();

	//create cylinder Dome	
	glPushMatrix();
	glTranslated(3.5,0,-42.5);
    int ch;

	//turn on two sided lighting 
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[15]);

    //Form cylinder
    glBegin(GL_QUAD_STRIP);
	//loop through angles until you reach 360 degrees
    for (ch=0;ch<=332;ch+=1)
    {
            if(ch == 0||ch == 180) {glColor3f(rcolor[0][0],rcolor[0][1],rcolor[0][2]);}
            else if(ch == 36||ch == 216) {glColor3f(rcolor[1][0],rcolor[1][1],rcolor[1][2]);}
            else if(ch == 72||ch == 252) {glColor3f(rcolor[2][0],rcolor[2][1],rcolor[2][2]);}
            else if(ch == 108||ch == 288) {glColor3f(rcolor[3][0],rcolor[3][1],rcolor[3][2]);}
            else if(ch == 144||ch == 324) {glColor3f(rcolor[4][0],rcolor[4][1],rcolor[4][2]);}
			glNormal3d(Cos(ch), 0, Sin(ch));
			glTexCoord2f(ch/360.0*10.0,1);
            glVertex3f( Cos(ch+104)*10, 0, Sin(ch+104)*10);
			glTexCoord2f(ch/360.0*10.0,0);
            glVertex3f(Cos(ch+104)*10, 4.01, Sin(ch+104)*10);
    }
    glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//dome top 
	glPushMatrix();
	glTranslated(3.5,4,-42.5);
	glScaled(10,10,10);
	glEnable(GL_TEXTURE_2D);
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,texture[16]);
	int th,ph;
	for (ph=0;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph+d);
         Vertex(th,ph);
      }
      glEnd();
   }
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
   	//turn off two sided lighting 
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,0);

   glPopMatrix();
   ErrCheck("Dome Room");

}

/*
 *  Draw a cube
 *  texturenum1 is the texture name (integer) for front and back of cube
 *  texturenum2 is the texture name (integer) for rest of cube
 */
static void normalcube(int texturenum1, int texturenum2) 
{
   // lighting
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[texturenum1]);  

   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glNormal3f( 0, 0,1);
   glTexCoord2f(0,0); glVertex3f(0,0,1);
   glTexCoord2f(1,0); glVertex3f(1,0,1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,1);
   glTexCoord2f(0,1); glVertex3f(0,+1,1);
   //  Back
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(0,0,0);
   glTexCoord2f(1,0); glVertex3f(1,0,0);
   glTexCoord2f(1,1); glVertex3f(+1,+1,0);
   glTexCoord2f(0,1); glVertex3f(0,+1,0);

   glEnd();
   glDisable(GL_TEXTURE_2D);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[texturenum2]); 
   glBegin(GL_QUADS);
   //  Right
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,0,0);
   glTexCoord2f(1,0); glVertex3f(+1,0,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,0);
   //  Left
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(0,0,0);
   glTexCoord2f(1,0); glVertex3f(0,0,+1);
   glTexCoord2f(1,1); glVertex3f(0,+1,+1);
   glTexCoord2f(0,1); glVertex3f(0,+1,0);
   //  Top
   glNormal3f(0, 1, 0);
   glTexCoord2f(0,0); glVertex3f(0,+1,0);
   glTexCoord2f(1,0); glVertex3f(+1,+1,0);
   glTexCoord2f(1,1); glVertex3f(+1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(0,+1,+1);

   //  Bottom
   glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(0,0,0);
    glTexCoord2f(1,0); glVertex3f(+1,0,0);
    glTexCoord2f(1,0); glVertex3f(+1,0,+1);
    glTexCoord2f(0,1); glVertex3f(0,0,+1);


   glEnd();
   glDisable(GL_TEXTURE_2D);
   ErrCheck("Normal Cube");
}

/*
 *  Draw a cube
 *  texturenum is the texture name (integer)
 */
static void cube(int texturenum)
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
   glBindTexture(GL_TEXTURE_2D,texture[texturenum]); //marble texture, never do this between a glbegin and an glend 

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
	glDisable(GL_TEXTURE_2D);
}


/*
 *  Draw a short fatter cylinder 
 *  texturenum is the texture name (integer)
 */
static void minicylinder(int texturenum)
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
   glBindTexture(GL_TEXTURE_2D,texture[texturenum]); 

	//Form cylinder
	glBegin(GL_QUAD_STRIP);
	//loop through angles until you reach 360 degrees
	for (th=0;th<=360;th+=d)
	{
			glNormal3d(Cos(th)*1.25, 0, Sin(th)*1.25);
			glTexCoord2f(th/360.0,0);
			glVertex3f(Cos(th)*1.25, 0, Sin(th)*1.25);
			glTexCoord2f(th/360.0,1);
			glVertex3f( Cos(th)*1.25, -.5, Sin(th)*1.25);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	ErrCheck("Minicylinder");
}
/*
 *  Draw a column
 *     at (x,y,z)
 *     radius (r)
 *     rotated (rot) about the z axes
 */
static void column(double x,double y,double z,double rx, double ry, double rz, double rot)
{
   //  Save transformation
   glPushMatrix();

   //  Offset
   glTranslated(x,y,z);
   glRotated(rot,0,0,1);
   glScaled(rx,ry,rz);

   //put at 0 first - total unit diamter - 3, total unit height 5
   glPushMatrix();
   glTranslated(0,1,0);

   //create base cube - bottom
   cube(0);

   //create top to the mini cylinder - bottom
   glPushMatrix();
   glScaled(1.25,1,1.25);
   circle(0, false, false);
   glPopMatrix();

   //create mini embellishment cylinder - bottom
   minicylinder(7);

   //create main column
   cylinder(0,false);

   //create bottom to the mini cylinder on top of column - top
   glPushMatrix();
   glTranslated(0,3.5,0);
   minicylinder(7);
   glPopMatrix();

   //create top to the mini cylinder - top 
   glPushMatrix();
   glTranslated(0,3,0);
   glScaled(1.25,1,1.25);
   circle(0, false, false);
   glPopMatrix();

   //create top cube - top
   glPushMatrix();
   glTranslated(0,4.5,0);
   cube(0);
   glPopMatrix();

   //  Undo transformations for shifting up 1
   glPopMatrix();
   //  Undo transformations
   glPopMatrix();
   ErrCheck("Column");
}

/*
 *  Draw table top
 *  texturenum1 is the texture name (integer) for edges
 *  texturenum is the texture name (integer) for top and bottom
 */
static void tabletop(int texturenum1, int texturenum2)
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
   glBindTexture(GL_TEXTURE_2D,texture[texturenum1]); 

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
   glBindTexture(GL_TEXTURE_2D,texture[texturenum2]); 

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
 *  texturenum1 is the texture name (integer) for edges
 *  texturenum is the texture name (integer) for bottom
 */
static void tablebottom(int texturenum1, int texturenum2)
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
   glBindTexture(GL_TEXTURE_2D,texture[texturenum1]);

   //square at the bottom of table bottom structure
   glBegin(GL_QUADS);
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
   glBindTexture(GL_TEXTURE_2D,texture[texturenum2]);

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
 *     rotated (rot) about the z axes
 *      texturenum1 is the texture name (integer)
 */
static void table(double x,double y,double z,double rx, double ry, double rz, double rot, int texturenum)
{
   //  Save transformation
   glPushMatrix();
   
   //  Offset
   glTranslated(x,y,z);
   glRotated(rot,0,0,1);
   glScaled(rx,ry,rz);

   //create table top
   tabletop(7,texturenum);
   tablebottom(8,3);

   //  Undo transformations
   glPopMatrix();
   ErrCheck("Table");
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
         Vertex(th,ph);
         Vertex(th,ph+d);
	  }
	  glEnd();
   }

   glDisable(GL_TEXTURE_2D);
   ErrCheck("Vase");
}

/*
 *  Draw flower
 *  flowertexture is the texture name (integer) for flower petals
 *  centertexture is the texture name (integer) for center of flower
 */
static void flower(int flowertexture, int centertexture)
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
   glBindTexture(GL_TEXTURE_2D,texture[centertexture]); 

   //  semi sphere - flower center
   //transform a bit
   glPushMatrix();
   glTranslated(0,3,0);
    glScaled(.5,.5,.5);
   glColor3f(1 , 1 , 1); 

   for (ph=0;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();
	
    glPushMatrix();
    glTranslated(0,3,0);
    glScaled(.5,.5,.5);
    circle(9,false, false);
    glPopMatrix();

	
   //stem 
   glColor3f(0,0.392,0);
   glBegin(GL_LINES);
   glVertex3f(0, 0, 0);
   glVertex3f(0, 3, 0);
   glEnd();


   
   //use shader to make black transparent
    glUseProgram(transparent_shader);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[flowertexture]); 

    //petals - a disk
	glBegin(GL_POLYGON);
	for (th=0;th<=360;th+=d)
	{
	    glNormal3f(0,1,0);
		glTexCoord2f(.5 + Cos(th)*.5,.5 +Sin(th)*.5);
		glVertex3f(Cos(th)*1.5, 3.01, Sin(th)*1.5);
	}

	glEnd();
    glDisable(GL_TEXTURE_2D);
    glUseProgram(0);

   ErrCheck("Flower");
}


/*
 *  Draw a flower vase
 *     at (x,y,z)
 *     radius (r)
 *     rotated (rot) about the z axes
 *  flowertexture is the texture name (integer) for flower petals
 *  centertexture is the texture name (integer) for center of flower
 */
static void flowervase(double x,double y,double z,double r, double rot, int flowertexture, int centertexture)
{
   //  Save transformation
   glPushMatrix();

   //  Offset
   glTranslated(x,y,z);
   glRotated(rot,0,0,1);
   glScaled(r,r,r);

   //create vase
   glPushMatrix();
   glScaled(1, 1, 1);
   vase();
   glPopMatrix();
   
   //create flowers
   flower(flowertexture, centertexture);
   glPushMatrix();
   glRotated(60,0,0,1);
   flower(flowertexture, centertexture);
   glPopMatrix();
   glPushMatrix();
   glRotated(60,1,0,0);
   flower(flowertexture, centertexture);
   glPopMatrix();
   glPushMatrix();
   glRotated(-60,0,0,1);
   flower(flowertexture, centertexture);
   glPopMatrix();
   glPushMatrix();
   glRotated(-60,1,0,0);
   flower(flowertexture, centertexture);
   glPopMatrix();

   //  Undo transformations
   glPopMatrix();
   ErrCheck("FlowerVase");
}

/*
* Draw painting
* texturenum is the texture name (integer) for the painting
* width is the width
* height is the height
*/
void painting(int texturenum, int width, int height)
{
    //set ligthing values
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Enable textures
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[texturenum]); 

   //quad
   glBegin(GL_QUADS);
	// use given height and width
   glNormal3f( 0, 0,1);
   glTexCoord2f(0,0); glVertex3f(0,0,0);
   glTexCoord2f(1,0); glVertex3f(width,0,0);
   glTexCoord2f(1,1); glVertex3f(width,height,0);
   glTexCoord2f(0,1); glVertex3f(0,height,0);

   glEnd();
   glDisable(GL_TEXTURE_2D);
   ErrCheck("Painting");
}

/*
* Draw painting with frame 
*     at (x,y,z)
 *     radius (r)
 *     rotated (rot) about the z axes
 *  texturenum is the texture name (integer) for painting
 * width is the width
 * height is the height
*/
void paintingwithframe(double x,double y,double z,double rot,int texturenum, int width, int height)
{
   float border = 0.2;
   float thinkness = 0.2;
   
   //painting
   glPushMatrix();
   glTranslated(x,y,z+ thinkness +.01);
   glRotated(rot,0,1,0);
   painting(texturenum, width, height);
 
   //Frame
   glPushMatrix();
   glTranslated(-border,-border,-0.2-0.01);
   glScaled(width+(border*2), height+(border*2), thinkness);
   normalcube(24, 25);
   glPopMatrix();

   glPopMatrix();


   ErrCheck("Painting With Frame");
}

/*
* Draw diamond
* r,g,b are the color values to set color
*/
void diamond(float r, float g, float b)
{

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[35]); 

   //transparency
   glEnable(GL_BLEND);
	glColor4f(r,g,b,.1);
    glBlendFunc(GL_ONE,GL_ONE_MINUS_DST_COLOR); 
	glDepthMask(0);

   //top
    glBegin(GL_TRIANGLE_FAN);
    //top point
	glNormal3f( 1, 0, 0);glTexCoord2f(.5,.5); glVertex3f(0,2, 0);

    glNormal3f( 1, 0, 0);glTexCoord2f(0,0); glVertex3f(.5,1, .5);
	glNormal3f( 0, 0, -1);glTexCoord2f(1,0); glVertex3f(.5,1, -.5);
	glNormal3f( -1, 0, 0);glTexCoord2f(1,1); glVertex3f(-.5,1, -.5);
    glNormal3f( 1, 0, 0);glTexCoord2f(0,1); glVertex3f(-.5,1, .5);
    glNormal3f( 1, 0, 0);glTexCoord2f(0,0); glVertex3f(.5,1, .5);
	//  End
	glEnd();

    //bottom
    glBegin(GL_TRIANGLE_FAN);
    //bottom point
	glNormal3f( 1, 0, 0);glTexCoord2f(.5,.5); glVertex3f(0,0, 0);

    glNormal3f( 1, 0, 0);glTexCoord2f(0,0); glVertex3f(.5,1, .5);
	glNormal3f( 0, 0, -1); glTexCoord2f(1,0); glVertex3f(.5,1, -.5);
	glNormal3f( -1, 0, 0); glTexCoord2f(1,1);  glVertex3f(-.5,1, -.5);
    glNormal3f( 1, 0, 0); ;glTexCoord2f(0,1); glVertex3f(-.5,1, .5);
    glNormal3f( 1, 0, 0); glTexCoord2f(0,0); glVertex3f(.5,1, .5);

	//  End
	glEnd();
    glDisable(GL_BLEND);
    glDepthMask(1);
    glDisable(GL_TEXTURE_2D);
    ErrCheck("Diamond");
}

/*
* Draw ceiling peices (diamonds)
*/
void ceilingpieces()
{
    //cycle through colors
    int k = 0;
    for(double i = -13; i>=-26; i=i-6.5)
    {
        for(double j = -8; j <=15; j=j+5.25)
        {
            glPushMatrix();
            glTranslated(j,6,i);
            diamond(rainbow[k][0],rainbow[k][1],rainbow[k][2]);
            glPopMatrix();
            k++;
        }
    }
    ErrCheck("Ceiling Pieces");
}

/*
* Draw sculpture parts
* texturenum is the texture for the sculture piece
* division is how big of the gap is in the sphere
*/
void sculpture(int texturenum, int division)
{
    //lighting
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glColor3f(1,1,1);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[texturenum]); 

   //sphere-ish shape
	int th,ph;
   for (ph=-90;ph<90;ph+=division)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=5)
      {
        Vertex(th,ph);
        Vertex(th,ph+5);
      }
      glEnd();
   }

   glDisable(GL_TEXTURE_2D);
   ErrCheck("Sculpture");
}

/*
* Draw sculpture in totallity
*     at (x,y,z)
*     texturenum1 is the texture for the sculture outer piece
*     texturenum2 is the texture for the sculture inner piece
*/
void ensamble(double x,double y,double z, int texturenum1, int texturenum2)
{
    glPushMatrix();
    glTranslated(x,y,z);

    //big ball
    glPushMatrix();
    glScaled(1.5,1.5,1.5);
    sculpture(texturenum1, 20);
    glPopMatrix();

    //small ball
    glPushMatrix();
    glScaled(1,1,1);
    sculpture(texturenum2, 10);
    glPopMatrix();
    //smaller solid ball
    glPushMatrix();
    glScaled(.5,.5,.5);
    sculpture(texturenum1, 1);
    glPopMatrix();

    //table
    table(0,-2,0,1, .4,1,0,39);

    glPopMatrix();
    ErrCheck("Ensamble");
}
/* Draw pottery using textures and shader
 *  texturenum is the texture name (integer) for painting
 * width is the width
 * height is the height
*/
void pottery(int texturenum, int width, int height)
{
    //set ligthing values
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Enable textures and shader to remove black
   glColor3f(1,1,1);
   glUseProgram(transparent_shader);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[texturenum]); 

   //quad
   glBegin(GL_QUADS);
	//  Front
   glNormal3f( 0, 0,1);
   glTexCoord2f(0,0); glVertex3f(0,0,0);
   glTexCoord2f(1,0); glVertex3f(width,0,0);
   glTexCoord2f(1,1); glVertex3f(width,height,0);
   glTexCoord2f(0,1); glVertex3f(0,height,0);

   glEnd();
   glDisable(GL_TEXTURE_2D);
   glUseProgram(0);
   ErrCheck("Pottery");
}

/*
* Draw podium for pottery
*     at (x,y,z)
*     texturenum is the texture for the podium
*/
void podium(double x,double y,double z, int texturenum)
{
    glPushMatrix();
    glTranslated(x,y,z);

    //top
    glPushMatrix();
    glTranslated(.25,.75,.25);
    glScaled(.25,.25,.25);
    tabletop(42,42);
    glPopMatrix();

    //bottom
    glPushMatrix();
    glScaled(.5,1,.5);
    normalcube(42,42);
    glPopMatrix();

    //pottery
    glPushMatrix();
    glTranslated(.125,1.05,.25);
    //glRotated(90,0,1,0);
    glScaled(.3,.3,.3);
    pottery(texturenum,1,2);
    glPopMatrix();


    glPopMatrix();
    ErrCheck("Podium");
}

/*
* Distribute pottery and podium together
*/
void potterycollection()
{
   glPushMatrix();
   glTranslated(3.5,0,-43);
   glRotated(-2,0,1,0);
   int deg;
   int k = 43;
   for (deg=0;deg<=360;deg+=36)
   {
        podium(Cos(deg)*9,0,Sin(deg)*9,k);
        k++;
   }
   glPopMatrix();
   ErrCheck("Pottery Collection");
}

/*
* Draw banner using shaders
*/
void banner()
{
    //set ligthing values
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

    glUseProgram(transparent_shader);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[36]); 

    //quad
   glBegin(GL_QUADS);
	//  Front
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(1,4,-26.9);
   glTexCoord2f(1,0); glVertex3f(6,4,-26.9);
   glTexCoord2f(1,1); glVertex3f(6,5,-26.9);
   glTexCoord2f(0,1); glVertex3f(1,5,-26.9);

   glEnd();

    glDisable(GL_TEXTURE_2D);
    glUseProgram(0);
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
         Vertex(th,ph);
         Vertex(th,ph+inc);
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
   //  Perspective - set eye position
   if (mode == 0)
   {
      Ex = -2*dim*Sin(th)*Cos(ph);
      Ey = +2*dim        *Sin(ph);
      Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  First person = set "where we are looking" position based on the rotation user made
   else if (mode == 1)
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
        float Position0[]  = {distance*Cos(zh)+3.5,7.7,distance*Sin(zh)-19.5,1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position0[0],Position0[1],Position0[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position0);
   }
   else
   {
		glDisable(GL_LIGHTING);
	}

    
   //draw hallway
   hallway(0);

   //draw main floor
   mainroom(11);

   //draw dome room
   domeroom(13);

   //  Draw columns
   for(double i = .75; i<= 6.25; i = i +5.5) 
   {
       for(double j = -.75; j>=-11.25; j = j - 1.5)
       {
	       column(i,0,j,.5, .8, .5,0);
	       column(i,0,j,.5, .8, .5,0);
	    }
   }
   

   //Paintings
   paintingwithframe(10, 2, -12.5, 180, 26, 2, 3);
    paintingwithframe(-1, 2, -12.5, 180, 27, 2, 3);
   //right wall
   paintingwithframe(15.5, 2, -20.7, -90, 28, 2, 3);
   paintingwithframe(15.5, 2, -24.95, -90, 30, 3, 2);
   paintingwithframe(15.5, 2, -17.45, -90, 31, 3, 2);
   //left wall
   paintingwithframe(-8.5, 2, -18.7, 90, 29, 2, 3);
   paintingwithframe(-8.5, 2, -21.95, 90, 32, 3, 2);
   paintingwithframe(-8.5, 2, -14.45, 90, 33, 3, 2);

   //sculptures
   ensamble(8,2,-19.5,37,38);
   ensamble(-1,2,-19.5,40,41);

   //pottery on podiums
   potterycollection();
   

   //  Tables with vase of flowers - no for loop since texture parameter is different
   //right  
   table(15,0,-14.5,.4,.4,.4,0,2);
   flowervase(15,(.4*1.2)+(.2),-14.5,.2,0,10,22);
   table(15,0,-17,.4,.4,.4,0,2);
   flowervase(15,(.4*1.2)+(.2),-17,.2,0,19,22);
   table(15,0,-19.5,.4,.4,.4,0,2);
   flowervase(15,(.4*1.2)+(.2),-19.5,.2,0,18,22);
   table(15,0,-22,.4,.4,.4,0,2);
   flowervase(15,(.4*1.2)+(.2),-22,.2,0,20,22);
   table(15,0,-24.5,.4,.4,.4,0,2);
   flowervase(15,(.4*1.2)+(.2),-24.5,.2,0,21,22);
   //left
   table(-8,0,-14.5,.4,.4,.4,0,2);
   flowervase(-8,(.4*1.2)+(.2),-14.5,.2,0,10,22);
   table(-8,0,-17,.4,.4,.4,0,2);
   flowervase(-8,(.4*1.2)+(.2),-17,.2,0,19,22);
   table(-8,0,-19.5,.4,.4,.4,0,2);
   flowervase(-8,(.4*1.2)+(.2),-19.5,.2,0,18,22);
   table(-8,0,-22,.4,.4,.4,0,2);
   flowervase(-8,(.4*1.2)+(.2),-22,.2,0,20,22);
   table(-8,0,-24.5,.4,.4,.4,0,2);
   flowervase(-8,(.4*1.2)+(.2),-24.5,.2,0,21,22);

   
   //stained glass 
   stainedglass(34);

   //Ceiling pieces 
   ceilingpieces();


   //banner
   banner();


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
   Print("Dim=%.1f FOV=%d Light=%s",
     dim,fov,light?"On":"Off");
   if (!mode) Print(" Projection(p) = Perspective");
   else if (mode) Print(" Projection(p) = First Person");

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

//collision detection function
bool detect(double new_POVx, double new_POVz)
{
    //if in halway, revent walking into left and right walls
    if(POVz <= 0 && POVz >= -12 && POVx >= 0 && POVx <= 7)
    {
        if(new_POVx < 2 || new_POVx > 5.5 )
        {
            return false;
        }
        else{
            return true;
        }
    }
    //if in left side of main room, prevent walking into left, front and back walls
    else if(POVz < -12 && POVz >= -27 && POVx <= 0 && POVx >= -9)
    {
        //left side
        if(new_POVx < -7)
        {
            return false;
        }
        else
        {
             //front
            if(new_POVz > -13)
            {
                return false;
            }
            else
            {
                //back
                if(new_POVz < -26)
                {
                    return false;
                }
                else
                {
                    //sculpture
                    if(new_POVx >= -2 && new_POVx <=2 && new_POVz <= -17.5 && new_POVz >= -21.5 )
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
            }
        }
    }
    //if in right side of main room, prevent walking into right, front and back walls
    else if(POVz < -12 && POVz >= -27 && POVx >=7 && POVx <=16)
    {
        //right side
        if(new_POVx > 14)
        {
            return false;
        }
        else
        {
             //front
            if(new_POVz > -13)
            {
                return false;
            }
            else
            {
                //back
                if(new_POVz < -26)
                {
                    return false;
                }
                else
                {
                    //sculpture
                    if(new_POVx >= 5 && new_POVx <=9 && new_POVz <= -17.5 && new_POVz >= -21.5 )
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
            }
        }
    }
    //if in small hallway prevent walking into right and left
    else if(POVz < -27 && POVz >= -33 && POVx >= 1 && POVx <= 6)
    {
        if(new_POVx < 2 || new_POVx > 5 )
        {
            return false;
        }
        else{
            return true;
        }
    }
    //if in dome room
    else if(POVz < -33 && POVz >= -53 && POVx >= -6.5 && POVx <= 13.5)
    {
        //back
        if(new_POVz <= -50)
        {
            return false;
        }
        else
        {
            //left
            if(new_POVx <= -3.5)
            {
                return false;
            }
            else
            {
                //right
                if(new_POVx >= 10.5)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
    }
    else
    {
        return true;
    }
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
	//if in first person view
	if(mode ==1)
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
            if(detect(POVx + Viewx*dist, POVz + Viewz*dist)){
            POVx += Viewx*dist; 
            POVz += Viewz*dist;}
           //printf("MOVED UP: %f, %f Camera: %f, %f \n", POVx, POVz, Viewx, Viewz);
           
	   }
	   //  Down arrow key - move eye placement vector back by camera placement 
	   else if (key == GLUT_KEY_DOWN)
	   {
            if(detect(POVx - Viewx*dist, POVz - Viewz*dist)){
            POVx -= Viewx*dist; 
            POVz -= Viewz*dist;}
           //printf("MOVED DOWN: %f, %f Camera: %f, %f\n", POVx, POVz, Viewx, Viewz);
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
	   else if (key == GLUT_KEY_PAGE_DOWN)
		  dim += 0.1;
	   //  PageDown key - decrease dim
	   else if (key == GLUT_KEY_PAGE_UP && dim>1)
		  dim -= 0.1;
   }
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   POVth %= 360;
   //  Update projection
   Project(fov,asp,dim);
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
		dim = 16.0;
   }    
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
   {
      mode = (mode+1)%2;
      //if you go back to first person perspective, set dim back to default (16)
      if(mode == 1) {dim = 16.0;}
    }
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Reproject
   Project(fov,asp,dim);
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
   Project(fov,asp,dim);
}

/*
 *  Read text file
 */
char* ReadText(char *file)
{
   int   n;
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}

/*
 *  Print Shader Log
 */
void PrintShaderLog(int obj,char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error compiling %s\n",file);
}

/*
 *  Print Program Log
 */
void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}

/*
 *  Create Shader
 */
int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Return name
   return shader;
}

/*
 *  Create Shader Program
 */
int CreateShaderProg(char* VertFile,char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   int vert = CreateShader(GL_VERTEX_SHADER  ,VertFile);
   //  Create and compile fragment shader
   int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
   //  Attach vertex shader
   glAttachShader(prog,vert);
   //  Attach fragment shader
   glAttachShader(prog,frag);
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_ALPHA);
   glutInitWindowSize(600,600);
   glutCreateWindow("Final Project: Deekshitha Thumma");
   #ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
    #endif
   //glClearColor(0.902, 0.902, 0.7,1);
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
    //  Load textures
   texture[0] = LoadTexBMP("marble.bmp");
   texture[2] = LoadTexBMP("tabletop.bmp");
   texture[3] = LoadTexBMP("tablebottom.bmp");
   texture[4] = LoadTexBMP("vase.bmp");
   texture[7] = LoadTexBMP("velvet.bmp");
   texture[8] = LoadTexBMP("bluemarble.bmp");
   texture[9] = LoadTexBMP("flowerbottom.bmp");
   texture[10] = LoadTexBMP("daisy.bmp");
   texture[11] = LoadTexBMP("purplemarble.bmp");
   texture[12] = LoadTexBMP("whitetile.bmp");
   texture[13] = LoadTexBMP("wall.bmp");
   texture[14] = LoadTexBMP("roof2.bmp");
   texture[15] = LoadTexBMP("border.bmp");
   texture[16] = LoadTexBMP("ombre.bmp");
   texture[17] = LoadTexBMP("tallcolumn.bmp");
   texture[18] = LoadTexBMP("flower1.bmp");
   texture[19] = LoadTexBMP("flower2.bmp");
   texture[20] = LoadTexBMP("flower3.bmp");
   texture[21] = LoadTexBMP("flower4.bmp");
   texture[22] = LoadTexBMP("flowercenter1.bmp");
   texture[24] = LoadTexBMP("frame.bmp");
   texture[25] = LoadTexBMP("frameside.bmp");
   texture[26] = LoadTexBMP("painting1.bmp");
   texture[27] = LoadTexBMP("painting2.bmp");
   texture[28] = LoadTexBMP("painting3.bmp");
   texture[29] = LoadTexBMP("painting4.bmp");
   texture[30] = LoadTexBMP("painting5.bmp");
   texture[31] = LoadTexBMP("painting6.bmp");
   texture[32] = LoadTexBMP("painting7.bmp");
   texture[33] = LoadTexBMP("painting8.bmp");
   texture[34] = LoadTexBMP("purplewindow2.bmp");
   texture[35] = LoadTexBMP("diamond.bmp");
   texture[36] = LoadTexBMP("banner.bmp");
   texture[37] = LoadTexBMP("sculpture1.bmp");
   texture[38] = LoadTexBMP("sculpture2.bmp");
   texture[39] = LoadTexBMP("circleombre.bmp");
   texture[40] = LoadTexBMP("sculpture3.bmp");
   texture[41] = LoadTexBMP("sculpture4.bmp");
   texture[42] = LoadTexBMP("wood.bmp");
   texture[43] = LoadTexBMP("pottery.bmp");
   texture[44] = LoadTexBMP("pottery1.bmp");
   texture[45] = LoadTexBMP("pottery2.bmp");
   texture[46] = LoadTexBMP("pottery3.bmp");
   texture[47] = LoadTexBMP("pottery4.bmp");
   texture[48] = LoadTexBMP("pottery5.bmp");
   texture[49] = LoadTexBMP("pottery6.bmp");
   texture[50] = LoadTexBMP("pottery7.bmp");
   texture[51] = LoadTexBMP("pottery8.bmp");
   texture[52] = LoadTexBMP("pottery9.bmp");
   texture[53] = LoadTexBMP("stars.bmp");


   //shader
   transparent_shader = CreateShaderProg("blacktransparent.vert","blacktransparent.frag");

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
