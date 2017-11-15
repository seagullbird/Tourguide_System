#ifndef WELCOME_H
#define WELCOME_H

#include <QDialog>
#include <QMessageBox>
#include "sign_up.h"
#include "mainwindow.h"
#include "small_funs.h"
#include "user.h"
namespace Ui {
class Welcome;
}

class Welcome : public QDialog, public Database
{
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent = 0);
    ~Welcome();
    User *user;

signals:
    void Mode(QString mode);

private slots:
    void on_Visitor_Mode_clicked();

    void on_Sign_in_clicked();

    void on_Sign_up_clicked();

    void main_sign_out_clicked();

private:
    Ui::Welcome *ui;
    MainWindow main;
};

#endif // WELCOME_H
