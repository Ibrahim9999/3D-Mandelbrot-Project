#include "camera.h"
#include "vector.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
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
}
