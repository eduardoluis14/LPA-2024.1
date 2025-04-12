#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <GL/freeglut.h>

typedef struct {
    float x;
    float y;
} Ponto;

typedef struct {
    Ponto start;
    Ponto end;
} Segmento;

int largura, altura, quantidade_segmentos;
Segmento segmentos[10000];


bool ccw(Ponto A, Ponto B, Ponto C) {
    return (C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x);
}

bool intersect(Segmento s1, Segmento s2) {
    return (ccw(s1.start, s2.start, s2.end) != ccw(s1.end, s2.start, s2.end)) && (ccw(s1.start, s1.end, s2.start) != ccw(s1.start, s1.end, s2.end));
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);

    for (int i = 0; i < quantidade_segmentos; i++) {
        Ponto start, end;

        start.x = (float)(rand() % largura);
        start.y = (float)(rand() % altura);
        end.x = (float)(rand() % largura);
        end.y = (float)(rand() % altura);

        int intersecao = 0;
        Segmento current_segment = { start, end };
        for (int j = 0; j < i; j++) {
            if (intersect(current_segment, segmentos[j])) {
                intersecao = 1;
                break;
            }
        }

        if (!intersecao) {
            glVertex2f(start.x, start.y);
            glVertex2f(end.x, end.y);
            segmentos[i] = current_segment;
        }
    }

    glEnd();
    glFlush();

}

int main(int argc, char** argv) {
    largura = atoi(argv[1]);
    altura = atoi(argv[2]);
    quantidade_segmentos = atoi(argv[3]);

    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(largura, altura);
    glutCreateWindow("Segmentos Aleatorios");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, largura, 0.0, altura, -1.0, 1.0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}