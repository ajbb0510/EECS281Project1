//
//  main.cpp
//  eecs281project1
//
//  Created by Andrew Bennett-Belch on 5/2/17.
//  Copyright © 2017 Andrew Bennett-Belch. All rights reserved.
//


#include <stdlib.h>
#include <getopt.h>
#include "Station.h"

using namespace std;


int main(int argc, char *argv []) {
    ios_base::sync_with_stdio(false);
    
    if (getenv("INPUT")) {
        freopen(getenv("INPUT"), "r", stdin);
        freopen(getenv("OUTPUT"), "w", stdout);
    } // if
    ostringstream os;
    
    opterr = true; //help with errors
    struct option longopts[] = {
        { "help", no_argument, nullptr, 'h' },
        { "queue", no_argument, nullptr, 'q'},
        { "stack", no_argument, nullptr, 's'},
        { "output", required_argument, nullptr, 'o'},
        { nullptr, 0, nullptr, '\0' }
    };//struct
    
    int index = 0;
    int c; //single char for option
    //presence of command line arguments
    bool spresent = false;
    bool qpresent = false;
    string oarg;
    
    // use getopt long to determine what is on command line
    while ((c = getopt_long(argc, argv, "hqso:", longopts, &index)) != -1) {
        switch (c) {
            case 'h':
                os << "This program finds the way back to your old spacecraft\n"
                    << "Include flags:/n" << "-help or -h to see error message and exit/n"
                    << "-queue or -q to use queue-based routing/n" << "-stack or -s to use stack-based routing/n"
                    << "-output or -o with argument M or L to specify output mode map or list";
                return 0;
            case 'q':
                qpresent = true;
                break;
            case 's':
                spresent = true;
                break;
            case 'o':
                oarg = optarg;
                break;
        }//switch
    }//while
    
    
    //command line error checking
    if ((qpresent) && (spresent)) {
        cerr << "Do not specify both stack and queue modes";
        exit(1);
    }
    
    if ((!qpresent) && (!spresent)) {
        cerr << "Must specify either stack or queue mode";
        exit(1);
    }
    
    //Read in input mode
    char inputmode;
    cin >> inputmode;
    
    //create station and hangar
    Station station(inputmode);
    Coordinate hangar;

    hangar = station.findhangar(qpresent);
    
    //check for list or output mode
    bool lpresent = false;
    if (oarg == "L") {
        lpresent = true;
    }
    
    //If there is no valid route
    if (hangar.p == nullptr) {
        if (lpresent) {
            os << station.numlevels << "\n" << station.levelsize << "\n" << "//path taken\n";
        }
        else {
            os << station.numlevels << "\n" << station.levelsize << "\n";
            for (int level = 0; level < station.numlevels; ++level) {
                os << "//level " << level << "\n";
                for (int row = 0; row < station.levelsize; ++row) {
                    for (int col = 0; col < station.levelsize; ++col) {
                        os << station.map[level][row][col].symbol;
                    }//for
                    os << "\n";
                }//for
            }//for

        }//else
        cout << os.str();
        return 0;
    }//if
    
    //If there is a valid route
    station.output(hangar, os, lpresent);
    
    //dump string stream to standard out
    cout << os.str();
    return 0;
}







