#include "camera.h"
#include "vector.h"
#include <time.h>
#include <stdlib.h>

#define ANGLE_PER_CLOCK 1
#define UNIT_PER_CLOCK .01

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
        Yaw((oldMouseX-x)*ANGLE_PER_CLOCK, &totalRotation, &cameradir, &horizontalAxis, &verticalAxis, &depthAxis);
        Pitch((oldMouseY-y)*ANGLE_PER_CLOCK, &totalRotation, &cameradir, &horizontalAxis, &verticalAxis,
            &depthAxis);
    }

    oldMouseX = x;
    oldMouseY = y;
}

//Move camera based on keyboard
void cameraMoveKeyboard(char key, clock_t time_held)
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
			bail -= 1;
	if (key == 'v')
		if (bail < 1000)
			bail += 1;
	if (key == 'x')
		if (step < 2)
			step += .0001;
	if (key == 'c')
		if (step > .0001)
			step -= .0001;
	if (key == 'p')
		power += UNIT_PER_CLOCK;
	if (key == 'o')
		power -= UNIT_PER_CLOCK;
	if (key == 'n')
		phi -= ANGLE_PER_CLOCK;
	if (key == 'm')
		phi += ANGLE_PER_CLOCK;
	if (key == ',')
		theta -= ANGLE_PER_CLOCK;
	if (key == '.')
		theta += ANGLE_PER_CLOCK;
	
    //Rotations
	if (key == 'q')
		Roll(-ANGLE_PER_CLOCK, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'e')
		Roll(ANGLE_PER_CLOCK, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'k')
		Yaw(-ANGLE_PER_CLOCK, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'h')
		Yaw(ANGLE_PER_CLOCK, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'j')
		Pitch(-ANGLE_PER_CLOCK, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	else if (key == 'u')
		Pitch(ANGLE_PER_CLOCK, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	
    //Move
    if (key == 'w')
		camerapos = MoveAlongAxis(camerapos, verticalAxis, UNIT_PER_CLOCK);
    else if (key == 's')
		camerapos = MoveAlongAxis(camerapos, verticalAxis, -UNIT_PER_CLOCK);
	else if (key == 'a')
		camerapos = MoveAlongAxis(camerapos, horizontalAxis, UNIT_PER_CLOCK);
	else if (key == 'd')
		camerapos = MoveAlongAxis(camerapos, horizontalAxis, -UNIT_PER_CLOCK);
	// Zoom
	if (key == 'r')
		camerapos = MoveAlongAxis(camerapos, depthAxis, UNIT_PER_CLOCK);
	else if (key == 'f')
		camerapos = MoveAlongAxis(camerapos, depthAxis, -UNIT_PER_CLOCK);
}
