//
//  Small_Funs.hpp
//  Tourguide_System
//
//  Created by Seagullbird on 16/3/17.
//  Copyright © 2016年 Seagullbird. All rights reserved.
//

#ifndef Small_Funs_hpp
#define Small_Funs_hpp

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include "Database_Connection.hpp"
using namespace std;

string int_to_string(int);

void print_map_table(map_table &);

int get_time(int &, int &);

int exist_in_db(string &);

void clear_map_table(map_table &);
#endif /* Small_Funs_hpp */
