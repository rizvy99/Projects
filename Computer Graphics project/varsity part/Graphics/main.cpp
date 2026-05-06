#include <GL/glut.h>
#include <cmath>
#include <algorithm>

#define PI 3.14159265f

static float g_timeOfDay = 1.57f;
static float g_dayFactor = 1.0f;
static float g_sunY      = 6.4f;

// DDA Line Algorithm
void drawLineDDA(float x1, float y1, float z1, float x2, float y2, float z2) {

    float dx = x2 - x1, dy = y2 - y1, dz = z2 - z1;
    float steps = std::max((float)fabs(dx), std::max((float)fabs(dy), (float)fabs(dz))) * 100.0f;
    float xInc = dx / steps, yInc = dy / steps, zInc = dz / steps;
    float x = x1, y = y1, z = z1;
    glBegin(GL_POINTS);
    for (int i = 0; i <= (int)steps; i++) {
        glVertex3f(x, y, z);
        x += xInc; y += yInc; z += zInc;
    }
    glEnd();
}

// Midpoint Circle Algorithm
void drawCircleMidpoint(float x0, float y0, float z, float radius) {

    for (float r = 0; r <= radius; r += 0.01f) {
        int x = 0, y = (int)(r * 100), p = 1 - y;
        glBegin(GL_POINTS);
        while (x <= y) {
            float fx = x / 100.0f, fy = y / 100.0f;
            glVertex3f(x0 + fx, y0 + fy, z); glVertex3f(x0 - fx, y0 + fy, z);
            glVertex3f(x0 + fx, y0 - fy, z); glVertex3f(x0 - fx, y0 - fy, z);
            glVertex3f(x0 + fy, y0 + fx, z); glVertex3f(x0 - fy, y0 + fx, z);
            glVertex3f(x0 + fy, y0 - fx, z); glVertex3f(x0 - fy, y0 - fx, z);
            x++;
            if (p < 0) p += 2 * x + 1;
            else { y--; p += 2 * (x - y) + 1; }
        }
        glEnd();
    }
}

// Midpoint Line Algorithm
void drawLineMidpoint(float x1, float y1, float z1, float x2, float y2, float z2) {

    float dx = (float)fabs(x2 - x1), dy = (float)fabs(y2 - y1), dz = (float)fabs(z2 - z1);
    float steps = std::max(dx, std::max(dy, dz)) * 100.0f;
    float xInc = (x2 - x1) / steps, yInc = (y2 - y1) / steps, zInc = (z2 - z1) / steps;
    float x = x1, y = y1, z = z1;
    glBegin(GL_POINTS);
    for (int i = 0; i <= (int)steps; i++) {
        glVertex3f(x, y, z);
        x += xInc; y += yInc; z += zInc;
    }
    glEnd();
}

// Box Drawing Helper
void drawBox(float x, float y, float z, float w, float h, float d) {

    glPushMatrix();
    glTranslatef(x + w / 2.0f, y + h / 2.0f, z + d / 2.0f);
    glScalef(w, h, d);
    glutSolidCube(1.0);
    glPopMatrix();
}

// Left Outdoor Scene
static void drawOutdoor() {
    float s = (sin(g_timeOfDay) + 1.0f) * 0.5f;
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    glColor3f(0.24f * s, 0.54f * s, 0.90f * s); glVertex3f(-8.4f, 8.0f, -13.0f); glVertex3f(-8.4f, 8.0f, 15.0f);
    glColor3f(0.62f * s, 0.84f * s, 0.98f * s); glVertex3f(-8.4f, 3.4f, 15.0f); glVertex3f(-8.4f, 3.4f, -13.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.35f * s, 0.62f * s, 0.24f * s); glVertex3f(-8.4f, 3.4f, -13.0f); glVertex3f(-8.4f, 3.4f, 15.0f);
    glColor3f(0.20f * s, 0.44f * s, 0.15f * s); glVertex3f(-8.4f, 0.0f, 15.0f); glVertex3f(-8.4f, 0.0f, -13.0f);
    glEnd();
    glPushMatrix();
    glTranslatef(-12.5f, 3.8f, 0.0f);
    glColor3f(0.68f * s, 0.75f * s, 0.87f * s); glutSolidSphere(3.2f, 36, 28);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}


// Right Outdoor Scene
static void drawRightOutdoor() {
    float s = (sin(g_timeOfDay) + 1.0f) * 0.5f;
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    glColor3f(0.30f * s, 0.60f * s, 0.95f * s); glVertex3f(10.9f, 8.0f, -13.0f); glVertex3f(10.9f, 8.0f, 15.0f);
    glColor3f(0.68f * s, 0.88f * s, 1.00f * s); glVertex3f(10.9f, 3.2f, 15.0f); glVertex3f(10.9f, 3.2f, -13.0f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.38f * s, 0.65f * s, 0.25f * s); glVertex3f(10.9f, 3.2f, -13.0f); glVertex3f(10.9f, 3.2f, 15.0f);
    glColor3f(0.22f * s, 0.46f * s, 0.16f * s); glVertex3f(10.9f, 0.0f, 15.0f); glVertex3f(10.9f, 0.0f, -13.0f);
    glEnd();
    glEnable(GL_LIGHTING);
}


// Sun
static void drawSun(float x, float y, float z, float r) {
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(r, 20, 20);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}



// Back Outdoor Scene
static void drawBackOutdoor() {
    float s = (sin(g_timeOfDay) + 1.0f) * 0.5f;
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    glColor3f(0.2f * s, 0.4f * s, 0.8f * s); glVertex3f(-12, 10, -13.4f); glVertex3f(12, 10, -13.4f);
    glColor3f(0.6f * s, 0.8f * s, 1.0f * s); glVertex3f(12, 3, -13.4f); glVertex3f(-12, 3, -13.4f);
    glEnd();
    if (s > 0.15f) drawSun(1.0f, 2.0f + 3.5f * s, -13.25f, 0.8f);
    glBegin(GL_QUADS);
    glColor3f(0.1f * s, 0.4f * s, 0.1f * s); glVertex3f(-12, 3, -13.4f); glVertex3f(12, 3, -13.4f);
    glColor3f(0.0f, 0.2f * s, 0.0f); glVertex3f(12, 0, -13.4f); glVertex3f(-12, 0, -13.4f);
    glEnd();
    glEnable(GL_LIGHTING);
}





// Floor
static void drawFloor() {

    glDisable(GL_LIGHTING);
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 14; j++) {
            bool light = (i + j) % 2 == 0;
            glColor3f(light ? 0.91f : 0.83f, light ? 0.89f : 0.81f, light ? 0.86f : 0.78f);
            float x0 = -8.0f + i * 1.541f + 0.02f, z0 = -13.0f + j * 1.643f + 0.02f;
            drawBox(x0, 0.01f, z0, 1.5f, 0.01f, 1.6f);
        }
    }
    glEnable(GL_LIGHTING);
}

// Ceiling
static void drawCeiling() {

    glColor3f(0.93f, 0.93f, 0.95f);
    drawBox(-8.0f, 7.0f, -13.0f, 18.5f, 0.1f, 23.0f);
}

// Left Wall
static void drawLeftWall() {
    float s = (sin(g_timeOfDay) + 1.0f) * 0.5f;
    glColor3f(0.87f, 0.87f, 0.91f);
    drawBox(-8.0f, 6.45f, -13.0f, 0.15f, 0.55f, 23.0f);
    drawBox(-8.0f, 0.0f, -13.0f, 0.15f, 1.10f, 23.0f);
    float wC[] = {8.8f, 6.0f, 3.1f, 0.1f, -2.9f, -5.9f, -8.8f};
    for(int i=0; i<7; i++) drawBox(-8.0f, 1.1f, wC[i]-0.15f, 0.15f, 5.35f, 0.3f);
    glEnable(GL_BLEND); glDisable(GL_LIGHTING); glColor4f(0.68f * s, 0.84f * s, 1.00f * s, 0.12f);
    drawBox(-8.05f, 1.1f, -13.0f, 0.01f, 5.35f, 23.0f);
    glDisable(GL_BLEND); glEnable(GL_LIGHTING);
}


// Right Wall
static void drawWalls() {
    float s = (sin(g_timeOfDay) + 1.0f) * 0.5f;
    glColor3f(0.87f, 0.87f, 0.91f);
    drawBox(10.5f, 6.2f, -13.0f, 0.15f, 0.8f, 23.0f);
    drawBox(10.5f, 0.0f, -13.0f, 0.15f, 1.2f, 23.0f);
    float RWZ[] = {7.5f, 4.5f, 1.5f, -1.5f, -4.5f, -7.5f, -10.5f};
    for(int i=0; i<7; i++) drawBox(10.5f, 1.2f, RWZ[i]-0.3f, 0.15f, 5.0f, 0.6f);
    glEnable(GL_BLEND); glDisable(GL_LIGHTING); glColor4f(0.68f * s, 0.84f * s, 1.00f * s, 0.12f);
    drawBox(10.55f, 1.2f, -13.0f, 0.01f, 5.0f, 23.0f);
    glDisable(GL_BLEND); glEnable(GL_LIGHTING);
}


// Back Wall
static void drawBackWall() {
    float s = (sin(g_timeOfDay) + 1.0f) * 0.5f;
    glColor3f(0.87f, 0.87f, 0.91f);
    drawBox(-8.0f, 6.7f, -13.0f, 18.5f, 0.3f, 0.3f);
    drawBox(-8.0f, 0.0f, -13.0f, 18.5f, 0.3f, 0.3f);
    drawBox(-8.0f, 0.0f, -13.0f, 0.3f, 7.0f, 0.3f);
    drawBox(10.2f, 0.0f, -13.0f, 0.3f, 7.0f, 0.3f);
    glEnable(GL_BLEND); glDisable(GL_LIGHTING); glColor4f(0.68f * s, 0.84f * s, 1.00f * s, 0.09f);
    drawBox(-8.0f, 0.0f, -12.94f, 18.5f, 7.0f, 0.01f);
    glDisable(GL_BLEND); glEnable(GL_LIGHTING);
}


// AC Unit
static void drawAC(float x, float y, float z) {

    glColor3f(0.91f, 0.91f, 0.93f); drawBox(x - 1.75f, y - 0.28f, z - 0.475f, 3.5f, 0.28f, 0.95f);
}

// Projector
static void drawProjector(float x, float y, float z) {

    glColor3f(0.50f, 0.50f, 0.52f); drawBox(x - 0.03f, y - 0.32f, z - 0.03f, 0.06f, 0.32f, 0.06f);
    glColor3f(0.18f, 0.18f, 0.20f); drawBox(x - 0.26f, y - 0.65f, z - 0.18f, 0.52f, 0.20f, 0.36f);
}

// Desk
static void drawArmChair(float x, float y, float z) {

    glPushMatrix(); glTranslatef(x, y, z);
    glColor3f(0.14f, 0.14f, 0.16f);
    drawBox(-0.3585f, 0.0f, -0.3585f, 0.057f, 1.10f, 0.057f);
    drawBox( 0.3015f, 0.0f, -0.3585f, 0.057f, 1.10f, 0.057f);
    drawBox(-0.3585f, 0.0f,  0.3015f, 0.057f, 1.10f, 0.057f);
    drawBox( 0.3015f, 0.0f,  0.3015f, 0.057f, 1.10f, 0.057f);
    glColor3f(0.15f, 0.15f, 0.17f); drawBox(-0.33f, 1.10f, -0.325f, 0.66f, 0.08f, 0.65f);
    glPushMatrix(); glTranslatef(0.0f, 1.60f, -0.28f); glRotatef(-4.0f, 1.0f, 0.0f, 0.0f); drawBox(-0.325f, -0.4f, -0.035f, 0.65f, 0.80f, 0.07f); glPopMatrix();
    glColor3f(0.14f, 0.14f, 0.16f); drawBox(0.33f, 1.34f, -0.33f, 0.07f, 0.10f, 0.66f);
    glColor3f(0.48f, 0.38f, 0.28f); drawBox(0.31f, 1.44f, -0.38f, 0.45f, 0.05f, 1.15f);
    glPopMatrix();
}

// Book
static void drawBook(float x, float y, float z, float rotY, float r, float g, float b) {

    glPushMatrix(); glTranslatef(x, y, z); glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glColor3f(r, g, b); drawBox(-0.22f, -0.0275f, -0.3f, 0.44f, 0.055f, 0.60f);
    glColor3f(0.96f, 0.96f, 0.92f); drawBox(-0.2f, -0.02f, 0.2975f, 0.40f, 0.040f, 0.015f);
    glPopMatrix();
}

// Student
static void drawStudent(float x, float y, float z, float sR, float sG, float sB, float pR, float pG, float pB, float kR, float kG, float kB, int hair, int glass) {

    glPushMatrix(); glTranslatef(x, y, z);
    glColor3f(pR, pG, pB); drawBox(-0.35f, 0.0f, -0.15f, 0.3f, 1.6f, 0.3f); drawBox(0.05f, 0.0f, -0.15f, 0.3f, 1.6f, 0.3f);
    glColor3f(sR, sG, sB); drawBox(-0.36f, 1.55f, -0.22f, 0.72f, 0.95f, 0.43f);
    glColor3f(kR, kG, kB); drawBox(-0.085f, 2.45f, -0.085f, 0.17f, 0.22f, 0.17f);
    glPushMatrix(); glTranslatef(0.0f, 2.95f, 0.0f); glRotatef(20.0f, 1.0f, 0.0f, 0.0f); glutSolidSphere(0.3f, 16, 12); glPopMatrix();
    glPopMatrix();
}

// Main Display
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); glLoadIdentity();
    gluLookAt(-5.2, 4.9, 9.8, 3.8, 2.4, -7.5, 0, 1, 0);

    float s = (sin(g_timeOfDay) + 1.0f) * 0.5f;
    GLfloat amb[] = {0.2f * s, 0.2f * s, 0.2f * s, 1.0f};
    GLfloat dif[] = {0.7f * s, 0.7f * s, 0.7f * s, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb); glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);

    drawOutdoor(); drawRightOutdoor(); drawBackOutdoor();

    drawFloor(); drawCeiling(); drawWalls(); drawLeftWall(); drawBackWall();
    drawAC(2.0f, 7.0f, -1.0f); drawProjector(8.5f, 7.0f, 2.8f);
    static const float rowZ[] = {5.0f, 2.5f, 0.0f, -2.5f, -5.0f, -7.5f};
    static const float colX[] = {-3.5f, 0.5f, 4.0f, 7.5f};
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (r == 1 && (c == 0 || c == 1)) continue;
            drawArmChair(colX[c], 0.0f, rowZ[r]);
        }
        drawArmChair(colX[3] + 3.2f, 0.0f, rowZ[r]);
    }
    drawArmChair(colX[0], 0.0f, rowZ[1]); drawArmChair(colX[1], 0.0f, rowZ[1]);
    drawBook(colX[0]+0.35f, 1.4f, rowZ[1]-0.2f, 10, 0.12f, 0.18f, 0.52f);
    drawBook(colX[1]+0.38f, 1.4f, rowZ[1]-0.18f, -8, 0.1f, 0.16f, 0.5f);
    drawStudent(colX[0], 0.0f, rowZ[1]+0.85f, 0.34f, 0.4f, 0.23f, 0.18f, 0.18f, 0.22f, 0.88f, 0.73f, 0.59f, 1, 0);
    drawStudent(colX[1], 0.0f, rowZ[1]+0.85f, 0.11f, 0.17f, 0.44f, 0.42f, 0.34f, 0.28f, 0.76f, 0.6f, 0.47f, 0, 1);
    glColor3f(0.28f, 0.27f, 0.3f); drawBox(colX[0]-1.425f, 0.0f, rowZ[1]+0.51f, 0.55f, 0.84f, 0.38f);
    glutSwapBuffers();
}


// Idle Function
void idle() { g_timeOfDay += 0.002f; if (g_timeOfDay > 2*PI) g_timeOfDay -= 2*PI; glutPostRedisplay(); }


// Reshape
void reshape(int w, int h) { if (h == 0) h = 1; glViewport(0, 0, w, h); glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluPerspective(48.0, (double)w/h, 0.4, 130.0); glMatrixMode(GL_MODELVIEW); }

// Keyboard
void keyboard(unsigned char k, int x, int y) { if (k == 27) exit(0); }

// Init
void init() {
    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); glEnable(GL_COLOR_MATERIAL);
    GLfloat pos[] = {2, 6.5f, 0, 1}, amb[] = {0.3f, 0.3f, 0.3f, 1}, dif[] = {0.8f, 0.8f, 0.8f, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, pos); glLightfv(GL_LIGHT0, GL_AMBIENT, amb); glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
}


// Main
int main(int argc, char** argv) {

    glutInit(&argc, argv); glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1100, 750); glutCreateWindow("Simplified Classroom Scene");
    init(); glutDisplayFunc(display); glutReshapeFunc(reshape); glutKeyboardFunc(keyboard); glutIdleFunc(idle);
    glutMainLoop(); return 0;
}
