#include "camera.h"
#include "vector.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define ANGLE_PER_SEC 45
#define DIST_PER_SEC 1
#define BAIL_PER_SEC 1
#define STEP_PER_SEC .001
#define POWER_PER_SEC 1
#define PHI_SHIFT_PER_SEC 1
#define THETA_SHIFT_PER_SEC 1
#define LIGHT_SHIFT_PER_SEC 1
#define LIGHT_INTENSITY_PER_SEC 1
#define ANGLE_PER_PIXEL 0.5

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
void cameraMoveKeyboard(char key, float time)
{
	printf("*********************************\n");
	printf("totalRotation: %f, <%f,%f,%f>\n", totalRotation.w, totalRotation.x, totalRotation.y, totalRotation.z);
	printf("horizontalAxis: %f,%f,%f\n", horizontalAxis.x, horizontalAxis.y, horizontalAxis.z);
	printf("verticalAxis: %f,%f,%f\n", verticalAxis.x, verticalAxis.y, verticalAxis.z);
	printf("depthAxis: %f,%f,%f\n", depthAxis.x, depthAxis.y, depthAxis.z);
	printf("****\n");
	printf("time: %f\n", time);
	printf("camerapos: %f,%f,%f\n", camerapos.x, camerapos.y, camerapos.z);
	printf("lightpos: %f,%f,%f\n", lightpos.x, lightpos.y, lightpos.z);
	printf("intensity: %f\n", intensity);
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
		bail -= time*BAIL_PER_SEC;
	else if (key == 'v')
		bail += round(/*time**/BAIL_PER_SEC); // += DOES NOT WORK WITH TIME, POSSIBLE FLOATING POINT ERROR?

	if (key == 'z')
		step += time*STEP_PER_SEC;
	else if (key == 'x')
		step -= time*STEP_PER_SEC;

	if (key == 'p')
		power += time*POWER_PER_SEC;
	else if (key == 'o')
		power -= time*POWER_PER_SEC;

	else if (key == '7')
		phi -= time*PHI_SHIFT_PER_SEC;
	else if (key == '8')
		phi += time*PHI_SHIFT_PER_SEC;

	else if (key == '9')
		theta -= time*THETA_SHIFT_PER_SEC;
	else if (key == '0')
		theta += time*THETA_SHIFT_PER_SEC;

	//Pointlight
	if (key == 't')
		lightpos.x -= time*LIGHT_SHIFT_PER_SEC;
	else if (key == 'y')
		lightpos.x += time*LIGHT_SHIFT_PER_SEC;
	if (key == 'g')
		lightpos.y -= time*LIGHT_SHIFT_PER_SEC;
	else if (key == 'h')
		lightpos.y += time*LIGHT_SHIFT_PER_SEC;
	if (key == 'b')
		lightpos.z -= time*LIGHT_SHIFT_PER_SEC;
	else if (key == 'n')
		lightpos.z += time*LIGHT_SHIFT_PER_SEC;


	if (key == 'u')
		intensity -= time*LIGHT_INTENSITY_PER_SEC;
	else if (key == 'm')
		intensity += time*LIGHT_INTENSITY_PER_SEC; // += DOES NOT WORK, POSSIBLE FLOATING POINT ERROR?

	//Rotations
	else if (key == 'q')
		Roll(-time*ANGLE_PER_SEC, &totalRotation, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'e')
		Roll(time*ANGLE_PER_SEC, &totalRotation, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'l')
		Yaw(-time*ANGLE_PER_SEC, &totalRotation, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'j')
		Yaw(time*ANGLE_PER_SEC, &totalRotation, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'k')
		Pitch(-time*ANGLE_PER_SEC, &totalRotation, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'i')
		Pitch(time*ANGLE_PER_SEC, &totalRotation, &horizontalAxis,
			&verticalAxis, &depthAxis);

	//Move
	else if (key == 'w')
		camerapos = MoveAlongAxis(camerapos, verticalAxis, -time*DIST_PER_SEC);
	else if (key == 's')
		camerapos = MoveAlongAxis(camerapos, verticalAxis, time*DIST_PER_SEC);
	else if (key == 'a')
		camerapos = MoveAlongAxis(camerapos, horizontalAxis, time*DIST_PER_SEC);
	else if (key == 'd')
		camerapos = MoveAlongAxis(camerapos, horizontalAxis, -time*DIST_PER_SEC);

	// Zoom
	else if (key == 'r')
		camerapos = MoveAlongAxis(camerapos, depthAxis, time*DIST_PER_SEC);
	else if (key == 'f')
		camerapos = MoveAlongAxis(camerapos, depthAxis, -time*DIST_PER_SEC);
}
