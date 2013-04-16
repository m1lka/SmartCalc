#ifndef GRAFIC_WINDOW_H
#define GRAFIC_WINDOW_H

#include <QDialog>
#include "include/grafic_window.h"
#include "include/glwindow.h"

namespace Ui {
class Grafic_window;
}

class Grafic_window : public QDialog
{
    Q_OBJECT
    
public:
    explicit Grafic_window(QWidget *parent = 0);
    ~Grafic_window();

    void set_limit_x_in_glwindow(float begin_x_user_mw, float end_x_user_mw);
    void set_text_for_parse(char *text);

private slots:
    void on_close_gwindow_clicked();

private:
    Ui::Grafic_window *ui;
    GLWindow *main_gl_window;
};

#endif // GRAFIC_WINDOW_H
