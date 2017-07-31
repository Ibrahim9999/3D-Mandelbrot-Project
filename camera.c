#include "camera.h"
#include "vector.h"

#define ANGLE_PER_PIXEL 1

extern vec4f totalRotation;
extern vec3f cameradir, horizontalAxis, verticalAxis, depthAxis;

static int oldMouseX = -1, oldMouseY = -1;

//Rotate camera based on mouse
void handleMouse(int x, int y) {
    if (oldMouseX != -1 && oldMouseY != -1) {
        Yaw(oldMouseX-x, &totalRotation, &cameradir, &horizontalAxis, &verticalAxis, &depthAxis);
        Pitch(oldMouseY-y, &totalRotation, &cameradir, &horizontalAxis, &verticalAxis, &depthAxis);
    }

    oldMouseX = x;
    oldMouseY = y;
}
