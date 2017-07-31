#include "camera.h"
#include "vector.h"
#include <time.h>

#define ANGLE_PER_PIXEL 1
#define ROTATE_SPEED CLOCKS_PER_SEC/100000000

extern vec4f totalRotation;
extern vec3f cameradir, horizontalAxis, verticalAxis, depthAxis;

static int oldMouseX = -1, oldMouseY = -1;

//Rotate camera based on mouse
void cameraMoveMouse(int x, int y) {
    if (oldMouseX != -1 && oldMouseY != -1) {
        Yaw(oldMouseX-x, &totalRotation, &cameradir, &horizontalAxis, &verticalAxis, &depthAxis);
        Pitch(oldMouseY-y, &totalRotation, &cameradir, &horizontalAxis, &verticalAxis,
            &depthAxis);
    }

    oldMouseX = x;
    oldMouseY = y;
}

//Move camera based on keyboard
void cameraMoveKeyboard(char key, clock_t time_held) {
    if (key == 'q') {
        Roll((float)time_held*ROTATE_SPEED, &totalRotation, &cameradir, &horizontalAxis,
            &verticalAxis, &depthAxis);
    }
    else if (key == 'e') {
        Roll(-time_held*ROTATE_SPEED, &totalRotation, &cameradir, &horizontalAxis,
            &verticalAxis, &depthAxis);
    }
}
