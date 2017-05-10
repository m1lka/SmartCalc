#include "include/grafic_window.h"
#include "ui_grafic_window.h"
#include <cstdio>
#include <cstring>

char text_function[256];

Grafic_window::Grafic_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Grafic_window)
{
    ui->setupUi(this);
    main_gl_window = new GLWindow(this);
    ui->vl->addWidget(main_gl_window);
}

Grafic_window::~Grafic_window()
{
    delete ui;
    delete main_gl_window;
}

void Grafic_window::on_close_gwindow_clicked()
{
    close();
}

void Grafic_window::set_limit_x_in_glwindow(float begin_x_user_mw, float end_x_user_mw)
{
    main_gl_window->set_limit_x(begin_x_user_mw, end_x_user_mw);
}

void Grafic_window::set_text_for_parse(char *text)
{
    strcpy(text_function, text);
    main_gl_window->set_text_func(text_function);
}
