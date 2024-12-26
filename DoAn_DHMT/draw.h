#pragma once
#include <GL/glut.h>

struct Color {
    float r, g, b;
};
extern Color buildingColors[9];
extern float cloudPositions[10][3];

void drawPlaneBody();
void drawPlaneWings();
void drawPlaneTail();
void drawPropeller();
void drawPlane(float& planeX, float planeY, float planeZ, float planeSpeed);
void updatePlane(int value);
void drawWindow(float size);
void drawDoor(float width, float height);
void drawBuilding(float x, float y, float z, float width, float height, float depth, int colorIndex);
void drawRoundTree(float x, float y, float z);
void drawTriangleTree(float x, float y, float z);
void drawPineTree(float x, float y, float z);
void drawRoad(float x, float y, float z, float width, float length);
void drawRoadSystem();
void drawRoadMarkings();
void drawTrafficLight(float x, float y, float z, float rotation);
void drawGround();
void drawCustomCloud(float x, float y, float z, float size);
void updateClouds(int value);
void drawSun(float x, float y, float z);
void drawMoon(float x, float y, float z);
void drawCar(float x, float y, float z, float r, float g, float b, bool isVertical = false);
void updateCars(int value);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void reshape(int w, int h);
void drawRasterText(float x, float y, float z);