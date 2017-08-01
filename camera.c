#include "camera.h"
#include "vector.h"
#include <time.h>
#include <stdlib.h>

#define ANGLE_PER_PIXEL 0.2
#define ROTATE_SPEED CLOCKS_PER_SEC/100000000

extern vec4f totalRotation;
extern vec3f cameradir, horizontalAxis, verticalAxis, depthAxis;
extern vec3f camerapos;
extern int bail;
extern float power;
extern float step;

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
void cameraMoveKeyboard(char key, clock_t time_held) {
	if (key == 'b')
		if (bail > -1)
			bail -= 1;
	if (key == 'v')
		if (bail < 1000)
			bail += 1;
	if (key == 'p')
		power += .01;
	if (key == 'o')
		power -= .01;
	if (key == 'x')
		if (step < 2)
			step += .0001;
	if (key == 'c')
		if (step > .0001)
			step -= .0001;
	
    //Rotations
	if (key == 'q') {
		Roll((float).1*time_held*ANGLE_PER_PIXEL, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	}
	else if (key == 'e') {
		Roll(-(float).1*time_held*ANGLE_PER_PIXEL, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	}
	else if (key == 'k') {
		Yaw((float).1*time_held*ANGLE_PER_PIXEL, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	}
	else if (key == 'h') {
		Yaw(-(float).1*time_held*ANGLE_PER_PIXEL, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	}
	else if (key == 'j') {
		Pitch((float).1*time_held*ANGLE_PER_PIXEL, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	}
	else if (key == 'u') {
		Pitch(-(float).1*time_held*ANGLE_PER_PIXEL, &totalRotation, &cameradir, &horizontalAxis,
			&verticalAxis, &depthAxis);
	}
    //Move
    if (key == 'w') {
        camerapos.x += verticalAxis.x*time_held/10000;
        camerapos.y += verticalAxis.y*time_held/10000;
        camerapos.z += verticalAxis.z*time_held/10000;
    }
    else if (key == 's') {
        camerapos.x -= verticalAxis.x*time_held/10000;
        camerapos.y -= verticalAxis.y*time_held/10000;
        camerapos.z -= verticalAxis.z*time_held/10000;
    }
	else if (key == 'a') {
		camerapos.x -= horizontalAxis.x*time_held / 10000;
		camerapos.y -= horizontalAxis.y*time_held / 10000;
		camerapos.z -= horizontalAxis.z*time_held / 10000;
	}
	else if (key == 'd') {
		camerapos.x += horizontalAxis.x*time_held / 10000;
		camerapos.y += horizontalAxis.y*time_held / 10000;
		camerapos.z += horizontalAxis.z*time_held / 10000;
	}
	// Zoom
	if (key == 'r') {
		camerapos.x += depthAxis.x*time_held / 10000;
		camerapos.y += depthAxis.y*time_held / 10000;
		camerapos.z += depthAxis.z*time_held / 10000;
	}
	else if (key == 'f') {
		camerapos.x -= depthAxis.x*time_held / 10000;
		camerapos.y -= depthAxis.y*time_held / 10000;
		camerapos.z -= depthAxis.z*time_held / 10000;
	}
    
}
