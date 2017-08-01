#include "camera.h"
#include "vector.h"
#include <time.h>
#include <stdlib.h>

#define ANGLE_PER_PIXEL 0.2
#define ROTATE_SPEED CLOCKS_PER_SEC/100000000

extern vec4f totalRotation;
extern vec3f cameradir, horizontalAxis, verticalAxis, depthAxis;
extern vec3f camerapos;

static int oldMouseX = -1, oldMouseY = -1;

//Rotate camera based on mouse
void cameraMoveMouse(int x, int y) {
    //Reset if change too large
    if (abs(x-oldMouseX) > 10 || abs(y-oldMouseY) > 10)
        oldMouseX = oldMouseY = -1; 

    //Rotate if not first click
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
    //Roll
    if (key == 'q') {
        Roll((float)time_held*ROTATE_SPEED, &totalRotation, &cameradir, &horizontalAxis,
            &verticalAxis, &depthAxis);
    }
    else if (key == 'e') {
        Roll(-time_held*ROTATE_SPEED, &totalRotation, &cameradir, &horizontalAxis,
            &verticalAxis, &depthAxis);
    }
    //Move
    else if (key == 'w') {
        camerapos.x += depthAxis.x*time_held/10000;
        camerapos.y += depthAxis.y*time_held/10000;
        camerapos.z += depthAxis.z*time_held/10000;
    }
    else if (key == 's') {
        camerapos.x -= depthAxis.x*time_held/10000;
        camerapos.y -= depthAxis.y*time_held/10000;
        camerapos.z -= depthAxis.z*time_held/10000;
    }
    else if (key == 'a') {
        camerapos.x -= horizontalAxis.x*time_held/10000;
        camerapos.y -= horizontalAxis.y*time_held/10000;
        camerapos.z -= horizontalAxis.z*time_held/10000;
    }
    else if (key == 'd') {
        camerapos.x += horizontalAxis.x*time_held/10000;
        camerapos.y += horizontalAxis.y*time_held/10000;
        camerapos.z += horizontalAxis.z*time_held/10000;
    }
    
}
