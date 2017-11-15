#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    onTravel = false;
    db = new Database;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete db;
    delete CThread;
}

void MainWindow::mode_received(QString mode)
{
    //Set Time Thread
    time = time.fromString(QString::fromStdString(db->getDateTime("TIME")), "hh:mm");
    date = date.fromString(QString::fromStdString(db->getDateTime("DATE")), "yyyy/MM/dd");
    init_date = init_date.fromString(QString::fromStdString((db->getInitDate())), "yyyy/MM/dd");
    ui->dateSet->setDateRange(init_date,init_date.addDays(7));
    CThread = new Clock_Thread(this);
    connect(CThread, SIGNAL(timer_signal()), this, SLOT(showtime()));
    CThread->start();

    //Set Date
    ui->Date->setText(date.toString("yyyy/MM/dd"));
    ui->dateSet->setDate(date);
    searcher = new Searcher(init_date);

    ui->label_9->hide();
    ui->date_time->hide();
    ui->tabWidget->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);
    this->mode = mode;
    if(mode == "Visitor")
    {
        ui->tabWidget->setTabEnabled(1, false);
        ui->sign_out->setText("Quit");
        ui->cur_account->setText("Visitor");
        ui->Del_account->hide();
        ui->change_pass->hide();
        ui->label_10->hide();
        ui->start_tour->hide();
        ui->cur_tour->hide();
    }
    if(mode == "User")
    {
        ui->tabWidget->setTabEnabled(1, true);
        ui->sign_out->setText("Sign Out");
        ui->cur_account->setText(QString::fromStdString(user->get_username()));
        ui->Del_account->show();
        ui->change_pass->show();
        ui->label_10->show();
        ui->start_tour->show();
        ui->cur_tour->hide();
        ui->seahi_tree->setColumnCount(6);
        ui->seahi_tree->setHeaderLabels(QStringList() << "DEPARTURE TIME" << "DEPARTURE CITY" << "METHOD"  << "METHOD NUMBER" << "ARRIVAL TIME" << "DESTINATION" << "TOTAL PRICE");
        ui->trahi_tree->setColumnCount(6);
        ui->trahi_tree->setHeaderLabels(QStringList() << "DEPARTURE TIME" << "DEPARTURE CITY" << "METHOD"  << "METHOD NUMBER" << "ARRIVAL TIME" << "DESTINATION" << "TOTAL PRICE");

        init_history("SEARCH");
        init_history("TRAVEL");
    }
}

void MainWindow::init_history(string type)
{
    map_table history;
    map_table *map_ptr = &history;
    db->open_database();
    string pre_sql = "SELECT * FROM user_" + user->get_username() + " WHERE STATUS = 'DONE' AND RECORD_TYPE = '" + type + "';";
    const char* sql = pre_sql.c_str();
    /* Execute SQL statement */
    db->rc = sqlite3_exec(db->db, sql, callback, (void*)map_ptr, &db->zErrMsg);
    if( db->rc != SQLITE_OK ){
        cout << sql << endl;
        fprintf(stderr, "SQL error: %s\n", db->zErrMsg);
        sqlite3_free(db->zErrMsg);
    }
    db->close_database();

    QTreeWidgetItem *itm;
    for(auto &x : history)
    {
        if(x.second["SEQ"] == "0")
        {
            itm = new QTreeWidgetItem(type == "SEARCH" ? ui->seahi_tree : ui->trahi_tree);
            itm->setText(0, QString::fromStdString(time_convert(x.second["DEPARTURE_TIME"], init_date)));
            itm->setText(1, QString::fromStdString(x.second["START"]));
            itm->setText(2, QString::fromStdString(x.second["METHOD"]));
            itm->setText(3, QString::fromStdString(x.second["NUMBER"]));
            itm->setText(4, QString::fromStdString(time_convert(x.second["ARRIVAL_TIME"], init_date)));
            itm->setText(5, QString::fromStdString(x.second["END"]));
            itm->setText(6, QString::fromStdString(x.second["TOTAL_PRICE"]));
            ui->seahi_tree->addTopLevelItem(itm);
        }
        else
        {
            QTreeWidgetItem *child = new QTreeWidgetItem(itm);
            child->setText(0, QString::fromStdString(time_convert(x.second["DEPARTURE_TIME"], init_date)));
            child->setText(1, QString::fromStdString(x.second["START"]));
            child->setText(2, QString::fromStdString(x.second["METHOD"]));
            child->setText(3, QString::fromStdString(x.second["NUMBER"]));
            child->setText(4, QString::fromStdString(time_convert(x.second["ARRIVAL_TIME"], init_date)));
            child->setText(5, QString::fromStdString(x.second["END"]));
            child->setText(6, QString::fromStdString(x.second["TOTAL_PRICE"]));
            itm->addChild(child);
        }
    }
    //delete map_ptr;
}

void MainWindow::showtime()
{
    int flag = time.hour() == 23 ? 1 : 0;
    time = time.addSecs(1);
    if(time.hour() == 0 && flag)
        date = date.addDays(1);

    ui->statusBar->showMessage(time.toString("hh : mm"));
    ui->Date->setText(date.toString("yyyy/MM/dd"));

    if(onTravel)
    {
        QString city_tour;
        QString color;
        int cur_time = 10000 * init_date.daysTo(date) + atoi(time.toString("hhmm").toStdString().c_str());
        int i;
        for(i = route_first_save; i <= (int)searcher->route.size(); ++i)
        {
            if(atoi(searcher->route[i]["DEPARTURE_TIME"].c_str()) <= cur_time && cur_time <= atoi(searcher->route[i]["ARRIVAL_TIME"].c_str()))
            {
                if(searcher->route[i]["METHOD"] == "FLIGHT") color = "blue";
                if(searcher->route[i]["METHOD"] == "TRAIN") color = "green";
                if(searcher->route[i]["METHOD"] == "CAR") color = "red";

                city_tour = QString::fromStdString(searcher->route[i]["START"]) + "<font color = " + color + ">➡️</font>" + QString::fromStdString(searcher->route[i]["END"]);
                ui->cur_city->setText(city_tour);
                ui->next->setText("Next: " + QString::fromStdString(searcher->route[i]["END"]));
                ui->at->setText("At: " + QString::fromStdString(time_convert(searcher->route[i]["ARRIVAL_TIME"], init_date)));
                route_first_save = i;
                break;
            }
            else if(cur_time < atoi(searcher->route[i]["DEPARTURE_TIME"].c_str()))
            {
                city_tour = QString::fromStdString(searcher->route[i]["START"]);
                ui->cur_city->setText(city_tour);
                ui->next->setText("Next: " + QString::fromStdString(searcher->route[i]["METHOD"]) + " " + QString::fromStdString(searcher->route[i]["NUMBER"]) + " to " + QString::fromStdString(searcher->route[i]["END"]));
                ui->at->setText("At: " + QString::fromStdString(time_convert(searcher->route[i]["DEPARTURE_TIME"], init_date)));
                route_first_save = i;
                break;
            }
        }
        if(i > (int)searcher->route.size())
        {
            //ARRIVED
            onTravel = false;
            ui->stackedWidget->setCurrentIndex(0);
            QMessageBox::information(this, "ARRIVED", "Congratulations! Your journey ends up here. Goodbye:) ");
            ui->cur_tour->hide();
            //renew db
            recorder->tour_done();
            add_histree(searcher->route, searcher->tw->total_price, "travel");
        }
    }
}

void MainWindow::on_All_clicked(bool checked)
{
    if(checked)
    {
        ui->Flight->setChecked(true);
        ui->Train->setChecked(true);
        ui->Bus->setChecked(true);
    }
    else
    {
        ui->Flight->setChecked(false);
        ui->Train->setChecked(false);
        ui->Bus->setChecked(false);
    }
}

void MainWindow::on_change_pass_clicked()
{
    Change_Pass pass_dia;
    connect(&pass_dia, SIGNAL(condition(bool)), this, SLOT(new_pass_condition(bool)));
    pass_dia.setWindowTitle("New Password");
    pass_dia.setModal(true);
    pass_dia.user = user;
    pass_dia.exec();
}

void MainWindow::new_pass_condition(bool condition)
{
    if(condition)
    {
        this->close();
        emit signed_out();
    }
}

void MainWindow::on_sign_out_clicked()
{
    if(onTravel)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Are you Sure?", "This will stop your current traveling issue.", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            recorder->stop_traveling();
            this->onTravel = false;
            ui->cur_city->setText("Canceled");
            ui->cur_tour->hide();
            ui->trahi_tree->clear();
            ui->seahi_tree->clear();
            this->hide();
            emit signed_out();
        }
    }
    else
    {
        cout << "closeEvent called" << endl;
        db->open_database();
        string last_date = date.toString("yyyy/MM/dd").toStdString();
        string last_time = time.toString("hh:mm").toStdString();
        db->exec_sql("UPDATE DATE_TIME SET DATE = '" + last_date + "', TIME = '" + last_time + "' WHERE ID = 1;");
        db->close_database();
        CThread->quit();
        this->hide();
        emit signed_out();
    }
}

/*On GO clicked*/
void MainWindow::on_go_clicked()
{
    if(ui->start_city->text().isEmpty() || ui->end_city->text().isEmpty() || (!ui->Flight->isChecked() && !ui->Bus->isChecked() && !ui->Train->isChecked() && !ui->All->isChecked()))
    {
        QMessageBox::information(this, "Error", "Information not complete!");
    }
    else
    {
        ui->result_Browser->clear();
        //Set all values
        searcher->set_city(ui->start_city->text().toStdString(), ui->end_city->text().toStdString());
        searcher->set_clock(date, time);
        searcher->set_strategy(ui->strategy->currentText().toStdString());
        if(ui->strategy->currentText() == "Mix")
            searcher->set_latest_a_time(ui->date_time->dateTime());
        if(ui->All->isChecked())
            searcher->set_method(true);
        else
            searcher->set_method(false, ui->Flight->isChecked(), ui->Train->isChecked(), ui->Bus->isChecked());

        ui->strategy->currentText() == "Mix" ? searcher->Strategy3() : searcher->simple_min_strategy();
        ui->stackedWidget->setCurrentIndex(1);
        if(!searcher->route.empty())
        {
            print_map_table(searcher->route);
            if(mode == "User")
            {
                temp_time_save = atoi(searcher->route[1]["DEPARTURE_TIME"].c_str());
                recorder->add_search_history(searcher->route, to_string(searcher->tw->total_price));
                add_histree(searcher->route, searcher->tw->total_price, "search");
            }
        }
        else
        {
            ui->result_Browser->append("<font size = 40 color = red>Can't find a proper result!</font>");
        }
    }
}

void MainWindow:: add_histree(map_table & route, int &total_price, string parent)
{
    QString departure_time = QString::fromStdString(time_convert(route.begin()->second["DEPARTURE_TIME"], init_date));
    QString start = QString::fromStdString(route.begin()->second["START"]);
    QString arrival_time = QString::fromStdString(time_convert(route[route.size()]["ARRIVAL_TIME"], init_date));
    QString end = QString::fromStdString(route[route.size()]["END"]);
    QString price = QString::number(total_price);
    AddRoot(parent, route, departure_time, start, arrival_time, end, price);
}

void MainWindow::AddRoot(string parent, map_table & route, QString departure_time, QString start, QString arrival_time, QString end, QString price)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(parent == "search" ? ui->seahi_tree : ui->trahi_tree);
    itm->setText(0, departure_time);
    itm->setText(1, start);
    itm->setText(2, "-");
    itm->setText(3, "-");
    itm->setText(4, arrival_time);
    itm->setText(5, end);
    itm->setText(6, price);
    ui->seahi_tree->addTopLevelItem(itm);
    for(auto &x : route)
    {
        QString departure_time = QString::fromStdString(time_convert(x.second["DEPARTURE_TIME"], init_date));
        QString start = QString::fromStdString(x.second["START"]);
        QString method = QString::fromStdString(x.second["METHOD"]);
        QString number = QString::fromStdString(x.second["NUMBER"]);
        QString arrival_time = QString::fromStdString(time_convert(x.second["ARRIVAL_TIME"], init_date));
        QString end = QString::fromStdString(x.second["END"]);
        QString price = QString::fromStdString(x.second["PRICE"]);
        AddChild(itm, departure_time, start, method, number, arrival_time, end, price);
    }
}

void MainWindow::AddChild(QTreeWidgetItem *parent, QString departure_time, QString start, QString method, QString number, QString arrival_time, QString end, QString price)
{
    QTreeWidgetItem *itm = new QTreeWidgetItem(parent);
    itm->setText(0, departure_time);
    itm->setText(1, start);
    itm->setText(2, method);
    itm->setText(3, number);
    itm->setText(4, arrival_time);
    itm->setText(5, end);
    itm->setText(6, price);
    parent->addChild(itm);
}

void MainWindow::on_back_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_Del_account_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Are you Sure?", "This will delete your account permanently.", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        user->del_account();
        this->close();
        emit signed_out();
    }
}

void MainWindow::on_strategy_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Mix")
    {
        ui->date_time->setDate(date.addDays(1));
        ui->date_time->setTime(time);
        ui->label_9->show();
        ui->date_time->show();
    }
    else
    {
        ui->label_9->hide();
        ui->date_time->hide();
    }

}

void MainWindow::on_Bus_clicked(bool checked)
{
    if(!checked)
        ui->All->setChecked(false);
    else if(ui->Train->isChecked() && ui->Flight->isChecked())
        ui->All->setChecked(true);
}

void MainWindow::on_Train_clicked(bool checked)
{
    if(!checked)
        ui->All->setChecked(false);
    else if(ui->Bus->isChecked() && ui->Flight->isChecked())
        ui->All->setChecked(true);
}

void MainWindow::on_Flight_clicked(bool checked)
{
    if(!checked)
        ui->All->setChecked(false);
    else if(ui->Train->isChecked() && ui->Bus->isChecked())
        ui->All->setChecked(true);
}

void MainWindow::print_map_table(map_table &table)
{
    QString color;
    QString departure_time, arrival_time;
    ui->result_Browser->append("<font size = 30>Final Result:</font><br><br>");

    for(auto &x : table)
    {
        if(x.second["METHOD"] == "FLIGHT") color = "blue";
        if(x.second["METHOD"] == "TRAIN") color = "green";
        if(x.second["METHOD"] == "CAR") color = "red";
        departure_time = QString::fromStdString(time_convert(x.second["DEPARTURE_TIME"], init_date));
        arrival_time = QString::fromStdString(time_convert(x.second["ARRIVAL_TIME"], init_date));

        ui->result_Browser->append(QString::number(x.first) + ". " + QString::fromStdString(x.second["START"]) + "(" + departure_time + ")" + "<font color = " + color + ">➡️</font>" + QString::fromStdString(x.second["END"]) + "(" + arrival_time + ")");
    }
    ui->result_Browser->append(QString::fromStdString("<br>Total time: " + to_string(searcher->tw->total_time/10000) + " days " + to_string(searcher->tw->total_time%10000/100) + " hours " + to_string(searcher->tw->total_time%100) + " mins."));
    ui->result_Browser->append(QString::fromStdString("Total price: ¥" + to_string(searcher->tw->total_price)));
    ui->result_Browser->append("<br><br><br>Flight: <font color = blue>➡</font>");
    ui->result_Browser->append("Train: <font color = green>➡</font>");
    ui->result_Browser->append("Bus: <font color = red>➡</font>");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    cout << "closeEvent called" << endl;
    if(onTravel)
    {
        recorder->stop_traveling();
        this->onTravel = false;
        ui->cur_city->setText("Canceled");
        ui->cur_tour->hide();
    }
    db->open_database();
    string last_date = date.toString("yyyy/MM/dd").toStdString();
    string last_time = time.toString("hh:mm").toStdString();
    db->exec_sql("UPDATE DATE_TIME SET DATE = '" + last_date + "', TIME = '" + last_time + "' WHERE ID = 1;");
    db->close_database();
    CThread->quit();

}

void MainWindow::on_clr_seahi_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Are you Sure?", "This will delete ALL your searching records.", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        recorder->clr_search_history();
        ui->seahi_tree->clear();
    }
}

void MainWindow::on_start_tour_clicked()
{
    if(!searcher->route.empty())
    {
        if(temp_time_save < time.toString("hhmm").toInt() + 10000 * init_date.daysTo(date))
            QMessageBox::information(this, "Error", "Too late! You've missed it. Search again:)");
        else if(db->exist_in_db("TRAVELING", "STATUS", "user_" + user->get_username()))
            QMessageBox::information(this, "Error", "You're already on a tour!");
        else
        {
            recorder->add_travel_history(searcher->route, to_string(searcher->tw->total_price));

            QMessageBox::information(this, "Congratulations!", "Your tour is arranged already. Safe journey!");
            route_first_save = 1;
            onTravel = true;
            ui->cur_tour->show();
            ui->stackedWidget->setCurrentIndex(2);
        }
    }
}

void MainWindow::on_clr_trahi_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Are you Sure?", "This will delete ALL your traveling records.", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {
        recorder->clr_travel_history();
        ui->trahi_tree->clear();
    }
}

void MainWindow::on_TD_set_button_clicked()
{
    date = ui->dateSet->date();
    time = ui->timeSet->time();
}

void MainWindow::on_back_to_main_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_cur_tour_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_stopTravel_clicked()
{
    if(onTravel)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Are you Sure?", "This will stop your current traveling issue.", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            recorder->stop_traveling();
            this->onTravel = false;
            ui->cur_city->setText("Canceled");
            ui->cur_tour->hide();
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(!onTravel)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Are you Sure?", "This will initialize system time and date to current time and date", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            time = time.currentTime();
            date = date.currentDate();
            ui->timeSet->setTime(time);
            ui->dateSet->setDate(date);
            db->open_database();
            db->exec_sql("UPDATE DATE_TIME SET DATE = '" + date.toString("yyyy/MM/dd").toStdString() + "', TIME = '" + time.toString("hh:mm").toStdString() + "' WHERE ID = 2;");
            db->close_database();
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Can't do this when you're on a tour.");
    }
}
