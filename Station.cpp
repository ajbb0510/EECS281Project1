//
//  Position.cpp
//  eecs281project1
//
//  Created by Andrew Bennett-Belch on 5/4/17.
//  Copyright © 2017 Andrew Bennett-Belch. All rights reserved.
//

#include "Station.h"

using namespace std;

//Position default constructor
Position::Position() {
    symbol = '.';
    visited = false;
    direction = '\0';
}

//Coordinate default constructor
Coordinate::Coordinate() {
    p = nullptr;
    level = 0;
    row = 0;
    col = 0;
}

//station default constructor
Station::Station() {
    start.level = 0;
    start.row = 0;
    start.col = 0;
    start.p = nullptr;
    numlevels = 0;
    levelsize = 0;
    
    
}


Station::Station(char inputmode) {
    //map input mode
    if (inputmode == 'M') {
        createshipM();
    }
    //list input mode
    else {
        createshipL();
    }
    
}

void Station::createshipM() {
    char symbol;
    string comment;
    cin >> numlevels >> levelsize;
    
    //Make vector the correct size
    map.resize(numlevels);
    for (int i = 0; i < numlevels; ++i) {
        map[i].resize(levelsize);
        for (int j = 0; j < levelsize; ++j) {
            map[i][j].resize(levelsize);
        }
    }
    
    
    int level = 0;
    int row = 0;
    
    //read in first char of line
    while (cin >> symbol) {
        
        //if line is a comment read into a string
        if (symbol == '/') {
            getline(cin,comment);
            continue;
        }
        
        //check for valid symbol
        if ((symbol != '.') && (symbol != '#') && (symbol != 'S') && (symbol != 'H') && (symbol != 'E')) {
            cerr << "Invalid symbol";
            exit(1);
        }
        //initialize first col  in 3d vector
        map[level][row][0].symbol = symbol;
        if (symbol == 'S') {
            start.level = level;
            start.row = row;
            start.col = 0;
            start.p = &(map[level][row][0]);
        }
        
        
        //read in the rest of the symbols in the row and initialize positions in vector
        for (int col = 1; col < levelsize; col++) {
            cin >> symbol;
            if ((symbol != '.') && (symbol != '#') && (symbol != 'S') && (symbol != 'H') && (symbol != 'E')) {
                cerr << "Invalid symbol";
                exit(1);
            }
            map[level][row][col].symbol = symbol;
            if (symbol == 'S') {
                start.level = level;
                start.row = row;
                start.col = col;
                start.p = &(map[level][row][col]);
            }
            
        }//for
        row += 1;
        
        //If you have filled every row in level go to next level
        if (row >= levelsize) {
            row = 0;
            level += 1;
        }
        //If you have filled every level break loop
        if (level >= numlevels) {
            break;
        }
    }
}


void Station::createshipL() {
    char symbol;
    string comment;
    char comma;
    cin >> numlevels >> levelsize;
    
    //make vector the correct size
    map.resize(numlevels);
    for (int i = 0; i < numlevels; ++i) {
        map[i].resize(levelsize);
        for (int j = 0; j < levelsize; ++j) {
            map[i][j].resize(levelsize);
        }
    }
    
    int level;
    int row;
    int col;
    
    //read in first char of line
    while (cin >> symbol) {
        
        //if line is a comment read into a string
        if (symbol == '/') {
            getline(cin,comment);
            continue;
        }
        if (symbol == '(') {
            cin >> level;
            if ((level < 0) || (level >= numlevels)) {
                cerr << "Invalid level";
                exit(1);
            }
            cin >> comma >> row;
            if ((row < 0) || (row >= levelsize)) {
                cerr << "Invalid row";
                exit(1);
            }
            cin >> comma >> col;
            if ((col < 0) || (col >= levelsize)) {
                cerr << "Invalid column";
                exit(1);
            }
            cin >> comma >> symbol;
            //check for valid symbol
            if ((symbol != '.') && (symbol != '#') && (symbol != 'S') && (symbol != 'H') && (symbol != 'E')) {
                cerr << "Invalid symbol";
                exit(1);
            }
            cin >> comma;
            map[level][row][col].symbol = symbol;
            if (symbol == 'S') {
                start.level = level;
                start.row = row;
                start.col = col;
                start.p = &(map[level][row][col]);
            }
            
        }//if
    }//while
}



Coordinate Station::findhangar(bool qpresent) {
    
    
    deque<Coordinate> deque;
    
    //push back or front depending on queue or stack mode
    if (qpresent) {
        deque.push_back(start);
    }
    else {
        deque.push_front(start);
    }
    
    Coordinate c;
    Coordinate hangar;
    Coordinate nopath;
    
    
    while (true) {
        //if nothing on the deque then no valid route return null coordinate
        if (deque.size() == 0) {
            return nopath;
        }
        
        c = deque.front();
        deque.pop_front();
        
        int level = c.level;
        int row = c.row;
        int col = c.col;
        
        Coordinate north;
        char northsymbol = '\0';
        //Put north,east,south, and west coordinates into variables for debugging
        if (row > 0) {
            north.level = level;
            north.row = row - 1;
            north.col = col;
            north.p = &map[north.level][north.row][north.col];
            northsymbol = north.p->symbol;
        }
        
        Coordinate east;
        char eastsymbol = '\0';
        
        if (col < levelsize - 1) {
            east.level = level;
            east.row = row;
            east.col = col + 1;
            east.p = &map[east.level][east.row][east.col];
            eastsymbol = east.p->symbol;
        }
        
        Coordinate south;
        char southsymbol = '\0';
        
        if (row < levelsize - 1) {
            south.level = level;
            south.row = row + 1;
            south.col = col;
            south.p = &map[south.level][south.row][south.col];
            southsymbol = south.p->symbol;
        }
        
        Coordinate west;
        char westsymbol = '\0';
        
        if (col > 0) {
            west.level = level;
            west.row = row;
            west.col = col - 1;
            west.p = &map[west.level][west.row][west.col];
            westsymbol = west.p->symbol;
        }
        
        
        
        //Check if you can move north
        if (northsymbol != '\0') {
            if (ismovable(northsymbol)) {
                //if north is hangar return
                if (northsymbol == 'H') {
                    north.p->direction = 'n';
                    hangar = north;
                    return hangar;
                }
                //if north is not hangar but can be visited mark it and put on deque
                if (!(north.p->visited)) {
                    north.p->visited = true;
                    north.p->direction = 'n';
                    if (qpresent) {
                        deque.push_back(north);
                    }
                    else {
                        deque.push_front(north);
                    }
                }
            }
        }
        //repeat with east
        if (eastsymbol != '\0') {
            if (ismovable(eastsymbol)) {
                if (eastsymbol == 'H') {
                    east.p->direction = 'e';
                    hangar = east;
                    return hangar;
                }
                if (!(east.p->visited)) {
                    east.p->visited = true;
                    east.p->direction = 'e';
                    if (qpresent) {
                        deque.push_back(east);
                    }
                    else {
                        deque.push_front(east);
                    }
                }
            }
        }
        
        //repeat with south
        if (southsymbol != '\0') {
            if (ismovable(southsymbol)) {
                if (southsymbol == 'H') {
                    south.p->direction = 's';
                    hangar = south;
                    return hangar;
                }
                if (!(south.p->visited)) {
                    south.p->visited = true;
                    south.p->direction = 's';
                    if (qpresent) {
                        deque.push_back(south);
                    }
                    else {
                        deque.push_front(south);
                    }
                }
            }
        }
        
        //repeat with west
        if (westsymbol != '\0') {
            if (ismovable(westsymbol)) {
                if (westsymbol == 'H') {
                    west.p->direction = 'w';
                    hangar = west;
                    return hangar;
                }
                if (!(west.p->visited)) {
                    west.p->visited = true;
                    west.p->direction = 'w';
                    if (qpresent) {
                        deque.push_back(west);
                    }
                    else {
                        deque.push_front(west);
                    }
                }
            }
        }
        
        //if it is an elevator space
        if (c.p->symbol == 'E') {
            //loop through levels
            for(int i = 0; i < numlevels; ++i) {
                Coordinate elevator;
                elevator.p = &map[i][c.row][c.col];
                
                //if the corresponding space on other level is
                //an elevator and has not been visited mark it and put on deque
                if (elevator.p->symbol == 'E') {
                    if (!(elevator.p->visited)) {
                        int level = c.level;
                        elevator.p->visited = true;
                        elevator.p->direction = char(level);
                        elevator.level = i;
                        elevator.row = c.row;
                        elevator.col = c.col;
                        if (qpresent) {
                            deque.push_back(elevator);
                        }
                        else {
                            deque.push_front(elevator);
                        }//else
                    }//if
                }//if
            }//for
        }//if
    }//while
    
}

//helper function
bool Station::ismovable(char c) {
    if ((c == '.') || (c == 'H') || (c == 'E') ) {
        return true;
    }
    return false;
}

void Station::output(Coordinate &hangar, ostringstream &os, bool lpresent) {
    
    //Create a stack to hold path coordinates
    stack<Coordinate> stack;
    stack.push(hangar);
    
    Coordinate c;
    c = hangar;
    char symbol = c.p->symbol;
    char direction = c.p->direction;
   
    //Break loop when it gets back to start
    while (symbol != 'S') {
        //move to corresponding adjacent tile in path and push on stack
        if (direction == 'n') {
            c.row = c.row + 1;
            c.p = &map[c.level][c.row][c.col];
            stack.push(c);
        }
        else if (direction == 'e') {
            c.col = c.col - 1;
            c.p = &map[c.level][c.row][c.col];
            stack.push(c);
        }
        else if (direction == 's') {
            c.row = c.row - 1;
            c.p = &map[c.level][c.row][c.col];
            stack.push(c);
        }
        else if (direction == 'w') {
            c.col = c.col + 1;
            c.p = &map[c.level][c.row][c.col];
            stack.push(c);
        }
        else {
            for (int i = 0; i < numlevels; ++i) {
                if (direction == (char)i) {
                    int prevlevel = c.level;
                    c.level = i;
                    c.p->direction = (char) prevlevel;
                    c.p = &map[i][c.row][c.col];
                    stack.push(c);
                }
            }
        }
        symbol = c.p->symbol;
        direction = c.p->direction;
    }
    
    
    //print number of levels and size of levels
    os << numlevels << "\n" << levelsize << "\n";
    
    //list output mode
    if (lpresent) {
        os << "//path taken\n";
        //loop through stack printing coordinates and directions
        size_t stacksize = stack.size();
        for (size_t i = 0; i < stacksize - 1; ++i) {
            c = stack.top();
            stack.pop();
            os << "(" << c.level << "," << c.row << "," << c.col << ",";
            char direct = stack.top().p->direction;
            if (direct >= '\x00' && direct < '\x0a') {
                int x = (int)stack.top().p->direction;
                os << x << ")\n";
            }
            else {
                os << stack.top().p->direction << ")\n";
            }
        }
    }
    //map output mode
    else {
        //loop through stack modifying symbols to directions
        size_t stacksize = stack.size();
        for (size_t i = 0; i < stacksize - 1; ++i) {
            c = stack.top();
            stack.pop();
            char printdirection = stack.top().p->direction;
            c.p->symbol = printdirection;
        }
        
        //loop through map and print
        for (int level = 0; level < numlevels; ++level) {
            os << "//level " << level << "\n";
            for (int row = 0; row < levelsize; ++row) {
                for (int col = 0; col < levelsize; ++col) {
                    char symbol = map[level][row][col].symbol;
                    if (symbol >= '\x00' && symbol < '\x0a') {
                        int x = (int)symbol;
                        os << x;
                    }
                    else {
                        os << map[level][row][col].symbol;
                    }
                }//for
                os << "\n";
            }//for
        }//for
    }//else
    
}





