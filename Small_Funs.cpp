//
//  Small_Funs.cpp
//  Tourguide_System
//
//  Created by Seagullbird on 16/3/17.
//  Copyright © 2016年 Seagullbird. All rights reserved.
//

#include "Small_Funs.hpp"
#include "Database_Connection.hpp"

string int_to_string(int a)
{
    stringstream ss;
    string b;
    ss << a;
    ss >> b;
    return b;
}

void print_map_table(map_table &table)
{
    for(auto &x : table)
    {
        cout << x.first << endl;
        for(auto &y : x.second)
        {
            cout << y.first << " == " << y.second << endl;
        }
        cout << endl;
    }
}

int get_time(int & end_time, int & start_time)
{
    //Get time
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
    
    result = result_hr * 100 + result_min;
    return result;
}

int exist_in_db(string &city_name)
{
    sqlite3 *db;
    int rc;
    const char* sql;
    string pre_sql;
    
    /* Open database */
    rc = sqlite3_open("The_Map.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    
    pre_sql = "SELECT COUNT(*) FROM ALL_INFO WHERE END = '" + city_name + "';";
    sql = pre_sql.c_str();

    sqlite3_stmt *pstmt;
    sqlite3_prepare(db, sql, (int)strlen(sql), &pstmt, NULL);
    sqlite3_step(pstmt);
    int count=sqlite3_column_int(pstmt,0);
    sqlite3_finalize(pstmt);
    
    if(count > 0)
        return 1;
    
    return 0;
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