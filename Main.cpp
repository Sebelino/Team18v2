#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "Sokoban.h"
//#include "Tester.h"

using namespace std;

/**
 * Performs unit tests if called with the test parameter:
 * ./sokoban test
 * and runs the program otherwise.
 */
int main(int argc, char **argv) {
    if(argc == 2 && argv[1] == string("test")){
        //runTests();
    }else{
        sokoban();
    }
    return 0;
}
