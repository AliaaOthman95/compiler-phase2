#ifndef PARSER_H
#define PARSER_H
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

class Parser
{
public:
    Parser();
    virtual ~Parser();
    void mainEngineForParser(void);

    map< pair<string,string>, vector<string> > parsing_table;


protected:
    map< string, vector < vector < string> > > productionsMap;
    vector< string > nonTerminals;
    vector< string > terminals;
    map< string, vector<string>> first;
    map< string, vector<string>> follow;
    //vector <string> terminals_symbols;
    //map< pair<string,string>, std::vector<string> > parsing_table;
    stack <string> parsing_stack;
    string starting_symbol;

    void getProductions(vector<string> productions);
    void getFirst();
    void getProductionsForFirst(vector<vector<string>> production, string nonTerminal);
    bool findInTerminal(string terminal);

    void getFollow(int NTIndex);
    void populateFollow();
    void addToFollow(string key,vector<string> data);
    int  getNTIndex(string nonTerminal);

    void createTable();
    int  findProduction(string start, vector < vector < string> > productions);
    bool hasDSign(vector<string> vec);
    bool is_terminal(string token);
    string get_next_token(int index);
    void parse_tokens();
    bool isSemicolon(string input);


private:

};

#endif // PARSER_H
