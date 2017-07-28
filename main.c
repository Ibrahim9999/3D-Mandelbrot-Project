#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "shader.h"
#include "vector.h"

#define EXIT_SUCCESS 0
#define EXIT_ERROR 1

shaderprogram mandelbulb_shader;
vec3f fov, camerapos, cameradir, color, horizontalAxis, verticalAxis, depthAxis;
vec4f totalRotation;
float step;
int bail;
float power;
float phi;
float theta;

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
    Yaw(1, &totalRotation, &cameradir, &horizontalAxis, &verticalAxis, &depthAxis);
    loadMandelbulbVars(mandelbulb_shader, fov, camerapos, cameradir, color, step , bail, power, phi, theta, totalRotation);
    render();
}

//Main
int main(int argc, char* argv[]) {

    printf("hello main\n");
    fflush(stdout);

    //Set vars
    setFOVvec(&fov, 50, 50);
    InitializeCamera(&cameradir, &camerapos, &depthAxis, &horizontalAxis, &verticalAxis);
    //camerapos.x=0; camerapos.y=0; camerapos.z=-2;
    //cameradir.x=0; cameradir.y=0; cameradir.z=1;
    totalRotation.x = 0; totalRotation.y = 0; totalRotation.z = 0; totalRotation.w = 1;
    color.x=0; color.y=1; color.z=1;
    step = 0.1;
    bail = 4;
    power = 8;
    phi = 0;
    theta = 0;

    //Setup window
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(400,400);
	glutCreateWindow("3D Mandelbulb Viewer");

    //Setup functions
    glutDisplayFunc(render);
    glutIdleFunc(idle);

    glewInit();
   
    printf("starting shaders\n");
    fflush(stdout);

    //Setup shaders
    loadMandelbulbProgram(&mandelbulb_shader, fov, camerapos, cameradir, color, step,
        bail, power, phi, theta, totalRotation);
    printf("loaded program\n");
    fflush(stdout);
    printProgramLog(mandelbulb_shader);

    //Loop
    glutMainLoop();

    return EXIT_SUCCESS;
}
