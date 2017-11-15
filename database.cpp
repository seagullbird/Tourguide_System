#include "database.h"
Database::Database()
{
}

void Database::open_database()
{
    QString db_path;
    db_path = QCoreApplication::applicationDirPath() + "/The_Map.db";
    rc = sqlite3_open(db_path.toStdString().c_str(), &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    }
}

void Database::exec_sql(string pre_sql)
{
    cout << pre_sql << endl;
    /* Execute SQL statement */
    rc = sqlite3_exec(db, pre_sql.c_str(), NULL, NULL, &zErrMsg);
    if( rc != SQLITE_OK ){
        cout << pre_sql << endl;
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

int Database::getInt_exec_sql(string pre_sql)
{
    const char* sql = pre_sql.c_str();
    sqlite3_stmt *pstmt;
    sqlite3_prepare(db, sql, (int)strlen(sql), &pstmt, NULL);
    sqlite3_step(pstmt);
    int data = sqlite3_column_int(pstmt,0);
    sqlite3_finalize(pstmt);
    return data;
}

string Database::getString_exec_sql(string pre_sql)
{
    const char* sql = pre_sql.c_str();
    sqlite3_stmt *pstmt;
    sqlite3_prepare(db, sql, (int)strlen(sql), &pstmt, NULL);
    sqlite3_step(pstmt);
    string data = (const char*)sqlite3_column_text(pstmt,0);
    sqlite3_finalize(pstmt);
    return data;
}

void Database::close_database()
{
    sqlite3_close(db);
}

bool Database::exist_in_db(const string &name, const string &title, const string &table)
{
    open_database();
    string pre_sql = "SELECT COUNT(*) FROM " + table + " WHERE " + title + " = '" + name + "';";
    const char* sql = pre_sql.c_str();
    sqlite3_stmt *pstmt;
    sqlite3_prepare(db, sql, (int)strlen(sql), &pstmt, NULL);
    sqlite3_step(pstmt);
    int count = sqlite3_column_int(pstmt,0);
    sqlite3_finalize(pstmt);
    sqlite3_close(db);
    if(count > 0)
        return true;
    return false;
}

string Database::getDateTime(string title)
{
    string re;
    open_database();
    string pre_sql = "SELECT " + title + " FROM DATE_TIME WHERE ID = 1;";
    const char* sql = pre_sql.c_str();

    sqlite3_stmt *pstmt;
    sqlite3_prepare(db, sql, (int)strlen(sql), &pstmt, NULL);
    sqlite3_step(pstmt);
    re = (const char*)sqlite3_column_text(pstmt, 0);
    sqlite3_finalize(pstmt);
    sqlite3_close(db);
    return re;
}

string Database::getInitDate()
{
    string re;
    open_database();
    string pre_sql = "SELECT DATE FROM DATE_TIME WHERE ID = 2;";
    const char* sql = pre_sql.c_str();

    sqlite3_stmt *pstmt;
    sqlite3_prepare(db, sql, (int)strlen(sql), &pstmt, NULL);
    sqlite3_step(pstmt);
    re = (const char*)sqlite3_column_text(pstmt, 0);
    sqlite3_finalize(pstmt);
    sqlite3_close(db);
    return re;
}

/* Defination of class Result_Obtainer */
result_obtainer::result_obtainer()
{

}

void result_obtainer::set_value(string st, int e_t, string choice, string tl, string LAST_AT, double price_rate, double time_rate)
{
    //构造函数
    this->start = st;
    this->earliest_time = e_t;
    this->tool = tl;
    this->price_rate = price_rate;
    this->time_rate = time_rate;
    if (choice == "PRICE") {
        pre_sql = "SELECT ROW_ID, NUMBER, START, END, METHOD, DEPARTURE_TIME, ARRIVAL_TIME, DURATION, MIN(PRICE) AS PRICE "
                "FROM ALL_INFO "
                "WHERE START = '" + start + "' "
                "AND (" + tool + ") "
                "AND DEPARTURE_TIME > " + to_string(earliest_time) + " "
                "GROUP BY END; ";
    }
    else if(choice == "ARRIVAL_TIME")
    {
        pre_sql = "SELECT ROW_ID, NUMBER, START, END, METHOD, DEPARTURE_TIME,MIN(ARRIVAL_TIME) AS ARRIVAL_TIME, DURATION, PRICE "
                "FROM ALL_INFO "
                "WHERE START = '" + start + "' "
                "AND (" + tool + ") "
                "AND DEPARTURE_TIME > " + to_string(earliest_time) + " "
                "GROUP BY END; ";
    }
    else if(choice == "MIX")
    {
        pre_sql = "SELECT ROW_ID, NUMBER, START, END, METHOD, DEPARTURE_TIME, ARRIVAL_TIME, DURATION, PRICE, "
                "MIN(PRICE * " + to_string(price_rate) + " + ((ARRIVAL_TIME/10000*2)%(ARRIVAL_TIME/10000*2-1) * (-7600) + ARRIVAL_TIME  - 40 - " + LAST_AT + ") * " + to_string(time_rate) + ") AS MIX "
                "FROM ALL_INFO "
                "WHERE START = '" + start + "' "
                "AND (" + tool + ") "
                "AND DEPARTURE_TIME > " + to_string(earliest_time) + " "
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
    open_database();
    const char* sql = pre_sql.c_str();
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)map_ptr, &zErrMsg);
    if( rc != SQLITE_OK ){
        cout << sql << endl;
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    close_database();
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
