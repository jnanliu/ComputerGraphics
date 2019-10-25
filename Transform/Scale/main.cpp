#include<bits/stdc++.h>
#include<GL/glut.h>
#include<math.h>
using namespace std;

struct Point {
    double x, y;
    Point() {};
    Point(double x_, double y_) {
        x = x_;
        y = y_;
    }
};

struct Matrix {
    double mat [71][71];
    int m, n;
    Matrix() {};
    Matrix(int m_, int n_) {
        m = m_;
        n = n_;
    }
    Matrix operator * (const Matrix t) {
        Matrix ret = Matrix(this->m, t.n);
        for(int i = 0; i < ret.m; ++i) {
            for(int j = 0; j < ret.n; ++j) {
                ret.mat[i][j] = 0;
                for(int k = 0; k < t.m; ++k) {
                    ret.mat[i][j] += this->mat[i][k] * t.mat[k][j];
                }
            }
        }
        return ret;
    }
};

vector<Point>P;
Point temp_p[71][71];
Matrix Scale = Matrix(3, 3);
Matrix reScale = Matrix(3, 3);

void DrawPixel(double x, double y, int point_size)
{
    glEnable(GL_POINT_SMOOTH);
    glPointSize(point_size);
    glBegin(GL_POINTS);
    glVertex2d(x, y);
    glEnd();
}

void DrawLine(double x0, double y0, double x1, double y1, int point_size)
{
    glEnable(GL_POINT_SMOOTH);
    glLineWidth(point_size);
    glBegin(GL_LINES);
    glVertex2d(x0, y0);
    glVertex2d(x1, y1);
    glEnd();
}

void Move() {
    Matrix initial = Matrix(1, 3), ret;
    vector<Point>t;
    for(unsigned i = 0; i < P.size(); ++i) {
        initial.mat[0][0] = P[i].x;
        initial.mat[0][1] = P[i].y;
        initial.mat[0][2] = 1;
        ret = initial * Scale;
        Point tempStart = Point(ret.mat[0][0], ret.mat[0][1]);
        t.push_back(tempStart);
    }
    P.clear();
    for(auto s : t)
        P.push_back(s);
}

void DrawBezier(vector<Point>pts) {
    if(pts.size() <= 0) 
        return;

    if(pts.size() > 0) {
        double t = 0;
        double dt = 0.0002;

        while(t <= 1) {
            for(int i = 1; i < pts.size(); ++i) {
                if (i == 1) {
                    for(int j = 0; j < pts.size() - i; ++j) {
                        temp_p[i][j].x = (1 - t) * pts[j].x + t * pts[j + 1].x;
                        temp_p[i][j].y = (1 - t) * pts[j].y + t * pts[j + 1].y;
                    }
                } else {
                    for(int j = 0; j < pts.size() - i; ++j) {
                        temp_p[i][j].x = (1 - t) * temp_p[i - 1][j].x + t * temp_p[i - 1][j + 1].x;
                        temp_p[i][j].y = (1 - t) * temp_p[i - 1][j].y + t * temp_p[i - 1][j + 1].y;
                    }
                }
            }
            DrawPixel(temp_p[pts.size() - 1][0].x, temp_p[pts.size() - 1][0].y, 2);
            t += dt;
        }
        return;
    }
}

void reMove() {
    Matrix initial = Matrix(1, 3), ret;
    vector<Point>t;
    for(unsigned i = 0; i < P.size(); ++i) {
        initial.mat[0][0] = P[i].x;
        initial.mat[0][1] = P[i].y;
        initial.mat[0][2] = 1;
        ret = initial * reScale;
        Point tempStart = Point(ret.mat[0][0], ret.mat[0][1]);
        t.push_back(tempStart);
    }
    P.clear();
    for(auto s : t)
        P.push_back(s);
}

void MouseHit(int button, int state, int x, int y) {
    if(button == 0 and state == 1) {
        Move();
    } else if (button == 2 and state == 1) {
        P.clear();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glutSwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glutSwapBuffers();
        glutPostRedisplay();
        printf("Clear screen!\n");
    } else if(button == 1 and state == 1) {
        reMove();
    }
}

void Display() {
    if(P.size() > 0) {
        glColor3f(255.0 / 255.0f, 163.0 / 255.0f, 1.0f);
        for(auto p : P)
            DrawPixel(p.x, p.y, 6);
        glColor3f(171.0 / 255.0f, 98.0 / 255.0f, 180.0/255.0f);
        for(unsigned i = 0; i < P.size() - 1; ++i) {
            DrawLine(P[i].x, P[i].y, P[i + 1].x, P[i + 1].y, 3);
        }
        glColor3f(44.0 / 255.0f, 163.0 / 255.0f, 1.0f);
        DrawBezier(P);
    }
    glutSwapBuffers(); 
	glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    P.push_back(Point(-60.0, 90.0));
    P.push_back(Point(-60.0, 150.0));
    P.push_back(Point(-10.0, 150.0));
    Scale.mat[0][0] = 2; Scale.mat[0][1] = 0; Scale.mat[0][2] = 0;
    Scale.mat[1][0] = 0; Scale.mat[1][1] = 2; Scale.mat[1][2] = 0;
    Scale.mat[2][0] = 0; Scale.mat[2][1] = 0; Scale.mat[2][2] = 1;
    reScale.mat[0][0] = 0.5; reScale.mat[0][1] = 0; reScale.mat[0][2] = 0;
    reScale.mat[1][0] = 0; reScale.mat[1][1] = 0.5; reScale.mat[1][2] = 0;
    reScale.mat[2][0] = 0; reScale.mat[2][1] = 0; reScale.mat[2][2] = 1;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Transform");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-300, 300, -300, 300);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutMouseFunc(MouseHit);
    glutDisplayFunc(Display);
    glutMainLoop();
    return 0;
}