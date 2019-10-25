#include<bits/stdc++.h>
#include<GL/glut.h>
using namespace std;

struct Point {
    int x, y;
    Point() {};
    Point(int x_, int y_) {
        x = x_;
        y = y_;
    }
};

vector<Point>ePoint;

void DrawPixel(int x, int y, int PointSize) {
    glEnable(GL_POINT_SMOOTH);
    glPointSize(PointSize);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void DrawPixelsEllipse(int x, int y, int OffsetX, int OffsetY, int PointSize) {

    DrawPixel(x + OffsetX, y + OffsetY, PointSize); DrawPixel(x + OffsetX, -y + OffsetY, PointSize);
    DrawPixel(-x + OffsetX, y + OffsetY, PointSize); DrawPixel(-x + OffsetX, -y + OffsetY, PointSize);
}

void DrawEllipse(vector<Point>pts) {
    int b = (int)sqrt(pow(pts[0].x - pts[1].x, 2) + pow(pts[0].y - pts[1].y, 2));
    int a = (int)sqrt(pow(pts[0].x - pts[2].x, 2) + pow(pts[0].y - pts[2].y, 2));
    if(a < b) 
        swap(a, b);
    int OffsetX = pts[0].x, OffsetY = pts[0].y;
    int k = 4 * b * b + a * a * (1 - 4 * b);
    int i = 0, j = b;
    while(a * a * (2 * j - 1) >= b * b * (2 * i + 2)) {
        DrawPixelsEllipse(i, j, OffsetX, OffsetY, 1);
        if(k >= 0) {
            k = k + b * b * (8 * i + 12) + a * a * (8 - 8 * j);
            ++i; --j;
        } else {
            k = k + b * b * (8 * i + 12);
            ++i;
        }
    }
    while(j >= 0) {
        DrawPixelsEllipse(i, j, OffsetX, OffsetY, 1);
        if(k < 0) {
            k = k + b * b * (8 * i + 8) + a * a * (12 - 8 * j);
            ++i; --j;
        } else {
            k = k + a * a * (12 - 8 * j);
            --j;
        }
    }
}

void MouseHit(int button, int state, int x, int y) {
    
    if(button == 0 && state == 1) {
        ePoint.push_back(Point(x - 300, 300 - y));
        printf("Create point: (%d %d)\n", x - 300, 300 - y);
    } 
    if(button == 2 && state == 1) {
        ePoint.clear();
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        printf("Clear screen!");
        glutPostRedisplay();
    }
}

void Display() {
    if(ePoint.size() > 0) {
        glColor3f(44.0 / 255.0f, 163.0 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        if(ePoint.size() <= 3) {
            for(auto p : ePoint) {
                DrawPixel(p.x, p.y, 7);
            }
            if(ePoint.size() == 3) {
                glColor3f(171.0 / 255.0f, 98.0 / 255.0f, 180.0/255.0f);
                DrawEllipse(ePoint);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Circle");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-300, 300, -300, 300);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(Display);
    glutMouseFunc(MouseHit);
    glutMainLoop();
    return 0;
}