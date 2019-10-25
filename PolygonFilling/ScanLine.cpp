#include<bits/stdc++.h>
#include<GL/glut.h>
#define WindowHeight 600
#define WindowWidth 600
using namespace std;

struct Point {
    int x, y;
    Point() {};
    Point(int x_, int y_) {
        x = x_;
        y = y_;
    }
};


struct CrossOverPoint {
    int x, y;
    double delta, flag;
    CrossOverPoint() {};
    CrossOverPoint(Point m, Point n) {
        if(m.y <= n.y) {
            x = m.x; y = n.y; delta = (m.x - n.x) * 1.0 / (m.y - n.y); flag = 0;
        } else {
            x = n.x; y = m.y; delta = (m.x - n.x) * 1.0 / (m.y - n.y); flag = 0;
        }
    };
    bool operator < (const CrossOverPoint m) const {
        return x > m.x;
    }
};  

vector<Point>ePoint;
CrossOverPoint CopyQue[100];
vector<CrossOverPoint>Line[WindowHeight + 1];
priority_queue<CrossOverPoint>que;

void DrawPixel(int x, int y, int PointSize) {
    glEnable(GL_POINT_SMOOTH);
    glPointSize(PointSize);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

Point Compare(Point m, Point n) {
    return m.y <= n.y ? m : n;
}

void Filling(vector<Point>pts) {
    Point temp;
    bool IsFill = false;
    int pointer, pre;
    int PointNum = pts.size(), start = WindowHeight + 1, end = -WindowHeight - 1;
    for(int i = 0; i <= WindowHeight; ++i)
        Line[i].clear();
    while(que.size())
        que.pop();
    for(int i = 0; i < PointNum; ++i) {
        if(i != PointNum - 1) {
            temp = Compare(pts[i], pts[i + 1]);
            Line[temp.y + WindowHeight / 2].push_back(CrossOverPoint(pts[i], pts[i + 1]));
        } else {
            temp = Compare(pts[i], pts[0]);
            Line[temp.y + WindowHeight / 2].push_back(CrossOverPoint(pts[i], pts[0]));
        }
        start = min(pts[i].y, start);
        end = max(pts[i].y, end);
    }
    for(int i = start; i <= end; ++i) {
        for(auto cp : Line[i + WindowHeight / 2]) {
            que.push(cp);
        }
        pointer = -1;
        IsFill = false;
        while(que.size()) {
            CrossOverPoint cp = que.top();
            que.pop();
            if(IsFill) {
                for(int j = pre; j < cp.x; ++j) {
                    DrawPixel(j, i, 1);
                }
            }
            CopyQue[++pointer] = cp;
            IsFill ^= true;
            pre = cp.x;
        }
        for(int j = 0; j <= pointer; ++j) {
            CopyQue[j].flag += CopyQue[j].delta;
            if(CopyQue[j].flag > 1) {
                
                CopyQue[j].x += (int)(CopyQue[j].flag + 0.5);
                CopyQue[j].flag -= (int)(CopyQue[j].flag + 0.5);
            } else if(CopyQue[j].flag < -1) {
                CopyQue[j].x += (int)(CopyQue[j].flag - 0.5);
                CopyQue[j].flag -= (int)(CopyQue[j].flag - 0.5);
            }
            if(i + 1 < CopyQue[j].y)
                que.push(CopyQue[j]);
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
            Filling(ePoint);
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