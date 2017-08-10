#include "command.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

extern float power;
extern void updateMandelbulbVars();
extern void screenshot(char* filename, int dim);

void processCommand(char* command) {
    char* tokens[80];
    int tokennum = 0;
    tokens[tokennum++] = command+1;

    ++command;
    while (*(command++) != '\0') {
        if (*(command-1) == ' ') {
            tokens[tokennum++] = command;
            *(command-1) = '\0';
        }
    }

    runCommand(tokennum, tokens);
}

void runCommand(int numtok, char* tokens[]) {
    if (numtok == 0)
        return;

    if (strcmp(tokens[0], "pow") == 0) {
        if (numtok == 2) {
            power = atof(tokens[1]);
            updateMandelbulbVars();
        }
    }
    else if (strcmp(tokens[0], "screenshot") == 0) {
        if (numtok == 3) {
            screenshot(tokens[1], pow(2,atoi(tokens[2])));
        }
    }
}
