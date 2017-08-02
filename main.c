#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "shader.h"
#include "vector.h"
#include "camera.h"

#define EXIT_SUCCESS 0
#define EXIT_ERROR 1

#define true 1
#define false 0

#define MENU_FOCUS 0
#define VIEW_FOCUS 1

#define START_HEIGHT 400
#define START_WIDTH 400

#define START_FOV 50.0

//Functions
void sendKeySignals();
void printMonitors();

//Mandelbulb shader and variables
shaderprogram mandelbulb_shader;
vec2f resolution;
vec3f fov, camerapos, cameradir, color, horizontalAxis, verticalAxis, depthAxis;
vec4f totalRotation;
float step;
int bail, multisampling;
float power;
float phi;
float theta;

//Camera variables
float vfov, hfov;
float cameradist;

//State of program (view or menu)
int userfocus = VIEW_FOCUS;

//Timer
float lastframe;
float lastlastframe;

//Render Funcion
void render() {    

    //Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUADS);
	    glVertex3f(1.0f, 1.0f, 0.0);
	    glVertex3f(1.0f, -1.0f, 0.0);
	    glVertex3f(-1.0f, -1.0f, 0.0);
        glVertex3f(-1.0f, 1.0f, 0.0);
    glEnd();

    glutSwapBuffers();
    glFinish();

    lastlastframe = lastframe;
    lastframe = glutGet(GLUT_ELAPSED_TIME);

    printMonitors();
}

//Idle Function
void idle() {
    printf("*********************************\n");
    printf("cameradir: %f,%f,%f\n", cameradir.x, cameradir.y, cameradir.z);
    printf("horizontalAxis: %f,%f,%f\n", horizontalAxis.x, horizontalAxis.y, horizontalAxis.z);
    printf("verticalAxis: %f,%f,%f\n", verticalAxis.x, verticalAxis.y, verticalAxis.z);
    printf("depthAxis: %f,%f,%f\n", depthAxis.x, depthAxis.y, depthAxis.z);
    
    sendKeySignals();


    //phi+=.01;
    loadMandelbulbVars(mandelbulb_shader, fov, camerapos, cameradir, horizontalAxis, verticalAxis, depthAxis, color, step , bail,
        power, phi, theta, totalRotation, resolution, multisampling);
    render();
}

//Handle mouse input
void handleMouse(int x, int y) {
    if (userfocus == VIEW_FOCUS) {
        cameraMoveMouse(x, y);
    }
}


//Handle keyboard input
static unsigned char kbstate[256];
static clock_t kbtime[256];
static clock_t kblasttime[256];

void clearKeyBuffer() {
    int i = 0;

    while (i < 256) {
        kbstate[i] = false;
        kbtime[i] = 0;
        kblasttime[i] = 0;
        i++;
    }
}

void handleKeyboard(unsigned char key, int x, int y) {
    clock_t time = clock();
    
    if (kbstate[key] == false) {
        kbstate[key] = true;
        kbtime[key] = time;
        kblasttime[key] = time;
    }
}

void handleKeyboardUp(unsigned char key, int x, int y) {
    kbstate[key] = false;
    kbtime[key] = 0;
    kblasttime[key] = 0;

    //printf("%c", key);
}

void sendKeySignals() {
    clock_t time = clock();    
    unsigned char key;
    
    for (key = 0; key != 255; key++) {
        if (kbstate[key] == true) {
            putchar(key);
            printf("*********************************\n");
            printf("cameradir: %f,%f,%f\n", cameradir.x, cameradir.y, cameradir.z);
            printf("horizontalAxis: %f,%f,%f\n", horizontalAxis.x, horizontalAxis.y, horizontalAxis.z);
            printf("verticalAxis: %f,%f,%f\n", verticalAxis.x, verticalAxis.y, verticalAxis.z);
            printf("depthAxis: %f,%f,%f\n", depthAxis.x, depthAxis.y, depthAxis.z);
            if (userfocus == VIEW_FOCUS) {
                cameraMoveKeyboard(key, (float)(time-kblasttime[key])/CLOCKS_PER_SEC);
                //printf("HEY: %d, %d", key, (int)(time-kblasttime[key]));
                kblasttime[key] = time;
            }
        }
    }

}

void handleResolution(int w, int h) {
    //printf("nw: %d", w);
    //printf("HEY: %f\n", hfov);
    //printf("dist %f\n", cameradist);

    //Set viewport
    glViewport(0, 0, w, h);

    hfov = atan(w/(2*cameradist))/(2*PI_CONST)*720; 
    vfov = atan(h/(2*cameradist))/(2*PI_CONST)*720;
    //printf("newfov: %f\n", hfov);

    setFOVvec(&fov, vfov, hfov);
    //printf("nvec: %f, %f, %f\n", fov.x, fov.y, fov.z);
}

void printMonitors() {
    char string[80];    
    float fps = 1000/(lastframe-lastlastframe);
    
    strcpy(string, "FPS:");

    sprintf(string+4, "%f", fps);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();             
    glLoadIdentity();   
    int w = glutGet( GLUT_WINDOW_WIDTH );
    int h = glutGet( GLUT_WINDOW_HEIGHT );
    glOrtho( 0, w, 0, h, -1, 1 );

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable( GL_DEPTH_TEST ); 

    glColor3f(1, 0, 0);

    glRasterPos2i(20, 20);
    void *font = GLUT_BITMAP_HELVETICA_18; 
    for (char* c=string; *c != '\0'; c++) 
    {
        glutBitmapCharacter(font, *c); 
    }

    glEnable (GL_DEPTH_TEST);  

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();  

}

//Main
int main(int argc, char* argv[]) {

    //printf("hello main\n");
    fflush(stdout);

    //Set vars
    hfov = vfov = START_FOV;
    cameradist = START_WIDTH/(2*tan(START_FOV/360*PI_CONST));
    setFOVvec(&fov, vfov, hfov);
    InitializeCamera(&cameradir, &camerapos, &depthAxis, &horizontalAxis, &verticalAxis); 

    totalRotation.x = 0; totalRotation.y = 0; totalRotation.z = 0; totalRotation.w = 1;
    color.x=0; color.y=1; color.z=1;
    step = 0.01;
    bail = 10;
    multisampling=1;
    power = 10;
    phi = 0;
    theta = 0;
    resolution.x = START_WIDTH; resolution.y = START_HEIGHT;

    //Setup window
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(resolution.x, resolution.y);
	glutCreateWindow("3D Mandelbulb Viewer");
    
    //Setup Input
    glutIgnoreKeyRepeat(true);
    clearKeyBuffer();

    //Setup functions
    glutDisplayFunc(render);
    glutIdleFunc(idle);
    glutMotionFunc(handleMouse);
    glutKeyboardFunc(handleKeyboard);
    glutKeyboardUpFunc(handleKeyboardUp);
    glutReshapeFunc(handleResolution);

    glewInit();
   
    //printf("starting shaders\n");
    fflush(stdout);

    //Setup shaders
    loadMandelbulbProgram(&mandelbulb_shader, fov, camerapos, cameradir,
		horizontalAxis, verticalAxis, depthAxis, color, step, bail, power,
		phi, theta, totalRotation, resolution, multisampling);
    printf("loaded program\n");
    fflush(stdout);
    printProgramLog(mandelbulb_shader);

    //Loop
    glutMainLoop();

    return EXIT_SUCCESS;
}
