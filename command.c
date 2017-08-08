#include "command.h"
#include <string.h>

extern float power;
extern void updateMandelbulbVars();

void processCommand(char* command) {
    if (strcmp(command, "power") == 0) {
        power += 1.0;
        updateMandelbulbVars();
    }
}
