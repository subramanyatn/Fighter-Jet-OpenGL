#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void mydisplay(void);
void initWindow(void);

float angle = 0.0;
float b1x1=40,b1x2=35,b1x3=35,b1x4=40;
float b1y1=23,b1y2=23,b1y3=18,b1y4=18;
int left, right;
int leftTime, rightTime;
int thrust, thrustTime;
float x = 20, y = 20, xv, yv, v;
int shield = 0, joyShield = 0, cursor = 1;
int lastTime;
int paused = 0;
int resuming = 1;
int originalWindow = 0, currentWindow;
int flag1=0, flag2=0, flag=0,flag3=0;


void display();
void drawstring(float x,float y,float z,char *string)
{
 char *c;
 glRasterPos3f(x,y,z);
 for(c=string;*c!='\0';c++)
 {
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*c);
 }
}
void screen(void)
{
	
	 glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(1.0,0.0,0.0);
	 drawstring(15,37,0.0,"FIGHTER JET");
	glColor3f(1.0,0.0,1.0);
	 drawstring(5,34,0.0,"PRESENTS");
	glColor3f( 0.0,0.0,1.0 );  
	 drawstring(10,30,0.0,"FRIENDS"); 
	 glColor3f( 1.0,0.0,0.0 );  
	 drawstring(5,27,0.0,"BY:");
	 glColor3f( 0.9,0.0,1.0 );  
	 drawstring(10,24,0.0,"Put Your Name");
	drawstring(10,20,0.0,"Put Your Name");
	glColor3f( 1.0,0.0,0.0 ); 
	 drawstring(5,17,0.0,"GUIDE : ");
	  glColor3f( 0.9,0.0,1.0 );
	drawstring(15,15,0.0,"Guide Name");
	glColor3ub( rand()%1000, rand()%1000, rand()%1000 );  
	 drawstring(10,2,0.0,"Press w to Start");
	glFlush();
}                   


void mydisplay(void)
{   if(flag2==0)
		screen ();
 
   
   if(flag==1 && flag1==0 &&flag2==0)
	display();

 
}



void drawsquare()
{
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_POLYGON);
		glVertex2f(b1x1,b1y1);
		glVertex2f(b1x2,b1y2);
		glVertex2f(b1x3,b1y3);
		glVertex2f(b1x4,b1y4);
		glEnd();
}


typedef struct
 {
  int inuse;

  float x;
  float y;

  float v;
  float xv;
  float yv;

  int expire; 
	
} Bullet;

#define BULLETS 100

Bullet bullet[BULLETS];


int allocBullet(void)
{
  int i;

  for (i=0; i<BULLETS; i++) {
	if (!bullet[i].inuse) {
	  return i;
	}
  }
  return -1;
}

void initBullet(int i, int time)
{
  float c = cos(angle*M_PI/180.0);
  float s = sin(angle*M_PI/180.0);

  bullet[i].inuse = 1;
  bullet[i].x = x + 3.5 * c;
  bullet[i].y = y + 3.5 * s;
  bullet[i].v = 0.025;
  bullet[i].xv = xv + c * bullet[i].v;
  bullet[i].yv = yv + s * bullet[i].v;
  bullet[i].expire = time + 600;
}

void advanceBullets(int delta, int time)
{
  int i;
	  int a = 0;		
  for (i=0; i<BULLETS; i++) {
	if (bullet[i].inuse) {
	  float x, y;

	  if (time > bullet[i].expire) {
		bullet[i].inuse = 0;
		continue;
	  }
	  x = bullet[i].x + bullet[i].xv * delta;
	  y = bullet[i].y + bullet[i].yv * delta;
	  x = x / 40.0;
	  bullet[i].x = (x - floor(x))*40.0;
	  y = y / 40.0;
	  bullet[i].y = (y - floor(y))*40.0;
	  
	  if((bullet[i].x<=b1x1 && bullet[i].y<=b1y1 && bullet[i].y>=b1y4  && bullet[i].x>=b1x3)){
		   a=1;
		   break;
	  }
	}
  }
  if(a==1){
	   bullet[i].inuse = 0;
	   int zx1=(rand()%39);
	   int zy1=(rand()%34);
	   b1x1 = zx1;
	   b1x4 = zx1;
	   b1x2 = zx1-5;
	   b1x3 = zx1-5;
	   
	   b1y1=zy1;
	   b1y2=zy1;
	   b1y3=zy1-5;
	   b1y4=zy1-5;

	}  	
}

void shotBullet(void)
{
  int entry;
	
  entry = allocBullet();
  if (entry >= 0) 
  {
	initBullet(entry, glutGet(GLUT_ELAPSED_TIME));
  }
}

void drawBullets(void)
{
  int i;

  glBegin(GL_POINTS);
  glColor3f(1.0, 0.0, 1.0);
  for (i=0; i<BULLETS; i++) 
  {
	if (bullet[i].inuse)
	 {
	  glVertex2f(bullet[i].x, bullet[i].y);
	}
  }
  
  glEnd();
}

void drawshield(void)
{
	float rad;
	 glColor3f(0.1, 0.1, 1.0);
	glBegin(GL_LINE_LOOP);
	for (rad=0.0; rad<11.0; rad += 1.0) {
	  glVertex2f(3.75 * cos(2*rad/M_PI)+0.4, 3.0 * sin(2*rad/M_PI));}
	 glEnd();
}


void drawJet(float angle)
{
  

  glPushMatrix();
  glTranslatef(x, y, 0.0);
  glRotatef(angle, 0.0, 0.0, 1.0);
  if (thrust) 
  {
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-0.75, -0.5);
	glVertex2f(-1.75, 0);
	glVertex2f(-0.75, 0.5);
	glEnd();
  }
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_LINE_LOOP);
		glVertex2f(3.5,0.0);
		glVertex2f(0.5, -0.25);
		glVertex2f(-1.0, -1.0);
		glVertex2f(-0.5, 0.0);
		glVertex2f(-1.0, 1.0);
		glVertex2f(0.5,0.25);
		glVertex2f(3.5, 0.0);
  glEnd();
  
 
	
  if (shield) {
			
   drawshield();
	
   
  }
  glPopMatrix();
}

void display(void)
{ 
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.0,0.0,0.0,0.0);
  
	if(flag2==0)
	{
	screen();
	}
	else
	{
	  
	drawJet(angle);
	drawBullets();
	drawsquare();
	glFlush();
}
	glutSwapBuffers();
 
}


void idle(void)
{
  int time, delta;

  time = glutGet(GLUT_ELAPSED_TIME);
  if (resuming) {
	lastTime = time;
	resuming = 0;
  }
  if (left) {
	delta = time - leftTime;
	angle = angle + delta * 0.4;
	leftTime = time;
  }
  if (right) {
	delta = time - rightTime;
	angle = angle - delta * 0.4;
	rightTime = time;
  }
  if (thrust) {
	delta = time - thrustTime;
	v = delta * 0.000004;
	xv = xv + cos(angle*M_PI/180.0) * v;
	yv = yv + sin(angle*M_PI/180.0) * v;
	thrustTime = time;
  }
   
  delta = time - lastTime;
  x = x + xv * delta;
  y = y + yv * delta;
  x = x / 40.0;
  x = (x - floor(x))*40.0;
  y = y / 40.0;
  y = (y - floor(y))*40.0;
  lastTime = time;
  advanceBullets(delta, time);
  
  
  glutPostWindowRedisplay(currentWindow);
  
}

void visible(int vis)
{
  if (vis == GLUT_VISIBLE) {
	if (!paused) {
	  glutIdleFunc(idle);
	}
  } else {
	glutIdleFunc(NULL);
 }
}


void key(unsigned char key, int px, int py)
{
  switch (key) {
  case 27:
	exit(0);
	break;
  
  case 'S':
  case 's':
	shield = 1;
	break;
  case 'C':
  case 'c':
	cursor = !cursor;
	glutSetCursor(
	  cursor ? GLUT_CURSOR_INHERIT : GLUT_CURSOR_NONE);
	break;
  case 'z':
  case 'Z':
	x = 20;
	y = 20;
	xv = 0;
	yv = 0;
	break;
  case 'f':
  case 'F':
	glutGameModeString("400x300:16@60");
	glutEnterGameMode();
	initWindow();
	break;
  
  case 'l':
	if (originalWindow != 0 && currentWindow != originalWindow) {
	  glutLeaveGameMode();
	  currentWindow = originalWindow;
	}
	break;
  case 'P':
  case 'p':
	paused = !paused;
	if (paused) 
	{
	  glutIdleFunc(NULL);
	} else 
	{
	  glutIdleFunc(idle);
	  resuming = 1;
	}
	break;
  case 'Q':
  case 'q':
  case ' ':
	shotBullet();

	 break;
  case  'w':
  case  'W':
			flag2=1;
			break;
		
  case 'a': 
			flag3=1;
			break;    
  }
} 


void keyup(unsigned char key, int x, int y)
{
  switch (key) {
  
  case 'S':
  case 's':
	shield = 0;
	break;
  }
}


void special(int key, int x, int y)
{
  switch (key) {
 
  case GLUT_KEY_UP:
	thrust = 1;
	thrustTime = glutGet(GLUT_ELAPSED_TIME);
	break;
  
  case GLUT_KEY_LEFT:
	left = 1;
	leftTime = glutGet(GLUT_ELAPSED_TIME);
	break;
  case GLUT_KEY_RIGHT:
	right = 1;
	rightTime = glutGet(GLUT_ELAPSED_TIME);
	break;
  }
}


void
specialup(int key, int x, int y)
{
  switch (key) {
  case GLUT_KEY_UP:
	thrust = 0;
	break;
  case GLUT_KEY_LEFT:
	left = 0;
	break;
  case GLUT_KEY_RIGHT:
	right = 0;
	break;
  }
}


void initWindow(void)
{
  glutDisplayFunc(display);
  glutVisibilityFunc(visible);
  glutKeyboardFunc(key);
  glutKeyboardUpFunc(keyup);
  glutSpecialFunc(special);
  glutSpecialUpFunc(specialup);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 40, 0, 40, 0, 40);
  glMatrixMode(GL_MODELVIEW); 
  glPointSize(3.0);

  currentWindow = glutGetWindow();
}

void reshape (int w, int h)
{
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(w<=h)
  gluOrtho2D(0.0,40.0,0.0*(GLfloat)h/(GLfloat)w,40*(GLfloat)h/(GLfloat)w);
  else 
  gluOrtho2D(0.0*(GLfloat)w/(GLfloat)h,40.0*(GLfloat)w/(GLfloat)h,0.0,40.0);
  glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  if (argc > 1 && !strcmp(argv[1], "-fullscreen"))
   {
	glutGameModeString("640x480:16@60");
	glutEnterGameMode();
   } else 
  {
	originalWindow = glutCreateWindow("Fighter Jet");
  }
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  initWindow();

  glutMainLoop();
  return 0;  
		   
}

