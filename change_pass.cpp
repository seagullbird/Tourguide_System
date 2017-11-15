#include "change_pass.h"
#include "ui_change_pass.h"

Change_Pass::Change_Pass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Change_Pass)
{
    ui->setupUi(this);
}

Change_Pass::~Change_Pass()
{
    delete ui;
}

void Change_Pass::on_buttonBox_accepted()
{
    if(!ui->newpass->text().isEmpty() && !ui->ensurance->text().isEmpty() && ui->ensurance->text() == ui->newpass->text())
    {
        user->new_password(ui->newpass->text().toStdString());
        emit condition(true);
    }
    else
    {
        emit condition(false);
        if(ui->newpass->text().isEmpty() || ui->ensurance->text().isEmpty())
        {

            QMessageBox::information(this, "Error", "Not complete infomation.");

        }
        else if(ui->newpass->text() != ui->ensurance->text())
            QMessageBox::information(this, "Error", "Ensurance not the same!");

    }
}
