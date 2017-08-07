#ifndef SHADER
#define SHADER

#include "vector.h"

#define PI_CONST 3.141592

typedef struct shaderprogram_struct {
    GLuint prog;
    GLuint vert;
    GLuint frag;
} shaderprogram;

char* loadTextFile(const char* filename);
void loadShaders(shaderprogram *program, const char* vname, const char* fname);
void printProgramLog(shaderprogram program);
void loadMandelbulbVars(shaderprogram program, vec3f fov, vec3f camerapos,
	vec3f color, float step, int bail, float power, float phi, float theta,
	vec4f totalRotation, vec2f resolution, int multisampling, vec3f lightpos,
	float intensity);
void loadMandelbulbProgram(shaderprogram *program, vec3f fov, vec3f camerapos,
	vec3f color, float step, int bail, float power, float phi, float theta,
	vec4f totalRotation, vec2f resolution, int multisampling, vec3f lightpos,
	float intensity);

#endif
