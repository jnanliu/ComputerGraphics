#include<bits/stdc++.h>
#include<GL/glut.h>
using namespace std;

struct Point {
    int x, y;
    Point() {};
    Point(int x_, int y_) : x(x_), y(y_) {};
    int operator * (const Point &m) const {
        return x * m.x + y * m.y;
    }
    Point operator - (const Point &m) const {
        return Point(x - m.x, y - m.y);
    }
};

vector<Point>ePoint; 
vector<Point>FramePoints;

void Crop(Point p1, Point p2, vector<Point>pts, double &tl, double &tr) {
    Point N, A; double t; tl = -1e9, tr = 1e9;
    for(unsigned i = 0; i < pts.size(); ++i) {
        if(i == pts.size() - 1)
            N = Point(pts[i].y - pts[0].y, pts[0].x - pts[i].x);
        else
            N = Point(pts[i].y - pts[i + 1].y, pts[i + 1].x - pts[i].x);
        A = pts[i];
        if(N * (Point(0, 0) - A) < 0) 
            N = Point(-N.x, -N.y);
        if(N * (p2 - p1) == 0)
            continue;
        if(N * (p2 - p1) > 0) {
            tl = max(tl, -1.0 * (N * (p1 - A)) / (N * (p2 - p1)));
        } else {
            tr = min(tr, -1.0 * (N * (p1 - A)) / (N * (p2 - p1)));
        }
    }
    tl = max(tl, 0.0); tr = min(tr, 1.0);
}

void DrawFrame() {
    glEnable(GL_POINT_SMOOTH);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2i(0, 150);
    glVertex2i(100, 75);
    glVertex2i(100, 75);
    glVertex2i(100, -75);
    glVertex2i(100, -75);
    glVertex2i(0, -150);
    glVertex2i(0, -150);
    glVertex2i(-100, -75);
    glVertex2i(-100, -75);
    glVertex2i(-100, 75);
    glVertex2i(-100, 75);
    glVertex2i(0, 150);
    glEnd();
}

void DrawPixel(int x, int y, int PointSize) {
    glEnable(GL_POINT_SMOOTH);
    glPointSize(PointSize);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void DrawLine(Point s, Point e) {
    glEnable(GL_POINT_SMOOTH);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2i(s.x, s.y);
    glVertex2i(e.x, e.y);
    glEnd();
}

void MouseHit(int button, int state, int x, int y) {
    if(button == 0 and state == 1) {
        ePoint.push_back(Point(x - 300, 300 - y));
        printf("Select point: (%d %d)\n", x - 300, 300 - y);
    }
    if(button == 2 and state == 1) {
        ePoint.clear();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glutSwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glutSwapBuffers();
        glutPostRedisplay();
    }
}

void Display() {
    glColor3f(44.0 / 255.0f, 163.0 / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawFrame();
    double tl, tr;
    if(ePoint.size() > 0) {
        glColor3f(44.0 / 255.0f, 1.0f, 163.0 / 255.0f);
        for(auto p : ePoint) 
            DrawPixel(p.x, p.y, 6);
        for(unsigned i = 0; i < ePoint.size(); i += 2) {
            if(i + 1 >= ePoint.size())
                break;
             Crop(ePoint[i], ePoint[i + 1], FramePoints, tl, tr);
             if(tl > tr) {
                 glColor3f(44.0 / 255.0f, 1.0f, 1.0f);
                 DrawLine(ePoint[i], ePoint[i + 1]);
             } else {
                 Point temp1 = ePoint[i + 1] - ePoint[i], temp2 = ePoint[i + 1] - ePoint[i];
                 temp1 = Point(temp1.x * tl + ePoint[i].x, temp1.y * tl + ePoint[i].y);
                 temp2 = Point(temp2.x * tr + ePoint[i].x, temp2.y * tr + ePoint[i].y);
                 DrawLine(ePoint[i], temp1);
                 glColor3f(1.0f, 0.0f, 0.0f);
                 DrawLine(temp1, temp2);
                 glColor3f(44.0 / 255.0f, 1.0f, 1.0f);
                 DrawLine(temp2, ePoint[i + 1]);
             }
        }
    }
    glutSwapBuffers();
}

int main(int argc, char *argv[]) {

    FramePoints.push_back(Point(0, 150)); FramePoints.push_back(Point(100, 75));
    FramePoints.push_back(Point(100, -75)); FramePoints.push_back(Point(0, -150));
    FramePoints.push_back(Point(-100, -75)); FramePoints.push_back(Point(-100, 75));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Crop");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-300, 300, -300, 300);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutDisplayFunc(Display);
    glutMouseFunc(MouseHit);
    glutMainLoop();
    return 0;
}