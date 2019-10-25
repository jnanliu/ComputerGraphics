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
Matrix translation = Matrix(3, 3);

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
    initial.mat[0][0] = P[0].x;
    initial.mat[0][1] = P[0].y;
    initial.mat[0][2] = 1;
    ret = initial * translation;
    Point tempStart = Point(ret.mat[0][0], ret.mat[0][1]);
    initial.mat[0][0] = P[1].x;
    initial.mat[0][1] = P[1].y;
    initial.mat[0][2] = 1;
    ret = initial * translation;
    Point tempEnd = Point(ret.mat[0][0], ret.mat[0][1]);
    P.clear();
    P.push_back(tempStart);
    P.push_back(tempEnd);
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
    }
}

void Display() {
    if(P.size() > 0) {
        glColor3f(44.0 / 255.0f, 163.0 / 255.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        DrawLine(P[0].x, P[0].y, P[1].x, P[1].y, 3);
    }
    glutSwapBuffers(); 
	glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    P.push_back(Point(-100.0, -130.0));
    P.push_back(Point(130.0, 100.0));
    translation.mat[0][0] = 1;  translation.mat[0][1] = 0; translation.mat[0][2] = 0;
    translation.mat[1][0] = 0; translation.mat[1][1] = 1; translation.mat[1][2] = 0;
    translation.mat[2][0] = 0; translation.mat[2][1] = 10; translation.mat[2][2] = 1;
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