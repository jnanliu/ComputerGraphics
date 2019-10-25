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
bool mark[WindowWidth][WindowHeight];
bool color[WindowWidth][WindowHeight];

void DrawPixel(int x, int y, int PointSize) {
    color[x + HalfWindowWidth][y + HalfWindowHeight] = true;
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

int dx[4] = {0, 0, 1, -1};
int dy[4] = {-1, 1, 0, 0};

void Fill(vector<Point>pts) {
    memset(mark, false, sizeof(mark));
    memset(color, false, sizeof(color));
    int max_ = -WindowWidth, mix = WindowWidth, may = -WindowHeight, miy = WindowHeight;
    for(unsigned i = 0; i < pts.size(); ++i) {
        if(i != pts.size() - 1) {
            MarkLine(pts[i], pts[i + 1]);
        } else {
            MarkLine(pts[i], pts[0]);
        }
        max_ = max(max_, pts[i].x - 1);
        mix = min(mix, pts[i].x + 1);
        may = max(may, pts[i].y - 1);
        miy = min(miy, pts[i].y + 1);
    }
    queue<Point>que;
    que.push(Point(pts[0]));
    color[pts[0].x + HalfWindowWidth][pts[0].y + HalfWindowHeight] = true;
    while(que.size()) { 
        Point temp = que.front();
        que.pop();
        DrawPixel(temp.x, temp.y, 1);
        for(int i = 0; i < 4; ++i) {
            int nx = temp.x + dx[i];
            int ny = temp.y + dy[i];
            if(mark[nx + HalfWindowWidth][ny + HalfWindowHeight] || color[nx + HalfWindowWidth][ny + HalfWindowHeight])
                continue;
            if(nx > max_ || nx < mix || ny > may || ny < miy) 
                continue;
            color[nx + HalfWindowWidth][ny + HalfWindowHeight] = true;
            que.push(Point(nx, ny));
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