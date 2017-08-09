#ifndef SHADER
#define SHADER

#include "vecmath.h"
#include <GL/glut.h>
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
	vec2f resolution, int multisampling, vec3f lightpos, float intensity,
	vec3f horizontalAxis, vec3f verticalAxis, vec3f depthAxis, float wVar,
	int orbittrap);
void loadMandelbulbProgram(shaderprogram* program, vec3f fov, vec3f camerapos,
	vec3f color, float step, int bail, float power, float phi, float theta,
	vec2f resolution, int multisampling, vec3f lightpos, float intensity,
	vec3f horizontalAxis, vec3f verticalAxis, vec3f depthAxis, float wVar,
	int orbittrap);
void loadTextureProgram(shaderprogram* program, GLuint texture, GLuint *sampler);
void loadTextureVars(shaderprogram program, GLuint texture, GLuint sampler);

#endif
