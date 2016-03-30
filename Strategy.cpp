//
//  Strategy.cpp
//  Tourguide_System
//
//  Created by Seagullbird on 16/3/28.
//  Copyright © 2016年 Seagullbird. All rights reserved.
//

#include "Strategy.hpp"
#include "Database_Connection.hpp"
#include "Small_Funs.hpp"
//记得改city_number
map_table simple_min_strategy(string start, string end, string strategy, string method)
{
    int min;
    //初始earliest_time为系统当前时间，现在先设为7:00
    int earliest_time = 700;
    
    map_table route;
    
    if (exist_in_db(end)) {
        if (strategy == "PRICE" || strategy == "ARRIVAL_TIME") {
            result_obtainer from_the_start(start, earliest_time, strategy, method);
            from_the_start.Get_Result();
            
            map<string, int> final;
            typedef struct route_to_start
            {
                int total;      //保存该城市目前到起点关键值的和（价格和/时间和）
                map_row one_arc;
                struct route_to_start *nextptr;
            }rts, *rtsptr;
            map<string, rtsptr> D;                                  //键值为城市，值为城市到起点的路线链表
            string temp_city;
            rtsptr temp_rtsptr = nullptr;
            //初始化
            for(auto &x : from_the_start.Result_list)
            {
                D[x.second["END"]] = new rts;
                D[x.second["END"]]->total = atoi(x.second[strategy].c_str()) - (strategy == "PRICE" ? 0 : earliest_time);
                D[x.second["END"]]->one_arc = x.second;
                D[x.second["END"]]->nextptr = nullptr;
            }
            
            clear_map_table(from_the_start.Result_list);
            for(int city_count = 0; city_count < city_number; ++city_count)
            {
                /*-----寻找最短距离-----*/
                min = INFI;
                //遍历D
                for(auto &x : D)
                {
                    //如果该城市不在集合S中并且关键值（strategy）小于min
                    if (!final[x.first] && x.second->total < min) {
                        min = x.second->total;
                        temp_city = x.first;
                        temp_rtsptr = x.second;
                    }
                }
                /*----将最短加入集合----*/
                if (temp_city == end || temp_city == "") {
                    break;
                }
                final[temp_city] = 1;
                /*-----更新最短距离-----*/
                //以temp_city为起点get
                result_obtainer from_this_city(temp_city, atoi(temp_rtsptr->one_arc["ARRIVAL_TIME"].c_str()), strategy, method);
                from_this_city.Get_Result();
                
                //更新D
                for(auto &x : from_this_city.Result_list)
                {
                    if (!D[x.second["END"]]) {
                        //如果D里面没有这个城市将这个城市加入D，路径就是这个城市到temp_city到start
                        D[x.second["END"]] = new rts;
                        D[x.second["END"]]->total = temp_rtsptr->total + atoi(x.second[strategy].c_str()) - (strategy == "PRICE" ? 0 : atoi(temp_rtsptr->one_arc["ARRIVAL_TIME"].c_str()));
                        D[x.second["END"]]->one_arc = x.second;
                        D[x.second["END"]]->nextptr = temp_rtsptr;
                    }
                    else
                    {
                        //如果D里面有这个城市则更新之
                        if (!final[x.second["END"]] && min + atoi(x.second[strategy].c_str()) - (strategy == "PRICE" ? 0 : atoi(temp_rtsptr->one_arc["ARRIVAL_TIME"].c_str())) < D[x.second["END"]]->total) {
                            D[x.second["END"]]->total = min + atoi(x.second[strategy].c_str()) - (strategy == "PRICE" ? 0 : atoi(temp_rtsptr->one_arc["ARRIVAL_TIME"].c_str()));
                            D[x.second["END"]]->one_arc = x.second;
                            D[x.second["END"]]->nextptr = temp_rtsptr;
                        }
                    }
                }
                clear_map_table(from_this_city.Result_list);
            }
            if (D[end]) {
                rtsptr temp_ptr = D[end];
                int i = 1;
                while (temp_ptr != nullptr) {
                    route[i++] = temp_ptr->one_arc;
                    temp_ptr = temp_ptr->nextptr;
                }
                cout << "Total" << (strategy == "PRICE" ? " PRICE" : " TIME") << " == " << D[end]->total << endl;
            }
            else
            {
                cout << "No such start city!" << endl;
            }
        }
        else
        {
            cout << "Please input the correct strategy!" << endl;
        }
    }
    else
    {
        cout << "No such end city!" << endl;
    }
        
    return route;
}