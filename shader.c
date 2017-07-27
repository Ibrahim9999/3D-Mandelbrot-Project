#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include "vector.h"
#include "shader.h"

char* loadTextFile(const char* filename) {
    printf("loading files\n");
    fflush(stdout);

    FILE* file = fopen(filename, "r");
    long size;
    char* text;
  
    fseek(file, 0, SEEK_END);
    size = ftell(file); 
    text = (char*) malloc(size);
    rewind(file);

    if (fread(text, sizeof(char), size, file) == 0) {
        fprintf(stderr, "cannot load file\n");
        exit(1);
    }

    fclose(file);

    return text;
}

void setFOVvec(vec3f *vector, float vertFOV, float horiFOV) {
    vector->z = cos(vertFOV/360*PI_CONST);
    vector->y = sin(horiFOV/360*PI_CONST);
    vector->x = cos(vertFOV/360*PI_CONST)*tan(horiFOV/360*PI_CONST);

    *vector = v3f_normalize(*vector);

    printf("************ %f, %f, %f", vector->x, vector->y, vector->z);
}

void printProgramLog(shaderprogram program) {
    int length, written;
    char* log;

    glGetProgramiv(program.prog, GL_INFO_LOG_LENGTH, &length);

    if (length > 0) {
        log = (char *) malloc(length);
        glGetProgramInfoLog(program.prog, length, &written, log);
        printf("%s\n", log);
        free(log);
    }
}

void printShaderLog(GLuint shader) {
    int length, written;
    char* log;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    if (length > 0) {
        log = (char *) malloc(length);
        glGetShaderInfoLog(shader, length, &written, log);
        printf("%s\n", log);
        free(log);
    }
}

void loadShaders(shaderprogram *program, const char* vname, const char* fname) {
    char *vs=NULL, *fs=NULL;
    const char *cvs, *cfs;
    
    printf("making a shader\n");
    fflush(stdout);

    program->vert = glCreateShader(GL_VERTEX_SHADER);
    program->frag = glCreateShader(GL_FRAGMENT_SHADER);

    printf("loading the files\n");
    fflush(stdout);

    vs = loadTextFile(vname);
    fs = loadTextFile(fname);

    cvs = vs, cfs = fs;

    printf("starting compile\n");
    fflush(stdout);

    glShaderSource(program->vert, 1, &cvs, NULL);
    glShaderSource(program->frag, 1, &cfs, NULL);

    glCompileShader(program->vert);
    glCompileShader(program->frag);

    printShaderLog(program->vert);
    printShaderLog(program->frag);
    

    printf("compiled\n");
    fflush(stdout);

    program->prog = glCreateProgram();
    glAttachShader(program->prog, program->vert);
    glAttachShader(program->prog, program->frag);

    printf("linking\n");
    glLinkProgram(program->prog);
}

void loadMandelbulbVars(shaderprogram program, vec3f fov, vec3f camerapos,
    vec3f cameradir, vec3f color, float step, int bail, int power) {
    GLint fov_loc, camerapos_loc, cameradir_loc, color_loc, step_loc, bail_loc, power_loc;  

    fov_loc = glGetUniformLocation(program.prog, "FOV");
    camerapos_loc = glGetUniformLocation(program.prog, "camerapos");
    cameradir_loc = glGetUniformLocation(program.prog, "cameradir");
    color_loc = glGetUniformLocation(program.prog, "color");
    step_loc = glGetUniformLocation(program.prog, "step");
    bail_loc = glGetUniformLocation(program.prog, "bail");
    power_loc = glGetUniformLocation(program.prog, "power");

    printf("************ %f, %f, %f", fov.x, fov.y, fov.z);

    glUniform3f(fov_loc, fov.x, fov.y, fov.z);
    glUniform3f(camerapos_loc, camerapos.x, camerapos.y, camerapos.z);
    glUniform3f(cameradir_loc, cameradir.x, cameradir.y, cameradir.z);
    glUniform3f(color_loc, color.x, color.y, color.z);
    glUniform1f(step_loc, step);
    glUniform1i(bail_loc, bail);
    glUniform1i(power_loc, power);
}

void loadMandelbulbProgram(shaderprogram* program, vec3f fov, vec3f camerapos,
    vec3f cameradir, vec3f color, float step, int bail, int power) {

    loadShaders(program, "shaders/mandelbulb_shader.vert", "shaders/mandelbulb_shader.frag");

    loadMandelbulbVars(*program, fov, camerapos, cameradir, color, step, bail, power);
}


