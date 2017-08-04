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

using namespace std;
map< pair<int,string>, std::vector<int> > NFA_table;
vector<pair<vector<int>, bool>> e_closure;
map<pair<int, string>, int > DFA_table;
vector<vector<int>> states;
vector <pair<string, string>> classes;
map<string, vector<int>> terminals;
vector<int> non_terminals;
vector<pair<int, string>> acceptor;
vector <pair<vector<int>,bool>> minimization_gps;
map<pair<int, string>, int > minimized_table;
vector<vector<int>> minimized_states;
map<string, vector<int>> minimized_terminals;
vector <string> tokens;
int counter = 0;
ofstream myfile2 ("dfa.txt");

///////////////////////////////////////////////////////////
int globalCounter = 0;
string lamda = "lamda";
std::vector<int> starters ;
///////////////////////////////////////////////////////////
int globalStart = 25;              // for now
int numberOfStates = 44;
int numberOfClasses = 68;

int table [44][68];

map<string,int> classesMap;
vector < pair <string, pair<int, int> > > finalClasses;
vector <string> sTable;
string idString = "id";
vector< pair<int, string > > finalAcceptor ;   // from 3laia part
///////////////////////////////////////////////////////////////////
struct element
{
    int startNode;
    int lastNode;
    bool done = false;
    string grammer;
};

int priority(char a)
{
    int temp;
    if (a == '*' || a == '+')
        temp = 1;
    else  if (a == '&')
        temp = 2;
    else  if (a == '|')
        temp = 3;
    return temp;
}

string infix2postfix(string infix)
{
    stack<char> operator_stack;

    stringstream output;

    for (unsigned i = 0; i < infix.length(); i++)
    {
        if (infix[i] == '+' || infix[i] == '&' || infix[i] == '*' || infix[i] == '|' )
        {
            while (!operator_stack.empty() && priority(operator_stack.top()) <= priority(infix[i]))
            {
                output << " "<<operator_stack.top()<<" ";
                operator_stack.pop();
            }
            operator_stack.push(infix[i]);
        }
        else if (infix[i] == '(')
        {
            operator_stack.push(infix[i]);
        }
        else if (infix[i] == ')')
        {
            while (operator_stack.top() != '(')
            {
                output << " "<<operator_stack.top()<<" ";
                operator_stack.pop();
            }
            operator_stack.pop();
        }
        else
        {
            output <<infix[i];
        }
    }

    while (!operator_stack.empty())
    {
        output << " "<<operator_stack.top()<<" ";
        operator_stack.pop();
    }

    return output.str();
}

void checkClessInsert(string grammer)
{
    bool enter = false;

    for (size_t i = 0; i < classes.size(); i++)
    {
        string s = classes[i].first;

        if( s.compare(grammer) == 0)
        {
            enter = true;
        }
    }

    if(!enter)
    {
        if(grammer != "lamda")
        {
            pair<string, string> p(grammer, grammer);
            classes.push_back(p);
        }
    }
}

void checkIfDone(element e1, element e2)
{
    if(!e1.done)
    {
        std::vector<int> s ;
        s.push_back(e1.lastNode);
        if(e1.grammer == "letter")
        {
            for(char i = 'a'; i<= 'z'; i++)
            {
                stringstream ss;
                string sss;
                ss<< i;
                ss>>sss;
                NFA_table.insert( std::pair<pair<int,string>, std::vector<int> >(make_pair(e1.startNode,sss),s));
                checkClessInsert(sss);
            }
            for(char i = 'A'; i<= 'Z'; i++)
            {
                stringstream ss;
                string sss;
                ss<< i;
                ss>>sss;
                NFA_table.insert( std::pair<pair<int,string>, std::vector<int> >(make_pair(e1.startNode,sss),s));
                checkClessInsert(sss);
            }
        }
        else
        {
            NFA_table.insert( std::pair<pair<int,string>, std::vector<int> >(make_pair(e1.startNode,e1.grammer),s));
            checkClessInsert(e1.grammer);
        }

        e1.done = true;

    }
    if(!e2.done)
    {
        std::vector<int> s2 ;
        s2.push_back(e2.lastNode);
        if(e2.grammer == "letter")
        {
            for(char i = 'a'; i<= 'z'; i++)
            {
                stringstream ss;
                string sss;
                ss<<i;
                ss>>sss;
                NFA_table.insert( std::pair<pair<int,string>, std::vector<int> >(make_pair(e2.startNode,sss),s2));
                checkClessInsert(sss);
            }

            for(char i = 'A'; i<= 'Z'; i++)
            {
                stringstream ss;
                string sss;
                ss<<i;
                ss>>sss;
                NFA_table.insert( std::pair<pair<int,string>, std::vector<int> >(make_pair(e2.startNode,sss),s2));
                checkClessInsert(sss);
            }
        }
        else
        {
            NFA_table.insert( std::pair<pair<int,string>, std::vector<int> >(make_pair(e2.startNode,e2.grammer),s2));
            checkClessInsert(e2.grammer);
        }
        e2.done = true;
    }
}

void checkIfDone(element e1)
{

    stringstream ss;
    string sss;

    if(!e1.done)
    {
        std::vector<int> s ;
        s.push_back(e1.lastNode);
        if(e1.grammer == "letter")
        {
            for(char i = 'a'; i<= 'z'; i++)
            {
                ss<< i;
                ss>>sss;
                NFA_table.insert( std::pair<pair<int,string>, std::vector<int> >(make_pair(e1.startNode,sss),s));
                checkClessInsert(sss);
            }

            for(char i = 'A'; i<= 'Z'; i++)
            {
                ss<< i;
                ss>>sss;
                NFA_table.insert( std::pair<pair<int,string>, std::vector<int> >(make_pair(e1.startNode,sss),s));
                checkClessInsert(sss);
            }
        }
        else
        {
            NFA_table.insert( std::pair<pair<int,string>, std::vector<int> >(make_pair(e1.startNode,e1.grammer),s));
            checkClessInsert(e1.grammer);
        }

        e1.done = true;

    }
}

void makeOr(element e1, element e2)
{

    globalCounter++;
    vector<int> s ;
    s.push_back(e1.startNode);
    s.push_back(e2.startNode);
    NFA_table.insert( std::pair<pair<int,string>, std::vector<int> >(make_pair(globalCounter, lamda ),s));

    globalCounter++;
    pair <int, string> p1 (e1.lastNode, lamda );
    NFA_table[p1].push_back(globalCounter);

    pair <int, string> p2 (e2.lastNode, lamda );
    NFA_table[p2].push_back(globalCounter);

}
void makeAnd(element e1, element e2)
{

    pair <int, string> p1 (e1.lastNode, lamda );
    NFA_table[p1].push_back(e2.startNode);

}

void makeAstric(element e1)
{

    pair <int, string> p2 (e1.lastNode, lamda );
    NFA_table[p2].push_back(e1.startNode);
    globalCounter++;
    vector<int> s ;
    s.push_back(e1.startNode);
    s.push_back(globalCounter+1);
    NFA_table.insert( std::pair<pair<int,string>, std::vector<int> >(make_pair(globalCounter, lamda ),s));

    globalCounter++;
    pair <int, string> p1 (e1.lastNode, lamda );
    NFA_table[p1].push_back(globalCounter);

}

void makePlus(element e1)
{

    pair <int, string> p2 (e1.lastNode, lamda );
    NFA_table[p2].push_back(e1.startNode);

}

void createTableNFA(char s[], string accept)
{
    std::stack<element> stackOfElements;
    element e1;
    element e2;
    element e3 ;
    char *pch;

    pch = strtok (s," ");

    while (pch != NULL)
    {
        std::string str(pch);

        if(str.compare("|") == 0)
        {
            e1.startNode = stackOfElements.top().startNode;
            e1.lastNode = stackOfElements.top().lastNode;
            e1.grammer = stackOfElements.top().grammer;
            e1.done = stackOfElements.top().done;
            stackOfElements.pop();

            e2.startNode = stackOfElements.top().startNode;
            e2.lastNode = stackOfElements.top().lastNode;
            e2.grammer = stackOfElements.top().grammer;
            e2.done = stackOfElements.top().done;
            stackOfElements.pop();

            checkIfDone(e1,e2);
            makeOr(e1,e2);


            e3.startNode = globalCounter-1;
            e3.lastNode = globalCounter;
            e3.done = true;

            stackOfElements.push(e3);

        }
        else if (str.compare("&") == 0)
        {
            e2.startNode = stackOfElements.top().startNode;
            e2.lastNode = stackOfElements.top().lastNode;
            e2.grammer = stackOfElements.top().grammer;
            e2.done = stackOfElements.top().done;
            stackOfElements.pop();

            e1.startNode = stackOfElements.top().startNode;
            e1.lastNode = stackOfElements.top().lastNode;
            e1.grammer = stackOfElements.top().grammer;
            e1.done = stackOfElements.top().done;
            stackOfElements.pop();

            checkIfDone(e1,e2);
            makeAnd(e1, e2);

            e3.startNode = e1.startNode;
            e3.lastNode = e2.lastNode;
            e3.done = true;

            stackOfElements.push(e3);

        }
        else if (str.compare("*") == 0  && stackOfElements.size() > 0 )
        {
            e1.startNode = stackOfElements.top().startNode;
            e1.lastNode = stackOfElements.top().lastNode;
            e1.grammer = stackOfElements.top().grammer;
            e1.done = stackOfElements.top().done;
            stackOfElements.pop();

            checkIfDone(e1);
            makeAstric(e1);

            e3.startNode = globalCounter-1;
            e3.lastNode = globalCounter;
            e3.done = true;

            stackOfElements.push(e3);

        }
        else if (str.compare("+") == 0 && stackOfElements.size() > 0 )
        {
            e1.startNode = stackOfElements.top().startNode;
            e1.lastNode = stackOfElements.top().lastNode;
            e1.grammer = stackOfElements.top().grammer;
            e1.done = stackOfElements.top().done;
            stackOfElements.pop();

            checkIfDone(e1);
            makePlus(e1);

            e3.startNode = e1.startNode;
            e3.lastNode = e1.lastNode;
            e3.done = true;

            stackOfElements.push(e3);

        }
        else
        {
            element temp1;
            temp1.grammer = str;
            temp1.startNode = ++globalCounter;
            temp1.lastNode = ++globalCounter;
            stackOfElements.push(temp1);
        }

        pch = strtok (NULL, " ");

    }

    // empty the stack
    e1.startNode = stackOfElements.top().startNode;
    e1.lastNode = stackOfElements.top().lastNode;
    e1.grammer = stackOfElements.top().grammer;
    e1.done = stackOfElements.top().done;
    stackOfElements.pop();

    checkIfDone(e1);

    pair <int, string> p2 (e1.lastNode, accept );
    acceptor.push_back(p2);

    starters.push_back(e1.startNode);


}
bool hasEnding (std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length())
    {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    }
    else
    {
        return false;
    }
}

void spiltStringOnChar(string input)
{

    std::stringstream test(input);
    std::string segment;
    std::getline(test, segment, ':');
    string accept = segment;
    std::getline(test, segment, ':');
    char tab2[100];
    string s = infix2postfix(segment);
    strcpy(tab2, s.c_str());
    createTableNFA(tab2,accept);
}
void splitClasses(string input)
{
    char s[100];
    char *p;
    strcpy(s,input.c_str());
    p = strtok(s," =");

    std::string className (p);
    std::stringstream test(input);
    std::string segment;
    std::getline(test, segment, '=');
    std::getline(test, segment, '=');
    if(className == "letter" )
    {
        // pair <string, string> p1 (className, segment );
        //classes.push_back(p1);
    }
    else if(className == "digits" )
    {
        //pair <string, string> p1 (className, segment );
        //classes.push_back(p1);
    }
    else
    {
        pair <string, string> p1 (className, segment );
        classes.push_back(p1);
    }
}

void handlingKeyWord(string s)
{
    stringstream output;

    for(unsigned i = 0 ; i < s.size() ; i++)
    {
        if(s.at(i) != '{' && s.at(i) != '}')
        {
            output << s.at(i);
        }
    }

    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(output, segment, ' '))
    {
        seglist.push_back(segment);
    }
    for(vector<string>::iterator t=seglist.begin(); t!=seglist.end(); ++t)
    {
        stringstream output2;
        string j = *t;

        if(j.size() != 0)
        {
            for(unsigned k = 0 ; k < j.size() ; k++)
            {
                if(k < j.size()-1 )
                    output2 << j.at(k) << " & ";
                else
                    output2 << j.at(k);
            }

            string ss = infix2postfix(output2.str());
            char c[100];
            strcpy(c, ss.c_str());
            createTableNFA(c, j);
        }
    }
}

void handSpecialChar(string s)
{
    stringstream output;

    for(unsigned i = 0 ; i < s.size() ; i++)
    {
        if(s.at(i) != '[' && s.at(i) != ']')
        {
            output << s.at(i);
        }
    }
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(output, segment, ' '))
    {
        seglist.push_back(segment);
    }
    for(vector<string>::iterator t=seglist.begin(); t!=seglist.end(); ++t)
    {
        stringstream output2;
        string j = *t;

        char c[100];
        strcpy(c, j.c_str());
        createTableNFA(c, j);
    }
}

void parseLine(char s[])
{
    string line = s;
    char charArray[100];
    strcpy(charArray,s);
    char *pch;
    pch = strtok (s," ");

    std::string str(pch);

    if(hasEnding(str,":"))
    {
        spiltStringOnChar(line);
    }
    else
    {
        if(str.at(0) == '{')
        {
            handlingKeyWord(line);
        }
        else if(str.at(0) == '[')
        {
            handSpecialChar(line);
        }
        else
        {
            splitClasses(line);
        }
    }
}

string putAndSplit(string input)
{
    std::stringstream test(input);
    std::string segment;
    std::getline(test, segment, ':');
    string accept = segment;
    std::getline(test, segment, ':');
    return segment;
}
string putAnds(string line)
{
    string line2 = line;
    char s[100];
    char * pch;
    strcpy(s,line.c_str());
    pch = strtok (s," ");

    std::string str2(pch);

    stringstream output;

    if(hasEnding(str2,":"))
    {

        string str3 = putAndSplit(line);

        output << str2<<" ";

        for(unsigned i = 0; i<str3.size(); i++)
        {

            char c = str3.at(i);

            if(c == '(' )
            {
                output<< " & ( ";
            }
            else if(c == ')')
            {
                output<< " ) ";
            }
            else if(c == '|')
            {
                output<< " | ";
            }
            else if(c == '*')
            {
                output<< " * ";
            }
            else if(c == '.')
            {
                output<< " & .  & ";
            }
            else if(c == 'L')
            {
                output<< "lamda";
            }
            else if(c == 's' && str3.at(i-1) == 't')
            {
                output<< "+";
            }
            else if(c == 'E' )
            {
                output<< " E & ";
            }
            else if(i != 0 && !( ( c >= 'a' && c <= 'z' ) || (c >= 'A' && c <= 'Z') ) && i != str3.size()-1&& str3.at(i+1) == '=')
            {
                output<< c <<" & ";
            }
            else
            {
                output << c;
            }

        }
    }
    else
    {
        return line2;
    }
    return output.str();

}

string removeSlash(string line)
{

    stringstream output;
    for(unsigned i = 0; i < line.size() ; i++)
    {

        if(line.at(i) != '\\')
        {
            output<<line.at(i);
        }
    }

    return output.str();
}

//////////////////////////////////////////////////////////
//get e_closure of each node recursively

vector<int> find_e_closure(int state)
{
    vector<int> result;
    vector<int> result2;
    vector<int>  state_eclosure;
    int temp;
    auto it = NFA_table.find(make_pair(state, "lamda"));
    if(it != NFA_table.end())
    {
        state_eclosure = it -> second;
    }
    else
    {
        //base case, if no e-closure found return
        return result;
    }
    for(int i = 0; i< state_eclosure.size(); i++)
    {
        temp = state_eclosure.at(i);
        result.push_back(temp);
        if(e_closure.at(temp).second == false)
        {
            result2 = find_e_closure(temp);
            e_closure.at(temp).second = true;
        }
        else
        {
            result2 = e_closure.at(temp).first;
        }

        result.insert(result.end(), result2.begin(), result2.end());
        e_closure.at(state).first = result;
    }
    return result;
}


int check_repeated_state(vector<int> new_state)
{
    int counter = 0;
    for(int i = 0; i < states.size(); i++)
    {
        if(new_state.size() == states.at(i).size())
        {
            for(int k = 0; k < new_state.size(); k++)
            {
                for(int n = 0; n < new_state.size(); n++)
                {
                    if(new_state.at(k) == states.at(i).at(n))
                    {
                        counter++;
                        break;
                    }
                }
            }

            if(counter == new_state.size())
            {
                return i;
            }
            counter = 0;

        }
    }

    return -1;
}

void consruct_DFA_table(vector<int> vect)
{
    vector<int> new_state;
    vector<int> temp;
    vector<int> temp2;
    for(int j = 0; j < classes.size(); j++)
    {
        for(int i = 0; i < vect.size(); i++)
        {
            auto it = NFA_table.find(make_pair(vect.at(i), classes.at(j).first));
            if(it != NFA_table.end())
            {
                temp = it -> second;
            }
            for(int k = 0; k < temp.size(); k++)
            {
                new_state.push_back(temp.at(k));
                temp2 = e_closure.at(temp.at(k)).first;
                new_state.insert(new_state.end(), temp2.begin(), temp2.end());
            }
            temp.clear();
        }

        int repeated = check_repeated_state(new_state);
        if(repeated != -1)
        {
            DFA_table.insert(std::pair<pair<int,string>, int>
                             (make_pair(counter,  classes.at(j).first), repeated));
            myfile2 << counter << "/"<<  classes.at(j).first << "/"<<repeated << endl;
            new_state.clear();
        }
        else
        {
            if(new_state.size() != 0)
            {
                DFA_table.insert(std::pair<pair<int,string>, int>
                                 (make_pair(counter,  classes.at(j).first),  states.size()));
                myfile2 << counter << "/"<<  classes.at(j).first << "/"<< states.size() << endl;
                states.push_back(new_state);
                new_state.clear();
            }
        }
    }
    counter++;
}

void find_DFA()
{
    for(int i = 0; i < globalCounter+1; i++)
    {
        find_e_closure(i);
    }
    vector<int> e_closure0 = e_closure.at(0).first;
    e_closure0.push_back(0);
    states.push_back(e_closure0);
    int old_size = states.size();
    consruct_DFA_table(states.at(0));
    int new_size = states.size();
    int temp ;
    while(new_size - old_size > 0)
    {
        temp = states.size();
        for(int k = old_size; k < new_size; k++)
        {
            consruct_DFA_table(states.at(k));
        }
        old_size = temp ;
        new_size = states.size();
    }
}

void delete_repeated_states ()
{
    vector<int> temp2 =terminals["id"];
    vector <int> copy_of = temp2;

    for(auto iterate = terminals.cbegin(); iterate != terminals.cend(); ++iterate)
    {
        if((iterate-> first) != "id")
        {
            vector<int> t = terminals[iterate -> first];

            for (int i = 0 ; i < t.size(); i++)
            {

                for (int j = 0 ; j < temp2.size(); j++)
                {

                    if(t.at(i) == temp2.at(j))
                    {
                        std::vector<int>::iterator it;
                        for(it = copy_of.begin(); it < copy_of.end() ; it++)
                        {
                            if(temp2.at(j) == *it)
                            {
                                break;
                            }
                        }
                        if(it != copy_of.end())
                            copy_of.erase(it);
                    }

                }

            }

        }

    }
    terminals["id"]= copy_of;
}


void find_terminals()
{
    bool found = false;
    for(int i = 0; i < states.size(); i++)
    {
        for(int j = 0; j < states.at(i).size(); j++)
        {

            for(int k = 0; k < acceptor.size(); k++)
            {
                // if you find it in acceptor vector given from NFA phase then it is terminal
                if(acceptor.at(k).first == states.at(i).at(j))
                {
                    terminals[acceptor.at(k).second].push_back(i);
                    found = true;
                    break;
                }
            }
        }
        if(!found) // if not terminal put it in non terminal vector
        {
            non_terminals.push_back(i);
        }
        found = false;

    }
    // delete repeated states from different groups
    delete_repeated_states();
}

void collect_minimization_groups()
{
    for(auto it = terminals.cbegin(); it != terminals.cend(); ++it)
    {
        vector<int> temp = it->second;
        if (temp.size() != 0 )
        {
            minimization_gps.push_back(make_pair(it->second,true));

        }
    }
    minimization_gps.push_back(make_pair(non_terminals,true));
}

bool partition_classes(vector<int> state, vector<int> original, int index)
{
    map<int, vector<int>> gps;
    int old_j = -1,new_j;
    bool different_groups = false;
    for(int j = 0; j < original.size(); j++)
    {
        gps[j];
    }
    int new_size = minimization_gps.size();
    int amada = state.size();
    bool found = false;
    bool phi_is_exist = false;
    for(int i = 0; i < state.size(); i++)
    {
        found =false;
        for(int j = 0; j < new_size && !found && state.at(i) != -1; j++)
        {
            for(int k = 0; k < minimization_gps.at(j).first.size(); k++)
            {

                if(minimization_gps.at(j).second && state.at(i) == minimization_gps.at(j).first.at(k))
                {
                    new_j = j;
                    if(new_j != old_j && old_j != -1)
                    {
                        different_groups = true;
                    }
                    gps[j].push_back(original.at(i));
                    old_j = j ;
                    found = true;
                    break;

                }

            }

        }

    }
    bool check_different = false;
    for(int z = 0; z < state.size(); z++)
    {
        if(state.at(z) == -1)
        {
            phi_is_exist = true;
        }
        else
        {
            check_different = true;
        }
    }

    if(different_groups || (check_different && phi_is_exist))
    {
        minimization_gps.at(index).second = false;
        for(auto it = gps.cbegin(); it != gps.cend(); ++it)
        {
            if(it->second.size() != 0)
                minimization_gps.push_back(make_pair(it->second,true));
        }
        vector<int> vect;
        for(int i = 0 ; i < state.size(); i++)
        {
            if(state.at(i) == -1)
            {
                vect.push_back(original.at(i));
            }
        }
        minimization_gps.push_back(make_pair(vect,true));
    }
    return different_groups || (check_different && phi_is_exist) ;
}

void minimize_DFA()
{
    bool enter_different_groups= false;
    find_terminals();
    collect_minimization_groups();
    vector<int> output;
    int old_size = minimization_gps.size();
    int temp_size;
    for(int i = 0; i < old_size ; i++)
    {
        if(minimization_gps.at(i).second == true && minimization_gps.at(i).first.size()!=1 )
        {
            for(int k = 0; k < classes.size(); k++)

            {
                output.clear();
                temp_size = minimization_gps.at(i).first.size();
                for(int j = 0; j < temp_size; j++)

                {
                    auto it = DFA_table.find(make_pair(minimization_gps.at(i).first.at(j), classes.at(k).first));

                    if(it != DFA_table.end())
                    {
                        output.push_back(it -> second);

                    }
                    else
                    {
                        output.push_back(-1);

                    }

                }

                if(output.size() != 0 )
                    enter_different_groups = partition_classes(output, minimization_gps.at(i).first,i);
                if(enter_different_groups)
                {
                    enter_different_groups = false;
                    break;
                }
                output.clear();
            }
        }
    }
    output.clear();
    int new_size = minimization_gps.size();
    int temp ;
    while(new_size - old_size > 0)
    {

        temp = minimization_gps.size();
        for(int w = old_size; w < new_size; w++)
        {
            if(minimization_gps.at(w).second == true && minimization_gps.at(w).first.size()!= 1 )
            {
                for(int k = 0; k < classes.size(); k++)

                {
                    output.clear();
                    int temporary_size =  minimization_gps.at(w).first.size();
                    for(int j = 0; j < temporary_size ; j++)

                    {
                        auto it = DFA_table.find(make_pair(minimization_gps.at(w).first.at(j), classes.at(k).first));
                        if(it != DFA_table.end())
                        {
                            output.push_back(it -> second);
                        }
                        else
                        {
                            output.push_back(-1);

                        }

                    }
                    if(output.size() != 0 )
                        enter_different_groups = partition_classes(output, minimization_gps.at(w).first,w);

                    if(enter_different_groups)
                    {
                        enter_different_groups = false;
                        break;
                    }
                    output.clear();
                }
            }
        }

        old_size = temp ;
        new_size = minimization_gps.size();
    }

}

int find_state(int state)
{
    for(int i = 0; i < minimized_states.size(); i++)
    {
        for(int j = 0; j < minimized_states.at(i).size(); j++)
        {
            if(minimized_states.at(i).at(j) == state)
            {
                return i;
            }

        }

    }
}
void construct_minimized_table()
{
    int counter  =0;
    for(int j = 0 ; j < minimization_gps.size() ; j++)
    {
        if( minimization_gps.at(j).second == true && minimization_gps.at(j).first.size()!=0)
        {

            minimized_states.push_back(minimization_gps.at(j).first);
        }
    }
    ofstream myfile ("minimized.txt");
    for(int i = 0; i < minimized_states.size(); i++)
    {
        if ( minimized_states.at(i).size() != 0)
        {
            int state  = minimized_states.at(i).at(0);

            for(int j = 0; j < classes.size(); j++)
            {

                auto it = DFA_table.find(make_pair(state, classes.at(j).first));

                if(it != DFA_table.end())
                {

                    minimized_table.insert(std::pair<pair<int,string>, int>
                                           (make_pair(i,  classes.at(j).first),  find_state(it->second)));

                    myfile << "from "<< i << " input: "<<  classes.at(j).first << " to "<< find_state(it->second) << endl;

                }
            }
        }

    }

}

void construct_terminals()
{
    bool found = false;
    for(auto iterate = terminals.cbegin(); iterate != terminals.cend(); ++iterate)
    {
        for(int i = 0; i < iterate->second.size(); i++)
        {
            found = false ;
            int state = find_state(iterate->second.at(i));
            vector<int> min_group = minimized_terminals[iterate->first];
            for (int j =0 ; j< min_group.size() ; j ++)
            {
                if (min_group.at(j) == state )
                {
                    found = true;
                }
            }
            if(!found)
            {
                minimized_terminals[iterate->first].push_back(state);
            }

        }
    }
}
/////////////////////////////////////////////////////////
void convertTable()
{
    for(int i = 0; i < numberOfStates ; i++ )
    {
        for(int j = 0; j < numberOfClasses ; j++ )
        {
            table[i][j] = numberOfStates;

        }

    }

    int classNumber = -1;
    cout<<" size" << DFA_table.size()<<endl;
    for(auto it = minimized_table.cbegin(); it != minimized_table.cend(); it++)
    {
        //cout << " mynumber"<<(*it).first.first;
        classNumber = classesMap[(*it).first.second];
        table[(*it).first.first][classNumber] = (*it).second;

    }
    }

int findClass(char c)
{
    int i,j, cStart, cEnd;

    for(i = 0; i< finalClasses.size() ; i++)
    {

        cStart = finalClasses[i].second.first ;
        cEnd = finalClasses[i].second.second ;
        if(cStart <= c && c <= cEnd)
        {
            return i;
        }

    }
    //cout << "not found \n" ;
    return -1;
}



void getFinalClasses()
{

    for(int i = 0; i < classes.size(); i++)
    {

        if(classes[i].second.length() ==1)
        {
            pair<int, int> p1 (classes[i].second[0],classes[i].second[0]);

            pair<string, pair<int, int> > theClass(classes[i].first, p1);

            finalClasses.push_back(theClass);
        }
        else
        {

            if(classes[i].second.length() == 2)
            {

                pair<int, int> p1 (59,59);
                string s = ";";
                pair<string, pair<int, int> > theClass(s, p1);

                finalClasses.push_back(theClass);

            }
            else
            {

                pair<int, int> p1 (classes[i].second[1],classes[i].second[5]);
                pair<string, pair<int, int> > theClass(classes[i].first, p1);

                finalClasses.push_back(theClass);

            }
        }
        classesMap.insert(pair<string, int>(make_pair(classes[i].first, i)));

        cout << classes[i].first << "--" <<classes[i].second<<"\n";
    }


    for(int i = 0; i < finalClasses.size(); i++)
    {
        cout<< i << " "<<finalClasses[i].first <<" "<< finalClasses[i].second.first <<"--" << finalClasses[i].second.second << "\n";
    }

}

void runC ()
{
    ifstream inputFile("test.txt");     // open file
    ofstream output;

    output.open ("output.txt");

    char c,currentChar;
    int classNum = -1;
    int currentState = globalStart;
    vector < string > walkingAcceptor;
    vector <char > buffer;

    int readChar = 1;
    int maxMatch = 0, position = -1;
    string currentLexim = "";
    string lastAcceptor = "";

    int condition = inputFile.peek();

    while(condition != -1)
    {
        if(readChar == 1)
        {
            inputFile.get(c);
            currentChar = c;
            buffer.push_back(c);
            position++;
        }

        classNum = findClass(currentChar);        //get class number

        if(classNum != -1)
        {
            currentState = table[currentState][classNum];
           // cout<<" after: "<<currentState<<endl;

            if(currentState != numberOfStates)
            {

                string currentAccept ="";
                for(int i = 0 ; i < finalAcceptor.size(); i++)
                {
                    if(currentState == finalAcceptor[i].first)
                    {
                        currentAccept += finalAcceptor[i].second;  //ana b accept eh
                        break;
                    }
                }

                if(currentAccept != "")
                {
                    lastAcceptor = currentAccept;
                    maxMatch = position + 1;
                }

                if(position+1 == buffer.size())
                {
                    condition = inputFile.peek();
                    readChar = 1;
                }
                else
                {
                    readChar = 0;
                    currentChar = buffer[position];
                }

            }
            else
            {
                if(lastAcceptor != "")                          // ana b accept 7aga
                {
                    for(int i = 0; i < maxMatch; i++ )
                    {
                        currentLexim += buffer[0];
                        buffer.erase(buffer.begin());
                        position--;
                    }
                    tokens.push_back(lastAcceptor);
                    output << lastAcceptor <<"\n";
                    cout << lastAcceptor<< " -- " << currentLexim << endl;

                    if(lastAcceptor == "id")
                    {
                        sTable.push_back(currentLexim);
                    }

                    maxMatch =0;
                    lastAcceptor ="";
                    currentLexim ="";
                    currentState = globalStart;
                    if(position >= 0)
                    {
                        currentChar = buffer[0];
                        position = 0;
                        readChar = 0;
                    }
                    else
                    {
                        condition = inputFile.peek();
                        readChar = 1;
                    }

                }
                else                                // momkn lsa feh klam f al pos w curren char
                {
                    cout <<buffer[0] <<" error \n";
                    buffer.erase(buffer.begin());
                    currentState = globalStart;
                    if(buffer.size() > 0)
                    {
                        position = 0;
                        currentChar = buffer[0];
                        readChar = 0;
                    }
                    else
                    {
                        position = -1;
                        condition =  inputFile.peek();
                        readChar = 1;
                    }


                }

            }

        }
        else
        {

            if(lastAcceptor != "")                          // ana b accept 7aga
            {
                for(int i = 0; i < maxMatch; i++ )
                {
                    currentLexim += buffer[0];
                    buffer.erase(buffer.begin());
                    position--;
                }
               tokens.push_back(lastAcceptor);
                output << lastAcceptor <<"\n";
                cout << lastAcceptor<< " -- " << currentLexim << endl;

                if(lastAcceptor == "id")
                {
                    sTable.push_back(currentLexim);
                }

                maxMatch =0;
                lastAcceptor ="";
                currentLexim ="";
                currentState = globalStart;
                if(position >= 0)
                {
                    currentChar = buffer[0];
                    readChar = 0;
                }
                else
                {
                    condition = inputFile.peek();
                    readChar = 1;
                }

            }
            else if((currentChar == ' '||currentChar== '\n'||currentChar== '\t'))
            {
                if(buffer.size() >1)
                {
                    cout <<buffer[0] <<" error \n";
                }
                currentState = globalStart;

                buffer.clear();
                position = -1;
                condition = inputFile.peek();
                readChar = 1;
            }
            else
            {
                cout << currentChar << "error \n";
                currentState = globalStart;

                buffer.clear();
                position = -1;
                condition = inputFile.peek();
                readChar = 1;
            }


        }


    }

    if(lastAcceptor != "")
    {
        for(int i = 0; i < maxMatch; i++ )
        {
            currentLexim += buffer[0];
            buffer.erase(buffer.begin());
        }
        tokens.push_back(lastAcceptor);
        output << lastAcceptor <<"\n";
        cout << lastAcceptor<< " -- " << currentLexim << endl;
        if(lastAcceptor == "id")
        {
            sTable.push_back(currentLexim);
        }

    }

    inputFile.close();                // close file
    output.close();
    cout << "/////////////////////////////"<<endl;
    cout << "\n\nsTable" << "\n";
    for(int i =0; i<sTable.size(); i++)
    {
        cout << sTable[i] << "\n";
    }
}

void convertAcceptor()
{
    cout << "map size" << terminals.size() << "\n";
    for(auto it = minimized_terminals.cbegin(); it != minimized_terminals.cend(); it++)
    {
        cout << (*it).second.size() << "--" << (*it).first <<"\n" ;

        for(int i = 0 ; i< (*it).second.size() ; i++ )
        {

            pair<int, string> pa ((*it).second[i],(*it).first);
            finalAcceptor.push_back(pa);
        }

    }

    for(int i = 0 ; i< finalAcceptor.size() ; i++ )
    {
        cout << "aaaaa "<< finalAcceptor[i].first << " " << finalAcceptor[i].second<<"\n";
    }

}


////////////////////////////////////////////////////////

int main()
{
    char s[100] ;

    std::ifstream input( "grammar.txt");
    for( std::string line; getline( input, line ); )
    {
        if (line[line.size() - 1] == '\r')
            line.resize(line.size() - 1);

        line = removeSlash(line);
        string line2 = putAnds(line);
        parseLine(strcpy(s, line2.c_str()));

    }
    NFA_table.insert( std::pair<pair<int,string>, std::vector<int> >(make_pair(0,lamda),starters));

    for(int i = 0; i<globalCounter+1; i++)
    {
        vector<int> garbage;
        e_closure.push_back(make_pair(garbage, false));
    }

    for(int j = 0; j < classes.size(); j++)
    {
        terminals[classes.at(j).first];
        minimized_terminals[classes.at(j).first];
    }
    find_DFA();
    minimize_DFA();
    construct_minimized_table();
    construct_terminals();
    cout <<"1\n";
    convertAcceptor();
    cout <<"2\n";
    getFinalClasses();
    cout <<"3\n";
    convertTable();
    cout <<"4\n";
    runC();

    tokens.push_back("$");
    Parser p;
    p.mainEngineForParser();
    return 0;
}
