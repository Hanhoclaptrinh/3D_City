/**
    Sub: Do Hoa May Tinh
    Group: CODE KHONG BUG
    Le Vinh Han - 2251068192
    Nguyen Thai Bao Duy - 2251068181
*/

#include <GL/glut.h>
#include <math.h>
#define M_PI 3.14159265358979323846

float cameraAngleX = 0.0; // goc quay quanh truc x
float cameraAngleY = 0.0; // goc quay quanh truc y
float cameraDistance = 30.0; // khoang cach tu camera den trung tam thanh pho
bool isDragging = false; // co trang thai
int lastMouseX, lastMouseY;
float cloudSpeed = 0.01; // toc do di chuyen cua may
bool isNight = false; // co kiem tra trang thai ngay dem
bool isXRed = false; // co trang thai den giao thong
bool showClouds = true; // trang thai may (khong hien thi may khi troi toi)
float lightRotation = 90.0f; // goc xoay cho den giao thong

/*----------Vi tri may----------*/
float cloudPositions[10][3] = {
    {15.0, 10.0, -10.0},
    {5.0, 10.0, 16.0},
    {10.0, 11.0, -5.0},
    {10.0, 17.0, 10.0},
    {0.0, 20.0, 15.0},
    {-10.0, 12.0, 15.0},
    {-15.0, 15.0, -6.0},
    {-15.0, 20.0, 0.0},
    {-15.0, 10.0, -7.0},
    {15.0, 15.0, -2.0}
};

struct Color {
    float r, g, b;
};

// bang mau cho cac toa nha
Color buildingColors[] = {
    {0.9, 0.7, 0.5},  
    {0.4, 0.8, 0.8},  
    {0.9, 0.3, 0.3},  
    {0.9, 0.4, 0.6},  
    {0.4, 0.6, 0.9},  
    {0.6, 0.4, 0.7},  
    {0.9, 0.7, 0.4},  
    {0.4, 0.9, 0.6},  
    {0.9, 0.5, 0.4}   
};

// su kien phim
void keyboard(unsigned char key, int x, int y) {
    if (key == 'n') {
        isNight = true;
        showClouds = false;
        glClearColor(0.15, 0.15, 0.15, 1.0);
        glDisable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
    }
    else if (key == 'd') {
        isNight = false;
        showClouds = true;
        glClearColor(0.5, 0.8, 1.0, 1.0);
        glEnable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
    }
    else if (key == 'r') {
        isXRed = true;
    }
    else if (key == 'g') {
        isXRed = false;
    }
    glutPostRedisplay();
}

// vi tri va huong cua may bay
float planeSpeed = 0.03; // toc do may bay

float planeX1 = -15.0;
float planeY1 = 15.0;
float planeZ1 = 8.0;

float planeX2 = -20.0;
float planeY2 = 12.0;
float planeZ2 = -10.0;

float planeX3 = -10.0;
float planeY3 = 13.0;
float planeZ3 = -2.0;

// than may bay
void drawPlaneBody() {
    glPushMatrix();
    glColor3f(0.9, 0.9, 0.9);
    glScalef(2.0, 0.5, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();
}

// canh may bay
void drawPlaneWings() {
    glPushMatrix();
    glColor3f(0.5, 0.8, 0.9);
    glScalef(1.0, 0.1, 3.0);
    glutSolidCube(1.0);
    glPopMatrix();
}

// Ve duoi may bay
void drawPlaneTail() {
    glPushMatrix();
    glColor3f(0.6, 0.6, 0.6);
    glTranslatef(-0.8, 0.3, 0.0);
    glScalef(0.5, 0.5, 0.8);
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
    planeX1 += planeSpeed;

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
    planeX1 += planeSpeed;
    planeX2 += planeSpeed;
    planeX3 += planeSpeed;

    // reset vi tri khi may bay ra khoi man hinh
    if (planeX1 > 20.0f) planeX1 = -20.0f;
    if (planeX2 > 20.0f) planeX2 = -20.0f;
    if (planeX3 > 20.0f) planeX3 = -20.0f;

    glutPostRedisplay();
    glutTimerFunc(16, updatePlane, 0);
}

// ve cua so
void drawWindow(float size) {
    if (isNight) {
        // thay doi mau cua so khi troi toi
        glColor3f(0.9f, 0.8f, 0.4f);
    }
    else {
        glColor3f(0.7f, 0.8f, 0.9f);
    }

    glPushMatrix();
    glScalef(size, size, 0.1);
    glutSolidCube(1.0);

    // khung cua so
    glColor3f(0.3f, 0.3f, 0.3f);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    // khung ngang
    glVertex3f(-0.5f, 0.0f, 0.1f);
    glVertex3f(0.5f, 0.0f, 0.1f);
    // khung doc
    glVertex3f(0.0f, -0.5f, 0.1f);
    glVertex3f(0.0f, 0.5f, 0.1f);
    glEnd();

    glPopMatrix();
}

// ve cua
void drawDoor(float width, float height) {
    glColor3f(0.4f, 0.2f, 0.1f);
    glPushMatrix();
    glScalef(width, height, 0.1);
    glutSolidCube(1.0);

    glColor3f(0.8f, 0.8f, 0.8f);
    glTranslatef(0.3f, 0.0f, 1.0f);
    glutSolidSphere(0.1, 8, 8);
    glPopMatrix();
}

/*----------Ve nha----------*/
void drawBuilding(float x, float y, float z, float width, float height, float depth, int colorIndex) {
    glPushMatrix();
    glTranslatef(x, y + height / 2, z);

    Color buildingColor = buildingColors[colorIndex % 8];
    glColor3f(buildingColor.r, buildingColor.g, buildingColor.b);
    glPushMatrix();
    glScalef(width, height, depth);
    glutSolidCube(1.0);
    glPopMatrix();

    int floors = (int)(height / 2.0);
    int windowsPerFloor = (int)(width / 1.2);
    float windowSize = 0.4;
    float windowSpacing = width / (windowsPerFloor + 1);
    float floorHeight = height / (floors + 1);

    glPushMatrix();
    glTranslatef(0, -height / 2 + 0.7, depth / 2 + 0.01);
    drawDoor(0.8, 1.4);
    glPopMatrix();

    // ve cua so cho moi tang
    for (int floor = 1; floor <= floors; floor++) {
        float y_pos = -height / 2 + floor * floorHeight;

        // cua so mat truoc
        for (int w = 0; w < windowsPerFloor; w++) {
            float x_pos = -width / 2 + (w + 1) * windowSpacing;

            glPushMatrix();
            glTranslatef(x_pos, y_pos, depth / 2 + 0.01);
            drawWindow(windowSize);
            glPopMatrix();

            // cua so mat sau
            glPushMatrix();
            glTranslatef(x_pos, y_pos, -depth / 2 - 0.01);
            drawWindow(windowSize);
            glPopMatrix();
        }

        // cua so hai ben
        if (depth > 1.5) {
            int sideWindows = (int)(depth / 1.2);
            float sideSpacing = depth / (sideWindows + 1);

            for (int w = 0; w < sideWindows; w++) {
                float z_pos = -depth / 2 + (w + 1) * sideSpacing;

                glPushMatrix();
                glTranslatef(-width / 2 - 0.01, y_pos, z_pos);
                glRotatef(90, 0, 1, 0);
                drawWindow(windowSize);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(width / 2 + 0.01, y_pos, z_pos);
                glRotatef(90, 0, 1, 0);
                drawWindow(windowSize);
                glPopMatrix();
            }
        }
    }
    glPopMatrix();
}

/*----------Ve cay----------*/
void drawRoundTree(float x, float y, float z) {
    // than cay
    glPushMatrix();
    glTranslatef(x, y + 0.5, z);
    glColor3f(0.55, 0.27, 0.07);
    glScalef(0.2, 1.0, 0.2);
    glutSolidCube(1.0);
    glPopMatrix();

    // tan cay (3 tang la)
    glColor3f(0.0, 0.8, 0.0);

    // tang duoi
    glPushMatrix();
    glTranslatef(x, y + 1.0, z);
    glutSolidSphere(0.6, 16, 16);
    glPopMatrix();

    // tang giua
    glPushMatrix();
    glTranslatef(x, y + 1.4, z);
    glutSolidSphere(0.5, 16, 16);
    glPopMatrix();

    // tang tren
    glPushMatrix();
    glTranslatef(x, y + 1.7, z);
    glutSolidSphere(0.3, 16, 16);
    glPopMatrix();
}

/*----------Ve cay----------*/
void drawTriangleTree(float x, float y, float z) {
    // than cay
    glPushMatrix();
    glTranslatef(x, y + 0.5, z);
    glColor3f(0.55, 0.27, 0.07);
    glScalef(0.2, 1.0, 0.2);
    glutSolidCube(1.0);
    glPopMatrix();

    glColor3f(0.0, 0.6, 0.0);

    // 3 tang la hinh tam giac
    float sizes[] = { 1.2, 0.9, 0.6 }; // kich thuoc cac tang
    float heights[] = { 1.0, 1.4, 1.8 }; // do cao cac tang

    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(x, y + heights[i], z);
        glRotatef(-90, 1, 0, 0);
        glutSolidCone(sizes[i] * 0.5, 0.4, 12, 12);
        glPopMatrix();
    }
}

/*----------Ve cay----------*/
void drawPineTree(float x, float y, float z) {
    // than cay
    glPushMatrix();
    glTranslatef(x, y + 0.7, z);
    glColor3f(0.45, 0.23, 0.07);
    glScalef(0.15, 1.4, 0.15);
    glutSolidCube(1.0);
    glPopMatrix();

    // la thong (4 tang)
    glColor3f(0.0, 0.4, 0.0);
    float baseSize = 0.8;
    float height = 0.5;

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(x, y + 1.0 + (i * height), z);
        glRotatef(-90, 1, 0, 0);
        glutSolidCone(baseSize - (i * 0.15), height + 0.1, 12, 12);
        glPopMatrix();
    }
}

/*----------Ve duong----------*/
void drawRoad(float x, float y, float z, float width, float length) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(0.2, 0.2, 0.2);
    glScalef(width, 0.1, length);
    glutSolidCube(1.0);
    glPopMatrix();
}

/*----------He thong duong----------*/
void drawRoadSystem() {
    drawRoad(0, 0.05, 7, 50, 3);

    drawRoad(0, 0.05, -7, 50, 3);

    drawRoad(0, 0.05, 0, 3, 35);

    glPushMatrix();
    glColor3f(0.3, 0.3, 0.3);
    glTranslatef(0, 0, 0);
    glScalef(3, 0.1, 3);
    glutSolidCube(1.0);
    glPopMatrix();
}

/*----------Vach ke duong----------*/
void drawRoadMarkings() {
    glColor3f(1.0, 1.0, 1.0);

    for (float x = -23.0f; x < 25.0f; x += 2.0f) {
        glPushMatrix();
        glTranslatef(x, 0.1, 7);
        glScalef(1.0, 0.01, 0.2);
        glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(x, 0.1, -7);
        glScalef(1.0, 0.01, 0.2);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    for (float z = -17.0f; z < 18.0f; z += 2.0f) {
        glPushMatrix();
        glTranslatef(0, 0.1, z);  
        glScalef(0.2, 0.01, 1.0);
        glutSolidCube(1.0);
        glPopMatrix();
    }
}

/*----------Ve den giao thong----------*/
void drawTrafficLight(float x, float y, float z, float rotation) {
    glPushMatrix();
    glTranslatef(x, y + 4, z);
    glRotatef(rotation, 0.0, 1.0, 0.0);

    // than den
    glPushMatrix();
    glColor3f(0.1, 0.1, 0.1);
    glRotatef(90, 1.0, 0.0, 0.0);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.15, 0.15, 4.0, 20, 20);
    gluDeleteQuadric(quad);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 1.0, 0.0);

    // bong den
    if (rotation == 90) {
        glColor3f(0.2f, !isXRed ? 1.0f : 0.2f, 0.2f); 
    }
    else { 
        glColor3f(0.2f, isXRed ? 1.0f : 0.2f, 0.2f); 
    }
    glutSolidSphere(0.5, 20, 20);

    glTranslatef(0.0, -1.0, 0.0);

    if (rotation == 90) { 
        glColor3f(isXRed ? 1.0f : 0.2f, 0.0, 0.0); 
    }
    else {
        glColor3f(!isXRed ? 1.0f : 0.2f, 0.0, 0.0); 
    }
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();

    // hop den
    glPushMatrix();
    glColor3f(0.2, 0.2, 0.2);
    glRotatef(90, 0, 1, 0);
    glScalef(0.5, 2, 1);
    glTranslatef(0, 0.25, 0);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
}


/*----------Ve nen----------*/
void drawGround() {
    glPushMatrix();
    glTranslatef(0, -0.1, 0);
    glColor3f(0.5, 0.5, 0.5);
    glScalef(50.0, 0.3, 35.0);
    glutSolidCube(1.0);
    glPopMatrix();
}

/*----------Ve may----------*/
void drawCustomCloud(float x, float y, float z, float size) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0, 1.0, 1.0);

    // hinh cau trung tam
    glutSolidSphere(size, 16, 16);

    // cac phan phu
    float offset = size * 1.2;
    float smallerSize = size * 0.8;

    glTranslatef(offset, 0.0, 0.0);
    glutSolidSphere(smallerSize, 16, 16);

    glTranslatef(-offset * 2, 0.0, 0.0);
    glutSolidSphere(smallerSize, 16, 16);

    glTranslatef(offset, 0.0, offset);
    glutSolidSphere(smallerSize * 0.9, 16, 16);

    glTranslatef(0.0, 0.0, -offset * 2);
    glutSolidSphere(smallerSize * 0.9, 16, 16);

    glPopMatrix();
}

/*----------Cap nhat vi tri may (lam may troi)----------*/
void updateClouds(int value) {
    for (int i = 0; i < 10; i++) {
        cloudPositions[i][0] += cloudSpeed;
        if (cloudPositions[i][0] > 25.0f) {
            cloudPositions[i][0] = -25.0f;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, updateClouds, 0);
}

/*----------Ve mat troi----------*/
void drawSun(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0, 1.0, 0.0);
    glutSolidSphere(2.0, 50, 50);
    glPopMatrix();
}

/*----------Mat trang----------*/
void drawMoon(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(0.95, 0.95, 0.95); 
    glutSolidSphere(2.0, 50, 50);
    glPopMatrix();
}

// vi tri cac xe
float car1X = -19.0f;  // xe chay tren duong ngang
float car2X = 19.0f;   // xe chay tren duong ngang (nguoc chieu)
float car3Z = -14.0f;  // xe chay tren duong doc
float car4Z = 16.5f;   // xe chay tren duong doc (nguoc chieu)
float car5X = -21.5f;
float car6X = 21.5f;
float car7Z = -16.5f;
float car8Z = 14.0f;
float car9X = 19.0f;
float car10X = 21.5f;
float car11X = -21.5f;
float car12X = -19.0f;

// toc do cac xe
float carSpeedPos = 0.05f; // toc do cho xe chay theo truc X
float carSpeedNeg = -0.05f; // toc do cho xe chay theo truc X (nguoc chieu)

// ve xe voi mau sac tuy chinh
void drawCar(float x, float y, float z, float r, float g, float b, bool isVertical = false) {
    // than xe
    glPushMatrix();
    if (isVertical) {
        glTranslatef(x, y + 0.5, z);
        glRotatef(90, 0, 1, 0);
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
void updateCars(int value) {
    // Xe trục X chạy khi đèn X xanh (isXRed = false)
    // Xe trục Z chạy khi đèn Z xanh (isXRed = true)

    // Cập nhật xe trục X
    if (!isXRed) { // Khi đèn X xanh thì xe X chạy
        car1X += carSpeedPos;
        if (car1X > 20.0f) car1X = -20.0f;

        car2X += carSpeedNeg;
        if (car2X < -20.0f) car2X = 20.0f;

        car5X += carSpeedPos;
        if (car5X > 20.0f) car5X = -20.0f;

        car6X += carSpeedNeg;
        if (car6X < -20.0f) car6X = 20.0f;

        car9X += carSpeedNeg;
        if (car9X < -20.0f) car9X = 20.0f;

        car10X += carSpeedNeg;
        if (car10X < -20.0f) car10X = 20.0f;

        car11X += carSpeedPos;
        if (car11X > 20.0f) car11X = -20.0f;

        car12X += carSpeedPos;
        if (car12X > 20.0f) car12X = -20.0f;
    }

    // Cập nhật xe trục Z 
    if (isXRed) { // Khi đèn Z xanh (X đỏ) thì xe Z chạy
        car3Z += carSpeedPos;
        if (car3Z > 15.0f) car3Z = -15.0f;

        car4Z += carSpeedNeg;
        if (car4Z < -15.0f) car4Z = 15.0f;

        car7Z += carSpeedPos;
        if (car7Z > 15.0f) car7Z = -15.0f;

        car8Z += carSpeedNeg;
        if (car8Z < -15.0f) car8Z = 15.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateCars, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float cameraX = cameraDistance * sinf(cameraAngleY) * cosf(cameraAngleX);
    float cameraY = cameraDistance * sinf(cameraAngleX);
    float cameraZ = cameraDistance * cosf(cameraAngleY) * cosf(cameraAngleX);

    /*----------Camera----------*/
    gluLookAt(
        cameraDistance * sin(cameraAngleY) * cos(cameraAngleX),
        cameraDistance * sin(cameraAngleX),
        cameraDistance * cos(cameraAngleY) * cos(cameraAngleX),
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    );

    drawGround();
    if (isNight) {
        drawMoon(20.0, 30.0, -17.0);
    }
    else {
        drawSun(20.0, 30.0, -17.0);
    }

    /*----------He thong duong----------*/
    drawRoadSystem();
    drawRoadMarkings();

    /*----------Nha----------*/
    drawBuilding(-23, 0, -16, 2, 8, 2, 0);
    drawBuilding(-20, 0, -16, 2, 6, 2, 1);
    drawBuilding(-17, 0, -16, 3, 5, 2, 2);
    drawBuilding(-14, 0, -16, 2, 4, 2, 9);
    drawBuilding(-10, 0, -16, 2, 7, 2, 4);
    drawBuilding(-6, 0, -16, 3, 8, 2, 5);
    drawBuilding(-3, 0, -16, 2, 6, 2, 6);
    drawBuilding(5, 0, -16, 5, 12, 2, 7);
    drawBuilding(9, 0, -16, 2, 10, 2, 0);
    drawBuilding(12, 0, -16, 2, 12, 2, 1);
    drawBuilding(15, 0, -16, 2, 11, 2, 2);
    drawBuilding(18, 0, -16, 2, 9, 2, 9);
    drawBuilding(22, 0, -16, 2, 15, 2, 4);

    glPushMatrix();
    glTranslatef(-20, 0, 15);
    glRotatef(180, 0, 1, 0);
    drawBuilding(0, 0, 12, 10, 10, 3, 3);
    drawBuilding(-7, 0, 12, 3, 8, 3, 2);
    drawBuilding(-11, 0, 12, 3, 17, 3, 7);
    drawBuilding(-15, 0, 12, 3, 13, 3, 5);
    drawBuilding(-25, 0, 12, 3, 11, 3, 4);
    drawBuilding(-29, 0, 12, 3, 15, 3, 3);
    drawBuilding(-33, 0, 12, 3, 7, 3, 1);
    drawBuilding(-38, 0, 12, 3, 6, 3, 2);
    drawBuilding(-43, 0, 12, 3, 9, 3, 8);
    glTranslatef(20, 0, -10);
    glPopMatrix();

    /*----------Cay truoc nha----------*/
    drawTriangleTree(-24, 0, -13.5);
    drawRoundTree(-21, 0, -13.5);
    drawTriangleTree(-18, 0, -13.5);
    drawPineTree(-15, 0, -13.5);
    drawTriangleTree(-12, 0, -13.5);
    drawRoundTree(-9, 0, -13.5);
    drawPineTree(-5, 0, -13.5);
    drawPineTree(-2, 0, -13.5);
    drawTriangleTree(3, 0, -13.5);
    drawTriangleTree(6, 0, -13.5);
    drawRoundTree(10, 0, -13.5);
    drawTriangleTree(13, 0, -13.5);
    drawPineTree(16, 0, -13.5);
    drawTriangleTree(18, 0, -13.5);
    drawRoundTree(20, 0, -13.5);
    drawTriangleTree(22, 0, -13.5);
    drawPineTree(24, 0, -13.5);

    drawTriangleTree(3, 0, 0);
    drawTriangleTree(6, 0, 0);
    drawRoundTree(10, 0, 0);
    drawTriangleTree(13, 0, 0);
    drawPineTree(16, 0, 0);
    drawTriangleTree(18, 0, 0);
    drawRoundTree(20, 0, 0);
    drawTriangleTree(22, 0, 0);
    drawPineTree(24, 0, 0);
    drawTriangleTree(-24, 0, 0);
    drawRoundTree(-21, 0, 0);
    drawTriangleTree(-18, 0, 0);
    drawPineTree(-15, 0, 0);
    drawTriangleTree(-12, 0, 0);
    drawRoundTree(-9, 0, 0);
    drawPineTree(-5, 0, 0);
    drawPineTree(-2, 0, 0);

    /*----------Xe----------*/
    drawCar(car1X, 0, 7.75, 1.0, 0.0, 0.0);
    drawCar(car2X, 0, -7.75, 0.0, 1.0, 0.0);
    drawCar(car5X, 0, 7.75, 0.5, 0.8, 0.0);
    drawCar(car6X, 0.0, -7.75, 1.0, 1.0, 1.0);
    drawCar(car9X, 0.0, 6.25, 0, 0, 0);
    drawCar(car10X, 0.0, 6.25, 1.0, 1.0, 1.0);
    drawCar(car11X, 0, -6.25, 0.5, 1.0, 0.6);
    drawCar(car12X, 0, -6.25, 0.6, 0.8, 0.9);
    drawCar(-0.75, 0, car3Z, 0.0, 0.0, 1.0, true);
    drawCar(0.7, 0, car4Z, 1.0, 0.5, 0.0, true);
    drawCar(-0.75, 0, car7Z, 0.0, 0.5, 0.0, true);
    drawCar(0.7, 0, car8Z, 0.8, 0.5, 0.9, true);


    /*----------May----------*/
    if (showClouds) {
        drawCustomCloud(cloudPositions[0][0], cloudPositions[0][1], cloudPositions[0][2], 1.2);
        drawCustomCloud(cloudPositions[1][0], cloudPositions[1][1], cloudPositions[1][2], 0.8);
        drawCustomCloud(cloudPositions[2][0], cloudPositions[2][1], cloudPositions[2][2], 1.0);
        drawCustomCloud(cloudPositions[3][0], cloudPositions[3][1], cloudPositions[3][2], 0.9);
        drawCustomCloud(cloudPositions[4][0], cloudPositions[4][1], cloudPositions[4][2], 1.1);
        drawCustomCloud(cloudPositions[5][0], cloudPositions[5][1], cloudPositions[5][2], 2.0);
        drawCustomCloud(cloudPositions[6][0], cloudPositions[6][1], cloudPositions[6][2], 1.0);
        drawCustomCloud(cloudPositions[7][0], cloudPositions[7][1], cloudPositions[7][2], 1.6);
        drawCustomCloud(cloudPositions[8][0], cloudPositions[8][1], cloudPositions[8][2], 0.7);
        drawCustomCloud(cloudPositions[9][0], cloudPositions[9][1], cloudPositions[9][2], 2.3);
    }

    /*----------May bay----------*/
    drawPlane(planeX1, planeY1, planeZ1, planeSpeed);
    drawPlane(planeX2, planeY2, planeZ2, planeSpeed);
    drawPlane(planeX3, planeY3, planeZ3, planeSpeed);

    /*----------Den giao thong----------*/
    drawTrafficLight(2, 0, 9, 0);
    drawTrafficLight(2, 0, -9, 0);
    drawTrafficLight(-2, 0, 9, 90);
    drawTrafficLight(-2, 0, -5, 90);

    glutSwapBuffers();
}

/*----------Thiet lap lai vung hien thi----------*/
void reshape(int w, int h) {
    glViewport(0, 0, w, h);                                     // thiet lap lai width & height
    glMatrixMode(GL_PROJECTION);                               // chuyen sang che do ma tran chieu
    glLoadIdentity();                                         // ve ma tran don vi
    gluPerspective(45, (double)w / (double)h, 1.0, 100.0);   // thiet lap chieu phoi canh
    glMatrixMode(GL_MODELVIEW);                             // chuyen lai che do ma tran mo hinh
}

/*----------Tao su kien chuot----------*/
void mouse(int button, int state, int x, int y) {
    // kiem tra chuot duoc click down & scroll
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        isDragging = true;
        lastMouseX = x;
        lastMouseY = y;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        isDragging = false;
    }

    /*
        scroll event
        btn == 3: scroll down
        btn == 3: scroll up
    */
    if (button == 3) {
        cameraDistance -= 1.0f;
    }
    else if (button == 4) { // cuon xuong
        cameraDistance += 1.0f;
    }

    glutPostRedisplay();
}

/*----------Su kien keo tha chuot----------*/
void motion(int x, int y) {
    // kiem tra trang thai chuot dang xoay
    if (isDragging) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        // dieu chinh goc xoay 
        cameraAngleY -= dx * 0.005f;
        cameraAngleX += dy * 0.005f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

/*----------Khoi tao man hinh----------*/
void init() {
    glEnable(GL_DEPTH_TEST); // kiem tra do sau
    glEnable(GL_LIGHTING);  // bat hieu ung chieu sang
    glEnable(GL_LIGHT0);   // anh sang chinh
    glEnable(GL_LIGHT1);  // anh sang phu (anh sang den duong vao ban dem)
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.5, 0.8, 1.0, 1.0);

    /*----------Thiet lap anh sang ban ngay----------*/
    GLfloat dayLight_position[] = { 12.0, 8.5, 0.0, 1.0 };   // vi tri nguon sang
    GLfloat dayLight_ambient[] = { 0.4, 0.4, 0.4, 1.0 };    // do sang moi truong ban ngay
    GLfloat dayLight_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };   // anh sang khuech tan
    GLfloat dayLight_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // anh sang phan xa
    glLightfv(GL_LIGHT0, GL_POSITION, dayLight_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, dayLight_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dayLight_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, dayLight_specular);

    /*----------Thiet lap anh sang ban dem----------*/
    GLfloat nightLight_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat nightLight_diffuse[] = { 0.3, 0.3, 0.4, 1.0 };
    GLfloat nightLight_specular[] = { 0.5, 0.5, 0.5, 1.0 };
    glLightfv(GL_LIGHT1, GL_AMBIENT, nightLight_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, nightLight_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, nightLight_specular);

    glEnable(GL_NORMALIZE);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("CODE KHONG BUG");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutKeyboardFunc(keyboard);

    glutTimerFunc(16, updateClouds, 0);
    glutTimerFunc(16, updatePlane, 0);
    glutTimerFunc(16, updateCars, 0);

    glutMainLoop();
    return 0;
}
