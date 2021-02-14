#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <cmath>
using namespace std;
#include <tree.h>
#include <tData.h>

void test();

int main()
{
    test();
    return 0;
}


void test()
{
    DecisionTree t;
    tData dat;
    dat.read_from_file("a.txt",' ');
    vector<int> r = {0,1,0,1,0,1,0};
    t.fit(dat,r);
    t.showTree();
}
