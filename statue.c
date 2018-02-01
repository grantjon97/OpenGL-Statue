// Jonathan Grant
// OpenGL Wireframe Statue
// Displays a statue with 4 parts, stacked on top of
// each other, and sitting on a pedestal. The order
// of the 4 items can be changed in a variety of ways,
// by use of a menu (right-click to show menu).

#include <GL/glut.h>
#include <stdlib.h>

#define CUBE_SIZE 0.3
#define CONE_HEIGHT 0.3
#define BIG_SPHERE_RADIUS 0.25
#define LITTLE_SPHERE_RADIUS 0.15
#define BOTTOM_OF_STATUE -0.65
#define TOP_OF_STATUE 0.75

char objectOrder[4] = {'C', 'c', 'S', 's'};
char temp = 'C';
float offset = BOTTOM_OF_STATUE;
int randomInt = 0;

GLUquadricObj *cylinder, *topOfCylinder;

void init() {

    GLfloat mat_ambient[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_specular[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_position[] = {2.0, 2.0, 2.0, 1.0};
    GLfloat mat_shininess[] = {20.0};
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    cylinder = gluNewQuadric();
    topOfCylinder = gluNewQuadric();
}

void reshape (int w, int h) {

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

/*    if (w <= h)
        glOrtho(-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
            1.5*(GLfloat)h/(GLfloat)w, -10, -10);
    else
        glOrtho(-1.5*(GLfloat)w/(GLfloat)h,
            1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
*/

    gluPerspective(45.0, 1.0, 0.1, 10);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void drawBase() {

    glPushMatrix();
        glColor3f(0.2, 0.2, 0.2);
        glBegin(GL_QUADS);
            glVertex3f(-0.5, -0.5, -1.0);
            glVertex3f(-0.5, 0.5, -1.0);
            glVertex3f(0.5, 0.5, -1.0);
            glVertex3f(0.5, -0.5, -1.0);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.2, 0.2, 0.2);
        glTranslatef(0.0, 0.0, -1.01);
        gluCylinder(cylinder, 0.4, 0.4, 0.35, 60, 60);
        glTranslatef(0.0, 0.0, 0.35);
        gluDisk(topOfCylinder, 0.0, 0.4, 60, 60);
    glPopMatrix();
}

void drawCube() {

    glPushMatrix();
        glColor3f(1.0, 0, 0);
        glTranslatef(0.0, 0.0, offset + (CUBE_SIZE/2));
        glutWireCube(CUBE_SIZE);
    glPopMatrix();

    offset = offset + CUBE_SIZE;
}

void drawCone() {

    glPushMatrix();
        glColor3f(0, 1.0, 0);
        glTranslatef(0.0, 0.0, offset);
        glutWireCone(0.1, CONE_HEIGHT, 10, 10);
    glPopMatrix();

    offset = offset + CONE_HEIGHT;
}

void drawBigSphere() {

    glPushMatrix();
        glColor3f(0, 0, 1.0);
        glTranslatef(0.0, 0.0, offset + BIG_SPHERE_RADIUS);
        glutWireSphere(BIG_SPHERE_RADIUS, 10, 10);
    glPopMatrix();

    offset = offset + (2 * BIG_SPHERE_RADIUS);
}

void drawLittleSphere() {

    glPushMatrix();
        glColor3f(1.0, 1.0, 0);
        glTranslatef(0.0, 0.0, offset + LITTLE_SPHERE_RADIUS);
        glutWireSphere(LITTLE_SPHERE_RADIUS, 10, 10);
    glPopMatrix();

    offset = offset + (2 * LITTLE_SPHERE_RADIUS);
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up good viewing angle
    glPushMatrix();
    glRotatef(-95.0, 1.0, 0.0, 0.0);
    glRotatef(-7.0, 0.0, 1.0, 0.0);
    glRotatef(22.5, 0.0, 0.0, 1.0);

    // Bottom of statue is at z = -0.65
    // Top of statue is at    z =  0.75

    drawBase();

    // offset describes the current place on the z-axis
    // that we want to place an item.  The bottom of the item
    // will be placed right on the offset, and then the offset
    // is increased by the item's height.
    offset = BOTTOM_OF_STATUE;

    // Use the objectOrder array to decide which
    // order to draw the items in. This objectOrder
    // array is manipulated in the shiftItems() function.
    for (int i = 0; i < 4; i++) {

        if (objectOrder[i] == 'C')
            drawCube();
        else if (objectOrder[i] == 'c')
            drawCone();
        else if (objectOrder[i] == 'S')
            drawBigSphere();
        else
            drawLittleSphere();
    }

    glPopMatrix();

}

void shiftItems(int orderId) {

    // Shift items in a Clockwise direction
    if (orderId == 0) {

        temp = objectOrder[3];
        for (int i = 3; i > 0; i--) {
            objectOrder[i] = objectOrder[i-1];
        }
        objectOrder[0] = temp;
    }

    // Shift items in a Counter-Clockwise direction
    else if (orderId == 1) {

        temp = objectOrder[0];
        for (int i = 0; i < 3; i++) {
            objectOrder[i] = objectOrder[i+1];
        }
        objectOrder[3] = temp;
    }

    // Randomize the order of the items. The way this is
    // done is by swapping each item with another
    // random item until we have swapped 4 times.
    else {

        for (int i = 0; i < 4; i++) {

            do {
                randomInt = rand() % 4;
            } while (randomInt == i);

            temp = objectOrder[randomInt];
            objectOrder[randomInt] = objectOrder[i];
            objectOrder[i] = temp;
        } // end for
    } // end else
} // end shiftItems()

void menu(int id) {

    // Change order appropriately
    if ((0 <= id) && (id <= 2)) {
        shiftItems(id);
    }

    // Or quit.
    else {
        exit(0);
    }
}

void main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Jon's Statue");

    init();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    glutCreateMenu(menu);
    glutAddMenuEntry("CW Shift", 0);
    glutAddMenuEntry("CCW Shift", 1);
    glutAddMenuEntry("Random Order", 2);
    glutAddMenuEntry("Quit", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
}
