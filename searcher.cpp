#include "searcher.h"

Searcher::Searcher(QDate first_date)
{
    this->first_date = first_date;
    tw = new total_weight;
}

void Searcher::set_city(string start, string end)
{
    this->start_city = start;
    this->end_city = end;
}

void Searcher::set_strategy(string strategy)
{
    if(strategy == "Lower Price")
        this->strategy = "PRICE";
    else if(strategy == "Less Time")
        this->strategy = "ARRIVAL_TIME";
    else
        this->strategy = "MIX";
}

void Searcher::set_method(bool all, bool flight, bool train, bool car)
{
    method = "";
    if(all)
    {
        method = "METHOD = 'FLIGHT' OR METHOD = 'TRAIN' OR METHOD = 'CAR'";
    }
    else
    {
        if(flight)
        {
            method = "METHOD = 'FLIGHT'";
            if(train) method += "OR METHOD = 'TRAIN'";
            else if(car) method += "OR METHOD = 'CAR'";
        }
        else
        {
            if(train)
            {
                method = "METHOD = 'TRAIN'";
                if(car) method += "OR METHOD = 'CAR'";
            }
            else if(car) method = "METHOD = 'CAR'";
        }
    }
}

void Searcher::set_clock(QDate date, QTime time)
{
    this->cur_date = date;
    this->cur_time = time;
}

void Searcher::set_latest_a_time(QDateTime latime)
{
    QDateTime sttime(cur_date, cur_time);

    int second = sttime.secsTo(latime);
    int days = second / 86400;
    second %= 86400;
    int hour = second / 3600;
    second %= 3600;
    int min = second / 60;
    this->latest_a_time = days * 10000 + hour * 100 + min;
}

void Searcher::simple_min_strategy(double price_rate, double time_rate)
{
    route.clear();
    int min;
    int earliest_time = atoi(cur_time.toString("hhmm").toStdString().c_str()) + 10000 * (first_date.daysTo(cur_date));

    if (exist_in_db(end_city, "END", "ALL_INFO")) {
        if (strategy == "PRICE" || strategy == "ARRIVAL_TIME" || strategy == "MIX") {
            result_obtainer::set_value(start_city, earliest_time, strategy, method, to_string(earliest_time), price_rate, time_rate);
            result_obtainer::Get_Result();

            map<string, int> final;
            typedef struct route_to_start
            {
                int total;                                          //保存该城市目前到起点关键值的和（价格和/时间和/混合和）
                int total_time;                                     //保存时间和
                int total_price;                                    //保存价格和
                int arc_num = 1;                                        //保存路径段数
                map_row one_arc;
                struct route_to_start *nextptr;
            }rts, *rtsptr;
            map<string, rtsptr> D;                                  //键值为城市，值为城市到起点的路线链表
            string temp_city;
            rtsptr temp_rtsptr = nullptr;
            //初始化
            final[start_city] = 1;
            for(auto &x : Result_list)
            {
                D[x.second["END"]] = new rts;
                D[x.second["END"]]->total = atoi(x.second[strategy].c_str()) - ( strategy == "PRICE" || strategy == "MIX"? 0 : earliest_time);
                D[x.second["END"]]->total_price = atoi(x.second["PRICE"].c_str());
                D[x.second["END"]]->arc_num = 1;
                D[x.second["END"]]->one_arc = x.second;
                D[x.second["END"]]->nextptr = nullptr;
            }

            clear_map_table(Result_list);
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
                if (temp_city == end_city || temp_city == "") {
                    break;
                }
                final[temp_city] = 1;
                /*-----更新最短距离-----*/
                //以temp_city为起点get
                result_obtainer::set_value(temp_city, atoi(temp_rtsptr->one_arc["ARRIVAL_TIME"].c_str()), strategy, method, temp_rtsptr->one_arc["ARRIVAL_TIME"], price_rate, time_rate);
                result_obtainer::Get_Result();

                //更新D
                for(auto &x : Result_list)
                {
                    if (!D[x.second["END"]]) {
                        //如果D里面没有这个城市将这个城市加入D，路径就是这个城市到temp_city到start
                        D[x.second["END"]] = new rts;
                        D[x.second["END"]]->total = temp_rtsptr->total + atoi(x.second[strategy].c_str()) - ( strategy == "PRICE" || strategy == "MIX" ? 0 : atoi(temp_rtsptr->one_arc["ARRIVAL_TIME"].c_str()));
                        D[x.second["END"]]->total_price = temp_rtsptr->total_price + atoi(x.second["PRICE"].c_str());
                        D[x.second["END"]]->arc_num = temp_rtsptr->arc_num + 1;
                        D[x.second["END"]]->one_arc = x.second;
                        D[x.second["END"]]->nextptr = temp_rtsptr;
                    }
                    else
                    {
                        //如果D里面有这个城市则更新之
                        if (!final[x.second["END"]] && min + atoi(x.second[strategy].c_str()) - ( strategy == "PRICE" || strategy == "MIX" ? 0 : atoi(temp_rtsptr->one_arc["ARRIVAL_TIME"].c_str())) < D[x.second["END"]]->total) {
                            D[x.second["END"]]->total = min + atoi(x.second[strategy].c_str()) - ( strategy == "PRICE" || strategy == "MIX" ? 0 : atoi(temp_rtsptr->one_arc["ARRIVAL_TIME"].c_str()));
                            D[x.second["END"]]->total_price = temp_rtsptr->total_price + atoi(x.second["PRICE"].c_str());
                            D[x.second["END"]]->arc_num = temp_rtsptr->arc_num + 1;
                            D[x.second["END"]]->one_arc = x.second;
                            D[x.second["END"]]->nextptr = temp_rtsptr;
                        }
                    }
                }
                clear_map_table(Result_list);
            }
            if (D[end_city]) {
                int DEPA_TIME, ARRI_TIME;
                ARRI_TIME = atoi(D[end_city]->one_arc["ARRIVAL_TIME"].c_str());
                rtsptr temp_ptr = D[end_city];
                int i = D[end_city]->arc_num;
                while (temp_ptr != nullptr) {
                    route[i--] = temp_ptr->one_arc;
                    if (temp_ptr->nextptr == nullptr) {
                        DEPA_TIME = atoi(temp_ptr->one_arc["DEPARTURE_TIME"].c_str());
                    }
                    temp_ptr = temp_ptr->nextptr;
                }
                tw->total_time = get_time(ARRI_TIME, earliest_time);
                tw->total_price = D[end_city]->total_price;
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
}

void Searcher::init(double &a, double &b, int line, int early)
{
    double m = 1, t;
    if (line - early >= 1000)
        t = 1000;
    else if (line - early >= 500)
        t = 300;
    else if (line - early >= 100)
        t = 100;
    else
        t = 50;
    a = m / (m + t);
    b = t / (m + t);
}

inline void Searcher::UpFresh(double &a, double &b)
{
    a += UP_RATE;
    b -= UP_RATE;
}

inline void Searcher::DownFresh(double &a, double &b)
{
    a -= DW_RATE;
    b += DW_RATE;
}

void Searcher::reflash(double &a, double &b, int line, int current)
{
    double m = 1, t;
    if (current - line >= 100)
        t = 70;
    else if (current - line >= 40)
        t = 40;
    else if (current - line >= 30)
        t = 10;
    else
        t = 5;
    a = m / (m + t);
    b = t / (m + t);
}

void Searcher::Strategy3()
{
    double a, b;//a为金钱比率，b为时间比率
    set_strategy("ARRIVAL_TIME");
    simple_min_strategy();

    //PRINT ROUTE SO FAR
    for(auto &x : route)
    {
        cout << x.first << endl;
        for(auto &y : x.second)
            cout << y.first << " == " << y.second << endl;
    }
    cout << tw->total_time << endl;
    cout << latest_a_time << endl;
    set_strategy("MIX");
    if (tw->total_time <= latest_a_time)//策略三的先决条件当最短时间小于等于最晚到达时间时
    {
        init(a, b, latest_a_time, tw->total_time);
        while (tw->total_time < latest_a_time)
        {
            simple_min_strategy(a, b);
            UpFresh(a, b);
            if(a < 0 || b < 0) break;
            cout << "up while " << "a == " << a << " b == " << b << endl;
        }
        //reflash(a, b, latest_a_time, tw->total_time);
        while (tw->total_time > latest_a_time)
        {
            DownFresh(a, b);
            if(a < 0 || b < 0) break;
            simple_min_strategy(a, b);
            cout << "down while " << "a == " << a << "b == " << b << endl;
        }
    }
    else
    {
        route.clear();
    }
}
