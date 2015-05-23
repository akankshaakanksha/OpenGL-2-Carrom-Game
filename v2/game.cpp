#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
using namespace std;
#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
//Class
class Box {
	float len,colorr,colorg,colorb;
	public:
	 void set_values(float,float,float,float); 	
	 void draw_box();
};
void Box::set_values(float box_len,float r,float g,float b)
{
	len = box_len;
	colorr = r;
	colorg = g;
	colorb = b;
}
void Box::draw_box()   {
    
    glBegin(GL_QUADS);
    glColor3f(colorr, colorg, colorb);
    glVertex2f(-len / 2, -len / 2);
    glVertex2f(len / 2, -len / 2);
    glVertex2f(len / 2, len / 2);
    glVertex2f(-len / 2, len / 2);
    glEnd();
    
}

class Pocket  {
	public:
          float  x,y;
	  void set_values(float,float);
	  
};
void Pocket::set_values(float poc_x,float poc_y)
{
	x = poc_x;
	y = poc_y;
}	



class Coin      {
	public:
	float vel_x,vel_y,pos_x,pos_y,prev_vel_x,prev_vel_y,vel;
	int col,active;
	void draw()
    	{
			glTranslatef(pos_x, pos_y, -8.0f);
			if(col==0)
				glColor3f(0.0, 0.0, 0.0);
			else if(col==1)
				glColor3f(1.0, 1.0, 1.0);
			else
				glColor3f(0.6,0.247,0.423);
		        glBegin(GL_TRIANGLE_FAN);
 			float rad = 0.1f;
                        for(int i=0 ; i<360 ; i++) {
                       		glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
    			}
    			glEnd();	
		}
	void set_values(float x,float y,int color)
		{
			active=0;
			pos_x=x;
			pos_y=y;
			vel_x=0.0f;
			vel_y=0.0f;
			col=color;
			
		}

};
class Striker : public Coin   {
	public:
		int mass,active,shoot_status;
		float i,rad,temp_vel_x,temp_vel_y;
		void reset()
		{
			vel_x=0.0f;
			vel_y=0.0f;
			pos_x=0.0f;
			pos_y=-1.42f;
			rad=1.0f;
			i=90.0f;
			active=0;
			shoot_status=0;	
		}
		void set_pos(float x)
		{
			if(pos_x+x<1.45f && pos_x+x>-1.45f)
				pos_x+=x;
		}
		void change_speed(int dir)
		{
			draw_dir();
			update_speed();
			if(dir==0)
				rad+=0.1f;
			else if(dir==1 && rad>0.0f)
				rad-=0.1f;
				
		}
		void update_speed()
		{
			
			
			temp_vel_x=(rad/300.0f)*cos(DEG2RAD(i));
			temp_vel_y=(rad/300.0f)*sin(DEG2RAD(i));
			
		}
		void draw_dir()
		{
    		
			glPushMatrix();
			glTranslatef(pos_x, pos_y, 0.0f);
			glLineWidth(1.5); 
    			glColor3f(1.0, 0.0, 0.0);
    			glBegin(GL_LINES);
    			glVertex3f(0.0f, 0.0f, -8.0f);
    			glVertex3f(rad*cos(DEG2RAD(i)), rad*sin(DEG2RAD(i)),-8.0f);
			glPopMatrix();
    			glEnd();
			glutSwapBuffers();
		}
		void change_dir(int dir)
		{
			draw_dir();
			if(dir==0 && i<180.0f)
				i=i+2.0f;
			else if(dir==1 && i>0.0f)
				i=i-2.0f;
			update_speed();
		}
		void draw()
    		{
			glTranslatef(pos_x, pos_y, -8.0f);
		        glBegin(GL_TRIANGLE_FAN);
 			float rad = 0.13f;
                        for(int j=0 ; j<360 ; j++) {
                       		glVertex2f(rad * cos(DEG2RAD(j)), rad * sin(DEG2RAD(j)));
    			}
    			glEnd();	
		}
		void shoot()
		{
			if(shoot_status==0)
			{
				update_speed();
				vel_x=temp_vel_x;
				vel_y=temp_vel_y;
				shoot_status=1;	
			}
		}

};

class GamePlay{
	public:
		
		Coin* queen;
		int count;
		double friction;
		int points;
		double rad;
		int color;
		Coin* coins[10];		
		GamePlay(int coins1, int points1);
		void drawBoard();
		void loadCoins();
		void initialiseCoins();

};

// Function Declarations
void drawScene();
void drawBox(float len);
void initRendering();
void handleResize(int w, int h);
void drawText(char*string); 
void keyboard(unsigned char key, int x, int y);
void Draw_line(float startx,float starty,float endx, float endy);
void inputKey(int key, int x, int y);
void update(int value);


// Global Variables

Box outer,inner;
Pocket pock[4];
Striker striker;
Coin coin[9];
char scoreh[100];
int score=30;
time_t curtime,prevtime;
long long int tym_elapsed;
struct timeval tv;
int chosen;


int main(int argc, char **argv) {

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    int w = glutGet(GLUT_SCREEN_WIDTH);
    int h = glutGet(GLUT_SCREEN_HEIGHT);
    int windowWidth = w * 2 / 3;
    int windowHeight = h * 2 / 3;

    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

    glutCreateWindow("Carrom");
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    outer.set_values(4.5f,0.54509803921f, 0.27058823529f, 0.07450980392f);
    inner.set_values(4.0f,0.96078431372f, 0.87058823529f , 0.70196078431f);
    pock[0].set_values(1.85f,1.85f);
    pock[1].set_values(-1.85f,1.85f);
    pock[2].set_values(-1.85f,-1.85f);
    pock[3].set_values(1.85f,-1.85f);
   
    coin[0].set_values(0.0f,0.0f,2);
    coin[1].set_values(0.0f,0.4f,1);
    coin[2].set_values(0.4f,0.0f,1);
    coin[3].set_values(0.0f,-0.4f,1);
    coin[4].set_values(-0.4f,0.0f,1);
    coin[5].set_values(0.253f,0.253f,0);
    coin[6].set_values(-0.253f,0.253f,0);
    coin[7].set_values(0.253f,-0.253f,0);	 		
    coin[8].set_values(-0.253f,-0.253f,0);	
    striker.reset();
    initRendering();

    // Register callbacks
    glutDisplayFunc(drawScene);
    glutIdleFunc(drawScene);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(inputKey);
    glutReshapeFunc(handleResize);
    glutTimerFunc(20, update, 0);		
    
    glutMainLoop();
    return 0;
}

void striker_friction(void)
{
	
	float vel_striker,prev_vel_x,prev_vel_y;
	if(striker.active==1)
	{
		
		vel_striker=sqrt(pow(striker.vel_x,2) + pow(striker.vel_y,2));
		prev_vel_x=striker.vel_x;
		prev_vel_y=striker.vel_y;
		striker.vel_x=(striker.vel_x/vel_striker)*(vel_striker-0.0001f);
		striker.vel_y=(striker.vel_y/vel_striker)*(vel_striker-0.0001f);
		if(prev_vel_x*striker.vel_x<0 || prev_vel_y*striker.vel_y<0)
		{
			striker.vel_x=0;
			striker.vel_y=0;
			striker.active=0;
		}
			
	}

}
void coinfriction(int i)
{
	if(coin[i].active==1)
		{
			coin[i].vel=sqrt(pow(coin[i].vel_x,2) + pow(coin[i].vel_y,2));
			coin[i].prev_vel_x=coin[i].vel_x;
			coin[i].prev_vel_y=coin[i].vel_y;
			coin[i].vel_x=(coin[i].vel_x/coin[i].vel)*(coin[i].vel-0.00005f);
			coin[i].vel_y=(coin[i].vel_y/coin[i].vel)*(coin[i].vel-0.00005f);
			if(coin[i].prev_vel_x*coin[i].vel_x<0 || coin[i].prev_vel_y*coin[i].vel_y<0)
			{
				coin[i].vel_x=0;
				coin[i].vel_y=0;
				coin[i].active=0;
			}
			
		}
}

void update_pos(int tym_elapsed,int i)
{
	if(tym_elapsed>=0)
		{
		   coin[i].pos_x+=(tym_elapsed*coin[i].vel_x);
		   coin[i].pos_y+=(tym_elapsed*coin[i].vel_y);
		   
		}
	if (tym_elapsed>=0 && i==20)
		{
			striker.pos_x+=(tym_elapsed*striker.vel_x);
		   striker.pos_y+=(tym_elapsed*striker.vel_y);
		}
}

void wallcollision(int wallrad,int i)
{
	if(coin[i].pos_y>=wallrad || coin[i].pos_y<=-wallrad)
		{
			if(coin[i].vel_y>=0 && coin[i].pos_y>=1.9f)
				coin[i].vel_y*=-1;
			else if(coin[i].vel_y<=0 && coin[i].pos_y<=-1.9f)
				coin[i].vel_y*=-1;	
		}
	if(coin[i].pos_x>=wallrad|| coin[i].pos_x<=-wallrad)
		{
			if(coin[i].vel_x>=0 && coin[i].pos_x>=1.9f)
				coin[i].vel_x*=-1;
			else if(coin[i].vel_x<=0 && coin[i].pos_x<=-1.9f)
				coin[i].vel_x*=-1;
		}

	if(i==20){
	if(striker.pos_y>=wallrad || striker.pos_y<=-wallrad)
		{
			if(striker.vel_y>=0 && striker.pos_y>=1.87f)
				striker.vel_y*=-1;
			else if(striker.vel_y<=0 && striker.pos_y<=-1.87f)
				striker.vel_y*=-1;	
		}
	if(striker.pos_x>=wallrad || striker.pos_x<=-wallrad)
		{
			if(striker.vel_x>=0 && striker.pos_x>=1.87f)
				striker.vel_x*=-1;
			else if(striker.vel_x<=0 && striker.pos_x<=-1.87f)
				striker.vel_x*=-1;
		}}
}

float second;
void update(int value)
{
	
	gettimeofday(&tv,NULL);
	prevtime=curtime;
	curtime=tv.tv_usec;
	tym_elapsed=(curtime-prevtime)/1000;

		striker_friction();
		
	//motion of coins
	for(int i=0;i<9;i++)
	{
		
		coinfriction(i);
		update_pos(tym_elapsed,i);
		wallcollision(1.9f,i);
		
	
	}	

	wallcollision(1.87,20);
	update_pos(tym_elapsed,20);
	
	//printf("%lld %f\n",tym_elapsed,striker.vel_y);
      
	
	float mod_dir,sx_unit,sy_unit,length_s1,length_s2,s1x,s1y,ds1x,ds1y,s2x,s2y,ds2x,ds2y;
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(i!=j && +(pow(coin[j].pos_x- coin[i].pos_x,2) + pow(coin[j].pos_y-coin[i].pos_y,2)<=pow(0.23f,2)) && (((coin[i].pos_x-coin[j].pos_x)*(coin[i].vel_x - coin[j].vel_x)+((coin[i].pos_y-coin[j].pos_y)*(coin[i].vel_y-coin[j].vel_y))))<0)
			{
					
				mod_dir=sqrt(pow(coin[i].pos_x-coin[j].pos_x,2)+pow(coin[i].pos_y-coin[j].pos_y,2));
				sx_unit=((coin[j].pos_x-coin[i].pos_x)/mod_dir); 
				sy_unit=((coin[j].pos_y-coin[i].pos_y)/mod_dir);
				length_s1=sx_unit*coin[i].vel_x + sy_unit*coin[i].vel_y;
				s1x=sx_unit*length_s1;
				s1y=sy_unit*length_s1;
				ds1x=coin[i].vel_x-s1x;
				ds1y=coin[i].vel_y-s1y;
				length_s2=(coin[j].vel_x*-1*sx_unit)+(coin[j].vel_y*-1*sy_unit);
				s2x=sx_unit*-1*length_s2;
				s2y=sy_unit*-1*length_s2;
				ds2x=coin[j].vel_x-s2x;
				ds2y=coin[j].vel_x-s2y;
				coin[i].vel_x= s2x+ds1x;
				coin[i].vel_y=s2y+ds1y;
				coin[j].vel_x=s1x+ds2x;
				coin[j].vel_y=s1y+ds2y;
				coin[i].active=1;
				coin[j].active=1;
			}
		}
	}
	for(int i=0;i<9;i++)
	{
		
		
			if((pow(striker.pos_x- coin[i].pos_x,2) + pow(striker.pos_y-coin[i].pos_y,2)<=pow(0.23f,2)) && (((coin[i].pos_x-striker.pos_x)*(coin[i].vel_x - striker.vel_x)+((coin[i].pos_y-striker.pos_y)*(coin[i].vel_y-striker.vel_y))))<0)
			{
					
				mod_dir=sqrt(pow(coin[i].pos_x-striker.pos_x,2)+pow(coin[i].pos_y-striker.pos_y,2));
				sx_unit=((striker.pos_x-coin[i].pos_x)/mod_dir); 
				sy_unit=((striker.pos_y-coin[i].pos_y)/mod_dir);
				length_s1=sx_unit*coin[i].vel_x + sy_unit*coin[i].vel_y;
				s1x=sx_unit*length_s1;
				s1y=sy_unit*length_s1;
				ds1x=coin[i].vel_x-s1x;
				ds1y=coin[i].vel_y-s1y;
				length_s2=(striker.vel_x*-1*sx_unit)+(striker.vel_y*-1*sy_unit);
				s2x=sx_unit*-1*length_s2;
				s2y=sy_unit*-1*length_s2;
				ds2x=striker.vel_x-s2x;
				ds2y=striker.vel_x-s2y;
				coin[i].vel_x= s2x+ds1x;
				coin[i].vel_y=s2y+ds1y;
				striker.vel_x=s1x+ds2x;
				striker.vel_y=s1y+ds2y;
				coin[i].active=1;
			}
		
	}
	//coins fell in pocket
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<9;j++)
		{
			if((pow(coin[j].pos_x- pock[i].x,2) + pow(coin[j].pos_y-pock[i].y,2))<=pow(0.13f,2))
 	   		{
    				coin[j].pos_x=pock[i].x;
    				coin[j].pos_y=pock[i].y;
				coin[j].vel_x=0.0f;
				coin[j].vel_y=0.0f;
				coin[j].active=0;
				if(coin[j].col==chosen)
				{
					score+=10;
					coin[j].pos_x=3.0f;
					coin[j].pos_y=0.0f;
				}
				else if(coin[j].col==2)
				{
					score+=50;
					coin[j].pos_x=3.0f;
					coin[j].pos_y=1.0f;
				}
				else
				{
					coin[j].pos_x=-3.0f;
					coin[j].pos_y=0.0f;
					score-=5;
				}	
    			}
		}
		if((pow(striker.pos_x- pock[i].x,2) + pow(striker.pos_y-pock[i].y,2))<=pow(0.15f,2))
		{
    				striker.pos_x=pock[i].x;
    				striker.pos_y=pock[i].y;
				striker.vel_x=0.0f;
				striker.vel_y=0.0f;
				if(striker.active==1)
					score-=5;
				striker.active=0;	
    		}
	}
	
	glutTimerFunc(20, update, 0);	
}
void inputKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT : 
			striker.set_pos(-0.1f);
			break;
		case GLUT_KEY_RIGHT : 
			striker.set_pos(0.1f);
			break;
		case GLUT_KEY_UP :
			striker.change_speed(0);
			break;
		case GLUT_KEY_DOWN :
			striker.change_speed(1);
			break;
	}
}

void drawText(char* score) 
{ 
 char *c; 
 glPushMatrix(); 
 glTranslatef(-5.55555f,2.0f ,-8.0f); 
 glScalef(0.75/152.38, 0.75/152.38, 1/152.38);
 for (c=score; *c != '\0'; c++) 
 { 
  glutStrokeCharacter(GLUT_STROKE_ROMAN , *c); 
 } 
 glPopMatrix(); 
}
// Function to draw objects on the screen
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    sprintf(scoreh,"Score:%d",score); 
    glColor3f(1.0f,0.0f,0.0f);
    drawText(scoreh);	
    //Draw outer Box
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -8.0f);
    outer.draw_box();
    glPopMatrix();
    //Draw inner box
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -8.0f);
    inner.draw_box();
    glPopMatrix();


    //Pockets 
    for(int i=-1;i<=1;i+=2)
    {
		for(int j=-1;j<=1;j+=2)
		{
			glPushMatrix();
			glTranslatef(1.85*i, 1.85*j, -8.0f);
		    glColor3f(0.0f, 0.0f, 0.0f);
		    glBegin(GL_TRIANGLE_FAN);
		    float rad = 0.15f;
		    for(int i=0 ; i<360 ; i++) {
		       glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
		    }
		    glEnd();
	        glPopMatrix();
	    }
	}
    	
	//Decoration   
    glColor3f(0.80f, 0.20f, 0.20f);
    for(int i=-1;i<=1;i+=2)
    {
		for(int j=-1;j<=1;j+=2)
		{
			glPushMatrix();
	 		glTranslatef(1.45f*i,1.45f*j, -8.0f);
			glBegin(GL_TRIANGLE_FAN);
			float rad = 0.15f;
			for(int i=0 ; i<360 ; i++) {
	  			 glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
			}
	    glEnd();
        glPopMatrix();
    }
    }	
    //Line Decor
    Draw_line(1.45f,1.55f,-1.45f,1.55f);
    Draw_line(1.55f,1.45f,1.55f,-1.45f);
    Draw_line(-1.55f,1.45f,-1.55f,-1.45f);
    Draw_line(1.45f,-1.55f,-1.45f,-1.55f);
    Draw_line(1.45f,1.35f,-1.45f,1.35f);
    Draw_line(1.35f,1.45f,1.35f,-1.45f);
    Draw_line(-1.35f,1.45f,-1.35f,-1.45f);
    Draw_line(1.45f,-1.35f,-1.45f,-1.35f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -8.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    //Center Decor
    glBegin(GL_LINE_LOOP);
    float rad = 0.5f;
    for(int i=0 ; i<360 ; i++) {
       glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
    }
    glEnd();
    glPopMatrix();
    

    //Striker
    glTranslatef(0.0f, 0.0f, 0.0f);
    glColor3f(0.513f, 0.011f, 0.0f);
    glPushMatrix();
    striker.draw();
    glPopMatrix();
    
    //Coins 
    for(int i=0;i<9;i++)
    {
	glPushMatrix();
	coin[i].draw();
        glPopMatrix();
    }			
    
    glutSwapBuffers();
}

// Initializing some openGL 3D rendering options
void initRendering() {

    glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
    glEnable(GL_COLOR_MATERIAL);    // Enable coloring
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
      case 'a':
	striker.change_dir(0);
	break;
      case 'c':
	striker.change_dir(1);
	break;
	   case 'b':
			chosen=1;
        break;	
        case 'w':
			chosen=0;
        break;			
      case 32:
	striker.active=1;
	striker.shoot();
	break;
      case 13:
	striker.reset();	 	
   }
}



void Draw_line(float startx,float starty,float endx, float endy)
{
    glLineWidth(1.5); 
    glColor3f(0.80, 0.20, 0.20);
    glBegin(GL_LINES);
    glVertex3f(startx, starty, -8.0f);
    glVertex3f(endx, endy,-8.0f);
    glEnd();
}


