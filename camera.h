#ifndef CAMERA
#define CAMERA

#include <time.h>
#include "vecmath.h"

void InitializeCamera(vec3f* cameraPosition, vec3f* depthAxis,
	vec3f* horizontalAxis, vec3f* verticalAxis, vec3f* centerpos,
	vec3f* centerHAxis, vec3f* centerVAxis, vec3f* centerDAxis);
void cameraMoveMouse(int x, int y);
void cameraMoveKeyboard(int key, int shift, int ctrl, int alt);
void screenshot(char* filename, int width, int height);
void setFOVvec(vec3f* fov, float vertFOV, float horiFOV);
void changeFOV(float* vfov, float* hfov, int w, int h, float cameradist);
void changeFOVscale(float *vfov, float* hfov, int w, int h);

#endif
