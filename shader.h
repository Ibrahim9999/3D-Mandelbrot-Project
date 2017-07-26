#ifndef SHADER
#define SHADER

#include "vector.h"

#define PI_CONST 3.141592

typedef struct shaderprogram_struct {
    GLuint prog;
    GLuint vert;
    GLuint frag;
} shaderprogram;

char* loadTextFile(char* filename);
void setFOVvec(vec3f& vector, float vertFOV, float horiFOV);
void loadShaders(shaderprogram& program, char* vname, char* fname);
void printProgramLog(shaderprogram& program);
void loadMandelbulbVars(shaderprogram& program, vec3f fov, vec3f& camera, vec3f& color,
    float step, int bail);
void loadMandelbulbProgram(shaderprogram& program, vec3f& fov, vec3f& camerapos,
    vec3f& cameradir, vec3f& color, float step, int bail);

#endif
