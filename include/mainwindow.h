#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "helpdialog.h"
#include "parser.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    public:
        Parser P;
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_about_program_action_triggered();

        void on_program_help_action_triggered();

        void on_expr_text_edit_cursorPositionChanged();

        void on_calc_expr_clicked();

private:
        Ui::MainWindow *ui;
        HelpDialog *program_help_dialog;

        bool expr_text_changed;

};

#endif // MAINWINDOW_H
