#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstring>

#define STRINGIFY(s) #s

float value_expr = 0.0f;
char *ptr = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    program_help_dialog = new HelpDialog(this);

    expr_text_changed = false;
    P.parser_create();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete program_help_dialog;
}

void MainWindow::on_about_program_action_triggered()
{
    QMessageBox::about(this, QString::fromUtf8("О программе"), QString::fromUtf8("Автор: Молокин m1lka Руслан\nВерсия: 0.4.1"));
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
    QString value_expr_str;
    if(!ui->expr_text_edit->toPlainText().isEmpty()) {
        P.parse_text(ui->expr_text_edit->toPlainText().toAscii().data(), value_expr);
        ui->history_text->append(ui->expr_text_edit->toPlainText() + "=" + value_expr_str.sprintf("%f", value_expr));
    }
}

void MainWindow::on_expr_text_edit_textChanged()
{

}
