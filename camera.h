#ifndef CAMERA
#define CAMERA

#include <time.h>
#include "vector.h"

void InitializeCamera(vec3f* cameraPosition, vec3f* depthAxis, vec3f* horizontalAxis, vec3f* verticalAxis);
void cameraMoveMouse(int x, int y);
void cameraMoveKeyboard(int key, int modmask);

#endif
