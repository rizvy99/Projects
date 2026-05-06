#include <GL/glut.h>
#include <cmath>
#include <algorithm>

// Animation variables
float fanAngle = 0.0f;
float teacherX = 7.5f;
float teacherspeed = 0.002f;

// --- ALGORITHM 1: DDA Line Drawing (For Window Frame) ---
void drawLineDDA(float x1, float y1, float z1, float x2, float y2, float z2) {
    float dx = x2 - x1, dy = y2 - y1, dz = z2 - z1;
    float steps = std::max({std::abs(dx), std::abs(dy), std::abs(dz)}) * 100;
    float xInc = dx / steps, yInc = dy / steps, zInc = dz / steps;
    float x = x1, y = y1, z = z1;
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex3f(x, y, z);
        x += xInc; y += yInc; z += zInc;
    }
    glEnd();
}

// --- ALGORITHM 2: Midpoint Circle Drawing (For Character Heads) ---
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

// --- ALGORITHM 3: Midpoint Line Algorithm (For Door Frame) ---
void drawLineMidpoint(float x1, float y1, float z1, float x2, float y2, float z2) {
    float dx = std::abs(x2 - x1), dy = std::abs(y2 - y1), dz = std::abs(z2 - z1);
    float steps = std::max({dx, dy, dz}) * 100;
    float xInc = (x2 - x1) / steps, yInc = (y2 - y1) / steps, zInc = (z2 - z1) / steps;
    float x = x1, y = y1, z = z1;
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex3f(x, y, z);
        x += xInc; y += yInc; z += zInc;
    }
    glEnd();
}

void drawBox(float x, float y, float z, float w, float h, float d) {
    glPushMatrix();
    glTranslatef(x + w / 2.0f, y + h / 2.0f, z + d / 2.0f);
    glScalef(w, h, d);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawStudent(float x, float y, float z, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(r, g, b);
    drawBox(-0.4, 0, -0.2, 0.8, 1.2, 0.4);
    glColor3f(0.9, 0.7, 0.6);
    drawCircleMidpoint(0, 1.5, 0, 0.3);
    glColor3f(0.1, 0.1, 0.1);
    drawBox(-0.3, 1.65, -0.2, 0.6, 0.2, 0.4);
    glPopMatrix();
}

void drawTeacher(float xPos) {
    glPushMatrix();
    glTranslatef(xPos, 1.0, -7.5);
    glColor3f(0.1, 0.1, 0.4); drawBox(-0.5, 0, -0.3, 1, 1.6, 0.6);
    glColor3f(0.0, 0.0, 0.2);
    drawBox(-0.4, -1.0, -0.2, 0.3, 1.0, 0.4);
    drawBox(0.1, -1.0, -0.2, 0.3, 1.0, 0.4);
    glColor3f(0.9, 0.7, 0.6); drawCircleMidpoint(0, 1.9, 0, 0.35);
    glColor3f(0.05, 0.05, 0.05); drawBox(-0.35, 2.1, -0.25, 0.7, 0.2, 0.5);
    glPopMatrix();
}

void drawFan(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(0.1, 0.1, 0.1); drawBox(-0.05, 0, -0.05, 0.1, 1.0, 0.1);
    glRotatef(fanAngle, 0, 1, 0);
    for (int i = 0; i < 4; i++) {
        glRotatef(90, 0, 1, 0);
        glColor3f(0.3, 0.3, 0.3); drawBox(0.2, -0.08, -0.15, 1.8, 0.05, 0.3);
    }
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(7.5, 4.5, 22.0, 7.5, 3.2, 0.0, 0.0, 1.0, 0.0);

    // Environment
    glColor3f(0.8, 0.8, 0.8); drawBox(0, -0.1, -10, 15, 0.1, 30); // Floor
    glColor3f(0.95, 0.95, 0.95); drawBox(0, 10, -10, 15, 0.2, 30); // Ceiling

    // LEFT WALL STRUCTURE
    glColor3f(0.85, 0.85, 0.8);
    drawBox(0, 0, -10, 0.1, 10, 5);  // Wall back
    drawBox(0, 0, 5, 0.1, 10, 15);   // Wall front
    drawBox(0, 0, -5, 0.1, 3.5, 10); // Wall below window
    drawBox(0, 7.5, -5, 0.1, 2.5, 10); // Wall above window

    // --- DDA WINDOW FRAME ---
    glPointSize(4.0); glColor3f(0.0, 0.0, 0.0); // Solid Black Frame
    drawLineDDA(0.1, 3.5, -5.0, 0.1, 7.5, -5.0); // Vertical Left
    drawLineDDA(0.1, 3.5, 5.0, 0.1, 7.5, 5.0);   // Vertical Right
    drawLineDDA(0.1, 3.5, -5.0, 0.1, 3.5, 5.0);  // Horizontal Bottom
    drawLineDDA(0.1, 7.5, -5.0, 0.1, 7.5, 5.0);  // Horizontal Top
    drawLineDDA(0.1, 3.5, 0.0, 0.1, 7.5, 0.0);   // Middle vertical bar

    // --- TRANSPARENT WINDOW GLASS ---
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.6, 0.8, 1.0, 0.3); // Light blue transparent
    drawBox(0.05, 3.5, -5.0, 0.02, 4.0, 10.0);
    glDisable(GL_BLEND);

    // RIGHT WALL & DOOR (Midpoint Line)
    glColor3f(0.85, 0.85, 0.8);
    drawBox(14.9, 0, -10, 0.1, 10, 15); drawBox(14.9, 0, 10, 0.1, 10, 10);
    drawBox(14.9, 7.5, 5, 0.1, 2.5, 5);
    glColor3f(0.2, 0.1, 0.0); glPointSize(4.0);
    drawLineMidpoint(14.88, 0.0, 5.0, 14.88, 7.5, 5.0);
    drawLineMidpoint(14.88, 0.0, 10.0, 14.88, 7.5, 10.0);
    drawLineMidpoint(14.88, 7.5, 5.0, 14.88, 7.5, 10.0);
    glColor3f(0.4, 0.2, 0.1); drawBox(14.85, 0, 5, 0.03, 7.5, 5.0);
    glColor3f(1.0, 1.0, 0.0); drawBox(14.8, 3.2, 9.3, 0.1, 0.8, 0.5); // Yellow Locker

    // Back Wall & Whiteboard
    glColor3f(0.9, 0.9, 0.9); drawBox(0, 0, -10, 15, 10, 0.1);
    glColor3f(1.0, 1.0, 1.0); drawBox(4.5, 3.8, -9.8, 6.0, 3.5, 0.05);

    drawTeacher(teacherX);
    drawFan(4, 10, 0); drawFan(11, 10, 0); drawFan(7.5, 10, 8);

    for (int i = 0; i < 2; i++) {
        float z = 3 + i * 5;
        glColor3f(0.5, 0.3, 0.1); drawBox(3.5, 1.8, z, 8, 0.2, 1.5);
        glColor3f(0.6, 0.4, 0.2); drawBox(3.5, 0.8, z + 2.5, 8, 0.2, 0.8);
        drawStudent(5.5, 1, z + 2.7, 0.7, 0.1, 0.1);
        drawStudent(9.5, 1, z + 2.7, 0.1, 0.6, 0.1);
    }
    glutSwapBuffers();
}

void idle() {
    fanAngle += 4.5f;
    teacherX += teacherspeed;
    if (teacherX > 9.5f || teacherX < 5.5f) teacherspeed *= -1;
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(55, 1.33, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Classroom - DDA Window Fix");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
