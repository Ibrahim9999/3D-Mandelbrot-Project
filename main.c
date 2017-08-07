#include "main.h"

//Render quad
void draw() {
    //Clear
    glClear(GL_COLOR_BUFFER_BIT);

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

    draw();

    glutSwapBuffers();
    glFinish();

    lastlastframe = lastframe;
    lastframe = glutGet(GLUT_ELAPSED_TIME);

    printMonitors();
}

//Handle mouse input
void handleMouse(int x, int y) {
    if (userfocus == VIEW_FOCUS) {
        cameraMoveMouse(x, y);
    }
}


//Handle keyboard input
static unsigned int kbinputbuffer[KEYBUFFERLEN*4];
static int kbinputlen = 0;

void clearKeyBuffer() {
    kbinputlen = 0;

    int i = 0;

    while (i < KEYBUFFERLEN*4) {
        kbinputbuffer[i] = false;
        i++;
    }
}
    
void handleKeyboard(unsigned char key, int x, int y) {
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

void handleKeyboardUp(unsigned char key, int x, int y) {
    
}

void sendKeySignals() {
    int key;

    for (key = 0; key != kbinputlen; key++) {
            putchar(kbinputbuffer[key*4]);
            if (userfocus == VIEW_FOCUS) {
                cameraMoveKeyboard(kbinputbuffer[key*4], kbinputbuffer[key*4+1],
                    kbinputbuffer[key*4+2], kbinputbuffer[key*4+3]);
                glutPostRedisplay();
            }
    }

    clearKeyBuffer();
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

void printMonitors() {
    char string[80];    
    float fps = 1000/(lastframe-lastlastframe);

    strcpy(string, "FPS:");
	
    sprintf(string+4, "%f", fps);
	
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
        //glutBitmapCharacter(font, *c);
    }

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();  
	
}

//Update shader variables
void updateMandelbulbVars() {
    loadMandelbulbVars(mandelbulb_shader, fov, camerapos, color, step, bail,
		power, phi, theta, totalRotation, resolution, multisampling, lightpos,
		intensity, horizontalAxis, verticalAxis, depthAxis, wVar);
}

//Idle Function
void idle() {
	sendKeySignals();
    updateMandelbulbVars();
}

//Main
int main(int argc, char* argv[]) {

    //printf("hello main\n");
    fflush(stdout);

    //Set vars
    hfov = vfov = START_FOV;
    cameradist = START_WIDTH/(2*tan(START_FOV/360*PI_CONST));
    setFOVvec(&fov, vfov, hfov);
    InitializeCamera(&camerapos, &depthAxis, &horizontalAxis, &verticalAxis);

    totalRotation.x = 0;
	totalRotation.y = 0;
	totalRotation.z = 0;
	totalRotation.w = 1;

    color.x=0; color.y=1; color.z=1;
	
    lightpos.x= -0.178390;
	lightpos.y = -2.660062;
	lightpos.z = -0.930965;

	wVar = 0;
    step = 0.01;
    bail = 10;
    multisampling=1;
    power = 2;
    phi = 0;
    theta = 0;
    intensity = 4.50;
    resolution.x = START_WIDTH; resolution.y = START_HEIGHT;

    //Setup window
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
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
   
    //printf("starting shaders\n");
    fflush(stdout);

    //Setup shaders
    loadMandelbulbProgram(&mandelbulb_shader, fov, camerapos, color, step,
		bail, power, phi, theta, totalRotation, resolution, multisampling,
		lightpos, intensity, horizontalAxis, verticalAxis, depthAxis, wVar);

    printf("loaded program\n");

    fflush(stdout);
    printProgramLog(mandelbulb_shader);

    //Loop
    glutMainLoop();

    return EXIT_SUCCESS;
}
