#ifndef HISTORY_H
#define HISTORY_H
#include "user.h"

class History : public User
{
public:
    History(string username);
    void add_search_history(map_table &route, string total_price);
    void clr_search_history();
    void add_travel_history(map_table &route, string total_price);
    void tour_done();
    void clr_travel_history();
    void stop_traveling();
private:
    int search_count;
    int travel_count;
    string username;
};

#endif // HISTORY_H
