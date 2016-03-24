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
#include "sqlite3.h"
using namespace std;

/* INFO include: NUMBER, START, END, METHOD, DEPARTURE_TIME, ARRIVAL_TIME, DURATION, PRICE 8*/
/* tool farmat: "CAR", "TRAIN", "FLIGHT" */
typedef struct result
{
    string row[NUMBER_OF_INFO];
    struct result *nextptr;
}*Resultptr;

class result_obtainer {
private:
    string pre_sql;
    string start;
    int earliest_time;
    string tool;
    
public:
    Resultptr all_result;           //Results start from all_result->nextptr    
    result_obtainer(string, int, string, string tl = "ALL");
    void Get_Result();
};


int callback(void *, int, char **, char **);


#endif /* Database_Connection_hpp */
