#include<bits/stdc++.h>
#include <GL/glut.h>

GLfloat ctrlpoints[4][4][3] = {
{{-3, 0, 4.0}, {-2, 0, 2.0}, {-1, 0, 0.0}, {0, 0, 2.0}},
{{-3, 1, 1.0}, {-2, 1, 3.0}, {-1, 1, 6.0}, {0, 1, -1.0}},
{{-3, 2, 4.0}, {-2, 2, 0.0}, {-1, 2, 3.0}, {0, 2, 4.0}},
{{-3, 3, 0.0}, {-2, 3, 0.0}, {-1, 3, 0.0}, {0, 3, 0.0}}
};

void display(void) {
    int i, j;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix ();
    glRotatef(85.0, 1.0, 1.0, 1.0);
    for (j = 0; j <= 20; j++)
    {
        glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 20; i++)
            glEvalCoord2f((GLfloat)i/20.0, (GLfloat)j/20.0);
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 20; i++)
            glEvalCoord2f((GLfloat)j/20.0, (GLfloat)i/20.0);
        glEnd();
    }
    glPopMatrix ();
    glFlush();
}
void init(void) {
    glClearColor (1.0, 1.0, 1.0, 0.0);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3); 
    glOrtho(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0); 
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}