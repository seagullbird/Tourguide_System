#ifndef SEARCHER_H
#define SEARCHER_H
#include <QDateTime>
#include "database.h"
#include "small_funs.h"
#define INFI 99999999
//记得改总城市数量
#define city_number 14
#define UP_RATE 0.1
#define DW_RATE 0.001
typedef struct
{
    int total_time;
    int total_price;
}total_weight, *twptr;

class Searcher : public result_obtainer
{
public:
    twptr tw;
    map_table route;
    Searcher(QDate first_date);
    void set_city(string start, string end);
    void set_strategy(string strategy);
    void set_method(bool all, bool flight = true, bool train = true, bool car = true);
    void set_clock(QDate date, QTime time);
    void set_latest_a_time(QDateTime latime);
    void simple_min_strategy(double price_rate = 0, double time_rate = 0);
    void init(double &a, double &b, int line, int early);
    inline void UpFresh(double &a, double &b);
    inline void DownFresh(double &a, double &b);
    void reflash(double &a, double &b, int line, int current);
    void Strategy3();

private:
    string method;
    string strategy;
    string start_city;
    string end_city;
    int latest_a_time;
    QDate cur_date;
    QTime cur_time;
    QDate first_date;
};

#endif // SEARCHER_H
