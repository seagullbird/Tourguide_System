#ifndef SIGN_UP_H
#define SIGN_UP_H

#include <QDialog>
#include "small_funs.h"
#include <QMessageBox>
#include "user.h"
#include "database.h"
namespace Ui {
class Sign_up;
}

class Sign_up : public QDialog , public Database
{
    Q_OBJECT

public:
    explicit Sign_up(QWidget *parent = 0);
    ~Sign_up();
    User *user;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Sign_up *ui;
};

#endif // SIGN_UP_H
