#include "camera.h"
#include "vector.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>

#define D_ANGLE 1
#define D_CAMERA_DIST 0.05
#define D_BAIL 1
#define D_STEP .001
#define D_POWER 1
#define D_PHI 1
#define D_THETA 1
#define D_LIGHT_DIST 1
#define D_LIGHT_INTENSITY .1
#define ANGLE_PER_PIXEL 0.5

#define MAGMOD 10
#define MINMOD 0.1
#define MODCOEF 10

extern vec4f totalRotation;
extern vec3f camerapos;
extern vec3f horizontalAxis;
extern vec3f verticalAxis;
extern vec3f depthAxis;
extern vec3f lightpos;
extern int bail;
extern float power;
extern float step;
extern float theta;
extern float phi;
extern float intensity;
extern vec2f resolution;

extern int multisampling;

extern vec3f fov;
extern float hfov, vfov;
extern float cameradist;

extern void draw();
extern void render();
extern void updateMandelbulbVars();

static int oldMouseX = -1, oldMouseY = -1;

void InitializeCamera(vec3f* cameraPosition, vec3f* depthAxis, vec3f* horizontalAxis, vec3f* verticalAxis)
{
	vec3f v;

	v.x = 0;
	v.y = 0;
	v.z = -4;
	*cameraPosition = v;

	v.x = 0;
	v.y = 0;
	v.z = 1;
	*depthAxis = v;

	v.x = 1;
	v.y = 0;
	v.z = 0;
	*horizontalAxis = v;

	v.x = 0;
	v.y = 1;
	v.z = 0;
	*verticalAxis = v;
}

//Rotate camera based on mouse
void cameraMoveMouse(int x, int y) {
    //Reset if change too large
    if (abs(x-oldMouseX) > 10 || abs(y-oldMouseY) > 10)
        oldMouseX = oldMouseY = -1; 

    //Rotate if not first click
    if (oldMouseX != -1 && oldMouseY != -1) {
        Yaw((oldMouseX-x)*ANGLE_PER_PIXEL, &totalRotation, &horizontalAxis, &verticalAxis, &depthAxis);
        Pitch((oldMouseY-y)*ANGLE_PER_PIXEL, &totalRotation, &horizontalAxis, &verticalAxis,
            &depthAxis);
    }

    oldMouseX = x;
    oldMouseY = y;
}

//Move camera based on keyboard
void cameraMoveKeyboard(int key, int shift, int ctrl, int alt) {

    float mod = 1;
    float modmod = 1;

    if (alt)
        modmod*=MODCOEF;
    if (shift)
        mod*=MAGMOD*modmod;
    if (ctrl)
        mod*=MINMOD/modmod;


    printf("*********************************\n");
	printf("totalRotation: %f, <%f,%f,%f>\n", totalRotation.w, totalRotation.x, totalRotation.y, totalRotation.z);
	printf("horizontalAxis: %f,%f,%f\n", horizontalAxis.x, horizontalAxis.y, horizontalAxis.z);
	printf("verticalAxis: %f,%f,%f\n", verticalAxis.x, verticalAxis.y, verticalAxis.z);
	printf("depthAxis: %f,%f,%f\n", depthAxis.x, depthAxis.y, depthAxis.z);
	printf("****\n");
	printf("camerapos: %f,%f,%f\n", camerapos.x, camerapos.y, camerapos.z);
	printf("lightpos: %f,%f,%f\n", lightpos.x, lightpos.y, lightpos.z);
	printf("power: %f\n", power);
	printf("phi: %f\n", phi);
	printf("theta: %f\n", theta);
	printf("step: %f\n", step);
	printf("bail: %d\n", bail);
	//printf("bail: %f, %d, %d, %f\n", time, BAIL_PER_SEC, time*BAIL_PER_SEC, time*BAIL_PER_SEC);

	/*
	
	double unitScalar = (camerapos.x*camerapos.x + camerapos.y*camerapos.y + camerapos.z*camerapos.z) / 16;
	if (unitScalar > 16)
		unitScalar = 16;
	if (unitScalar < .000000001)
		unitScalar = .000000001;

	printf("unitScalar: %f\n", unitScalar);
	
	*/

	if (key == 'c')
		bail -= D_BAIL*mod;
	else if (key == 'v')
		bail += D_BAIL*mod;

	if (key == 'z')
		step += D_STEP*mod;
	else if (key == 'x')
		step -= D_STEP*mod;

	if (key == 'p')
		power += D_POWER*mod;
	else if (key == 'o')
		power -= D_POWER*mod;

	else if (key == '7')
		phi -= D_PHI*mod;
	else if (key == '8')
		phi += D_PHI*mod;

	else if (key == '9')
		theta -= D_THETA*mod;
	else if (key == '0')
		theta += D_THETA*mod;

	//Pointlight
	if (key == 't')
		lightpos.x -= D_LIGHT_DIST*mod;
	else if (key == 'y')
		lightpos.x += D_LIGHT_DIST*mod;
	if (key == 'g')
		lightpos.y -= D_LIGHT_DIST*mod;
	else if (key == 'h')
		lightpos.y += D_LIGHT_DIST*mod;
	if (key == 'b')
		lightpos.z -= D_LIGHT_DIST*mod;
	else if (key == 'n')
		lightpos.z += D_LIGHT_DIST*mod;


	if (key == 'u')
		intensity -= D_LIGHT_INTENSITY*mod;
	else if (key == 'm')
		intensity += D_LIGHT_INTENSITY*mod; // += DOES NOT WORK, POSSIBLE FLOATING POINT ERROR?

	//Rotations
	else if (key == 'q')
		Roll(-D_ANGLE*mod, &totalRotation, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'e')
		Roll(D_ANGLE*mod, &totalRotation, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'l')
		Yaw(-D_ANGLE*mod, &totalRotation, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'j')
		Yaw(D_ANGLE*mod, &totalRotation, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'k')
		Pitch(-D_ANGLE*mod, &totalRotation, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'i')
		Pitch(mod*D_ANGLE, &totalRotation, &horizontalAxis,
			&verticalAxis, &depthAxis);

	//Move
	else if (key == 'w')
		camerapos = MoveAlongAxis(camerapos, verticalAxis, -D_CAMERA_DIST*mod);
	else if (key == 's')
		camerapos = MoveAlongAxis(camerapos, verticalAxis, D_CAMERA_DIST*mod);
	else if (key == 'a')
		camerapos = MoveAlongAxis(camerapos, horizontalAxis, D_CAMERA_DIST*mod);
	else if (key == 'd')
		camerapos = MoveAlongAxis(camerapos, horizontalAxis, -D_CAMERA_DIST*mod);

	// Zoom
	else if (key == 'r')
		camerapos = MoveAlongAxis(camerapos, depthAxis, D_CAMERA_DIST*mod);
	else if (key == 'f')
		camerapos = MoveAlongAxis(camerapos, depthAxis, -D_CAMERA_DIST*mod);

    else if (key == '1')
        screenshot("screenshot.ppm", 1024, 1024);
}

void screenshot(char* filename, int width, int height) {
    GLuint texture;
    GLuint fb=0, rb;
    FILE *file;
    int pixel;
    GLubyte *data;
    vec2f oldres;
    
    multisampling = 4;

    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    glGenRenderbuffers(1, &rb);
    glBindRenderbuffer(GL_RENDERBUFFER, rb);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rb);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
        GL_UNSIGNED_BYTE, 0);
   
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);

    oldres = resolution;
    resolution.x = width, resolution.y = height;
    changeFOVscale(&vfov, &hfov, width, height, cameradist);
    setFOVvec(&fov, vfov, hfov);
    updateMandelbulbVars();

    draw();

    pixel = width * height;;
    data = malloc(width * height * 3);

    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, width, height, 0);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    //glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    //Dispose of texture, framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &texture);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fb);
    glDeleteRenderbuffers(1, &rb);

    multisampling = 1;
    resolution = oldres;
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, resolution.x, resolution.y);
    glScissor(0, 0, resolution.x, resolution.y);
    changeFOV(&vfov, &hfov, resolution.x, resolution.y, cameradist);
    setFOVvec(&fov, vfov, hfov);
    updateMandelbulbVars();

    file = fopen(filename, "w");
    fprintf(file, "P6\n%d %d\n255\n", width, height);
    
    while (pixel) {
        fwrite(data+pixel*3, sizeof(GLubyte), 3, file);
        pixel--;
    }

    fclose(file);

    render();
}

void setFOVvec(vec3f* vector, float vertFOV, float horiFOV) {
    float tan_h = tan(horiFOV/360*PI);
    float tan_v = tan(vertFOV/360*PI);

    vector->z = sqrt(1/(tan_h*tan_h+tan_v*tan_v+1));
    vector->y = vector->z*tan_v;
    vector->x = vector->z*tan_h;

    *vector = v3f_normalize(*vector);
}

void changeFOV(float *vfov, float *hfov, int w, int h, float cameradist) {
    *hfov = atan(w/(2*cameradist))/(2*PI)*720;
    *vfov = atan(h/(2*cameradist))/(2*PI)*720;
}

void changeFOVscale(float *vfov, float* hfov, int w, int h) {
    if (w > h) {
       *hfov = *vfov; 
    }
    else {
        *vfov = *hfov;
    }   
}
