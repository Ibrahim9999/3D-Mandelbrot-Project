#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "shader.h"
#include "vector.h"

#define EXIT_SUCCESS 0
#define EXIT_ERROR 1

shaderprogram mandelbulb_shader;
vec3f fov, camerapos, cameradir, color;
float step;
int bail;
float power;

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
    camerapos.x += 0.001;
    if (camerapos.x > 3) camerapos.x = -3;
    loadMandelbulbVars(mandelbulb_shader, fov, camerapos, cameradir, color, step , bail, power);
    render();
}

//Main
int main(int argc, char* argv[]) {

    printf("hello main\n");
    fflush(stdout);

    //Set vars
    setFOVvec(&fov, 50, 50);
    camerapos.x=0; camerapos.y=0; camerapos.z=-2;
    cameradir.x=0; cameradir.y=0; cameradir.z=0;
    color.x=0; color.y=1; color.z=1;
    step = 0.05;
    bail = 20;
    power = 2;

    //Setup window
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(600,600);
	glutCreateWindow("3D Mandelbulb Viewer");

    //Setup functions
    glutDisplayFunc(render);
    glutIdleFunc(idle);

    glewInit();
   
    printf("starting shaders\n");
    fflush(stdout);

    //Setup shaders
    loadMandelbulbProgram(&mandelbulb_shader, fov, camerapos, cameradir, color, step,
        bail, power);
    printf("loaded program\n");
    fflush(stdout);
    printProgramLog(mandelbulb_shader);

    //Loop
    glutMainLoop();

    return EXIT_SUCCESS;
}
