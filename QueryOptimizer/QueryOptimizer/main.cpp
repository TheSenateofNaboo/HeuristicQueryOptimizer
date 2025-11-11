#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//Function will return a vector of vectors
//Each vector will hold the name of the table at the head
//The rest of the vector will be the column names of the table
vector<vector<string>> validTableName (string input) {
    //Will hold the vectors of tables and columns
    vector<vector<string>> tables;
    //Iterator
    int k = 0;

    //While we are still reading the schema
    while ( k < input.size() ) {
        //Holds a table and their columns
        vector<string> table;

        //Finds the title of the table
        size_t pos = input.find('(', k);
        string title = input.substr(k, pos - k);
        transform(title.begin(), title.end(), title.begin(), ::tolower);
        table.push_back(title);
        k += size(title);

        //Finds all column names
        while (input[k] == ',' || input[k] == '(') {
            k++;
            pos = input.find_first_of(",)", k);
            string column = input.substr(k, pos - k);
            transform(column.begin(), column.end(), column.begin(), ::tolower);
            if ((column.substr(0, column.find('(')) == "primarykey") || (column.substr(0, column.find('(')) == "unique")) {
                break;
            }
            else {
                table.push_back(column);
                k += column.size();
            }
        }

        //Get to the next table
        pos = input.find_first_of(")", k);
        string column = input.substr(k, pos - k);
        transform(column.begin(), column.end(), column.begin(), ::tolower);
        k += size(column) + 1;
        if (input[k] == ',') {
            k++;
            pos = input.find_first_of(")", k);
            string column = input.substr(k, pos - k);
            k += size(column) + 1;
        }
        if (input[k] == ')') {
            k++;
            if (k < input.size()) {
                if (input[k] == ';') {
                    k++;
                }
            }
        }

        //Put our table in our vector
        tables.push_back(table);
    }

    return tables;
}


//Will return a string of the selections made and the input altered
string selectStatement(string& input) {
    
    size_t pos = input.find("FROM", 0);
    string select = input.substr(0, pos);
    input.erase(0, pos);

    return select;
}

//Will return a string of the from field
string fromStatement(string& input) {
    string from;

    size_t pos = input.find("FROM", 0);
    if (pos == string::npos) {
        return "";
    }

    pos = input.find("WHERE", 0);
    if (pos == string::npos) {
        pos = input.find("GROUP BY", 0);
        if (pos == string::npos) {
            pos = input.find("HAVING", 0);
            if (pos == string::npos) {
                pos = input.find("ORDER BY", 0);
                if (pos == string::npos) {
                    from = input;
                    input = "";
                    return from;
                }

                from = input.substr(0, pos);
                input.erase(0, pos);

                return from;
            }

            from = input.substr(0, pos);
            input.erase(0, pos);

            return from;
        }

        from = input.substr(0, pos);
        input.erase(0, pos);

        return from;
    }

    from = input.substr(0, pos);
    input.erase(0, pos);

    return from;
}


//Will return a string of the where field
string whereStatement(string& input) {
    string where;

    size_t pos = input.find("WHERE", 0);
    if (pos == string::npos) {
        return "";
    }

    pos = input.find("GROUP BY", 0);
    if (pos == string::npos) {
        pos = input.find("HAVING", 0);
        if (pos == string::npos) {
            pos = input.find("ORDER BY", 0);
            if (pos == string::npos) {
                where = input;
                input = "";
                return where;
            }
            
            where = input.substr(0, pos);
            input.erase(0, pos);

            return where;
        }

        where = input.substr(0, pos);
        input.erase(0, pos);

        return where;
    }

    where = input.substr(0, pos);
    input.erase(0, pos);

    return where;
}

//Will return a string of the group by field
string groupByStatement(string& input) {
    string groupBy;

    size_t pos = input.find("GROUP BY", 0);
    if (pos == string::npos) {
        return "";
    }

    pos = input.find("HAVING", 0);
    if (pos == string::npos) {
        pos = input.find("ORDER BY", 0);
        if (pos == string::npos) {
            groupBy = input;
            input = "";
            return groupBy;
        }

        groupBy = input.substr(0, pos);
        input.erase(0, pos);

        return groupBy;
    }

    groupBy = input.substr(0, pos);
    input.erase(0, pos);

    return groupBy;
}

//Will return a string of the having field
string havingStatement(string& input) {
    string having;

    size_t pos = input.find("HAVING", 0);
    if (pos == string::npos) {
        return "";
    }

    pos = input.find("ORDER BY", 0);
    if (pos == string::npos) {
        having = input;
        input = "";
        return having;
    }

    having = input.substr(0, pos);
    input.erase(0, pos);

    return having;
}



int main() {
    //Will open the input file we need to optimize
    ifstream inputFile("input.txt");
    string line;

    //Creates and opens an output file
    ofstream outputFile("output.txt");

    //Schema
    string schema = "";

    //Query
    string query = "";
    //If we are at the start of the query block, begin adding lines to our query
    bool queryTime = false;


    //Parses through our input file
    //Collect the schema and query separately
    //Ignore comments
    while ( getline(inputFile >> ws, line) ) {
        //Check first word to see if we have hit our query block
        if (queryTime == false) {
            string firstWord = line.substr(0, line.find(' '));
            transform(firstWord.begin(), firstWord.end(), firstWord.begin(), ::tolower);
            if (firstWord == "select") {
                queryTime = true;
            }
        }

        //Ignore if line is a comment
        if (line[0] == '-' && line[1] == '-') {
            continue;
        }
        //Add line to the query
        else if (queryTime == true) {
            query += line + " ";
        }
        //Add line to the schema
        else {
            schema += line + " ";
        }
    }

    //Remove all whitespace
    schema.erase(remove(schema.begin(), schema.end(), ' '), schema.end());

    vector<vector<string>> tables;

    tables = validTableName(schema);

    /*
    for (int i = 0; i < tables.size(); i++) {
        for (int j = 0; j < tables[i].size(); j++) {
            cout << tables[i][j] << " ";
        }
        cout << "\n";
    }
    */

    //Seperate out the parts of the query
    string select = selectStatement(query);
    string from = fromStatement(query);
    string where = whereStatement(query);
    string groupBy = groupByStatement(query);
    string having = havingStatement(query);
    string orderBy = query;

    outputFile << select << "\n" << from << "\n" << where << "\n" << groupBy << "\n" << having << "\n" << orderBy;


    //Close the input and output file
    inputFile.close();
    outputFile.close();

	return 0;
}