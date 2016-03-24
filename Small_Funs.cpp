//
//  Small_Funs.cpp
//  Tourguide_System
//
//  Created by Seagullbird on 16/3/17.
//  Copyright © 2016年 Seagullbird. All rights reserved.
//

#include "Small_Funs.hpp"
#include "Database_Connection.hpp"

string NAMES[] = {"NUMBER", "START", "END", "METHOD", "DEPARTURE_TIME", "ARRIVAL_TIME", "DURATION", "PRICE"};
string int_to_string(int a)
{
    stringstream ss;
    string b;
    ss << a;
    ss >> b;
    return b;
}

//Print the chain
void printlist(Resultptr &head)
{
    Resultptr temp = head;
    if (temp == NULL) {
        cout << "No results returned." << endl;
        return;
    }
    while (temp != NULL) {
        for(int i=0;i<NUMBER_OF_INFO;++i)
            cout << NAMES[i] << " == " << temp->row[i] << endl;
        cout << endl;
        temp = temp->nextptr;
    }
}

