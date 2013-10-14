#include <string>

#ifndef _TESTER_H__
#define _TESTER_H__
void unitTest();
void verify(std::vector<std::vector<char> > board,bool detailed);
void testDeadlocks(std::vector<std::vector<char> > board);
#endif
