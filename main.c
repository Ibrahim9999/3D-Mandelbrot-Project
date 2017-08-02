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


//Mandelbulb shader and variables
shaderprogram mandelbulb_shader;
vec3f fov, camerapos, cameradir, color, horizontalAxis, verticalAxis, depthAxis;
vec4f totalRotation;
float step;
int bail;
float power;
float phi;
float theta;

//Camera variables
float vfov, hfov;
float cameradist;

//State of program (view or menu)
int userfocus = VIEW_FOCUS;

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

    //printf("rendered");
}

//Idle Function
void idle() {
    sendKeySignals();


    //phi+=.01;
	power = 10;
    loadMandelbulbVars(mandelbulb_shader, fov, camerapos, cameradir, horizontalAxis, verticalAxis, depthAxis, color, step , bail,
        power, phi, theta, totalRotation);

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

    printf("%c", key);
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

//Main
int main(int argc, char* argv[]) {

    printf("hello main\n");
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
    power = 10;
    phi = 0;
    theta = 0;

    //Setup window
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(START_WIDTH,START_HEIGHT);
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
   
    printf("starting shaders\n");
    fflush(stdout);

    //Setup shaders
    loadMandelbulbProgram(&mandelbulb_shader, fov, camerapos, cameradir,
		horizontalAxis, verticalAxis, depthAxis, color, step, bail, power,
		phi, theta, totalRotation);
    printf("loaded program\n");
    fflush(stdout);
    printProgramLog(mandelbulb_shader);

    //Loop
    glutMainLoop();

    return EXIT_SUCCESS;
}
