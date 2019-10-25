#include<bits/stdc++.h>
#include<GL/glut.h>
#define WindowHeight 600
#define WindowWidth 600
#define HalfWindowHeight (WindowHeight / 2)
#define HalfWindowWidth (WindowWidth / 2)
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
int MarkCount[WindowHeight];
bool mark[WindowWidth][WindowHeight];

void DrawPixel(int x, int y, int PointSize) {
    glEnable(GL_POINT_SMOOTH);
    glPointSize(PointSize);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void MarkLine(Point start, Point end) {

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
    if(HChange) {
        mark[start.y + HalfWindowWidth][start.x + HalfWindowHeight] = true;
    }
    else {
        mark[start.x + HalfWindowWidth][start.y + HalfWindowHeight] = true;
    }
    bool IsMark = false; 
    for(int i = start.x, j = start.y; i <= end.x; ++i) {
        IsMark = false;
        if(k < 0 && !HChange) {
            j = j + 1;
            IsMark = true;
            k = k + b + b;
        }
        if(k >= 0 && HChange) {
            j = j + 1;
            k = k + b + b;
        }
        if(HChange)
            IsMark = true;
        k = k + a + a;
        if(!IsMark)
            continue;
        switch (HChange | VChange) {
            case 0 :
                mark[i + HalfWindowWidth][j + HalfWindowHeight] = true;
                break;
            case 1 :
                mark[i + HalfWindowWidth][2 * start.y - j + HalfWindowHeight] = true;
                break;
            case 2 :
                mark[j + HalfWindowWidth][i + HalfWindowHeight] = true;
                break;
            case 3 :
                mark[j + HalfWindowWidth][2 * start.x - i + HalfWindowHeight] = true;
                break;
        }
    }
}

void Fill(vector<Point>pts) {
    int start = WindowHeight + 1, end = -WindowHeight - 1;
    memset(mark, false, sizeof(mark));
    memset(MarkCount, 0, sizeof(MarkCount));
    for(unsigned i = 0; i < pts.size(); ++i) {
        if(i != pts.size() - 1) {
            MarkLine(pts[i], pts[i + 1]);
        } else {
            MarkLine(pts[i], pts[0]);
        }
        start = min(start, pts[i].y);
        end = max(end, pts[i].y);
    }
    for(int i = start; i <= end; ++i) { 
        for(int j = -HalfWindowWidth; j <= HalfWindowWidth; ++j) {
            if(mark[j + HalfWindowWidth][i + HalfWindowHeight])
                MarkCount[i + HalfWindowHeight] += 1;
        }
    }
    for(int i = start; i <= end; ++i) { 
        bool inside = false;
        int cnt = 0;
        for(int j = -HalfWindowWidth; j <= HalfWindowWidth; ++j) {
            if(mark[j + HalfWindowWidth][i + HalfWindowHeight] | (MarkCount[i + HalfWindowHeight] == 1 and inside))
                inside ^= true, ++cnt;
            if(cnt > 1 and cnt < MarkCount[i + HalfWindowHeight] and MarkCount[i + HalfWindowHeight] > 2)
                inside = true;
            if(inside)
                DrawPixel(j, i, 1);
        }
    }
}

void MouseHit(int button, int state, int x, int y) {
    
    if(button == 0 && state == 1) {
        ePoint.push_back(Point(x - WindowWidth / 2, WindowHeight / 2 - y));
        printf("Create point: (%d %d)\n", x - WindowWidth / 2, WindowHeight / 2 - y);
    } 
    if(button == 2 && state == 1) {
        ePoint.clear();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glutSwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glutSwapBuffers();
        printf("Clear screen!\n");
        glutPostRedisplay();
    }
}

void Display() {
    if(ePoint.size() >= 0) {
        glColor3f(44.0 / 255.0f, 163.0 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(auto p : ePoint) 
            DrawPixel(p.x, p.y, 7);
        if(ePoint.size() >= 3) {
            glColor3f(171.0 / 255.0f, 98.0 / 255.0f, 180.0/255.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Fill(ePoint);
        }
    }
    glutSwapBuffers(); 
}

int main(int argc, char *argv[]) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WindowHeight, WindowWidth);
    glutCreateWindow("Filling");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-(WindowWidth / 2), WindowWidth / 2, -(WindowHeight / 2), WindowHeight / 2);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutDisplayFunc(Display);
    glutMouseFunc(MouseHit);
    glutMainLoop();
    return 0;
}