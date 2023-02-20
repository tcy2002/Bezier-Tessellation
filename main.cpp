#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "bezier/bezier.h"
#include "model/model.h"
#include "config/config.h"

bezierManager *bm;
ModelManager *mm;
bezierManager::selected_points *select;

int width = ORI_WIDTH;
int height = ORI_HEIGHT;

int mode = 0;
int rot = 0;

int lx = 0;
int ly = 0;

void myInit() {
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearDepth(1.0f);

    bm = new bezierManager();
    bm->upgrade(1.0f);
    mm = new ModelManager();
    mm->upgrade(1.0f);
    select = nullptr;
}

void myDelete() {
    delete bm;
    delete mm;
    delete select;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mode == 0)
        bm->render();
    else
        mm->render();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (w < 100) w = 100;
    if (h < 100) h = 100;
    width = w;
    height = h;
    glViewport(0, 0, w, h);

    auto aspect = (GLfloat)(1.0 * w / h);
    bm->upgrade(aspect);
    mm->upgrade(aspect);
    glutPostRedisplay();
}

Vector2f screenPos(int x, int y) {
    auto rx = (GLfloat)(2.0 * x / width - 1.0);
    auto ry = (GLfloat)(1.0 - 2.0 * y / height);
    if (width > height)
        rx *= (GLfloat)(1.0 * width / height);
    else
        ry /= (GLfloat)(1.0 * width / height);
    return {rx, ry};
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 1);
}

void mouse(int button, int state, int x, int y) {
    if (mode == 0) {
        if (button == 1 && state == 1) {
            bm->end();
            return;
        }

        if (x < width / 2)
            return;
        Vector2f v = screenPos(x, y);

        if (bm->getState() == ALL_DONE) {
            if (button == 0 && state == 0) {
                delete select;
                select = bm->findVertex(v);
            }
        } else if (button <= 2 && state == 1) {
            bm->addVertex(v);
            if (button == 2)
                bm->endCurve();
        }
    } else {
        if (button == 0 && state == 0) {
            rot = 1;
            lx = x;
            ly = y;
        } else if (button == 0 && state == 1) {
            rot = 0;
        } else if (button == 3 && state == 1) {
            mm->increase();
        } else if (button == 4 && state == 1) {
            mm->decrease();
        }
    }
}

void motion(int x, int y) {
    if (mode == 0) {
        if (x < width / 2)
            return;
        Vector2f v = screenPos(x, y);

        if (select != nullptr)
            for (const auto &vert: *select)
                bm->adjustVertex(vert, v);
    } else {
        if (rot == 1) {
            if (lx != x)
                mm->rotateHorizontal(x - lx);
            if (ly != y)
                mm->rotateVertical(y - ly);
            if (lx != x || ly != y)
                mm->rotate();
            lx = x;
            ly = y;
        }
    }
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'c':
            if (bm->getState() == ALL_DONE && mode == 0) {
                GLsizei size;
                Vector3f *curve;
                bm->to3dCurve(curve, size);
                mm->addRotationModel(curve, size);
                delete[] curve;
                mode = 1;
            }
            break;
        case 'p':
            if (mode == 1)
                mm->changeMode();
            break;
        case 't':
            if (mode == 1)
                mm->loadTexture();
            break;
        case 'n':
            if (mode == 1)
                mm->switchTexture();
            break;
        case 'l':
            if (mode == 0 && mm->loadRotationModel())
                mode = 1;
            break;
        case 'd':
            if (mode == 1)
                mm->downloadLastModel();
        default:
            break;
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowSize(ORI_WIDTH, ORI_HEIGHT);
    glutCreateWindow("hw2");

    glewInit();
    myInit();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 1);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    myDelete();

    return 0;
}
