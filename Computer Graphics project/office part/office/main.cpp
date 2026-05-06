#include <GL/glut.h>
#include <bits/stdc++.h>

float fanAngle = 0.0f;
bool lightOn = true;
float visitorX = -11.5f;
float visitorZ = -2.0f;
float doorAngle = 0.0f;
bool doorOpened = false;
bool doorClosed = false;
float watchAngle = 0.0f;
bool isReturning = false;

void drawLineDDA(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;
    float x = x1, y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2f(x, y);
        x += xInc;
        y += yInc;
    }
    glEnd();
}

void drawLineMidPoint(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    glBegin(GL_POINTS);
    while (true) {
        glVertex2i(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
    glEnd();
}

void drawCircleMidPoint(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 1 - r;
    glBegin(GL_POINTS);
    while (x <= y) {
        glVertex2i(xc + x, yc + y); glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y); glVertex2i(xc - x, yc - y);
        glVertex2i(xc + y, yc + x); glVertex2i(xc - y, yc + x);
        glVertex2i(xc + y, yc - x); glVertex2i(xc - y, yc - x);
        if (d < 0) d += 2 * x + 3;
        else { d += 2 * (x - y) + 5; y--; }
        x++;
    }
    glEnd();
}

void drawFilledCircle(int xc, int yc, int r) {
    for (int i = 0; i <= r; i++) {
        drawCircleMidPoint(xc, yc, i);
    }
}

void drawSolidCube(float x, float y, float z,
                   float sx, float sy, float sz,
                   float r, float g, float b, float a = 1.0f) {

    GLfloat mat_diffuse[] = { r, g, b, a };
    GLfloat mat_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat shininess[] = { 20.0f };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawOfficeInterior() {

    drawSolidCube(0, -1.0, 0, 24, 0.1, 30, 0.55f, 0.55f, 0.55f);
    drawSolidCube(0, 9.0, 0, 24, 0.1, 30, 0.9f, 0.9f, 0.9f);
    drawSolidCube(0, 8.9, 5, 4, 0.1, 4, 1.0f, 1.0f, 0.8f);

    drawSolidCube(12, 4, 0, 0.2, 10, 30, 0.75f, 0.75f, 0.75f);
    drawSolidCube(-12, 4, 0, 0.2, 10, 30, 0.75f, 0.75f, 0.75f);
    drawSolidCube(0, 4, 15, 24, 10, 0.2, 0.8f, 0.8f, 0.8f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawSolidCube(0, 4, -15, 24, 10, 0.1, 0.4f, 0.7f, 0.9f, 0.3f);
    glDisable(GL_BLEND);
    drawSolidCube(0, 4, -14.9, 0.2, 10, 0.2, 0.1f, 0.1f, 0.1f);
    drawSolidCube(0, 4, -14.9, 24, 0.2, 0.2, 0.1f, 0.1f, 0.1f);

    drawSolidCube(0, 8.5, 5, 0.2, 1.0, 0.2, 0.2f, 0.2f, 0.2f);
    drawSolidCube(0, 8.0, 5, 1.2, 0.4, 1.2, 0.3f, 0.3f, 0.3f);
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
            glTranslatef(0, 8.0, 5);
            glRotatef(fanAngle + (i * 120.0f), 0, 1, 0);
            glTranslatef(2.5, 0, 0);
            drawSolidCube(0, 0, 0, 4.0, 0.1, 0.8, 0.15f, 0.15f, 0.15f);
        glPopMatrix();
    }

    glPushMatrix();
        glTranslatef(-11.9, 2.5, -4.0);
        glRotatef(doorAngle, 0, 1, 0);
        glTranslatef(0, 0, 2.0);
        drawSolidCube(0, 0, 0, 0.1, 7, 4, 0.4f, 0.25f, 0.1f);
        drawSolidCube(0.1, 0, 1.5, 0.05, 1.0, 0.4, 0.6f, 0.6f, 0.6f);
        drawSolidCube(0.2, 0.1, 1.5, 0.2, 0.15, 0.8, 0.2f, 0.2f, 0.2f);
    glPopMatrix();

    drawSolidCube(9.0, 3.0, -13.5, 3.0, 8.0, 2.0, 1.0f, 0.85f, 0.0f);
    drawSolidCube(9.0, 3.0, -12.4, 2.8, 7.8, 0.1, 0.9f, 0.75f, 0.0f);
    drawSolidCube(8.0, 3.5, -12.3, 0.2, 0.8, 0.2, 0.1f, 0.1f, 0.1f);

    drawSolidCube(0, 0, -5, 10, 0.4, 5, 0.5f, 0.3f, 0.15f);
    drawSolidCube(0, -0.6, 1, 2.5, 0.3, 2.5, 0.15f, 0.15f, 0.15f);
    drawSolidCube(0, 1.2, 2.2, 2.5, 3.5, 0.4, 0.25f, 0.25f, 0.25f);

    GLfloat skin_color[] = { 0.8f, 0.7f, 0.6f, 1.0f };
    GLfloat hair_color[] = { 0.15f, 0.1f, 0.05f, 1.0f };

    drawSolidCube(0, 1.5, 0.5, 1.2, 2.2, 0.8, 0.2f, 0.3f, 0.6f);
    drawSolidCube(-0.3, 0.1, 0.5, 0.4, 1.0, 0.4, 0.1f, 0.1f, 0.3f);
    drawSolidCube(0.3, 0.1, 0.5, 0.4, 1.0, 0.4, 0.1f, 0.1f, 0.3f);
    glPushMatrix();
        glTranslatef(0, 3.1, 0.5);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, skin_color);
        glutSolidSphere(0.6, 40, 40);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, hair_color);
        glTranslatef(0, 0.3, 0); glScalef(1.1, 0.5, 1.1);
        glutSolidCube(1.0);
    glPopMatrix();

    drawSolidCube(visitorX, 1.8, visitorZ, 1.1, 2.4, 0.7, 0.5f, 0.2f, 0.2f);
    drawSolidCube(visitorX - 0.25, 0.3, visitorZ, 0.35, 1.2, 0.35, 0.2f, 0.1f, 0.1f);
    drawSolidCube(visitorX + 0.25, 0.3, visitorZ, 0.35, 1.2, 0.35, 0.2f, 0.1f, 0.1f);
    glPushMatrix();
        glTranslatef(visitorX, 3.5, visitorZ);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, skin_color);
        glutSolidSphere(0.55, 40, 40);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, hair_color);
        glTranslatef(0, 0.35, 0.1); glScalef(1.0, 0.4, 0.9);
        glutSolidCube(1.0);
    glPopMatrix();

    drawSolidCube(0, 0.4, -5, 3.0, 0.1, 2.2, 0.05f, 0.05f, 0.05f);
    drawSolidCube(3.0, 0.5, -3.5, 0.5, 0.8, 0.5, 0.1f, 0.1f, 0.1f);
    drawSolidCube(-3.5, 0.35, -4.5, 1.8, 0.3, 1.2, 0.6f, 0.1f, 0.1f);
    drawSolidCube(-3.5, 0.65, -4.5, 1.8, 0.3, 1.2, 0.1f, 0.4f, 0.2f);
    drawSolidCube(4.0, 0.3, -6.0, 1.0, 0.2, 1.0, 0.2f, 0.2f, 0.2f);
    drawSolidCube(4.0, 1.3, -6.0, 0.1, 2.0, 0.1, 0.3f, 0.3f, 0.3f);
    glPushMatrix();
        glTranslatef(4.0, 2.3, -6.0);
        glRotatef(-45, 1, 0, 0);
        drawSolidCube(0, 0, 0, 0.8, 0.6, 1.2, 0.8f, 0.2f, 0.2f);
    glPopMatrix();

    glDisable(GL_LIGHTING);

    glPushMatrix();
        glTranslatef(0.0, 6.0, -14.8);
        glScalef(0.02, 0.02, 0.02);
        glColor3f(0.0f, 0.0f, 0.0f);
        drawCircleMidPoint(0, 0, 50);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0, 6.0, -14.7);
        glScalef(0.02, 0.02, 0.02);
        glColor3f(0.0f, 0.0f, 1.0f);
        drawLineDDA(0, 0, 0, 40);

        glColor3f(1.0f, 0.0f, 0.0f);
        drawLineMidPoint(0, 0, 25, 0);
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glPushMatrix();
        glBegin(GL_QUADS);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-11.9f, -0.9f, 14.5f);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(-2.0f, -0.9f, 14.5f);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(-2.0f, 7.0f, 14.5f);
            glColor3f(1.0f, 1.0f, 0.0f);
            glVertex3f(-11.9f, 7.0f, 14.5f);
        glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glDisable(GL_LIGHTING);
    glPushMatrix();
        glTranslatef(0.0, 6.0, -14.8);
        glScalef(0.02, 0.02, 0.02);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawFilledCircle(0, 0, 50);
        glColor3f(0.0f, 0.0f, 0.0f);
        drawCircleMidPoint(0, 0, 50);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0, 6.0, -14.7);
        glScalef(0.02, 0.02, 0.02);
        glRotatef(watchAngle, 0, 0, 1);
        glColor3f(0.0f, 0.0f, 1.0f);
        drawLineDDA(0, 0, 0, 40);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0, 6.0, -14.7);
        glScalef(0.02, 0.02, 0.02);
        glRotatef(watchAngle * 0.1, 0, 0, 1);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawLineMidPoint(0, 0, 25, 0);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void timer(int value) {
    fanAngle += 15.0f;
    if (fanAngle > 360) fanAngle -= 360;

    if (!isReturning) {
        if (!doorOpened) {
            if (doorAngle > -90.0f) doorAngle -= 1.5f;
            else doorOpened = true;
        }
        else if (doorOpened && !doorClosed) {
            if (visitorX < -9.5f) visitorX += 0.06f;
            else {
                if (doorAngle < 0.0f) doorAngle += 1.5f;
                else doorClosed = true;
            }
        }
        else if (doorClosed) {
            if (visitorZ > -9.0f) visitorZ -= 0.06f;
            else if (visitorX < -2.0f) visitorX += 0.06f;
        }
    }
    else {
        if (visitorX > -9.5f && visitorZ <= -9.0f) {
            visitorX -= 0.06f;
        }
        else if (visitorZ < -2.0f) {
            visitorZ += 0.06f;
        }
        else if (doorAngle > -90.0f && visitorX > -12.0f) {
            doorAngle -= 1.5f;
        }
        else if (visitorX > -12.5f) {
            visitorX -= 0.06f;
        }
        else if (doorAngle < 0.0f) {
            doorAngle += 1.5f;
            if(doorAngle > 0.0f) doorAngle = 0.0f;
        }
    }

    watchAngle -= 0.3f;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 6.0, 14.5, 0.0, 3.0, -5.0, 0.0, 1.0, 0.0);

    GLfloat light_pos[] = { 0.0f, 8.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    drawOfficeInterior();
    glutSwapBuffers();
}
//Alrazi Hosen Rizvy
void keyboard(unsigned char key, int x, int y) {
    if (key == '1') {
        if (doorClosed && visitorX >= -2.0f) {
            isReturning = true;
            doorClosed = false;
        }
    }

    if (key == '2') {
        lightOn = !lightOn;
        if (lightOn) {
            glEnable(GL_LIGHT0);
        } else {
            glDisable(GL_LIGHT0);
        }
        glutPostRedisplay();
    }
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    GLfloat ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("Office Interior");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
