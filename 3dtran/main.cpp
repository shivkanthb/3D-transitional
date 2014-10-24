#include<iostream>
#include<GLUT/glut.h>
#include<math.h>

#define PI 3.14


using namespace std;

void display();
void specialKeys();

int Cx=0,Cy=0,r=0;
int mode = 0, sub = 0; //1 - Translate, 2 - Rotate, 3 - Scale, 4 - Reflection, 5 - Shear
int refX, refY,refZ;
float angle;
float Input[3][1], Output[3][1], Input1[4][1], Output1[4][1];
float InputPoints[4][2] = {{50,50},{150,50},{150,150},{50,150}};
float InputPoints1[8][2] = {{0,0},{0,50},{50,50},{50,0},{25,25},{25,75},{75,75},{75,25}};
float Transform[][3] = {{1,0,0},{0,1,0},{0,0,1}};
float Transform1[][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
float sX,sY,sh;

void myInit() {
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0f,0.0f,0.0f);
	glPointSize(2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-320.0,320.0,-240.0,240.0);
}

inline void plotVertex(int v)
{
    glVertex3f(overtices[v][0],overtices[v][1],overtices[v][2]);
}

void multiplyMatrices(float A[][3], float B[][1], int r1, int c1, int r2, int c2) {
	int i, j, k;
	for(i=0; i<r1; i++)	{
		for(j=0; j<c2; j++) {
			Output[i][j] = 0;
			for(k=0; k<c1; k++) {
				Output[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void translate() {
	int i;
	
	Transform1[0][3] = refX;
	Transform1[1][3] = refY;
    Transform1[2][3] = refZ;
    

	/*for(i=0; i<4; i++) {
		Input[0][0] = InputPoints[i][0];
		Input[1][0] = InputPoints[i][1];
		Input[2][0] = 1;
		
		multiplyMatrices(Transform,Input,3,3,3,1);
		glVertex2d(Output[0][0],Output[1][0]);
	}*/
    
    for(i=0; i<4; i++) {
		Input1[0][0] = InputPoints1[i][0];
		Input1[1][0] = InputPoints1[i][1];
		//Input1[2][0] = InputPoints1[i][2];
        Input1[3][0] = 1;
		
		multiplyMatrices(Transform,Input,3,3,3,1);
    }

}

void rotate() {
    
	Transform[0][0] = cos(angle);
    Transform[0][1] = -sin(angle);
	Transform[0][2] = refX*(1 - cos(angle)) + refY*sin(angle);
	Transform[1][0] = sin(angle);
    Transform[1][1] = cos(angle);
	Transform[1][2] = refY*(1 - cos(angle)) - refX*sin(angle);
    
	for(int i=0; i<4; i++) {
		Input[0][0] = InputPoints[i][0];
		Input[1][0] = InputPoints[i][1];
		Input[2][0] = 1;
		
		multiplyMatrices(Transform,Input,3,3,3,1);
		glVertex2d(Output[0][0],Output[1][0]);
	}
    
}

void scale() {
    
	Transform[0][0] = sX;
    Transform[0][2] = refX* (1-sX);
    Transform[1][1] = sY;
	Transform[1][2] = refY*(1 - sY);
    
	for(int i=0; i<4; i++) {
		Input[0][0] = InputPoints[i][0];
		Input[1][0] = InputPoints[i][1];
		Input[2][0] = 1;
		
		multiplyMatrices(Transform,Input,3,3,3,1);
		glVertex2d(Output[0][0],Output[1][0]);
	}
    
}

void reflect() {
    
    switch(sub) {
        case 1: Transform[1][1] = -1;
            break;
        case 2: Transform[0][0] = -1;
            break;
        case 3: Transform[0][0] = Transform[1][1] = -1;
            break;
        case 4: Transform[0][0] = Transform[1][1] = 0;
            Transform[0][1] = Transform[1][0] = 1;
            break;
    }
    
	for(int i=0; i<4; i++) {
		Input[0][0] = InputPoints[i][0];
		Input[1][0] = InputPoints[i][1];
		Input[2][0] = 1;
		
		multiplyMatrices(Transform,Input,3,3,3,1);
		glVertex2d(Output[0][0],Output[1][0]);
	}
}

void shear() {
    switch(sub) {
        case 1: Transform[0][1] = sh;
            Transform[0][2] = -sh*refY;
            break;
            
        case 2: Transform[1][0] = sh;
            Transform[1][2] = -sh*refX;
            break;
    }
    
    for(int i=0; i<4; i++) {
		Input[0][0] = InputPoints[i][0];
		Input[1][0] = InputPoints[i][1];
		Input[2][0] = 1;
		
		multiplyMatrices(Transform,Input,3,3,3,1);
		glVertex2d(Output[0][0],Output[1][0]);
	}
}

void getInput() {
	cout<<"Given a square with center (100,100) and side length 100, the following\
    \ntransformations can be applied:\
    \n1. Translation\
    \n2. Rotation\
    \n3. scaling\n4. Reflection\
    \n5. Shearing\
    \nEnter choice: ";
        cin>>mode;
	
	switch(mode) {
		case 1: cout<<"Enter the x co-ordinate of the translation vector: ";
            cin>>refX;
            cout<<"Enter the y co-ordinate of the translation vector: ";
            cin>>refY;
            cout<<"Enter the z co-ordinate of the translation vector: ";
            cin>>refZ;
            break;
		case 2: cout<<"Enter the x co-ordinate of the reference vector: ";
            cin>>refX;
            cout<<"Enter the y co-ordinate of the reference vector: ";
            cin>>refY;
            cout<<"Enter the angle of rotation (in degrees): ";
            cin>>angle;
            angle = angle * PI / 180;
            break;
		case 3: cout<<"Enter the x co-ordinate of the reference vector: ";
            cin>>refX;
            cout<<"Enter the y co-ordinate of the reference vector: ";
            cin>>refY;
            cout<<"Enter the x scaling factor: ";
            cin>>sX;
            cout<<"Enter the y scaling factor: ";
            cin>>sY;
            break;
        case 4: cout<<"\n1. Reflect wrt x-axis\n2. Reflect wrt y-axis\
            \n3. Reflect wrt origin\n4. Reflect wrt x=y\
            \nEnter your choice: ";
            cin>>sub;
            break;
        case 5: cout<<"Enter the x co-ordinate of the reference vector: ";
            cin>>refX;
            cout<<"Enter the y co-ordinate of the reference vector: ";
            cin>>refY;
            cout<<"\n1. X-shear\n2. Y-shear\nEnter your choice: ";
            cin>>sub;
            cout<<"Enter the shear magnitude: ";
            cin>>sh;
            break;
	}
}


void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
    Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    // Reset transform_matrixations
    glLoadIdentity();
    
    //Drawing the cube!
    
    //Multicolor - FRONT
    glBegin(GL_POLYGON);
    glColor3f( 1.0, 0.0, 0.0 );
    plotVertex(4);
    glColor3f( 0.0, 1.0, 0.0 );
    plotVertex(5);
    glColor3f( 0.0, 0.0, 1.0 );
    plotVertex(6);
    glColor3f( 1.0, 0.0, 1.0 );
    plotVertex(7);
    glEnd();
    
    // White side - BACK
    glBegin(GL_POLYGON);
    glColor3f(   1.0,  1.0, 1.0 );
    plotVertex(0);
    plotVertex(1);
    plotVertex(2);
    plotVertex(3);
    glEnd();
    
    // Purple side - RIGHT
    glBegin(GL_POLYGON);
    glColor3f(  1.0,  0.0,  1.0 );
    plotVertex(1);
    plotVertex(2);
    plotVertex(6);
    plotVertex(5);
    glEnd();
    
    // Green side - LEFT
    glBegin(GL_POLYGON);
    glColor3f(   0.0,  1.0,  0.0 );
    plotVertex(0);
    plotVertex(3);
    plotVertex(7);
    plotVertex(4);
    glEnd();
    
    // Blue side - TOP
    glBegin(GL_POLYGON);
    glColor3f(   0.0,  0.0,  1.0 );
    plotVertex(2);
    plotVertex(3);
    plotVertex(7);
    plotVertex(6);
    glEnd();
    
    // Red side - BOTTOM
    glBegin(GL_POLYGON);
    glColor3f(   1.0,  0.0,  0.0 );
    plotVertex(0);
    plotVertex(1);
    plotVertex(5);
    plotVertex(4);
    glEnd();
    
    glFlush();
    glutSwapBuffers();
    
}

	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_LINES);
    glVertex2d(-320,0);
    glVertex2d(320,0);
    glVertex2d(0,-240);
    glVertex2d(0,240);
	glEnd();
	
    /*glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBegin(GL_POLYGON);
    
    glColor3f( 1.0, 0.0, 0.0 );     glVertex3f(  50, -50, -50 );      // P1 is red
    glColor3f( 0.0, 1.0, 0.0 );     glVertex3f(  50,  50, -50 );      // P2 is green
    glColor3f( 0.0, 0.0, 1.0 );     glVertex3f( -50,  50, -50 );      // P3 is blue
    glColor3f( 1.0, 0.0, 1.0 );     glVertex3f( -50, -50, -50 );      // P4 is purple
    
    glEnd();
    glFlush();
     glutSwapBuffers();
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_QUADS);
    glVertex2d(0,0);
    glVertex2d(0,50);
    glVertex2d(50,50);
    glVertex2d(50,0);
	glEnd();
    
    glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_QUADS);
    glVertex2d(25,25);
    glVertex2d(25,75);
    glVertex2d(75,75);
    glVertex2d(75,25);
	glEnd();
    
    glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_QUADS);
    glVertex2d(0,50);
    glVertex2d(25,75);
    glVertex2d(75,75);
    glVertex2d(50,50);
	glEnd();
    
    glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_QUADS);
    glVertex2d(50,0);
    glVertex2d(50,50);
    glVertex2d(75,75);
    glVertex2d(75,25);
	glEnd();
    
    
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
    // Top face (y = 1.0f)
    // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f( 1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f,  1.0f);
    glVertex3f( 1.0f, 1.0f,  1.0f);
    
    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    
    // Front face  (z = 1.0f)
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f( 1.0f,  1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, 1.0f);
    
    // Back face (z = -1.0f)
    glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    
    // Left face (x = -1.0f)
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    
    // Right face (x = 1.0f)
    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(1.0f,  1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();  // End of drawing color-cube
    */
    
	 //glutSwapBuffers();
	
	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_QUADS);
	switch(mode) {
		case 1: translate();
            break;
		case 2: rotate();
            break;
		case 3: scale();
            break;
        case 4: reflect();
            break;
        case 5: shear();
            break;
	}
	
	glEnd();
	glFlush();
    glutSwapBuffers();
}

int main(int argc, char* argv[]) {
    
	getInput();
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(640,480);
	glutCreateWindow("2D Transformations");
	glutDisplayFunc(myDisplay);
	myInit();
	glutMainLoop();
    
	return 1;
}
