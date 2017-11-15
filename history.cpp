#include "history.h"

History::History(string username)
{
    this->username = username;

    open_database();
    search_count = getInt_exec_sql("SELECT MAX(ROUTE_ID) FROM user_" + username + " WHERE RECORD_TYPE = 'SEARCH';");
    travel_count = getInt_exec_sql("SELECT MAX(ROUTE_ID) FROM user_" + username + " WHERE RECORD_TYPE = 'TRAVEL';");
    close_database();
}

void History::add_search_history(map_table &route, string total_price)
{
    search_count++;
    open_database();
    exec_sql("INSERT INTO user_" + this->username + " VALUES("
                                                    "" + to_string(search_count) + ", "
                                                    "" + to_string(0) + ", "
                                                    "'" + route.begin()->second["DEPARTURE_TIME"] + "', "
                                                    "'" + route.begin()->second["START"] + "', '-', '-', "
                                                    "'" + route[route.size()]["ARRIVAL_TIME"] + "', "
                                                    "'" + route[route.size()]["END"] + "', "
                                                    "'" + total_price + "', "
                                                    "'DONE', "
                                                    "'SEARCH');");
    for(auto &x : route)
    {
        exec_sql("INSERT INTO user_" + this->username + " VALUES("
                                                  "" + to_string(search_count) + ", "
                                                  "" + to_string(x.first) + ", "
                                                  "'" + x.second["DEPARTURE_TIME"] + "', "
                                                  "'" + x.second["START"] + "', "
                                                  "'" + x.second["METHOD"] + "', "
                                                  "'" + x.second["NUMBER"] + "', "
                                                  "'" + x.second["ARRIVAL_TIME"] + "', "
                                                  "'" + x.second["END"] + "', "
                                                  "'" + x.second["PRICE"] + "', "
                                                  "'DONE', "
                                                  "'SEARCH');");
    }
    close_database();
}

void History::clr_search_history()
{
    search_count = 0;
    open_database();
    exec_sql("DELETE FROM user_" + this->username + " WHERE RECORD_TYPE = 'SEARCH';");
    close_database();
}

void History::add_travel_history(map_table &route, string total_price)
{
    travel_count++;
    open_database();
    exec_sql("INSERT INTO user_" + this->username + " VALUES("
                                                    "" + to_string(travel_count) + ", "
                                                    "" + to_string(0) + ", "
                                                    "'" + route.begin()->second["DEPARTURE_TIME"] + "', "
                                                    "'" + route.begin()->second["START"] + "', '-', '-', "
                                                    "'" + route[route.size()]["ARRIVAL_TIME"] + "', "
                                                    "'" + route[route.size()]["END"] + "', "
                                                    "'" + total_price + "', "
                                                    "'TRAVELING', "
                                                    "'TRAVEL');");
    for(auto &x : route)
    {
        exec_sql("INSERT INTO user_" + this->username + " VALUES("
                                                  "" + to_string(travel_count) + ", "
                                                  "" + to_string(x.first) + ", "
                                                  "'" + x.second["DEPARTURE_TIME"] + "', "
                                                  "'" + x.second["START"] + "', "
                                                  "'" + x.second["METHOD"] + "', "
                                                  "'" + x.second["NUMBER"] + "', "
                                                  "'" + x.second["ARRIVAL_TIME"] + "', "
                                                  "'" + x.second["END"] + "', "
                                                  "'" + x.second["PRICE"] + "', "
                                                  "'TRAVELING', "
                                                  "'TRAVEL');");
    }
    close_database();
}

void History::clr_travel_history()
{
    travel_count = 0;
    open_database();
    exec_sql("DELETE FROM user_" + this->username + " WHERE RECORD_TYPE = 'TRAVEL' AND STATUS != 'TRAVELING';");
    close_database();
}

void History::tour_done()
{
    open_database();
    exec_sql("UPDATE user_" + this->username + " SET STATUS = 'DONE' WHERE RECORD_TYPE = 'TRAVEL' AND ROUTE_ID = " + to_string(travel_count) + ";");
    close_database();
}

void History::stop_traveling()
{
    open_database();
    exec_sql("UPDATE user_" + this->username + " SET STATUS = 'CANCELED' WHERE RECORD_TYPE = 'TRAVEL' AND ROUTE_ID = " + to_string(travel_count) + ";");
    close_database();
}
