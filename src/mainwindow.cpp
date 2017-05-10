#include "include/mainwindow.h"
#include "glwindow.h"
#include "ui_mainwindow.h"
#include <cstring>
#include <cstdio>
#include "log.h"
#include <QMessageBox>


float value_expr = 0.0f;
char *ptr = NULL;

struct Error {
    unsigned num_error;
    const char text_error[256];
};

#define NUM_ERROR 5

Error error_table[] =
{
    {1, "деление на ноль!"},
    {2, "ожидается ')'"},
    {3, "неизвестная функция"},
    {4, "нету аргументов"},
    {5, "ололо ошибка"}
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    program_help_dialog = new HelpDialog(this);
    gw = new Grafic_window(this);
    if(!init_log())
        MSG("log don't create\n");
    expr_text_changed = false;
    function_text_changed = false;
}

MainWindow::~MainWindow()
{
    destroy_log();
    delete ui;
    delete gw;
    delete program_help_dialog;
}

void MainWindow::on_about_program_action_triggered()
{
    QMessageBox::about(this, QString::fromUtf8("О программе"), QString::fromUtf8("Автор: Молокин m1lka Руслан\nВерсия: 0.6.1 beta"));
}

void MainWindow::on_program_help_action_triggered()
{
    program_help_dialog->show();
}

void MainWindow::on_expr_text_edit_cursorPositionChanged()
{
    if(!expr_text_changed) {
        expr_text_changed = true;
        ui->expr_text_edit->setText("");
    }
}

void del_zero_from_result(float value, char *str_result)
{
    char str_src[50], *s_pointer = str_src;
    bool found_dot = false;
    int j = 0 ,n = -1;
    sprintf(str_src, "%f", value);
    while(*s_pointer != '\0') {
        if(*s_pointer == '.') {
            found_dot = true;
        }
        if(found_dot) {
            n++;
            if(*s_pointer != '0') {
                j = n;
            }
        }
        *str_result = *s_pointer;
        str_result++;
        s_pointer++;
    }
    j = n - j;
    if(j > 0){
        if(j < n)
            *(str_result - j) = '\0';
        else
            *(str_result - j - 1) = '\0';
    } else {
        *str_result++ = '\0';
    }
}

void MainWindow::on_calc_expr_clicked()
{
    unsigned error = 0;
    char result_str[50];

    QColor color_error(255, 0, 0), color_normal(0, 0, 0);
    if(expr_text_changed) {
        if(!ui->expr_text_edit->toPlainText().isEmpty()) {
            error = P.parse_text_for_calc(ui->expr_text_edit->toPlainText().toStdString().c_str(), value_expr);
            if(error == 0) {
                ui->history_text->setTextColor(color_normal);
                del_zero_from_result(value_expr, result_str);
                ui->history_text->append(ui->expr_text_edit->toPlainText() + " = " + result_str);
            }
            else {
                for(int i = 0; i < NUM_ERROR; i++) {
                    if(error == error_table[i].num_error)
                    {
                        ui->history_text->setTextColor(color_error);
                        ui->history_text->append(QString::fromUtf8(error_table[i].text_error));
                        break;
                    }
                }
            }
        } else {
            ui->history_text->setTextColor(color_error);
            ui->history_text->append(QString::fromUtf8("выражение отсутствует"));
        }
    }
}

void MainWindow::on_build_grafic_clicked()
{
    if(function_text_changed) {
        if(!ui->function_text_edit->toPlainText().isEmpty()) {
            gw->set_limit_x_in_glwindow(ui->begin_x->value(), ui->end_x->value());
            printf("function() = %s\n", ui->function_text_edit->toPlainText().toStdString().c_str());
            gw->set_text_for_parse((char *)ui->function_text_edit->toPlainText().toStdString().c_str());
            gw->show();
        } else {
            close();
        }
    }
}

void MainWindow::on_function_text_edit_cursorPositionChanged()
{
    if(!function_text_changed) {
        function_text_changed = true;
        ui->function_text_edit->setText("");
    }
}
