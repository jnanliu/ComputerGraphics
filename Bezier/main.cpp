#include<bits/stdc++.h>
#include<GL/glut.h>
#include<GL/freeglut.h>

using namespace std;

#define WindowHeight 600
#define WindowWidth 600

struct Point {
    double x, y;
    Point() {};
    Point(double x_, double y_) {
        x = x_;
        y = y_;
    }
};

vector<Point>P;
Point temp_p[71][71];

double Fac(int n) {
    
    double result = 1;
    for(int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;

}

double Combinate(int n, int k) {

    if (k == 0)
		return 1;
	double result = 0;
	result = Fac(n) / (Fac(k)*(Fac(n - k)));
	return result;

}

void DrawPixel(double x, double y, int point_size)
{
    glViewport(0, 0, (GLsizei)WindowWidth, (GLsizei)WindowHeight);
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

    double t = 0;
    double deltax = 0, deltay = 0;
    double dt = 0.0002;
    int n = pts.size() - 1;

    while(t <= 1) {

        for(int i = 0; i <= n; ++i) {
            double coe = Combinate(n, i) * pow(t, i) * pow(1 - t, n - i);
            deltax += coe * pts[i].x;
            deltay += coe * pts[i].y;
        }

        DrawPixel(deltax, deltay, 3);
        t += dt;
        deltax = 0;
        deltay = 0;

    }

}

void MouseHit(int button, int state, int x, int y) {
    /* state == 1 mean button up
     state == 0 mean button down
     button == 0 mean left button
     button == 1 mean middle button
     button == 2 mean right button
     [x, y] is the location of mouse pointer */

    if(button == 0 && state == 1) {
        P.push_back(Point(x, y));
        printf("Creating a point(%d %d)\n", (int)P[P.size() - 1].x, (int)P[P.size() - 1].y);
    }

    if(button == 2 && state == 1) {
        P.clear();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glutSwapBuffers();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glutSwapBuffers();
        glutPostRedisplay();
        printf("Clear screen!\n"); 
    }

    glutSwapBuffers();
    glutPostRedisplay();
}

void ChangeSize(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100.0, 100.0, -100.0, 100.0, 1.0, -1.0);
}

void Display() {

	glViewport(0, 0, (GLsizei)WindowWidth, (GLsizei)WindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WindowWidth, WindowHeight, 0);

	if (P.size() > 0){
		DrawPixel(P[P.size()-1].x, P[P.size() - 1].y, 7);
		glColor3f(171.0 / 255.0f, 98.0 / 255.0f, 180.0/255.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (P.size() < 70 ) {
			for(int i = 0; i < P.size(); i++)
				DrawPixel(P[i].x, P[i].y, 7);
            glColor3f(190.0 / 255.0f, 0.0 / 255.0f, 0.0/255.0f);
            for(unsigned i = 0; i < P.size() - 1; ++i)
                DrawLine(P[i].x, P[i].y, P[i + 1].x, P[i + 1].y, 3);
			glColor3f(44.0 / 255.0f, 163.0 / 255.0f, 1.0f);
            DrawBezier(P);
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
    glutCreateWindow("Bezier 2D");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutMouseFunc(MouseHit);
    //glutReshapeFunc(ChangeSize);
    glutDisplayFunc(Display);
    glutMainLoop();
    return 0;

}