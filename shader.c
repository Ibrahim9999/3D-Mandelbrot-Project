#include <stdio.h>
#include <stdlib.h>

char* loadFile(char* filename) {
    FILE* file = fopen(filename);
    long size = fsize(file);
    char* text = malloc(size);

    if (fread(text, sizeof(char), 1000, file)) {
        fprintf(stderr, "cannot load file");
        exit(1);
    }

    fclose(file);

    return text;
}

void loadShaders(GLuint& v, GLuint& c) {
    char *vs=NULL, *fs=NULL;
    
    /****
    NEXT TODO
    ****/
 


}
