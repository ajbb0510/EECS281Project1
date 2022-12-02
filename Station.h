//
//  Station.h
//  eecs281project1
//
//  Created by Andrew Bennett-Belch on 5/4/17.
//  Copyright © 2017 Andrew Bennett-Belch. All rights reserved.
//

#ifndef Station_h
#define Station_h


#endif /* Station_h */

#include <vector>
#include <deque>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include <stack>

class Position {
public:
    Position();
    char symbol;
    bool visited;
    char direction;
};

class Coordinate {
public:
    Coordinate();
    Position *p;
    int level;
    int row;
    int col;
};

class Station {
public:
    Station();
    Station(char inputmode);
    Coordinate start;
    std::vector<std::vector<std::vector<Position>>> map;
    int numlevels;
    int levelsize;
    void createshipM();
    void createshipL();
    Coordinate findhangar(bool qpresent);
    bool ismovable(char c);
    void output(Coordinate &hangar, std::ostringstream &os, bool lpresent);
};

