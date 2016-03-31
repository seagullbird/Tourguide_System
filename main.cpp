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
#include "System_Clock.hpp"
using namespace std;

/* 剩余工作：完成系统时间类 
        将头文件合并为一个
*/
System_Clock sys_clock;

int main(int argc, const char * argv[]) {
    twptr tw = new total_weight;
    map_table route = simple_min_strategy("CHENGDU", "BEIJING", "PRICE", tw);
    cout << "Final result:" << endl;
    print_map_table(route);
    cout << "Total time: " << tw->total_time << endl;
    cout << "Total price: " << tw->total_price << endl;
    clear_map_table(route);
    return 0;
}
