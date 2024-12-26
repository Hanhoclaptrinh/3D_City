#include "global.h"

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