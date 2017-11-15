#ifndef CHANGE_PASS_H
#define CHANGE_PASS_H

#include <QDialog>
#include <QMessageBox>
#include "user.h"
namespace Ui {
class Change_Pass;
}

class Change_Pass : public QDialog
{
    Q_OBJECT

public:
    explicit Change_Pass(QWidget *parent = 0);
    ~Change_Pass();
    User *user;

signals:
    void condition(bool);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Change_Pass *ui;
};

#endif // CHANGE_PASS_H
