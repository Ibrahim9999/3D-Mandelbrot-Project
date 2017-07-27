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
int bail, power;

//Render Funcion
void render() {
    //Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUADS);
		glVertex3f(1.0f,1.0f, 0.0);
		glVertex3f(1.0f, -1.0f, 0.0);
		glVertex3f(-1.0f, -1.0f, 0.0);
        glVertex3f(-1.0f, 1.0f, 0.0);
	glEnd();

    glutSwapBuffers();
}

//Main
int main(int argc, char* argv[]) {

    printf("hello main\n");
    fflush(stdout);

    //Set vars
    setFOVvec(&fov, 100, 100);
    camerapos.x=0; camerapos.y=0; camerapos.z=-4;
    cameradir.x=0; cameradir.y=0; cameradir.z=0;
    color.x=0; color.y=1.0; color.z=1.0;
    step = 0.01;
    bail = 50;
    power = 2;

    //Setup window
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(600,600);
	glutCreateWindow("3D Mandelbulb Viewer");

    //Setup functions
    glutDisplayFunc(render);

    glewInit();
   
    printf("starting shaders\n");
    fflush(stdout);

    //Setup shaders
    loadMandelbulbProgram(&mandelbulb_shader, fov, camerapos, cameradir, color, step,
        bail, power);
    printf("loaded program\n");
    fflush(stdout);
    printProgramLog(mandelbulb_shader);    
    glUseProgram(mandelbulb_shader.prog);

    //Loop
    glutMainLoop();

    return EXIT_SUCCESS;
}
