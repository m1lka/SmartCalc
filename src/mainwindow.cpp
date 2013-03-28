#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstring>
#include "log.h"

#define STRINGIFY(s) #s

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
    {5, "no expr"}
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    program_help_dialog = new HelpDialog(this);
    if(init_log())
        MSG("log don't create\n");
    expr_text_changed = false;
}

MainWindow::~MainWindow()
{
    destroy_log();
    delete ui;
    delete program_help_dialog;
}

void MainWindow::on_about_program_action_triggered()
{
    QMessageBox::about(this, QString::fromUtf8("О программе"), QString::fromUtf8("Автор: Молокин m1lka Руслан\nВерсия: 0.5 beta"));
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

void MainWindow::on_calc_expr_clicked()
{
    unsigned error = 0;
    QString value_expr_str;
    QColor color_error(255, 0, 0), color_normal(0, 0, 0);
    if(expr_text_changed) {
        if(!ui->expr_text_edit->toPlainText().isEmpty()) {
            error = P.parse_text(ui->expr_text_edit->toPlainText().toAscii().data(), value_expr);
            if(error == 0) {
                ui->history_text->setTextColor(color_normal);
                ui->history_text->append(ui->expr_text_edit->toPlainText() + " = " + value_expr_str.sprintf("%f", value_expr));
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

void MainWindow::on_expr_text_edit_textChanged()
{

}

void MainWindow::on_history_text_textChanged()
{

}

void MainWindow::on_calc_expr_clicked(bool checked)
{

}
