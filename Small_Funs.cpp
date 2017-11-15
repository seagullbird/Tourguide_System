//
//  Small_Funs.cpp
//  Tourguide_System
//
//  Created by Seagullbird on 16/3/17.
//  Copyright © 2016年 Seagullbird. All rights reserved.
//

#include "Small_Funs.h"

int get_time(int & end_time, int & start_time)
{
    //Get time
    int result_d = end_time / 10000 - start_time / 10000;
    end_time = end_time % 10000;
    start_time = start_time % 10000;

    int result;
    int end_hr = end_time / 100;
    int start_hr = start_time / 100;
    int end_min = end_time % 100;
    int start_min = start_time % 100;
    int result_min = 0;
    int result_hr = 0;


    result_hr = end_hr - start_hr;
    result_min = end_min - start_min;

    if (result_hr < 0)
        result_hr = 24 + result_hr;

    if (result_min < 0) {
        result_min = 60 + result_min;
        result_hr--;    //borrow
    }

    result = result_d * 10000 + result_hr * 100 + result_min;
    return result;
}



void clear_map_table(map_table &map)
{
    for(auto &x : map)
    {
        map_row empty;
        x.second.swap(empty);
        x.second.clear();
    }
    map_table empty_map;
    map.swap(empty_map);
    map.clear();

}

string time_convert(string origin, QDate init_date)
{
    int temp_origin = atoi(origin.c_str());
    QDate cur_date;
    cur_date = init_date.addDays(temp_origin / 10000);

    temp_origin %= 10000;
    string o_hour = to_string(temp_origin / 100);
    string o_min = to_string(temp_origin % 100);
    if(o_hour.length() < 2) o_hour = "0" + o_hour;
    if(o_min.length() < 2) o_min = "0" + o_min;
    string result = cur_date.toString("yyyy/MM/dd").toStdString() + " " + o_hour + ":" + o_min;
    return result;
}
