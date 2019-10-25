#include<bits/stdc++.h>
#include<GL/glut.h>
#define check(x, y) (-delat_y * x + delat_x * y + c)
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

void MouseHit(int button, int state, int x, int y) {

    if(button == 0 && state == 1) {
        ePoint.push_back(Point(x - 300, 300 - y));
        printf("Select point: (%d %d)\n", x - 300, 300 - y);
    } 

    if(button == 2 and state == 1) {
        ePoint.clear();
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        glutPostRedisplay();
        printf("Clear screen!\n");
    }

}

void DrawLine(vector<Point>pts) {
    int pointSize = 3;
    Point start = pts[0], end = pts[1];
    int HChange = 0, VChange = 0;
    if(start.x > end.x)
        swap(start, end);
    if(start.y > end.y) {
        VChange = 1;
        end.y = 2 * start.y - end.y;
    }
    int a = start.y - end.y;
    int b = end.x - start.x;
    if(-a > b) {
        HChange = 2;
        swap(a, b);
        swap(start.x, start.y);
        swap(end.x, end.y);
    }
    int k = 2 * a + b;
    if(HChange)
        DrawPixel(start.y, start.x, pointSize);
    else 
        DrawPixel(start.x, start.y, pointSize);
    for(int i = start.x, j = start.y; i <= end.x; ++i) {
        if(k < 0 && !HChange) {
            j = j + 1;
            k = k + b + b;
        }
        if(k >= 0 && HChange) {
            j = j + 1;
            k = k + b + b;
        }
        k = k + a + a;
        switch (HChange | VChange) {
            case 0 :
                DrawPixel(i, j, pointSize);
                break;
            case 1 :
                DrawPixel(i, 2 * start.y - j, pointSize);
                break;
            case 2 :
                DrawPixel(j, i, pointSize);
                break;
            case 3 :
                DrawPixel(j, 2 * start.x - i, pointSize);
                break;
        }
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
                DrawLine(ePoint);
            }
        }
    }

}

int main(int argc, char *argv[]) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Line");
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