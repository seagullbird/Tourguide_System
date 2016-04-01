//
//  Database_Connection.cpp
//  Tourguide_System
//
//  Created by Seagullbird on 16/3/17.
//  Copyright © 2016年 Seagullbird. All rights reserved.
//

#include "Database_Connection.hpp"
#include "Small_Funs.hpp"
//目前功能：查询从start城市（大写全拼）到所有与之直接相连的城市，earliest_time（无冒号）之后价格最低(price)或者到达时间最早(time)的交通工具tool的所有信息
/* Defination of class Result_Obtainer */
result_obtainer::result_obtainer(string st, int e_t, string choice, string tl, string LAST_AT, int price_rate, int time_rate)
{
    //构造函数
    this->start = st;
    this->earliest_time = e_t;
    this->tool = tl;
    if (choice == "PRICE") {
        this->pre_sql = "SELECT NUMBER, START, END, METHOD, DEPARTURE_TIME, ARRIVAL_TIME, DURATION, MIN(PRICE) AS PRICE "
        "FROM " + tool + "_INFO "
        "WHERE START = '" + start + "' "
        "AND NUMBER IN "
        "(SELECT NUMBER FROM " + tool + "_INFO "
        "WHERE DEPARTURE_TIME > " + int_to_string(earliest_time) + ") "
        "GROUP BY END; ";
    }
    else if(choice == "ARRIVAL_TIME")
    {
        this->pre_sql = "SELECT NUMBER, START, END, METHOD, DEPARTURE_TIME, MIN(ARRIVAL_TIME) AS ARRIVAL_TIME, DURATION, PRICE "
        "FROM " + tool + "_INFO "
        "WHERE START = '" + start + "' "
        "AND NUMBER IN "
        "(SELECT NUMBER FROM " + tool + "_INFO "
        "WHERE DEPARTURE_TIME > " + int_to_string(earliest_time) + ") "
        "GROUP BY END; ";
    }
    else if(choice == "MIX")
    {
        this->pre_sql = "SELECT NUMBER, START, END, METHOD, DEPARTURE_TIME, ARRIVAL_TIME, DURATION, PRICE, "
        "MIN(PRICE*" + int_to_string(price_rate) + " + (ARRIVAL_TIME - " + LAST_AT + ")*" + int_to_string(time_rate) + ") AS MIX"
        "FROM " + tool + "_INFO "
        "WHERE START = '" + start + "' "
        "AND NUMBER IN "
        "(SELECT NUMBER FROM " + tool + "_INFO "
        "WHERE DEPARTURE_TIME > " + int_to_string(earliest_time) + ") "
        "GROUP BY END; ";
        
    }
    else
    {
        cout << "You have to choose between 'PRICE' and 'ARRIVAL_TIME'!" << endl;
        exit(0);
    }
}

void result_obtainer::Get_Result()
{
    map_table *map_ptr = &Result_list;
    sqlite3 *db;
    int rc;
    char *zErrMsg = 0;
    const char* sql;
    
    /* Open database */
    rc = sqlite3_open("The_Map.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    
    sql = pre_sql.c_str();
    
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)map_ptr, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
}

/* ----------------------------------- */

int callback(void *map_ptr, int argc, char **argv, char ** azColName)
{
    //This function returns only one row of records each time it is called
    static int Row_num = 1;
    for(int i=0; i<argc; i++) {
        string temp_id(azColName[i]), temp_name(argv[i]);
        (*(map_table *)map_ptr)[Row_num][temp_id] = temp_name;
    }
    Row_num++;
    return 0;
}