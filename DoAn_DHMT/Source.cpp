/**
    Sub: Do Hoa May Tinh
    Group: CODE KHONG BUG
    Le Vinh Han - 2251068192
    Nguyen Thai Bao Duy - ???
*/

#include <GL/glut.h>
#include <math.h>

float cameraAngleX = 0.0; // goc quay quanh truc x
float cameraAngleY = 0.0; // goc quay quanh truc y
float cameraDistance = 30.0; // khoang cach tu camera den trung tam thanh pho
bool isDragging = false; // co trang thai
int lastMouseX, lastMouseY;

// luu vi tri cac dam may (luu boi mang hai chi)
float cloudPositions[10][3] = {
    {15.0, 10.0, 5.0},
    {5.0, 7.0, -10.0},
    {10.0, 8.0, -5.0},
    {10.0, 17.0, 10.0},
    {0.0, 20.0, 15.0},
    {-10.0, 9.0, -10.0},
    {-15.0, 10.0, 4.0},
    {-15.0, 20.0, 0.0},
    {-15.0, 10.0, -7.0},
    {7.0, 15.0, -2.0}
};

float cloudSpeed = 0.008; // toc do di chuyen cua may

bool isNight = false; // co kiem tra trang thai ngay dem

bool isXRed = false; // co trang thai den giao thong
bool showClouds = true; // trang thai may (khong hien thi may khi troi toi)

// vi tri den giao thong
float trafficLight1X = 9.0;  // den truc Z
float trafficLight1Z = 2.0;

float trafficLight2X = 5.0;  // den truc X
float trafficLight2Z = -2.0;

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
        showClouds = false; // tat may khi troi toi
        glClearColor(0.15, 0.15, 0.15, 1.0);
        glDisable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
    }
    else if (key == 'd') {
        isNight = false;
        showClouds = true; // hien thi may khi troi sang
        glClearColor(0.5, 0.8, 1.0, 1.0);
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
    glClearColor(0.5, 0.8, 1.0, 1.0);

    // anh sang ban ngay
    GLfloat dayLight_position[] = { 0.0, 10.0, 0.0, 1.0 };
    GLfloat dayLight_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
    GLfloat dayLight_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat dayLight_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, dayLight_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, dayLight_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dayLight_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, dayLight_specular);

    // anh sang ban dem
    GLfloat nightLight_ambient[] = { 0.1, 0.1, 0.15, 1.0 };
    GLfloat nightLight_diffuse[] = { 0.3, 0.3, 0.4, 1.0 };
    GLfloat nightLight_specular[] = { 0.5, 0.5, 0.5, 1.0 };

    glLightfv(GL_LIGHT1, GL_AMBIENT, nightLight_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, nightLight_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, nightLight_specular);

    glEnable(GL_NORMALIZE);
}

// vi tri va huong cua may bay
float planeX1 = -15.0;
float planeY1 = 15.0;
float planeZ1 = 5.0;
float planeSpeed1 = 0.01;

float planeX2 = -20.0;
float planeY2 = 12.0;
float planeZ2 = -10.0;
float planeSpeed2 = 0.01;

float planeX3 = -10.0;
float planeY3 = 6.0;
float planeZ3 = -2.0;
float planeSpeed3 = 0.01;

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

// ve nha
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

// ve cay
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

// ve cay 
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

// ve cay thong
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

// ve den duong
void drawLampPost(float x, float y, float z) {
    // than den
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
        glColor3f(0.9, 0.8, 0.0);
    }
    else {
        glColor3f(0.9, 0.9, 0.9);
    }

    glutSolidSphere(0.2, 16, 16);
    glPopMatrix();
}

// den giao thong
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

// he thong duong (nga tu)
void drawRoadSystem() {
    drawRoad(0, 0.05, 0, 50, 3);

    drawRoad(7, 0.05, 0, 3, 35);

    glPushMatrix();
    glColor3f(0.3, 0.3, 0.3);
    glTranslatef(0, 0, 0);
    glScalef(3, 0.1, 3);
    glutSolidCube(1.0);
    glPopMatrix();
}

// vach ke duong
void drawRoadMarkings() {
    glColor3f(1.0, 1.0, 1.0);

    for (float x = -23.0f; x < 25.0f; x += 2.0f) {
        glPushMatrix();
        glTranslatef(x, 0.1, 0);
        glScalef(1.0, 0.01, 0.2);
        glutSolidCube(1.0);
        glPopMatrix();
    }

    for (float z = -17.0f; z < 18.0f; z += 2.0f) {
        glPushMatrix();
        glTranslatef(7.0, 0.1, z);  
        glScalef(0.2, 0.01, 1.0);
        glutSolidCube(1.0);
        glPopMatrix();
    }
}

// ve mat dat
void drawGround() {
    glPushMatrix();
    glTranslatef(0, -0.1, 0);
    glColor3f(0.5, 0.5, 0.5);
    glScalef(50.0, 0.3, 35.0);
    glutSolidCube(1.0);
    glPopMatrix();
}

// ve may
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

// ve mat troi
void drawSun(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0, 1.0, 0.0);
    glutSolidSphere(2.0, 50, 50);
    glPopMatrix();
}

// mat trang
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

// toc do cac xe
float car1Speed = 0.05f;
float car2Speed = -0.05f;  // am de chay huong nguoc lai
float car3Speed = 0.05f;
float car4Speed = -0.05f; 
float car5Speed = 0.05f;
float car6Speed = -0.05f;
float car7Speed = 0.05f;
float car8Speed = -0.05f;

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

// cap nhat vi tri cac xe 
void updateCars(int value) {
    // cap nhat xe theo mau den giao thong
    if (!isXRed) { 
        car1X += car1Speed;
        if (car1X > 20.0f) car1X = -20.0f;

        car2X += car2Speed;
        if (car2X < -20.0f) car2X = 20.0f;

        car5X += car5Speed;
        if (car5X > 20.0f) car5X = -20.0f;

        car6X += car6Speed;
        if (car6X < -20.0f) car6X = 20.0f;
    }

    car3Z += isXRed ? car3Speed : 0.0f;
    if (car3Z > 15.0f) car3Z = -15.0f;

    car4Z += isXRed ? car4Speed : 0.0f;
    if (car4Z < -15.0f) car4Z = 15.0f;

    car7Z += isXRed ? car7Speed : 0.0f;
    if (car7Z > 15.0f) car7Z = -15.0f;

    car8Z += isXRed ? car8Speed : 0.0f;
    if (car8Z < -15.0f) car8Z = 15.0f;

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
    if (isNight) {
        drawMoon(20.0, 30.0, -17.0);
    }
    else {
        drawSun(20.0, 30.0, -17.0);
    }
    drawRoadSystem();
    drawRoadMarkings();

    drawBuilding(-23, 0, -16, 2, 8, 2, 0);
    drawBuilding(-20, 0, -16, 2, 6, 2, 1);
    drawBuilding(-17, 0, -16, 3, 5, 2, 2);
    drawBuilding(-14, 0, -16, 2, 4, 2, 9);
    drawBuilding(-10, 0, -16, 2, 7, 2, 4);
    drawBuilding(-6, 0, -16, 3, 8, 2, 5);
    drawBuilding(-3, 0, -16, 2, 6, 2, 6);
    drawBuilding(0, 0, -16, 2, 5, 2, 7);
    drawBuilding(3, 0, -16, 2, 10, 2, 0);
    drawBuilding(10, 0, -16, 2, 6, 2, 1);
    drawBuilding(15, 0, -16, 2, 6, 2, 2);
    drawBuilding(18, 0, -16, 2, 6, 2, 9);
    drawBuilding(22, 0, -16, 2, 6, 2, 4);

    glPushMatrix();
    glTranslatef(-20, 0, 15); // di chuyen toi vi tri toa nha
    glRotatef(180, 0, 1, 0);   // xoay toa nha quanh Y
    glTranslatef(20, 0, -10); 
    drawBuilding(-20, 0, 10.5, 10, 15, 5, 3);
    glPopMatrix();

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

    drawRoundTree(5, 0, 2);
    drawTriangleTree(0, 0, 2);
    drawPineTree(-5, 0, 2);
    drawRoundTree(-10, 0, 2);
    drawTriangleTree(-15, 0, 2);
    drawPineTree(-20, 0, 2);
    drawRoundTree(-25, 0, 2);
    drawPineTree(14, 0, 2);
    drawTriangleTree(19, 0, 2);
    drawPineTree(24, 0, 2);
    drawPineTree(5, 0, -5);
    drawRoundTree(5, 0, -10);
    drawTriangleTree(5, 0, -15);
    drawPineTree(9, 0, 5);
    drawTriangleTree(9, 0, 10);
    drawRoundTree(9, 0, 15);

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
        drawCustomCloud(cloudPositions[9][0], cloudPositions[9][1], cloudPositions[9][2], 3.0);
    }

    drawPlane(planeX1, planeY1, planeZ1, planeSpeed1);
    drawPlane(planeX2, planeY2, planeZ2, planeSpeed2);
    drawPlane(planeX3, planeY3, planeZ3, planeSpeed3);

    drawCar(car1X, 0, 0.6, 1.0, 0.0, 0.0);
    drawCar(car2X, 0, -0.6, 0.0, 1.0, 0.0);
    drawCar(6.25, 0, car3Z, 0.0, 0.0, 1.0, true);
    drawCar(7.75, 0, car4Z, 1.0, 0.5, 0.0, true);
    drawCar(car5X, 0, 0.6, 0.5, 0.8, 0.0);
    drawCar(car6X, 0.0, -0.6, 1.0, 1.0, 1.0);
    drawCar(6.25, 0, car7Z, 0.0, 0.5, 0.0, true);
    drawCar(7.75, 0, car8Z, 0.8, 0.5, 0.9, true);

    drawTrafficLight(trafficLight1X, 0, trafficLight1Z, !isXRed, false); 
    drawTrafficLight(trafficLight2X, 0, trafficLight2Z, isXRed, true);

    glutSwapBuffers();
}

// cap nhat vi tri cua dam may (lam may troi)
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
