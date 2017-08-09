#include "main.h"

//Render quad
void draw() {
    //Draw quad over screen
    glBegin(GL_QUADS);
	    glVertex3f(1.0f, 1.0f, 0.0);
	    glVertex3f(1.0f, -1.0f, 0.0);
	    glVertex3f(-1.0f, -1.0f, 0.0);
        glVertex3f(-1.0f, 1.0f, 0.0);
    glEnd();
}

//Render Funcion
void render() {    
    //Clear
    glClear(GL_COLOR_BUFFER_BIT);

    //Redraw fractal if changed
    if (varsupdated == true) {
        GLint backbuffer;

        draw();
        printf("RERENDERED");

        //Copy frambuffer
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &backbuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, lastrender);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, backbuffer);
        glBlitFramebuffer(0, 0, resolution.x, resolution.y, 0, 0, resolution.x, resolution.y,
                GL_COLOR_BUFFER_BIT, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, backbuffer);

        varsupdated = false;
    }
    //Otherwise just put back old frambuffer and then draw text
    else {
        GLint backbuffer;

        //Copy frambuffer
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &backbuffer);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, lastrender);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, backbuffer);
        glBlitFramebuffer(0, 0, resolution.x, resolution.y, 0, 0, resolution.x, resolution.y,
                GL_COLOR_BUFFER_BIT, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, backbuffer);
    }

    lastlastframe = lastframe;
    lastframe = glutGet(GLUT_ELAPSED_TIME);

    printMonitors();
    
    //glutSwapBuffers();
    glFinish();
}

//Handle mouse input
void handleMouse(int x, int y) {
    if (userfocus == VIEW_FOCUS)
	{
        cameraMoveMouse(x, y);
    }
}


//Handle keyboard input
static unsigned int kbinputbuffer[KEYBUFFERLEN*4];
static int kbinputlen = 0;
char line[INPUT_MAX];
int command_char = 0;

void clearKeyBuffer() {
    int i = 0;

    while (i < kbinputlen*4) {
        kbinputbuffer[i++] = false;
    }

    kbinputlen = 0;

    line[0] = '\0';
    command_char = 0;
}
    
void handleKeyboard(unsigned char key, int x, int y) {
    if (key == ':') {
        userfocus = COMMAND_FOCUS;
        return;
    }

    if (userfocus == VIEW_FOCUS) {
        if (kbinputlen <= KEYBUFFERLEN) {
            int shift=false, ctrl=false, alt=false;
            //printf("****************%d", key);       
            int mods = glutGetModifiers();

            if ((mods & GLUT_ACTIVE_SHIFT) == GLUT_ACTIVE_SHIFT) {
                shift = true; putchar('S');
            }
            if ((mods & GLUT_ACTIVE_CTRL) == GLUT_ACTIVE_CTRL) {
                ctrl = true; putchar('C');
            }
            if ((mods & GLUT_ACTIVE_ALT) == GLUT_ACTIVE_ALT) {
                alt = true; putchar('A');
            }

            if (ctrl == true) {
                key = key-1+'a';
            }
            else if (shift == true  && isalpha(key)) {
                key = key- 'A' + 'a';
            }

            kbinputbuffer[4*kbinputlen] = key;
            kbinputbuffer[4*kbinputlen+1] = shift;
            kbinputbuffer[4*kbinputlen+2] = ctrl;
            kbinputbuffer[4*kbinputlen+3] = alt;
            kbinputlen++;
        }
    }
    else if (userfocus == COMMAND_FOCUS) {
        if (key != '\r' && key != '\n') {
            line[command_char++] = key;
        }
        else {
            command_char = -1;
        }

        line[command_char] = '\0';

        monitorsupdated = true;
    }
}

void handleKeyboardUp(unsigned char key, int x, int y) {
    
}

void sendKeySignals() {
    int inputsent = false;

    if (userfocus == VIEW_FOCUS) {
        int key;
        for (key = 0; key != kbinputlen; key++) {
            putchar(kbinputbuffer[key*4]);
            cameraMoveKeyboard(kbinputbuffer[key*4], kbinputbuffer[key*4+1],
                    kbinputbuffer[key*4+2], kbinputbuffer[key*4+3]);
            inputsent = true;
        }

        clearKeyBuffer();
    }
    else if (userfocus == COMMAND_FOCUS) {
        if (command_char == -1) {
            processCommand(line);
            command_char = 0;
            userfocus = VIEW_FOCUS;
            clearKeyBuffer();
            inputsent = true;
        }
    }

    if (inputsent == true) {
        updateMandelbulbVars();
    }
}

void handleResolution(int w, int h) {
    //Set viewport
    glViewport(0, 0, w, h);

    changeFOV(&vfov, &hfov, w, h, cameradist);
    //printf("newfov: %f\n", hfov);

    setFOVvec(&fov, vfov, hfov);
    printf("nvec: %f, %f, %f\n", fov.x, fov.y, fov.z);
	
    resolution.x = w; resolution.y = h;
}

void printString(char* string) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    int w = glutGet( GLUT_WINDOW_WIDTH );
    int h = glutGet( GLUT_WINDOW_HEIGHT );
    glOrtho( 0, w, 0, h, -1, 1 );
	
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity(); 
	
    glColor3f(1, 0, 0);
	
    glRasterPos2i(20, 20);
    void *font = GLUT_BITMAP_HELVETICA_18;
    for (char* c=string; *c != '\0'; c++) 
    {
        glutBitmapCharacter(font, *c);
    }

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

//Print monitors (text)
void printMonitors() {
    printString(line);
}

//Update shader variables
void updateMandelbulbVars() {
    loadMandelbulbVars(mandelbulb_shader, fov, camerapos, color, step, bail,
		power, phi, theta, resolution, multisampling, lightpos, intensity,
		horizontalAxis, verticalAxis, depthAxis, wVar, orbittrap);

    varsupdated = true;
}

//Idle Function
void idle() {
	vec3f oldCameraPos = camerapos;
	vec3f oldLightPos = lightpos;
	vec3f oldHAxis = horizontalAxis;
	vec3f oldVAxis = verticalAxis;
	vec3f oldDAxis = depthAxis;
	float oldStep = step;
	float oldPower = power;
	float oldPhi = phi;
	float oldTheta = theta;
	float oldIntensity = intensity;
	float oldWVar = wVar;
	int oldBail = bail;
	int oldOrbitTrap = orbittrap;

	sendKeySignals();
    
	if (!VecEquals(camerapos, oldCameraPos) || !VecEquals(lightpos, oldLightPos)
		|| !VecEquals(horizontalAxis, oldHAxis) || !VecEquals(verticalAxis, oldVAxis)
		|| !VecEquals(depthAxis, oldDAxis) || step != oldStep || power != oldPower
		|| phi != oldPhi || theta != oldTheta || intensity != oldIntensity
		|| wVar != oldWVar || bail != oldBail || orbittrap != oldOrbitTrap ||
        monitorsupdated == true)
		glutPostRedisplay();
}

//Initialize stuff
void init() {
    GLint renderbuffer;
    
    //Initialize some render stuff
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &renderbuffer);
    glGenFramebuffers(1, &lastrender);
    glBindFramebuffers(GL_FRAMEBUFFER);
    glGenRenderBuffers(1, &lastrenderbuffer);
    
}

//Main
int main(int argc, char* argv[]) {

    //printf("hello main\n");
    fflush(stdout);

    //Set vars
    hfov = vfov = START_FOV;
    cameradist = START_WIDTH/(2*tan(START_FOV/360*PI_CONST));
    setFOVvec(&fov, vfov, hfov);

    InitializeCamera(&camerapos, &horizontalAxis, &verticalAxis, &depthAxis,
		&centerpos, &centerHAxis, &centerVAxis, &centerDAxis);

    color.x=0; color.y=1; color.z=1;
	
    lightpos.x= -0.178390;
	lightpos.y = -2.660062;
	lightpos.z = -0.930965;

	//lightpos = (vec3f) { 0, 0, 0 };

	wVar = 0;
    step = 0.01;
    bail = 10;
    multisampling=1;
    power = 2;
    phi = 0;
    theta = 0;
    intensity = 4.50;
	orbittrap = SPHERE;

    resolution.x = START_WIDTH; resolution.y = START_HEIGHT;

    //Setup window
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(resolution.x, resolution.y);
	glutCreateWindow("3D Mandelbulb Viewer");
    
    //Setup Input
    //glutIgnoreKeyRepeat(true);
    clearKeyBuffer();

    //Setup functions
    glutDisplayFunc(render);
    glutIdleFunc(idle);
    glutMotionFunc(handleMouse);
    glutKeyboardFunc(handleKeyboard);
    glutKeyboardUpFunc(handleKeyboardUp);
    glutReshapeFunc(handleResolution);

    glewInit();

    init();

    //printf("starting shaders\n");
    fflush(stdout);

    //Setup shaders
    loadMandelbulbProgram(&mandelbulb_shader, fov, camerapos, color, step,
		bail, power, phi, theta, resolution, multisampling, lightpos, intensity,
		horizontalAxis, verticalAxis, depthAxis, wVar, orbittrap);

    printf("loaded program\n");

    fflush(stdout);
    printProgramLog(mandelbulb_shader);

    //Loop
    glutMainLoop();

    return EXIT_SUCCESS;
}
