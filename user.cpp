#include "user.h"

User::User()
{
}

User::User(string username, string password)
{
    this->username = username;
    this->password = password;
}

void User::set_account(string username, string password)
{
    this->username = username;
    this->password = password;
}

string User::get_username() { return username;}
string User::get_password() { return password;}


void User::create_new_user()
{
    open_database();
    exec_sql("INSERT INTO USER VALUES('" + username + "', '" + password + "');");
    exec_sql("CREATE TABLE user_" + username + " (ROUTE_ID INT NOT NULL, "
                                               "SEQ INT NOT NULL, "
                                               "DEPARTURE_TIME TEXT NOT NULL, "
                                               "START TEXT NOT NULL, "
                                               "METHOD TEXT NOT NULL, "
                                               "NUMBER TEXT NOT NULL, "
                                               "ARRIVAL_TIME TEXT NOT NULL, "
                                               "END TEXT NOT NULL, "
                                               "TOTAL_PRICE REAL NOT NULL, "
                                               "STATUS TEXT NOT NULL, "
                                               "RECORD_TYPE TEXT NOT NULL);");
    close_database();
}

string User::check_password(string username)
{
    open_database();
    string pre_sql = "SELECT PASSWORD FROM USER WHERE USERNAME = '" + username + "';";
    const char* sql = pre_sql.c_str();
    sqlite3_stmt *pstmt;
    sqlite3_prepare(db, sql, (int)strlen(sql), &pstmt, NULL);
    sqlite3_step(pstmt);
    string password((char *)sqlite3_column_text(pstmt,0));

    sqlite3_finalize(pstmt);
    close_database();
    return password;
}

void User::del_account()
{
    open_database();
    exec_sql("DELETE FROM USER WHERE USERNAME = '" + username + "';");
    exec_sql("DROP TABLE user_" + username + ";");
    close_database();
}

void User::new_password(const string &newpass)
{
    open_database();
    exec_sql("UPDATE USER SET PASSWORD = '"+ newpass +"' WHERE USERNAME = '" + username +  "'");
    close_database();
}
