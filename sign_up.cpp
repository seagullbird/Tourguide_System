#include "sign_up.h"
#include "ui_sign_up.h"

Sign_up::Sign_up(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sign_up)
{
    ui->setupUi(this);
}

Sign_up::~Sign_up()
{
    delete ui;
}

void Sign_up::on_buttonBox_accepted()
{
    if(!ui->username->text().isEmpty() && !ui->password->text().isEmpty() && !ui->ensurance->text().isEmpty() && ui->ensurance->text() == ui->password->text())
    {
        if(!exist_in_db(ui->username->text().toStdString(), "USERNAME" ,"USER"))
        {
            user->set_account(ui->username->text().toStdString(), ui->password->text().toStdString());
            //create a new user
            user->create_new_user();
            QMessageBox::information(this, "Congratulations", "Account Created Successfully!", "OK");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Username exist!", "OK");
        }
    }
}
