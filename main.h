#ifndef HEADER_MAIN_DEFINED
#define HEADER_MAIN_DEFINED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <ctype.h>
#include "shader.h"
#include "math.h"
#include "camera.h"

#define EXIT_SUCCESS 0
#define EXIT_ERROR 1

#define true 1
#define false 0

#define MENU_FOCUS 0
#define VIEW_FOCUS 1

#define START_HEIGHT 400
#define START_WIDTH 400

#define START_FOV 50.0

#define KEYBUFFERLEN 256

//Functions
void sendKeySignals();
void printMonitors();

//Mandelbulb shader and variables
shaderprogram mandelbulb_shader;
vec3f fov, camerapos, color, horizontalAxis, verticalAxis, depthAxis, lightpos;
vec2f resolution;
float step;
int bail, multisampling;
float power;
float phi;
float theta;
float intensity;
float wVar;

//Camera variables
float vfov, hfov;
float cameradist;

//State of program (view or menu)
int userfocus = VIEW_FOCUS;

//Timer
float lastframe;
float lastlastframe;


#ifdef __cplusplus
}
#endif

#endif
