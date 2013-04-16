#include "glwindow.h"
#include <QtGui>
#include <GL/gl.h>
#include <cmath>
#include "include/parser.h"

float translate = 5.0f, value_y = 0.0f;
char text_function_grafic[256];

GLWindow::GLWindow(QWidget *parent) :
    QGLWidget(parent)
{
    gl_format.setDirectRendering(true);
    gl_format.setDoubleBuffer(true);
    gl_format.setSampleBuffers(true);
    gl_format.setSwapInterval(1);

    setFormat(gl_format);
    setFocusPolicy(Qt::StrongFocus);
    GLWindow::begin_x = 0.0;
    GLWindow::end_x = 0.0;
}

GLWindow::~GLWindow()
{

}

void GLWindow::initializeGL()
{
    GLfloat aspectRatio;
    glViewport(0, 0, width(), height());
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    aspectRatio = (GLfloat)width() / (GLfloat)height();
    if(width() <= height()) {
        glOrtho(-50.0, 50.0, -50/aspectRatio, 50/aspectRatio, 1.0, -1.0);
    } else {
        glOrtho(-50.0 * aspectRatio, 50.0 * aspectRatio, -50.0, 50.0, 1.0, -1.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWindow::paintGL()
{
    GLfloat begin_x = 0, end_x = 0, point_x1 = 0, point_x2 = 0, point_y1 = 0, point_y2 = 0, step = 0, x = 0;
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
        glVertex2f(-140, 0);
        glVertex2f(140, 0);

        glVertex2f(0, -99);
        glVertex2f(0, 99);
    glEnd();
    begin_x = GLWindow::begin_x;
    end_x = GLWindow::end_x;
    x = begin_x;
    point_x1 = begin_x * translate;
    //value_y = asinh(x);
    GLWindow::P.parse_text_for_grafic(text_function_grafic, x, value_y);
    //printf("func = %s", func_ololo);
    point_y1 = value_y * translate;
    step = 0.3;
    glBegin(GL_LINES);
    while(x <= end_x) {
        x += step;
        point_x2 = x * translate;
        //value_y = asinh(x);
        GLWindow::P.parse_text_for_grafic(text_function_grafic, x, value_y);
        point_y2 = value_y * translate;
        glVertex2f(point_x1, point_y1);
        glVertex2f(point_x2, point_y2);
        point_x1 = point_x2;
        point_y1 = point_y2;
    }
    glEnd();
    glFlush();
}

void GLWindow::resizeGL(int w, int h)
{
    GLfloat aspectRatio;
    glViewport(0, 0, width(), height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    aspectRatio = (GLfloat)w / (GLfloat)h;
    if(width() <= height()) {
        glOrtho(-100.0, 100.0, -100/aspectRatio, 100/aspectRatio, 1.0, -1.0);
    } else {
        glOrtho(-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100.0, 1.0, -1.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWindow::mouseReleaseEvent(QMouseEvent *event)
{
    switch(event->button()) {
    case Qt::LeftButton: translate++; break;
    case Qt::RightButton: translate--; break;
    default: break;
    };
    if(translate > 7)
        translate--;
    else if(translate < 2)
        translate++;
    updateGL();
}

void GLWindow::set_limit_x(float begin_x_user, float end_x_user)
{
    GLWindow::begin_x = begin_x_user;
    GLWindow::end_x = end_x_user;
}

void GLWindow::set_text_func(char *text)
{
    printf("g = %s\n", text);
    strcpy(text_function_grafic, text);
}
