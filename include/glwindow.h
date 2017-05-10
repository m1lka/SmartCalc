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
        virtual void initializeGL();
        virtual void paintGL();
        virtual void resizeGL(int w, int h);
        virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif // GLWINDOW_H
