#ifndef USER_H
#define USER_H
#include "small_funs.h"
#include "database.h"
class User : public Database
{
public:
    User();
    User(string, string);
    void create_new_user();

    string check_password(string username);

    void set_account(string username, string password);

    void del_account();

    void new_password(const string &newpass);

    string get_username();

    string get_password();

private:
    string username;
    string password;
};

#endif // USER_H
