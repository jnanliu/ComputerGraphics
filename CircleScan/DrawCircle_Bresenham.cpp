#include<bits/stdc++.h>
#include<GL/glut.h>
using namespace std;

#define pointSize 3

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

void DrawPixelsCircle(int x, int y, int OffsetX, int OffsetY, int PointSize) {

    DrawPixel(x + OffsetX, y + OffsetY, PointSize); DrawPixel(x + OffsetX, -y + OffsetY, PointSize);
    DrawPixel(-x + OffsetX, y + OffsetY, PointSize); DrawPixel(-x + OffsetX, -y + OffsetY, PointSize);
}

void DrawCircle(vector<Point>pts) {
    int r = (int)sqrt(pow(pts[0].x - pts[1].x, 2) + pow(pts[0].y - pts[1].y, 2));
    int OffsetX = pts[0].x, OffsetY = pts[0].y;
    int k = 2 - 2 * r, i = 0, j = r;
    while(j >= 0) {
        DrawPixelsCircle(i, j, OffsetX, OffsetY, pointSize);
        if(k >= 0) {
            if(2 * k - 2 * i - 1 <= 0) {
                k = k + 2 * (i - j) + 6;
                i = i + 1;
                j = j - 1;
            } else {
                k = k + 3 - 2 * j;
                j = j - 1;
            }
        } else {
            if(2 * k + 2 * j - 1 >= 0) {
                k = k + 2 * (i - j) + 6;
                i = i + 1;
                j = j - 1;
            } else {
                k = k + 3 + 2 * i;
                i = i + 1;
            }
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
        if(ePoint.size() <= 2) {
            for(auto p : ePoint) {
                DrawPixel(p.x, p.y, 7);
            }
            if(ePoint.size() == 2) {
                glColor3f(171.0 / 255.0f, 98.0 / 255.0f, 180.0/255.0f);
                DrawCircle(ePoint);
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