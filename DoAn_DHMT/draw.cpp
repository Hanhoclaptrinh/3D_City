#include "draw.h"
#include "global.h"
#include "raster_font.h"

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

/*----------Bang mau cho nha----------*/
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

/*----------Ve than may bay----------*/
void drawPlaneBody() {
    glPushMatrix();
    glColor3f(0.9, 0.9, 0.9);
    glScalef(2.0, 0.5, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();
}

/*----------Ve canh may bay----------*/
void drawPlaneWings() {
    glPushMatrix();
    glColor3f(0.5, 0.8, 0.9);
    glScalef(1.0, 0.1, 3.0);
    glutSolidCube(1.0);
    glPopMatrix();
}

/*----------Ve duoi may bay----------*/
void drawPlaneTail() {
    glPushMatrix();
    glColor3f(0.6, 0.6, 0.6);
    glTranslatef(-0.8, 0.3, 0.0);
    glScalef(0.5, 0.5, 0.8);
    glutSolidCube(1.0);
    glPopMatrix();
}

/*----------Ve canh quat may bay----------*/
void drawPropeller() {
    static float propAngle = 0.0f; // goc quay cua canh
    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);
    glRotatef(propAngle, 1.0f, 0.0f, 0.0f);

    // canh doc
    glColor3f(0.3f, 0.3f, 0.3f);
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

/*----------Ve may bay----------*/
void drawPlane(float planeX, float planeY, float planeZ, float planeSpeed) {
    glPushMatrix();

    // di chuyen may bay theo truc X
    glTranslatef(planeX, planeY, planeZ);
    drawPlaneBody();
    drawPlaneWings();
    drawPlaneTail();
    drawPropeller();

    glPopMatrix();
}

/*----------Cap nhat vi tri may bay----------*/
void updatePlane(int value) {
    // cap nhat vi tri cho ba may bay
    planeX1 += planeSpeed;
    planeX2 += planeSpeed;
    planeX3 += planeSpeed;

    // reset vi tri khi may bay ra khoi bien
    if (planeX1 > 20.0f) planeX1 = -20.0f;
    if (planeX2 > 20.0f) planeX2 = -20.0f;
    if (planeX3 > 20.0f) planeX3 = -20.0f;

    glutPostRedisplay(); // thuc hien ve lai
    glutTimerFunc(16, updatePlane, 0);
}

/*----------Ve cua so----------*/
void drawWindow(float size) {
    if (isNight) {
        glColor3f(0.9f, 0.8f, 0.4f); // thay doi mau cua so khi troi toi
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
    glVertex3f(-0.5f, 0.0f, 0.1f);
    glVertex3f(0.5f, 0.0f, 0.1f);
    glVertex3f(0.0f, -0.5f, 0.1f);
    glVertex3f(0.0f, 0.5f, 0.1f);
    glEnd();

    glPopMatrix();
}

/*----------Ve cua chinh----------*/
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

    Color buildingColor = buildingColors[colorIndex % 8]; // lay mau sac tu danh sach da duoc khoi tao (co 8 mau voi idx tu 0 - 7)
    glColor3f(buildingColor.r, buildingColor.g, buildingColor.b);
    glPushMatrix();
    glScalef(width, height, depth);
    glutSolidCube(1.0);
    glPopMatrix();

    int floors = (int)(height / 2.0);                       // so tang cua toa nha
    int windowsPerFloor = (int)(width / 1.2);              // so luong cua so cua moi tang
    float windowSize = 0.4;                               // kich thuoc cua so
    float windowSpacing = width / (windowsPerFloor + 1); // khoang cach giua cac cua so
    float floorHeight = height / (floors + 1);          // do cao cac tang

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
    float sizes[] = { 1.2, 0.9, 0.6 };    // kich thuoc cac tang
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

/*----------Cap nhat vi tri may----------*/
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
    glColor3f(0.75, 0.95, 0.0);
    glutSolidSphere(2.0, 50, 50);
    glPopMatrix();
}

/*----------Ve xe----------*/
void drawCar(float x, float y, float z, float r, float g, float b, bool isVertical) {
    // than xe
    glPushMatrix();
    if (isVertical) {
        glTranslatef(x, y + 0.6, z);
        glRotatef(90, 0, 1, 0);
    }
    else {
        glTranslatef(x, y + 0.6, z);
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
            glTranslatef(x + 0.3, y + 0.1, z + i);
            glutSolidSphere(0.15, 12, 12);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(x + 0.3, y + 0.1, z - i);
            glutSolidSphere(0.15, 12, 12);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(x - 0.3, y + 0.1, z + i);
            glutSolidSphere(0.15, 12, 12);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(x - 0.3, y + 0.1, z - i);
            glutSolidSphere(0.15, 12, 12);
            glPopMatrix();
        }
    }
    else {
        // banh xe cho xe chay ngang
        for (float i = -0.4; i <= 0.4; i += 0.8) {
            glPushMatrix();
            glTranslatef(x + i, y + 0.1, z + 0.3);
            glutSolidSphere(0.15, 12, 12);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(x - i, y + 0.1, z + 0.3);
            glutSolidSphere(0.15, 12, 12);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(x + i, y + 0.1, z - 0.3);
            glutSolidSphere(0.15, 12, 12);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(x - i, y + 0.1, z - 0.3);
            glutSolidSphere(0.15, 12, 12);
            glPopMatrix();
        }
    }
}

/*----------Cap nhat vi tri xe----------*/
void updateCars(int value) {
    // cap nhat xe chay tren truc X
    if (!isXRed) { // den xanh tren truc X
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

    // cap nhat xe truc Z
    if (isXRed) { // den xanh truc Z
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

/*----------Su kien phim----------*/
void keyboard(unsigned char key, int x, int y) {
    // chuyen trang thai ngay dem
    if (key == 'n') {
        isNight = true;
        showClouds = false;                   // tat may vao ban dem
        glClearColor(0.15, 0.15, 0.15, 1.0); // mau bau troi vao ban dem
        glDisable(GL_LIGHT0);               // tat nguon sang chinh
        glEnable(GL_LIGHT1);               // bat nguon sang phu
    }
    else if (key == 'd') {
        isNight = false;
        showClouds = true;                  // bat may neu troi sang
        glEnable(GL_LIGHT0);               // bat nguon sang chinh
        glDisable(GL_LIGHT1);             // tat nguon sang phu
        glClearColor(0.5, 0.8, 1.0, 1.0);// bau troi ban ngay
    }
    // chuyen doi trang thai den giao thong
    else if (key == 'r') {
        isXRed = true;
    }
    else if (key == 'g') {
        isXRed = false;
    }
    glutPostRedisplay();
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
        btn == 4: scroll up
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

/*----------Thiet lap lai vung hien thi----------*/
void reshape(int w, int h) {
    glViewport(0, 0, w, h);                                     // thiet lap lai width & height
    glMatrixMode(GL_PROJECTION);                               // chuyen sang che do ma tran chieu
    glLoadIdentity();                                         // ve ma tran don vi
    gluPerspective(45, (double)w / (double)h, 1.0, 100.0);   // thiet lap chieu phoi canh
    glMatrixMode(GL_MODELVIEW);                             // chuyen lai che do ma tran mo hinh
}

/*----------Name Raster----------*/
void drawRasterText(float x, float y, float z) {
    // H
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // nap du lieu pixel vao buffer (GL_UNPACK_ALIGNMENT)
    glBitmap(40, 40, 0, 0, 0, 0, H_char);

    // A
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    glBitmap(24, 24, -30, 0, 0, 0, A_char);

    // N
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    glBitmap(32, 32, -54, 10, 0, 0, N_char);

    // B
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBitmap(32, 64, -86, 10, 0, 0, B_char);

    // D
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBitmap(32, 64, -114, 10, 0, 0, D_char);

    // U
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBitmap(32, 64, -146, 10, 0, 0, U_char);

    // Y
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBitmap(32, 64, -178, 10, 0, 0, Y_char);
}