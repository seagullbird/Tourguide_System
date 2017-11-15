#ifndef SMALL_FUNS_H
#define SMALL_FUNS_H
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include "database.h"
#include <QTime>
#include <QDate>
#include <QMainWindow>
using namespace std;

string int_to_string(long);

int get_time(int &, int &);

void clear_map_table(map_table &);

string time_convert(string origin, QDate init_date);
#endif // SMALL_FUNS_H
