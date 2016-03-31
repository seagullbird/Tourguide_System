//
//  Database_Connection.hpp
//  Tourguide_System
//
//  Created by Seagullbird on 16/3/17.
//  Copyright © 2016年 Seagullbird. All rights reserved.
//

#ifndef Database_Connection_hpp
#define Database_Connection_hpp
#define NUMBER_OF_INFO 8
#include <iostream>
#include <string>
#include <map>
#include "sqlite3.h"
using namespace std;
/* INFO include: NUMBER, START, END, METHOD, DEPARTURE_TIME, ARRIVAL_TIME, DURATION, PRICE 8*/
/* tool farmat: "CAR", "TRAIN", "FLIGHT" */

typedef map<string, string> map_row;
typedef map<int , map_row> map_table;

class result_obtainer {
private:
    string pre_sql;
    string start;
    int earliest_time;
    string tool;
    
public:
    map_table Result_list;  
    result_obtainer(string, int, string, string, string LAST_AT = "0", int price_rate = 0, int time_rate = 0);
    void Get_Result();
};


int callback(void *, int, char **, char **);
#endif /* Database_Connection_hpp */
