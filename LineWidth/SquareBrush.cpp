#include<bits/stdc++.h>
#include<GL/glut.h>
#define width 2
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
vector<Point>Shell;
CrossOverPoint CopyQue[100];
vector<CrossOverPoint>Line[601];
priority_queue<CrossOverPoint>que;

void DrawPixel(int x, int y, int PointSize) {
    glEnable(GL_POINT_SMOOTH);
    glPointSize(PointSize);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void MouseHit(int button, int state, int x, int y) {

    if(button == 0 && state == 1) {
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
        printf("Clear screen!\n");
    }

}

Point Compare(Point m, Point n) {
    return m.y <= n.y ? m : n;
}

void Filling(vector<Point>pts) {
    Point temp;
    bool IsFill = false;
    int pointer, pre;
    int PointNum = pts.size(), start = 601, end = -601;
    for(int i = 0; i <= 600; ++i)
        Line[i].clear();
    while(que.size())
        que.pop();
    for(int i = 0; i < PointNum; ++i) {
        if(i != PointNum - 1) {
            temp = Compare(pts[i], pts[i + 1]);
            Line[temp.y + 300].push_back(CrossOverPoint(pts[i], pts[i + 1]));
        } else {
            temp = Compare(pts[i], pts[0]);
            Line[temp.y + 300].push_back(CrossOverPoint(pts[i], pts[0]));
        }
        start = min(pts[i].y, start);
        end = max(pts[i].y, end);
    }
    for(int i = start; i <= end; ++i) {
        for(auto cp : Line[i + 300]) {
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

void Display() {
    if(ePoint.size() > 0) {
        glColor3f(44.0 / 255.0f, 163.0 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(auto p : ePoint) {
            DrawPixel(p.x, p.y, 7);
        }
        if(ePoint.size() >= 2) {
            glColor3f(171.0 / 255.0f, 98.0 / 255.0f, 180.0/255.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            for(unsigned i = 0; i < ePoint.size(); ++i) {
                Shell.clear();
                if(i != ePoint.size() - 1) {
                    if(ePoint[i + 1].y - ePoint[i].y > ePoint[i + 1].x - ePoint[i].x) {
                        double k;
                        if(ePoint[i + 1].x == ePoint[i].x)
                            k = -0.75;
                        else 
                            k = 1.0 * (ePoint[i + 1].y - ePoint[i].y) / (ePoint[i + 1].x - ePoint[i].x);
                        if(k > 1)
                            k = 1;
                        Shell.push_back(Point(ePoint[i + 1].x - 2, ePoint[i + 1].y + (int)(2 * k + 0.5)));
                        Shell.push_back(Point(ePoint[i].x - 2, ePoint[i].y + (int)(2 * k + 0.5)));
                        Shell.push_back(Point(ePoint[i].x + 2, ePoint[i].y - (int)(2 * k + 0.5)));
                        Shell.push_back(Point(ePoint[i + 1].x + 2, ePoint[i + 1].y - (int)(2 * k + 0.5)));
                        Filling(Shell);
                    } else {
                        double k;
                        if(ePoint[i + 1].x == ePoint[i].x)
                            k = -0.75;
                        else 
                            k = 1.0 * (ePoint[i + 1].y - ePoint[i].y) / (ePoint[i + 1].x - ePoint[i].x);
                        if(k > 1)
                            k = 1;
                        Shell.push_back(Point(ePoint[i + 1].x - (int)(2 * k + 0.5), ePoint[i + 1].y + 2));
                        Shell.push_back(Point(ePoint[i].x - (int)(2 * k + 0.5), ePoint[i].y + 2));
                        Shell.push_back(Point(ePoint[i].x + (int)(2 * k + 0.5), ePoint[i].y - 2));
                        Shell.push_back(Point(ePoint[i + 1].x + (int)(2 * k + 0.5), ePoint[i + 1].y - 2));
                        Filling(Shell);
                    }
                }
            }
        }
    }
    glutSwapBuffers();
}

int main(int argc, char *argv[]) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Line");
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