#include <GL/glut.h>
#include <math.h>

float cameraAngleX = 0.0f; // goc quay quanh truc x
float cameraAngleY = 0.0f; // goc quay quanh truc y
float cameraDistance = 30.0f; // khoang cach tu camera den trung tam thanh pho
bool isDragging = false; // co trang thai
int lastMouseX, lastMouseY;

// vi tri ban dau cua xe bus
float busPositionX = -15.0f; // vi tri ban dau cua xe bus
float busSpeed = 0.05f; // toc do cua bus

// luu vi tri cac dam may
float cloudPositions[5][3] = {
    {15.0f, 10.0f, 5.0f},
    {5.0f, 7.0f, -10.0f},
    {10.0f, 8.0f, -5.0f},
    {10.0f, 5.0f, 10.0f},
    {0.0f, 10.0f, 15.0f}
};

float cloudSpeed = 0.007f; // toc do di chuyen cua may

bool isNight = false; // co kiem tra trang thai ngay dem

bool isXRed = false; // co trang thai den giao thong
bool showClouds = true; // trang thai may (khong hien thi may khi troi toi)

// vi tri den giao thong
float trafficLight1X = 9.0f;  // den truc Z
float trafficLight1Z = 2.0f;

float trafficLight2X = 5.0f;  // den truc X
float trafficLight2Z = -2.0f;

// su kien phim
void keyboard(unsigned char key, int x, int y) {
    if (key == 'n') {
        isNight = true;
        showClouds = false; // tat may khi troi toi
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glDisable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
    }
    else if (key == 'd') {
        isNight = false;
        showClouds = true; // hien thi may khi troi sang
        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
        glEnable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
    }
    else if (key == 'r') { // den do truc X
        isXRed = true;
    }
    else if (key == 'g') { // den do truc Z
        isXRed = false;
    }
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);  // anh sang chinh
    glEnable(GL_LIGHT1);  // anh sang phu (anh sang den duong vao ban dem)
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);

    // anh sang ban ngay
    GLfloat dayLight_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
    GLfloat dayLight_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat dayLight_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat dayLight_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, dayLight_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, dayLight_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dayLight_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, dayLight_specular);

    // anh sang ban dem
    GLfloat nightLight_ambient[] = { 0.1f, 0.1f, 0.15f, 1.0f };
    GLfloat nightLight_diffuse[] = { 0.3f, 0.3f, 0.4f, 1.0f };
    GLfloat nightLight_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    glLightfv(GL_LIGHT1, GL_AMBIENT, nightLight_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, nightLight_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, nightLight_specular);

    glEnable(GL_NORMALIZE);
}

// vi tri va huong cua may bay
float planeX1 = -15.0f;
float planeY1 = 15.0f;
float planeZ1 = 5.0f;
float planeSpeed1 = 0.01f;

float planeX2 = -20.0f;
float planeY2 = 12.0f;
float planeZ2 = -10.0f;
float planeSpeed2 = 0.01f;

float planeX3 = -10.0f;
float planeY3 = 6.0f;
float planeZ3 = -2.0f;
float planeSpeed3 = 0.01f;

// than may bay
void drawPlaneBody() {
    glPushMatrix();
    glColor3f(0.9f, 0.9f, 0.9f);
    glScalef(2.0f, 0.5f, 0.5f);
    glutSolidCube(1.0);
    glPopMatrix();
}

// canh may bay
void drawPlaneWings() {
    glPushMatrix();
    glColor3f(0.5f, 0.8f, 0.9f);
    glScalef(1.0f, 0.1f, 3.0f);
    glutSolidCube(1.0);
    glPopMatrix();
}

// Ve duoi may bay
void drawPlaneTail() {
    glPushMatrix();
    glColor3f(0.6f, 0.6f, 0.6f);
    glTranslatef(-0.8f, 0.3f, 0.0f);
    glScalef(0.5f, 0.5f, 0.8f);
    glutSolidCube(1.0);
    glPopMatrix();
}

// canh quat
void drawPropeller() {
    static float propAngle = 0.0f;
    glPushMatrix();
    // canh truoc 
    glTranslatef(1.0f, 0.0f, 0.0f);
    glRotatef(propAngle, 1.0f, 0.0f, 0.0f);

    glColor3f(0.3f, 0.3f, 0.3f);
    // canh doc
    glPushMatrix();
    glScalef(0.1f, 1.0f, 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();

    // canh ngang
    glPushMatrix();
    glScalef(0.1f, 0.1f, 1.0f);
    glutSolidCube(1.0);
    glPopMatrix();

    propAngle += 10.0f;
    glPopMatrix();
}

// ve toan bo may bay
void drawPlane(float& planeX, float planeY, float planeZ, float planeSpeed) {
    glPushMatrix();

    // di chuyen may bay theo truc X
    planeX1 += planeSpeed1;

    glTranslatef(planeX, planeY, planeZ);
    drawPlaneBody();
    drawPlaneWings();
    drawPlaneTail();
    drawPropeller();

    glPopMatrix();
}

// cap nhat may bay
void updatePlane(int value) {
    // cap nhat vi tri cho ba may bay
    planeX1 += planeSpeed1;
    planeX2 += planeSpeed2;
    planeX3 += planeSpeed3;

    // reset vi tri khi may bay ra khoi man hinh
    if (planeX1 > 20.0f) planeX1 = -20.0f;
    if (planeX2 > 20.0f) planeX2 = -20.0f;
    if (planeX3 > 20.0f) planeX3 = -20.0f;

    glutPostRedisplay();
    glutTimerFunc(16, updatePlane, 0);
}

// ve toa nha
void drawBuilding(float x, float y, float z, float width, float height, float depth) {
    glPushMatrix();
    glTranslatef(x, y + height / 2, z);
    glScalef(width, height, depth);
    glutSolidCube(1.0);
    glPopMatrix();
}

// ve cay
void drawTree(float x, float y, float z) {
    // than cay
    glPushMatrix();
    glTranslatef(x, y + 0.5, z);
    glColor3f(0.55, 0.27, 0.07);
    glScalef(0.2, 1.0, 0.2);
    glutSolidCube(1.0);
    glPopMatrix();

    // tan cay
    glPushMatrix();
    glTranslatef(x, y + 1.2, z);
    glColor3f(0.0, 0.8, 0.0);
    glutSolidSphere(0.5, 16, 16);
    glPopMatrix();
}

// ve den duong
void drawLampPost(float x, float y, float z) {
    // Thân đèn
    glPushMatrix();
    glTranslatef(x, y + 1.0, z);
    glColor3f(0.3, 0.3, 0.3);
    glScalef(0.1, 2.0, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();

    // bong den
    glPushMatrix();
    glTranslatef(x, y + 2.2, z);

    if (isNight) {
        // anh sang mat troi vao ban dem
        GLfloat lampLight_position[] = { x, y + 2.2f, z, 1.0f };
        GLfloat lampLight_diffuse[] = { 1.0f, 1.0f, 0.7f, 1.0f };
        GLfloat lampLight_ambient[] = { 0.2f, 0.2f, 0.1f, 1.0f };
        GLfloat lampLight_specular[] = { 1.0f, 1.0f, 0.7f, 1.0f };

        glLightfv(GL_LIGHT1, GL_POSITION, lampLight_position);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lampLight_diffuse);
        glLightfv(GL_LIGHT1, GL_AMBIENT, lampLight_ambient);
        glLightfv(GL_LIGHT1, GL_SPECULAR, lampLight_specular);

        // mau den duong sang
        glColor3f(1.0, 1.0, 0.7);
    }
    else {
        glColor3f(0.9, 0.9, 0.9);
    }

    glutSolidSphere(0.2, 16, 16);
    glPopMatrix();
}

void drawTrafficLight(float x, float y, float z, bool isRed, bool faceX) {
    glPushMatrix();
    glTranslatef(x, y + 1.5f, z);

    if (faceX) {
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f); 
    }

    // cot den
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.3f);
    glScalef(0.1f, 3.0f, 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();

    // hop den
    glPushMatrix();
    glTranslatef(0.0f, 1.85f, 0.0f);
    glColor3f(0.2f, 0.2f, 0.2f);
    glScalef(0.3f, 0.8f, 0.3f);
    glutSolidCube(1.0);
    glPopMatrix();

    // den do
    glPushMatrix();
    glTranslatef(0.0f, 1.7f, 0.15f);
    glColor3f(isRed ? 1.0f : 0.3f, 0.0f, 0.0f);
    glutSolidSphere(0.15f, 12, 12);
    glPopMatrix();

    // den xanh
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.15f);
    glColor3f(0.0f, isRed ? 0.3f : 1.0f, 0.0f);
    glutSolidSphere(0.15f, 12, 12);
    glPopMatrix();

    glPopMatrix();
}


// ve duong
void drawRoad(float x, float y, float z, float width, float length) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(0.2, 0.2, 0.2);
    glScalef(width, 0.1, length);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawRoadSystem() {
    drawRoad(0, 0, 0, 50, 3);

    drawRoad(7, 0, 0, 3, 35);

    glPushMatrix();
    glColor3f(0.3, 0.3, 0.3);
    glTranslatef(0, 0, 0);
    glScalef(3, 0.1, 3);
    glutSolidCube(1.0);
    glPopMatrix();
}

// ve xe buyt
void drawBus(float x, float y, float z) {
    // than xe
    glPushMatrix();
    glTranslatef(x + busPositionX, y + 0.5, z); // cap nhat vi tri cua bus
    glColor3f(1.0, 1.0, 0.0);
    glScalef(2.0, 1.0, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();

    // banh xe
    glColor3f(0.0, 0.0, 0.0);
    for (float i = -0.6; i <= 0.6; i += 1.2) {
        glPushMatrix();
        glTranslatef(x - 0.5 + busPositionX, y, z + i);
        glutSolidSphere(0.2, 16, 16);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(x + 0.5 + busPositionX, y, z + i);
        glutSolidSphere(0.2, 16, 16);
        glPopMatrix();
    }
}


// ve mat dat
void drawGround() {
    glPushMatrix();
    glTranslatef(0, -0.1, 0);
    glColor3f(0.66, 0.66, 0.66);
    glScalef(50.0, 0.1, 35.0);
    glutSolidCube(1.0);
    glPopMatrix();
}

// ve may
void drawCloud(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    glColor3f(1.0, 1.0, 1.0);

    glutSolidSphere(1.0, 16, 16);
    glTranslatef(1.2, 0.0, 0.0);
    glutSolidSphere(0.8, 16, 16);
    glTranslatef(-2.4, 0.0, 0.0);
    glutSolidSphere(0.8, 16, 16);

    glTranslatef(1.2, 0.0, 1.2);
    glutSolidSphere(0.7, 16, 16);

    glPopMatrix();
}

// ve mat troi
void drawSun(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0, 1.0, 0.0);
    glutSolidSphere(2.0, 50, 50);
    glPopMatrix();
}

// Them bien vi tri cho cac xe moi
float car1X = -15.0f;  // xe chay tren duong ngang
float car2X = 15.0f;   // xe chay tren duong ngang (nguoc chieu)
float car3Z = -15.0f;  // xe chay tren duong doc
float car4Z = 15.0f;   // xe chay tren duong doc (nguoc chieu)

// Toc do cac xe
float car1Speed = 0.05f;
float car2Speed = -0.05f;  // am de chay nguoc lai
float car3Speed = 0.05f;
float car4Speed = -0.05f;  // am de chay nguoc lai

// Ham ve xe voi mau sac tuy chinh
void drawCar(float x, float y, float z, float r, float g, float b, bool isVertical = false) {
    // than xe
    glPushMatrix();
    if (isVertical) {
        glTranslatef(x, y + 0.5, z);
        glRotatef(90, 0, 1, 0);  // xoay 90 do cho xe chay doc
    }
    else {
        glTranslatef(x, y + 0.5, z);
    }
    glColor3f(r, g, b);
    glScalef(1.5, 0.8, 0.8);
    glutSolidCube(1.0);
    glPopMatrix();

    // banh xe
    glColor3f(0.0, 0.0, 0.0);
    if (isVertical) {
        // banh xe cho xe chay doc
        for (float i = -0.4; i <= 0.4; i += 0.8) {
            glPushMatrix();
            glTranslatef(x + 0.4, y, z + i);
            glutSolidSphere(0.15, 12, 12);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(x - 0.4, y, z + i);
            glutSolidSphere(0.15, 12, 12);
            glPopMatrix();
        }
    }
    else {
        // banh xe cho xe chay ngang
        for (float i = -0.4; i <= 0.4; i += 0.8) {
            glPushMatrix();
            glTranslatef(x + i, y, z + 0.4);
            glutSolidSphere(0.15, 12, 12);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(x + i, y, z - 0.4);
            glutSolidSphere(0.15, 12, 12);
            glPopMatrix();
        }
    }
}

// Ham cap nhat vi tri cac xe moi
void updateCars(int value) {
    // cap nhat xe theo mau den giao thong
    if (!isXRed) { 
        car1X += car1Speed;
        if (car1X > 20.0f) car1X = -20.0f;

        car2X += car2Speed;
        if (car2X < -20.0f) car2X = 20.0f;
    }

    car3Z += isXRed ? car3Speed : 0.0f;
    if (car3Z > 15.0f) car3Z = -15.0f;

    car4Z += isXRed ? car4Speed : 0.0f;
    if (car4Z < -15.0f) car4Z = 15.0f;

    glutPostRedisplay();
    glutTimerFunc(16, updateCars, 0);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float cameraX = cameraDistance * sinf(cameraAngleY) * cosf(cameraAngleX);
    float cameraY = cameraDistance * sinf(cameraAngleX);
    float cameraZ = cameraDistance * cosf(cameraAngleY) * cosf(cameraAngleX);

    // camera
    gluLookAt(
        cameraDistance * sin(cameraAngleY) * cos(cameraAngleX),
        cameraDistance * sin(cameraAngleX),
        cameraDistance * cos(cameraAngleY) * cos(cameraAngleX),
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    );

    // ve cac vat the trong thanh pho
    drawGround();
    drawSun(20.0, 30.0, -17.0);
    drawRoadSystem();

    glColor3f(0.5, 0.5, 0.5);
    drawBuilding(-23, 0, -16, 2, 8, 2);
    drawBuilding(-20, 0, -16, 2, 6, 2);
    drawBuilding(-17, 0, -16, 3, 5, 2);
    drawBuilding(-14, 0, -16, 2, 4, 2);
    drawBuilding(-10, 0, -16, 2, 7, 2);
    drawBuilding(-6, 0, -16, 3, 8, 2);
    drawBuilding(-3, 0, -16, 2, 6, 2);
    drawBuilding(0, 0, -16, 2, 5, 2);
    drawBuilding(3, 0, -16, 2, 10, 2);
    drawBuilding(10, 0, -16, 2, 6, 2);
    drawBuilding(15, 0, -16, 2, 6, 2);
    drawBuilding(18, 0, -16, 2, 6, 2);
    drawBuilding(22, 0, -16, 2, 6, 2);

    if (isNight) {
        GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
        GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    }
    else {
        GLfloat mat_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
        GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    }

    drawTree(5, 0, 2);
    drawTree(0, 0, 2);
    drawTree(-5, 0, 2);
    drawTree(-10, 0, 2);
    drawTree(-15, 0, 2);
    drawTree(-20, 0, 2);
    drawTree(-25, 0, 2);
    drawTree(14, 0, 2);
    drawTree(19, 0, 2);
    drawTree(24, 0, 2);

    drawLampPost(0, 0, -2);
    drawLampPost(-5, 0, -2);
    drawLampPost(-10, 0, -2);
    drawLampPost(-15, 0, -2);
    drawLampPost(-20, 0, -2);
    drawLampPost(-25, 0, -2);
    drawLampPost(9, 0, -2);
    drawLampPost(14, 0, -2);
    drawLampPost(19, 0, -2);
    drawLampPost(24, 0, -2);

    drawBus(-4, 0, 0.6);

    if (showClouds) {
        for (int i = 0; i < 5; i++) {
            drawCloud(cloudPositions[i][0], cloudPositions[i][1], cloudPositions[i][2]);
        }
    }

    drawPlane(planeX1, planeY1, planeZ1, planeSpeed1);
    drawPlane(planeX2, planeY2, planeZ2, planeSpeed2);
    drawPlane(planeX3, planeY3, planeZ3, planeSpeed3);

    drawCar(car1X, 0, 0.6, 1.0, 0.0, 0.0);
    drawCar(car2X, 0, -0.6, 0.0, 1.0, 0.0);
    drawCar(6.25, 0, car3Z, 0.0, 0.0, 1.0, true);
    drawCar(7.75, 0, car4Z, 1.0, 0.5, 0.0, true);

    drawTrafficLight(trafficLight1X, 0, trafficLight1Z, !isXRed, false); 
    drawTrafficLight(trafficLight2X, 0, trafficLight2Z, isXRed, true);

    glutSwapBuffers();
}

// cap nhat vi tri cua bus (lam xe chay)
void updateBus(int value) {
    busPositionX += busSpeed; // cap nhat vi tri
    if (busPositionX > 20.0f) { // reset vi tri khi xe ra khoi khung canh
        busPositionX = -20.0f;
    }
    glutPostRedisplay(); // ve lai
    glutTimerFunc(16, updateBus, 0); // interval
}

// cap nhat vi tri cua dam may (lam may troi)
void updateClouds(int value) {
    for (int i = 0; i < 5; i++) {
        cloudPositions[i][0] += cloudSpeed;
        if (cloudPositions[i][0] > 25.0f) {
            cloudPositions[i][0] = -25.0f;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, updateClouds, 0);
}

// reshape
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

/// su kien chuot
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        isDragging = true;
        lastMouseX = x;
        lastMouseY = y;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        isDragging = false;
    }

    // su kien cuon chuot
    if (button == 3) { // cuon len
        cameraDistance -= 1.0f;
    }
    else if (button == 4) { // cuon xuong
        cameraDistance += 1.0f;
    }

    glutPostRedisplay();
}

// su kien keo tha chuot
void motion(int x, int y) {
    if (isDragging) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        // dieu chinh goc xoay chuot
        cameraAngleY -= dx * 0.005f; // goc xoay y
        cameraAngleX += dy * 0.005f; // goc xoay x

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Nam mo a di da phat");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutKeyboardFunc(keyboard);

    glutTimerFunc(16, updateBus, 0);
    glutTimerFunc(16, updateClouds, 0);
    glutTimerFunc(16, updatePlane, 0);
    glutTimerFunc(16, updateCars, 0);

    glutMainLoop();
    return 0;
}