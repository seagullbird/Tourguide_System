#ifndef DATABASE_H
#define DATABASE_H
#include "sqlite3.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QMessageBox>
#include <map>
using namespace std;
typedef map<string, string> map_row;
typedef map<int, map_row> map_table;
int callback(void *, int, char **, char **);

class Database
{
public:
    Database();

    void open_database();
    void close_database();
    void exec_sql(string pre_sql);
    int getInt_exec_sql(string pre_sql);
    string getString_exec_sql(string pre_sql);
    bool exist_in_db(const string &name, const string &title, const string &table);
    string getDateTime(string title);
    string getInitDate();
    sqlite3 *db;
    int rc;
    char *zErrMsg = 0;
};

class result_obtainer : public Database
{
private:
    string pre_sql;
    string start;
    int earliest_time;
    string tool;
    string choice;
    string LAST_AT;
    double price_rate;
    double time_rate;


public:
    map_table Result_list;
    void set_value(string st, int e_t, string choice, string tl, string LAST_AT = "0", double price_rate = 0, double time_rate = 0);
    result_obtainer();
    void Get_Result();
};



#endif // DATABASE_H
