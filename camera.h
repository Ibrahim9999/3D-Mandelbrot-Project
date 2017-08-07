#ifndef CAMERA
#define CAMERA

#include <time.h>
#include "vector.h"

void InitializeCamera(vec3f* cameraPosition, vec3f* depthAxis, vec3f* horizontalAxis, vec3f* verticalAxis);
void cameraMoveMouse(int x, int y);
void cameraMoveKeyboard(int key, int shift, int ctrl, int alt);
void screenshot(char* filename, int width, int height);
void setFOVvec(vec3f* fov, float vertFOV, float horiFOV);
void changeFOV(float* vfov, float* hfov, int w, int h, float cameradist);

#endif
