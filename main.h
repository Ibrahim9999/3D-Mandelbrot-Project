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
#include "options.h"
#include "command.h"

#define EXIT_SUCCESS 0
#define EXIT_ERROR 1

#define true 1
#define false 0

#define COMMAND_FOCUS 0
#define VIEW_FOCUS 1

#define INPUT_MAX 80

#define START_HEIGHT 400
#define START_WIDTH 400

#define START_FOV 50.0

#define KEYBUFFERLEN 256

//Functions
void sendKeySignals();
void printMonitors();
void updateMandelbulbVars();

//Mandelbulb shader and variables
shaderprogram mandelbulb_shader;
vec3f fov, camerapos, color, horizontalAxis, verticalAxis, depthAxis, lightpos;
vec3f centerpos, centerHAxis, centerVAxis, centerDAxis;
vec2f resolution;
float step;
float power;
float phi;
float theta;
float intensity;
float wVar;
int bail, multisampling;
int orbittrap;

//Texture shader and variables
shaderprogram textureshader;
GLuint lastrender;
GLuint lastrendersampler;

//Camera variables
float vfov, hfov;
float cameradist;

//State of program
int userfocus = VIEW_FOCUS;
int varsupdated = true;
int monitorsupdated = false;

//Timer
float lastframe;
float lastlastframe;

#ifdef __cplusplus
}
#endif

#endif
