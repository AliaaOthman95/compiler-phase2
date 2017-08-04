#include "Bonus.h"

Bonus::Bonus()
{
    //ctor
}

Bonus::~Bonus()
{
    //dtor
}
 std::ofstream file ("new_CFG.txt");
void Bonus::getProductions(vector<string> productions)
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
        non_terminals.push_back(productionName);
        pointerChar = strtok (NULL, " ");

        vector<string> linePro;
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
        productions_vector.push_back(make_pair(productionName,productionvectors));
    }
}

void Bonus::eliminate_left_recursion()
{

    string left_side ;
    string temp ;
    string temp2;
    vector <vector<string>> production;
    vector <vector<string>> created_production;
    vector <string> new_production;
    vector<string> lamda;
    lamda.push_back("lamda");
    vector <vector<string>> production2;
    bool entered = false;
    int index;
    for(std::vector <pair< string, vector < vector < string>>>>::iterator iterate = productions_vector.begin(); iterate != productions_vector.end(); ++iterate)
    {
        {
            index = std::distance( productions_vector.begin(), iterate);
            left_side = (*iterate).first;
            for(int i=0; i<(*iterate).second.size(); i++)
            {
                temp = (*iterate).second.at(i).at(0);
                if(left_side == temp)
                {
                    entered = true;
                    production =  (*iterate).second;
                    temp2 = left_side + "_dash";
                    for(int j = 0 ; j< production.size() && j != i; j++ )
                    {

                        production.at(j).push_back(temp2);
                        production2.push_back(production.at(j));
                    }



                    for(int k = 1; k < (*iterate).second.at(i).size(); k++)
                    {

                        new_production.push_back((*iterate).second.at(i).at(k));
                    }
                    new_production.push_back(temp2);
                    created_production.push_back(new_production);
                    created_production.push_back(lamda);
                    productions_vector.at(index) = make_pair(left_side,production2) ;
                    temp_productions.push_back(make_pair(temp2,created_production));
                    production.clear();
                    production2.clear();
                    new_production.clear();
                    created_production.clear();
                    non_terminals.push_back(temp2);
                }
            }

        }
    }


}
vector <int> Bonus::detect_left_refactor(vector<vector<string>> production)
{
    vector<string> first;
    vector<int> indexes;
    bool entered = false;

    for(int i=0; i < production.size(); i++)
    {
        first.push_back(production.at(i).at(0));
    }

    for(int i = 0; i < first.size(); i++)
    {
        for(int j = i+1; j < first.size(); j++)
        {
            if(first.at(i) == first.at(j))
            {
                entered = true;
                indexes.push_back(j);
            }
        }

        if(entered)
        {
            indexes.push_back(i);
            entered = false;
            break;
        }
    }
    return indexes;
}

bool Bonus::found_in_indexes(int index, vector<int> indexes)
{
    for(int i = 0 ; i < indexes.size(); i++)
    {
        if(i == index)
        {
            return true;
        }
    }

    return false;
}


void Bonus::eliminate_left_factoring()
{
    vector <string>temp ;
    string temp2;
    vector <vector<string>> production;
    vector <vector<string>> created_production;
    vector <string> new_production;
    vector <vector<string>> production2;
    vector<int> indexes;
    vector <string> temp_vect;
    string left_side;
    int index;
    for(std::vector <pair< string, vector < vector < string>>>>::iterator iterate = productions_vector.begin(); iterate != productions_vector.end(); ++iterate)
    {
        {
            index = std::distance( productions_vector.begin(), iterate);
            left_side = (*iterate).first;
            indexes = detect_left_refactor((*iterate).second);

            if(indexes.size() > 0)
            {
                production =  (*iterate).second;
                temp2 = left_side + "_dash";
                temp.push_back(production.at(indexes.at(0)).at(0));
                temp.push_back(temp2);
                production2.push_back(temp);
                for(int j = 0 ; j< production.size() && !found_in_indexes(j, indexes); j++ )
                {
                    production2.push_back(production.at(j));
                }

                for(int k = 0; k < indexes.size(); k++)
                {
                    temp_vect = production.at(indexes.at(k));
                    if(temp_vect.size() == 1)
                    {
                        new_production.push_back("lamda");
                    }
                    else
                    {
                        for(int w = 1; w < temp_vect.size(); w++)
                        {
                            new_production.push_back(temp_vect.at(w));
                        }
                    }
                    created_production.push_back(new_production);
                    new_production.clear();
                }

                productions_vector.at(index) = make_pair(left_side,production2) ;
                temp_productions.push_back(make_pair(temp2,created_production));
                production.clear();
                production2.clear();
                temp_vect.clear();
                created_production.clear();
                non_terminals.push_back(temp2);
            }
        }
    }


}

bool Bonus::is_non_terminal(string token)
{
    for(string non_terminal : non_terminals)
    {
        if(non_terminal == token)
            return true;

    }
    return false;
}
void Bonus::print_dash (string left_side)
{
    string temp = left_side +"_dash";
    for(int w = 0 ; w < temp_productions.size() ; w++)
    {
        if(temp == temp_productions.at(w).first)
        {

            file << "# " <<  temp_productions.at(w).first << " = " ;
            for(int i = 0; i < temp_productions.at(w).second.size(); i++)
            {
                for(int j = 0; j < temp_productions.at(w).second.at(i).size(); j++)
                {
                    if(is_non_terminal(temp_productions.at(w).second.at(i).at(j)))
                    {
                        file << temp_productions.at(w).second.at(i).at(j) << " ";
                    }
                    else
                    {
                        file << "'"<<temp_productions.at(w).second.at(i).at(j)<< "'" << " ";
                    }
                }

                if(i != temp_productions.at(w).second.size()-1)
                    file << "| ";
            }
            file << endl;
        }

    }

}

void Bonus::engine()
{

    std::ifstream input( "CFG1.txt");

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

    // call getProductions function
    getProductions(productions);
    eliminate_left_recursion();
    eliminate_left_factoring();


    for(std::vector <pair< string, vector < vector < string>>>>::iterator iterate = productions_vector.begin(); iterate != productions_vector.end(); ++iterate)
    {
        file << "# " << iterate -> first << " = " ;
        for(int i = 0; i < iterate -> second.size(); i++)
        {
            for(int j = 0; j < iterate -> second.at(i).size(); j++)
            {
                if(is_non_terminal(iterate -> second.at(i).at(j)))
                {
                    file << iterate -> second.at(i).at(j) << " ";
                }
                else
                {
                    if(iterate -> second.at(i).at(j).compare("=")==0){
                     file << "'"<<"assign"<< "'" << " ";
                    }else{
                    file << "'"<<iterate -> second.at(i).at(j)<< "'" << " ";
                    }
                }
            }

            if(i != iterate -> second.size()-1)
                file << "| ";
        }
        file << endl;
        print_dash(iterate -> first);
    }
}

