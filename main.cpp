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

System_Clock sys_clock;

int main(int argc, const char * argv[]) {
    twptr tw = new total_weight;
    map_table route = simple_min_strategy("CHENGDU", "BEIJING", "PRICE", tw);
    cout << "Final result:" << endl;
    print_map_table(route);
    cout << "Total time: " << tw->total_time << endl;
    cout << "Total price: " << tw->total_price << endl;
    clear_map_table(route);
    
    string com;
    cout << "Done?" << endl;
    cin >> com;
    while (com != "Y") {
        cout << asctime(sys_clock.get_cur_system_time()) << endl;
        cout << "Done?" << endl;
        cin >> com;
    }
    reset_init_time(sys_clock.get_cur_system_time());
    return 0;
}
