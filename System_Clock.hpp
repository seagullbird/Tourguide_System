//
//  System_Clock.hpp
//  Tourguide_System
//
//  Created by Seagullbird on 16/3/31.
//  Copyright © 2016年 Seagullbird. All rights reserved.
//

#ifndef System_Clock_hpp
#define System_Clock_hpp

#include <iostream>
#include <ctime>
#include <string>
#include "Database_Connection.hpp"
#include "Small_Funs.hpp"
#define time_factor 40           //系统时间比电脑时间快39倍
using namespace std;

class System_Clock {
private:
    struct tm* init_time;
    time_t sys_start_time;
public:
    System_Clock();
    struct tm* get_cur_system_time();
};

struct tm* get_init_time();
void reset_init_time(struct tm*);
#endif /* System_Clock_hpp */
