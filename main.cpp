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
#include "Strategy.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
    
//    result_obtainer ob("CHENGDU", 700, "PRICE");
//    ob.Get_Result();
//    print_map_table(ob.Result_list);
    
    map_table route = simple_min_strategy("CHENGDU", "BEIJING", "PRICE");
    cout << "Final result:" << endl;
    print_map_table(route);
    
    clear_map_table(route);
    return 0;
}
