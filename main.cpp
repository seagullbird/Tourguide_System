//
//  main.cpp
//  Tourguide_System
//
//  Created by Seagullbird on 16/3/17.
//  Copyright © 2016年 Seagullbird. All rights reserved.
//

#include <iostream>
#include "sqlite3.h"
#include "Database_Connection.hpp"
#include "Small_Funs.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    result_obtainer ob("CHENGDU", 700, "time");
    ob.Get_Result();
    
    for(auto &x : ob.Result_list)
    {
        cout << x.first << endl;
        for(auto &y : x.second)
        {
            cout << y.first << " == " << y.second << endl;
        }
    }
    return 0;
}
