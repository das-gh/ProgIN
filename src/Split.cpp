/*
    http://stackoverflow.com/questions/236129/split-a-string-in-c
*/

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

string Split(const string& s, int elem)
{
    istringstream iss(s);
    string sub;

    for(int i = 0 ; i <= elem ; i++)
    {
        sub = "";
        iss >> sub;
    }
    return sub;
};
