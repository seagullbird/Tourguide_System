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
