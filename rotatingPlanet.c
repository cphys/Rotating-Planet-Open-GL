#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


float rotate;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
}

void display(void)
{

   glClear (GL_COLOR_BUFFER_BIT);

   glPushMatrix();

   GLUquadric *quad = gluNewQuadric();

   gluQuadricTexture(quad, 1);

   glEnable(GL_TEXTURE_2D);
   glRotatef ((GLfloat) rotate, 0, 1.0, 0);
   gluSphere(quad, 5, 20, 20);
   glPopMatrix();
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 0.1, 20);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt (0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void do_material()
{
float mat_ambient[] = {0.0,0.0,1.0,18.0};
float mat_diffuse[] = {0.5,0.5,0.9,5.0};
float mat_specular[] = {0.75,0.75,0.75,40.0};
float mat_shininess[] = {7.0};
glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
glMaterialfv(GL_FRONT,GL_SPECULAR,mat_diffuse);
glMaterialfv(GL_FRONT,GL_SHININESS,mat_specular);
glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}


void update(int value)
{
    rotate+=2.0f;
    if(rotate>360.f)
    {
        rotate-=360;
    }
    glutPostRedisplay();
    glutTimerFunc(25,update,0);
}


void do_lights()
{

float light0_ambient[] = { 0.0, 0.0, 1.0, 8.0 };
float light0_diffuse[] = { .25, .25, 1.0, 8.0 };
float light0_specular[] = { .70, 0.70, 1.0, 10.0 };
float light0_position[] = { 0.0, 0, 1.0, 0.0 };
float light0_direction[] = { 1.0, 1.0, 1.0, 1.0};


// Turn off scene default ambient. 
glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);

// Make specular correct for spots.
glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);

glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);

glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,1);
glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0);
glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_direction);

glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
}

void load_texture()
{
FILE *fopen(), *fptr;
char buf[512];
int im_size, im_width, im_height, max_color;
unsigned char *texture_bytes;
char *parse;

fptr=fopen("scuff.ppm","r");
fgets(buf,512,fptr);
do{
        fgets(buf,512,fptr);
        } while(buf[0]=='#');
parse = strtok(buf," ");
im_width = atoi(parse);

parse = strtok(NULL,"\n");
im_height = atoi(parse);

fgets(buf,512,fptr);
parse = strtok(buf," ");
max_color = atoi(parse);

im_size = im_width*im_height;
texture_bytes = (unsigned char *)calloc(3,im_size);
fread(texture_bytes,1,3*im_size,fptr);
fclose(fptr);

glBindTexture(GL_TEXTURE_2D,1);
glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,im_width,im_height,0,GL_RGB,
	GL_UNSIGNED_BYTE,texture_bytes);
glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

cfree(texture_bytes);
}


int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   load_texture();
   do_lights();
   do_material();
   glutTimerFunc(25, update, 0);
   glutMainLoop();
   return 0;
}

