//
//  Strategy.hpp
//  Tourguide_System
//
//  Created by Seagullbird on 16/3/28.
//  Copyright © 2016年 Seagullbird. All rights reserved.
//

#ifndef Strategy_hpp
#define Strategy_hpp

#include <iostream>
#include "Small_Funs.hpp"
#include "Database_Connection.hpp"
#define INFI 99999999
#define city_number 10
using namespace std;

map_table simple_min_strategy(string start, string end, string strategy, string method = "ALL");
#endif /* Strategy_hpp */
