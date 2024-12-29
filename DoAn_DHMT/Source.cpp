/*
    Do Hoa May Tinh
    CODE KHONG BUG
    Le Vinh Han - 2251068192
    Nguyen Thai Bao Duy - 2251068181
*/

#include <GL/glut.h>
#include <math.h>
#include "raster_font.h"
#include "global.h"
#include "draw.h"

/*----------Ve cac vat the len man hinh----------*/
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

    /*----------Name----------*/
    drawRasterText(0, 0, 0);

    glutSwapBuffers();
}

/*----------Khoi tao man hinh----------*/
void init() {
    glEnable(GL_DEPTH_TEST); // kiem tra do sau
    glEnable(GL_LIGHTING);  // bat hieu ung chieu sang
    glEnable(GL_LIGHT0);   // anh sang chinh
    glEnable(GL_LIGHT1);  // anh sang phu 
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