#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>

#define EXIT_SUCCESS 0

//Render Funcion
void render() {
    //Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
		glVertex3f(-1.0f,-1.0f, 0.0f);
		glVertex3f( 1.0f, 0.0f, 0.0);
		glVertex3f( 0.0f, 1.0f, 0.0);
	glEnd();

    glutSwapBuffers();
}

//Main
int main(int argc, char* argv[]) {

    //Setup window
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(600,600);
	glutCreateWindow("3D Mandelbulb Viewer");

    //Setup loop
    glutDisplayFunc(render);

    //Loop
    glutMainLoop();

    return EXIT_SUCCESS;
}
