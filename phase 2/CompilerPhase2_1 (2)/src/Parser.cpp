#include "Parser.h"
#include "Bonus.h"
#include "main.cpp"
Parser::Parser()
{
    //ctor

}
Parser::~Parser()
{
    //dtor

}


void Parser::getProductions(vector<string> productions)
{

    string productionLine;
    char *pointerChar;
    char productionArray[100];
    //string productionName;


    /// # TERM = FACTOR | TERM ‘mulop’ FACTOR

    for(int i=0; i<productions.size(); ++i)
    {

        vector<vector<string>> productionvectors;

        productionLine = productions[i];

        strcpy(productionArray,productionLine.c_str()); // convert string line to char array (function need this)

        pointerChar = strtok (productionArray," ");
        pointerChar = strtok (NULL, " ");

        std::string productionName(pointerChar);

        pointerChar = strtok (NULL, " ");

        vector<string> linePro;
        nonTerminals.push_back(productionName);
        while(pointerChar != NULL)
        {

            std::string partOfProduction(pointerChar);

            if(partOfProduction.at(0) != '=')
            {

                if(partOfProduction.at(0) == '|')
                {
                    productionvectors.push_back(linePro);
                    linePro.clear();

                }
                else
                {
                    if(partOfProduction.at(0) == '\'')
                    {
                        partOfProduction = partOfProduction.substr(1, partOfProduction.size() - 2);
                        terminals.push_back(partOfProduction);

                    }
                    else
                    {
                        //nonTerminals.push_back(productionName);
                    }

                    linePro.push_back(partOfProduction);
                }

            }
            pointerChar = strtok (NULL, " ");
        }
        productionvectors.push_back(linePro);

        // for debuging only
        cout << "productionName: "<<productionName <<"conent of vector of vector"<<endl;
        for(int i = 0; i <productionvectors.size(); i++)
        {

            for(int j = 0; j < productionvectors[i].size(); j++)
            {
                cout<< productionvectors[i][j]<< ' ';
            }
            cout << endl;
            cout << endl;
        }
        // end of debuging

        productionsMap.insert(make_pair(productionName,productionvectors));
    }
}

void Parser::getFirst()
{

    for(int i = nonTerminals.size() - 1; i >=0; i--)
    {

        vector<vector<string>> productions = productionsMap[nonTerminals[i]];
        getProductionsForFirst(productions, nonTerminals[i]);
    }
    //for test only
    for(int i = nonTerminals.size()-1; i>=0 ; i--)
    {
        vector<string> v = first[nonTerminals[i]];
        cout<< "nonterminal name : "<<nonTerminals[i]<<endl;
        cout<<"its first:"<<endl;
        for(int j = 0 ; j < v.size(); j++)
        {

            cout<< j<<"-->"<<v[j]<<endl;
        }
    }
    //for test only

}
bool Parser::findInTerminal(string terminal)
{

    for(int i = 0; i <terminals.size(); i++)
    {
        if(terminal == terminals[i])
        {
            return true;
        }
    }
    return false;
}

void Parser::getProductionsForFirst(vector<vector<string>> production, string nonTerminal)
{
    vector<string> firstVector;
    for(int i = 0; i <production.size(); i++)
    {

        if(findInTerminal(production[i][0]))
        {
            firstVector.push_back(production[i][0]);
        }
        else
        {

            vector<string> vectorOfFirst = first[production[i][0]];
            vector<string> temp ;
            bool exithere = false;
            for(int i = 0 ; i < vectorOfFirst.size(); i++)
            {

                for(int j = 0 ; j < firstVector.size(); j++)
                {

                    if(vectorOfFirst[i] == firstVector[j])
                    {
                        exithere = true;
                    }
                }
                if(!exithere)
                {
                    temp.push_back(vectorOfFirst[i]);
                }
                exithere = false;
            }

            for(int j = 0 ; j < temp.size(); j++)
            {
                firstVector.push_back(temp[j]);
            }
        }
    }

    first.insert(make_pair(nonTerminal,firstVector));

}


//    map< pair<string,string>, vector<string> > parsing_table;

void Parser::createTable(void)
{
    vector < vector < string> > productions;
    vector < string> myFollow;
    vector < string> myFirst;
    int foundProduction;
    map< pair<string,string> , vector<string>>::iterator it;
    for(int row= 0; row < nonTerminals.size(); row++)
    {
        cout << nonTerminals[row] << endl;
        productions = productionsMap[nonTerminals[row]];
        myFollow = follow[nonTerminals[row]];
        myFirst = first[nonTerminals[row]];

        for(int col=0; col < myFirst.size(); col++)
        {
            if(myFirst[col] != "lamda")
            {
                if(productions.size()==1)
                {
                    pair<string, string> pr (nonTerminals[row],myFirst[col]);
                    parsing_table.insert(make_pair(pr,productions[0]));
                }
                else
                {
                    foundProduction = findProduction(myFirst[col], productions);
                    pair<string, string> pr (nonTerminals[row],myFirst[col]);
                    parsing_table.insert(make_pair(pr,productions[foundProduction]));
                }

            }
            else
            {
                /*
                if(hasDSign(myFollow))
                {
                    pair<string, string> pr (nonTerminals[row],"$");
                    vector<string> ve ;
                    ve.push_back("lamda");
                    parsing_table.insert(make_pair(pr,ve));
                }*/

                vector<string> ve ;
                ve.push_back("lamda");

                for(int i=0; i< myFollow.size(); i++)
                {
                    pair<string, string> pr (nonTerminals[row],myFollow[i]);
                    parsing_table.insert(make_pair(pr,ve));


                }


            }

        }
        vector<string> syncVe ;
        syncVe.push_back("sync");
        for(int k=0; k< myFollow.size(); k++)
        {
             it = parsing_table.find(make_pair(nonTerminals[row],myFollow[k]));

            if (it == parsing_table.end())
            {

                    pair<string, string> pr (nonTerminals[row],myFollow[k]);
                    parsing_table.insert(make_pair(pr,syncVe));

            }


        }

    }



    for(auto iterate = parsing_table.cbegin(); iterate != parsing_table.cend(); ++iterate)
    {
        cout << "[ "<<(*iterate).first.first << " ,"<< (*iterate).first.second << " ]"<<endl;
        for(int i=0;i<(*iterate).second.size();i++)
        {
            cout << ((*iterate).second)[i]<< " ";
        }
        cout<< endl<<endl;
    }



}



int Parser::findProduction(string start, vector < vector < string> > productions)
{
    for(int i=0; i<productions.size(); i++)
    {
        if(findInTerminal( (productions[i])[0]) )
        {
            if( (productions[i])[0] == start)
            {
                return i;
            }

        }
        else
        {
            for(int j=0; j< (first[(productions[i])[0]]).size(); j++)

                if(  (first[(productions[i])[0]])[j]  == start)
                {
                    return i;
                }

        }

    }

    cout << "erroooooooooooooooooooooor\n";
    return -1;

}

bool Parser::hasDSign(vector<string> vec)
{
    for(int i=0; i<vec.size(); i++)
    {
        if(vec[i]== "$")
        {
            return true;
        }
    }

    return false;
}

void Parser::populateFollow(void)
{
    vector<string> followVector;
    followVector.push_back("$");
    follow.insert(make_pair(nonTerminals[0],followVector));

    map<string, vector<string>>::iterator it;
    getFollow(0);

    for(int i = 1; i < nonTerminals.size(); i++)
    {
        it = follow.find(nonTerminals[i]);

        if (it == follow.end())
        {
            //          cout << nonTerminals[i] << " not found " << i <<endl;

            getFollow(i);

        }


    }

//    cout <<"after\n";


    for(int i = 0; i < nonTerminals.size(); i++)
    {
        sort( follow[nonTerminals[i]].begin(), follow[nonTerminals[i]].end() );
        follow[nonTerminals[i]].erase(unique( follow[nonTerminals[i]].begin(), follow[nonTerminals[i]].end() ), follow[nonTerminals[i]].end() );
    }

    /*   ofstream output;

       output.open ("follow2.txt");

       for(int i = 0; i < nonTerminals.size(); i++)
       {
           output <<nonTerminals[i]<<": \n";

           for (int j = 0; j< follow[nonTerminals[i]].size(); j++)
           {
               output << j <<" "<<(follow[nonTerminals[i]])[j]<< endl;

           }
           output<< endl;
       }

    */
}

void Parser::getFollow(int NTIndex)
{

//  map<string , vector<string>> follow;
    int NTVectorLength;
    int NTProductionLength;
    string currentNT;
    vector<string> followVector;
    vector<vector<string>> NTVector;
    map<string, vector<string>>::iterator it;

    currentNT = nonTerminals[NTIndex];

    for(int m =0; m < nonTerminals.size(); m++)
    {

        NTVector = productionsMap[nonTerminals[m]];        // the main vector
        NTVectorLength = NTVector.size();                  // has how many vector for or

        for(int j = 0; j < NTVectorLength; j++)
        {
            NTProductionLength = NTVector[j].size();

            for(int k = 0; k < NTProductionLength; k++)
            {
                if((NTVector[j])[k] == currentNT)
                {
                    if( k+1 < NTProductionLength)    // has next
                    {
                        string next = (NTVector[j])[k+1];

                        if(findInTerminal(next))        // next is terminal
                        {
                            followVector.push_back(next);
                        }
                        else                            // next is nonterminal
                        {
                            int hasLamda = 0;
                            for(int l =0; l < first[next].size(); l++)
                            {
                                if((first[next])[l] == "lamda")
                                {
                                    hasLamda = 1;
                                    continue;
                                }

                                followVector.push_back((first[next])[l]);

                            }

                            if(hasLamda == 1)
                            {

                                it = follow.find(next);

                                if (it != follow.end())
                                {
                                    addToFollow(currentNT,follow[next]);
                                }
                                else
                                {
                                    getFollow( getNTIndex(next) );
                                    addToFollow(currentNT,follow[next]);
                                }

                            }
                        }

                    }
                    else                                //has no next
                    {
                        if(currentNT != nonTerminals[m] )
                        {
                            // cout << "check\n";
                            addToFollow(currentNT,follow[nonTerminals[m]]);
                        }
                        else
                        {
                            // cout << "ana wa2f 3and nafsiiii\n";
                        }

                    }

                }


            }


        }
    }

    if(followVector.size() > 0)
    {
        addToFollow(currentNT,followVector);
    }



}

void Parser::addToFollow(string key,vector<string> data)
{
    map<string, vector<string>>::iterator it;

    it = follow.find(key);

    if (it != follow.end())     //already has vector
    {
        for(int i = 0; i < data.size(); i++)
        {
            follow[key].push_back(data[i]);
        }
    }
    else
    {
        follow.insert(make_pair(key,data));

    }


}

int Parser:: getNTIndex(string nonTerminal)
{
    for(int i = 0; i < nonTerminals.size(); i++)
    {
        if(nonTerminal == nonTerminals[i])
        {
            return i;
        }
    }
    cout << "Error" << endl;
    return -1;
}

void Parser::mainEngineForParser(void)
{
    Bonus b;
    b.engine();
    std::ifstream input( "new_CFG.txt");

    string productionLine;

    vector<string> productions;

    for( std::string line; getline( input, line ); )
    {
        if (line[line.size() - 1] == '\r')
            line.resize(line.size() - 1);

        if(line.at(0) == '#')
        {
            productionLine = line;
            productions.push_back(productionLine);
        }
        else
        {
            productionLine += line;
            productions.pop_back();
            productions.push_back(productionLine);
        }


    }

    for (std::vector<string>::const_iterator i = productions.begin(); i != productions.end(); ++i)
        cout << *i <<endl;

    // call getProductions function
    getProductions(productions);
    getFirst();
    populateFollow();
    createTable();
    starting_symbol = nonTerminals[0];
    parse_tokens();

}
bool Parser::is_terminal(string token)
{
    for(string terminal : terminals)
    {

        if(terminal == token)
            return true;

    }
    return false;
}

string Parser::get_next_token(int index)
{
    return tokens.at(index);
}

bool Parser::isSemicolon(string input){
    char arr1[2];

    strcpy(arr1,input.c_str());
    //cout<<"insemi" << << <<endl;
    int first = (int)arr1[0];
    int second = (int)arr1[1];

    if(first == -51 && second == -66){
        return true;
    }
    return false;
}
void Parser::parse_tokens()
{
    std::ofstream parsing ("pasring_output.txt");
    int token_count = 0;
    parsing_stack.push("$");
    parsing_stack.push(starting_symbol);

    string top_of_stack;
    string input;
    vector<string> table_entry;

    while(parsing_stack.size() != 0)
    {
        input = get_next_token(token_count);
        top_of_stack = parsing_stack.top();
        if(isSemicolon(input) ){
                input = ";";
        }
        if(top_of_stack == "$" && input == "$")  //successful match
        {
            parsing << "accept" << endl;
            break;
        }

        else if(is_terminal(top_of_stack))
        {
            if(input == top_of_stack)
            {
               parsing << "match " << input << endl;
                parsing_stack.pop();
                token_count ++;
            }

            else
            {
               parsing << "error missing character " << top_of_stack << endl;
                parsing_stack.pop();
            }
        }
        else if(!is_terminal(top_of_stack))
        {
            auto it = parsing_table.find(make_pair(top_of_stack, input));
            if(it != parsing_table.end())
            {
                table_entry = it -> second;
                parsing_stack.pop();
                if(table_entry.at(0) != "sync" && table_entry.at(0) != "lamda")
                {
                   for(int i = table_entry.size()-1; i >= 0; i--)
                   {
                      parsing_stack.push(table_entry.at(i));
                   }
                  parsing << top_of_stack << " => ";
                   for(string s : table_entry)
                   {
                    parsing << s<<" ";
                   }
                  parsing << endl;
                }else if(table_entry.at(0) == "lamda")
                {

                   parsing << top_of_stack << " => " << "lamda"<<endl;
                }
                else{

                 parsing << "error sync"<< endl;
                   parsing_stack.pop();
                }
            }
            else
            {
              parsing << "illegal " << top_of_stack << endl;
               token_count++;
            }
        }
    }
}

