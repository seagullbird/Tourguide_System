#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include "change_pass.h"
#include "clock_thread.h"
#include "database.h"
#include "user.h"
#include "searcher.h"
#include "history.h"
#include <QTreeWidgetItem>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Clock_Thread *CThread;
    QString mode;
    Database *db;
    User *user;
    History *recorder;
    bool onTravel;
    void print_map_table(map_table &);
    void add_histree(map_table &, int &total_price, string parent);
signals:
    void signed_out();
    void timeRate(int value);

private slots:
    void mode_received(QString mode);

    void showtime();

    void on_All_clicked(bool checked);

    void on_change_pass_clicked();

    void on_sign_out_clicked();

    void on_go_clicked();

    void on_back_clicked();

    void on_Del_account_clicked();

    void new_pass_condition(bool);

    void on_strategy_currentTextChanged(const QString &arg1);

    void on_Bus_clicked(bool checked);

    void on_Train_clicked(bool checked);

    void on_Flight_clicked(bool checked);

    void on_clr_seahi_clicked();

    void on_start_tour_clicked();

    void on_clr_trahi_clicked();

    void on_TD_set_button_clicked();

    void on_back_to_main_clicked();

    void on_cur_tour_clicked();

    void on_stopTravel_clicked();

    void on_pushButton_clicked();

private:
    int temp_time_save;
    int route_first_save;
    Ui::MainWindow *ui;
    QTime time;
    QDate date;
    QDate init_date;
    Searcher *searcher;
    void init_history(string type);
    void AddRoot(string parent, map_table &route, QString departure_time, QString start, QString arrival_time, QString end, QString price);
    void AddChild(QTreeWidgetItem *parent, QString departure_time, QString start, QString method, QString number, QString arrival_time, QString end, QString price);
protected:
    void closeEvent(QCloseEvent * event);
};

#endif // MAINWINDOW_H
