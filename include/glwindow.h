#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QtOpenGL/QGLWidget>
#include "parser.h"

class GLWindow : public QGLWidget
{
        Q_OBJECT
        QGLFormat gl_format;
    public:
        GLWindow(QWidget *parent = 0);
        ~GLWindow();

        float begin_x, end_x;
        char *func;

        Parser P;

        void set_limit_x(float begin_x_user, float end_x_user);
        void set_text_func(char *text);

    protected:
        void initializeGL();
        void paintGL();
        void resizeGL(int w, int h);
        void mouseReleaseEvent(QMouseEvent *event);
};

#endif // GLWINDOW_H
