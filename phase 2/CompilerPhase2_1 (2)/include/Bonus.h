#ifndef BONUS_H
#define BONUS_H
#include <iostream>
#include <stack>
#include <stdio.h>
#include <string.h>
#include <map>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;
class Bonus
{
    public:
        Bonus();
        virtual ~Bonus();
        void getProductions(vector<string> productions);
        void eliminate_left_recursion();
        vector <int> detect_left_refactor(vector<vector<string>> production);
        bool found_in_indexes(int index, vector<int> indexes);
        void eliminate_left_factoring();
        void engine();
        bool is_non_terminal(string token);
        void print_dash (string left_side);

    protected:

    private:
        vector <pair< string, vector < vector < string>>>> productions_vector;
        vector <pair< string, vector < vector < string>>>> temp_productions;
        vector<string> non_terminals;


};

#endif // BONUS_H
