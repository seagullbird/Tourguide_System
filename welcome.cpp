#include "welcome.h"
#include "ui_welcome.h"

Welcome::Welcome(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Welcome)
{
    ui->setupUi(this);
    connect(this, SIGNAL(Mode(QString)), &main, SLOT(mode_received(QString)));
    connect(&main, SIGNAL(signed_out()), this, SLOT(main_sign_out_clicked()));
    user = new User;
    main.user = user;
}

Welcome::~Welcome()
{
    delete ui;
}

void Welcome::on_Visitor_Mode_clicked()
{
    this->close();
    emit Mode("Visitor");
    main.setWindowTitle("Tourguide System");
    main.show();
}

void Welcome::on_Sign_in_clicked()
{
    user->set_account(ui->username->text().toStdString(), ui->password->text().toStdString());

    if(ui->username->text().isEmpty())
        QMessageBox::warning(this, "Error", "Please input your username!", "OK" ,"Cancel");
    else if(ui->password->text().isEmpty())
        QMessageBox::warning(this, "Error", "Please input your password!", "OK" ,"Cancel");
    else if(!exist_in_db(ui->username->text().toStdString(), "USERNAME", "USER"))
        QMessageBox::warning(this, "Error", "Username doesn't exist!", "OK" ,"Cancel");
    else if(user->check_password(ui->username->text().toStdString()) != ui->password->text().toStdString())
        QMessageBox::warning(this, "Error", "Wrong password!", "OK" ,"Cancel");
    else
    {
        this->close();
        emit Mode("User");
        main.setWindowTitle("Tourguide System");
        //main.onTravel = user->exist_in_db("TRAVELING", "STATUS", "user_" + user->get_username()) ? true : false;
        main.recorder = new History(user->get_username());
        main.show();
    }
}

void Welcome::on_Sign_up_clicked()
{
    Sign_up sup;
    sup.setWindowTitle("Sign Up");
    sup.setModal(true);
    sup.user = user;
    sup.exec();
}

void Welcome::main_sign_out_clicked()
{
    this->exec();
}
