//
//  System_Clock.cpp
//  Tourguide_System
//
//  Created by Seagullbird on 16/3/31.
//  Copyright © 2016年 Seagullbird. All rights reserved.
//

#include "System_Clock.hpp"

struct tm* get_init_time()
{
    sqlite3 *db;
    int rc;
    const char* sql;
    string pre_sql;
    struct tm* pre_cp_time = new struct tm;
    struct tm* pre_sys_time = new struct tm;
    pre_sql = "SELECT * FROM START_TIME WHERE TYPE = 'COM_TIME';";
    /* Open database */
    rc = sqlite3_open("The_Map.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    
    sql = pre_sql.c_str();
    
    sqlite3_stmt *pstmt;
    sqlite3_prepare(db, sql, (int)strlen(sql), &pstmt, NULL);
    sqlite3_step(pstmt);
    pre_cp_time->tm_isdst = 0;
    pre_cp_time->tm_year = sqlite3_column_int(pstmt,0) - 1900;
    pre_cp_time->tm_mon = sqlite3_column_int(pstmt,1) - 1;
    pre_cp_time->tm_mday = sqlite3_column_int(pstmt,2);
    pre_cp_time->tm_hour = sqlite3_column_int(pstmt,3);
    pre_cp_time->tm_min = sqlite3_column_int(pstmt,4);
    pre_cp_time->tm_sec = sqlite3_column_int(pstmt,5);
    
    sqlite3_reset(pstmt);
    pre_sql = "SELECT * FROM START_TIME WHERE TYPE = 'SYS_TIME';";
    sql = pre_sql.c_str();
    sqlite3_prepare(db, sql, (int)strlen(sql), &pstmt, NULL);
    sqlite3_step(pstmt);
    pre_sys_time->tm_isdst = 0;
    pre_sys_time->tm_year = sqlite3_column_int(pstmt,0) - 1900;
    pre_sys_time->tm_mon = sqlite3_column_int(pstmt,1) - 1;
    pre_sys_time->tm_mday = sqlite3_column_int(pstmt,2);
    pre_sys_time->tm_hour = sqlite3_column_int(pstmt,3);
    pre_sys_time->tm_min = sqlite3_column_int(pstmt,4);
    pre_sys_time->tm_sec = sqlite3_column_int(pstmt,5);
    
    sqlite3_finalize(pstmt);
    sqlite3_close(db);
    
    time_t cur_cp_time_t = time(NULL);
    time_t pre_cp_time_t = mktime(pre_cp_time);
    time_t cur_sys_time_t = mktime(pre_sys_time) + (cur_cp_time_t - pre_cp_time_t) * time_factor;
    delete pre_cp_time;
    delete pre_sys_time;
    return localtime(&cur_sys_time_t);
}

void reset_init_time(struct tm* timeg)
{
    sqlite3 *db;
    int rc;
    const char* sql;
    string pre_sql;
    
    pre_sql = "UPDATE START_TIME SET YEAR = " + int_to_string(timeg->tm_year + 1900) + ", MONTH = " + int_to_string(timeg->tm_mon + 1) + ", DAY = " + int_to_string(timeg->tm_mday) + ", HOUR = " + int_to_string(timeg->tm_hour) + ", MINUTE = " + int_to_string(timeg->tm_min) + ", SECOND = " + int_to_string(timeg->tm_sec) + ""
        " WHERE TYPE = 'SYS_TIME';";
    
    /* Open database */
    rc = sqlite3_open("The_Map.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    
    sql = pre_sql.c_str();
    
    sqlite3_stmt *pstmt;
    sqlite3_prepare(db, sql, (int)strlen(sql), &pstmt, NULL);
    sqlite3_step(pstmt);
    sqlite3_reset(pstmt);
    
    time_t cur_cp_time = time(NULL);
    struct tm* cur_cp_tminfo = localtime(&cur_cp_time);

    pre_sql = "UPDATE START_TIME SET YEAR = " + int_to_string(cur_cp_tminfo->tm_year + 1900) + ", MONTH = " + int_to_string(cur_cp_tminfo->tm_mon + 1) + ", DAY = " + int_to_string(cur_cp_tminfo->tm_mday) + ", HOUR = " + int_to_string(cur_cp_tminfo->tm_hour) + ", MINUTE = " + int_to_string(cur_cp_tminfo->tm_min) + ", SECOND = " + int_to_string(cur_cp_tminfo->tm_sec) +
    " WHERE TYPE = 'COM_TIME';";
    
    sql = pre_sql.c_str();
    sqlite3_prepare(db, sql, (int)strlen(sql), &pstmt, NULL);
    sqlite3_step(pstmt);
    sqlite3_finalize(pstmt);
    sqlite3_close(db);
}

System_Clock::System_Clock()
{
    init_time = new struct tm;
    init_time = get_init_time();
    sys_start_time = time(NULL);
}


/* -------------- */

struct tm* System_Clock::get_cur_system_time()
{
    time_t cur_sys_time;
    cur_sys_time = time(NULL);
    cur_sys_time = mktime(init_time) + (cur_sys_time - sys_start_time) * time_factor;
    struct tm* tminfo = localtime(&cur_sys_time);
    return tminfo;
}