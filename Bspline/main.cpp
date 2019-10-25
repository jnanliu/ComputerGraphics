#include<bits/stdc++.h>
#include<GL/glut.h>
using namespace std;
#define WindowHeight 600
#define WindowWidth 600

int K = 3;

struct Point {
    double x, y;
    Point() {};
    Point(double x_, double y_) {
        x = x_;
        y = y_;
    }
};

vector<Point>P;
Point C[71][71];

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
    glPointSize(point_size);
    glBegin(GL_LINES);
    glVertex2d(x0, y0);
    glVertex2d(x1, y1);
    glEnd();
}

double getResultx(int j, int k, double t, vector<Point>pts) {
    if(k == 0)
        return pts[j].x;
    double alpha = (t - j) / (K + 1 - k);
    return alpha * getResultx(j, k - 1, t, pts) + (1 - alpha) * getResultx(j - 1, k - 1, t, pts);
}

double getResulty(int j, int k, double t, vector<Point>pts) {
    if(k == 0)
        return pts[j].y;
    double alpha = (t - j) / (K + 1 - k);
    return alpha * getResulty(j, k - 1, t, pts) + (1 - alpha) * getResulty(j - 1, k - 1, t, pts);
}

void DrawSpline(vector<Point>pts) {
    int k = K;
    int n = pts.size() - 1;
    double u = k, du = 0.002;
    for(int j = k; j <= n; ++j) {
        while(u < j + 1) {
                for(int r = 0; r <= k; ++r) {
                    for(int i = j - k + r; i <= j; ++i) {
                        if(r == 0) {
                            C[r][i] = pts[i];
                        } else {
                            if(k - r == -1) {
                                C[r][i].x = 0; 
                                C[r][i].y = 0;
                            } else {
                                double alpha = (u - i) / (1.0 * k - r + 1);
                                C[r][i].x = C[r - 1][i].x * alpha + 
                                    C[r - 1][i - 1].x * (1 - alpha);
                                C[r][i].y = C[r - 1][i].y * alpha + 
                                    C[r - 1][i - 1].y * (1 - alpha);
                            }
                        }
                    }
            }
            DrawPixel(C[k][j].x, C[k][j].y, 2);
            //DrawPixel(getResultx(j, k, u, pts), getResulty(j, k, u, pts), 3);
            u += du;
        }
    }
}

void MouseHit(int button, int state, int x, int y) {
    if(button == 0 and state == 1) {
        P.push_back(Point(x - 300, 300 - y));
        printf("Create a point(%d %d)\n", (int)P[P.size() - 1].x, (int)P[P.size() - 1].y);
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
        DrawPixel(P[P.size()-1].x, P[P.size() - 1].y, 15);
		glColor3f(171.0 / 255.0f, 98.0 / 255.0f, 180.0/255.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(P.size() < 70) {
            for(int i = 0; i < P.size(); i++)
				DrawPixel(P[i].x, P[i].y, 15);
            glColor3f(190.0 / 255.0f, 0.0 / 255.0f, 0.0/255.0f);
            for(unsigned i = 0; i < P.size() - 1; ++i)
                DrawLine(P[i].x, P[i].y, P[i + 1].x, P[i + 1].y, 3);
			glColor3f(44.0 / 255.0f, 163.0 / 255.0f, 1.0f);
            DrawSpline(P);
        }
    }
    glutSwapBuffers(); 
	glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WindowHeight, WindowWidth);
    glutCreateWindow("Bspline 2D");
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluOrtho2D(-300, 300, -300, 300);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutMouseFunc(MouseHit);
    glutDisplayFunc(Display);
    glutMainLoop();
    return 0;
}