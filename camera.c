#include "camera.h"
#include "vector.h"
#include <time.h>
#include <stdlib.h>

#define ANGLE_PER_SEC 1000
#define DIST_PER_SEC 10
#define BAIL_PER_SEC 5
#define STEP_PER_SEC 0.1
#define POWER_PER_SEC 1;
#define PHI_PER_SEC 0.1;
#define THETA_PER_SEC 0.1;
#define ANGLE_PER_PIXEL 0.5

extern vec4f totalRotation;
extern vec3f cameradir, horizontalAxis, verticalAxis, depthAxis;
extern vec3f camerapos;
extern int bail;
extern float power;
extern float step;
extern float theta;
extern float phi;

static int oldMouseX = -1, oldMouseY = -1;

//Rotate camera based on mouse
void cameraMoveMouse(int x, int y) {
    //Reset if change too large
    if (abs(x-oldMouseX) > 10 || abs(y-oldMouseY) > 10)
        oldMouseX = oldMouseY = -1; 

    //Rotate if not first click
    if (oldMouseX != -1 && oldMouseY != -1) {
        Yaw((oldMouseX-x)*ANGLE_PER_PIXEL, &totalRotation, &cameradir, &horizontalAxis, &verticalAxis, &depthAxis);
        Pitch((oldMouseY-y)*ANGLE_PER_PIXEL, &totalRotation, &cameradir, &horizontalAxis, &verticalAxis,
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

	double unitScalar = (camerapos.x*camerapos.x + camerapos.y*camerapos.y + camerapos.z*camerapos.z) / 16;
	if (unitScalar > 16)
		unitScalar = 16;
	if (unitScalar < .000000001)
		unitScalar = .000000001;

	printf("unitScalar: %f\n", unitScalar);
	if (key == 'b')
		if (bail > -1)
			bail -= time*BAIL_PER_SEC;
	if (key == 'v')
		if (bail < 1000)
			bail += time*BAIL_PER_SEC;
	if (key == 'x')
		if (step < 2)
			step += time*STEP_PER_SEC;
	if (key == 'c')
		if (step > .0001)
			step -= time*STEP_PER_SEC;
	if (key == 'p')
		power += time*POWER_PER_SEC;
	if (key == 'o')
		power -= time*POWER_PER_SEC;
	if (key == 'n')
		phi -= time*PHI_PER_SEC;
	if (key == 'm')
		phi += time*PHI_PER_SEC;
	if (key == ',')
		theta -= time*THETA_PER_SEC;
	if (key == '.')
		theta += time*THETA_PER_SEC;
	
    //Rotations
	if (key == 'q')
		Roll(-time*ANGLE_PER_SEC, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'e')
		Roll(time*ANGLE_PER_SEC, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'k')
		Yaw(-time*ANGLE_PER_SEC, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'h')
		Yaw(time*ANGLE_PER_SEC, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'j')
		Pitch(-time*ANGLE_PER_SEC, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'u')
		Pitch(time*ANGLE_PER_SEC, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	
    //Move
    if (key == 'w')
		camerapos = MoveAlongAxis(camerapos, verticalAxis, time*DIST_PER_SEC);
    else if (key == 's')
		camerapos = MoveAlongAxis(camerapos, verticalAxis, -time*DIST_PER_SEC);
	else if (key == 'a')
		camerapos = MoveAlongAxis(camerapos, horizontalAxis, time*DIST_PER_SEC);
	else if (key == 'd')
		camerapos = MoveAlongAxis(camerapos, horizontalAxis, -time*DIST_PER_SEC);
	// Zoom
	if (key == 'r')
		camerapos = MoveAlongAxis(camerapos, depthAxis, time*DIST_PER_SEC);
	else if (key == 'f')
		camerapos = MoveAlongAxis(camerapos, depthAxis, -time*DIST_PER_SEC);
}
