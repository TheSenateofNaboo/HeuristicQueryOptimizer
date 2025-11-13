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

    pos = input.find("JOIN", 0);
    if (pos == string::npos) {
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

    from = input.substr(0, pos);
    input.erase(0, pos);

    return from;
}

//Will return a string of the join field
string joinStatement(string& input) {
    string join;

    size_t pos = input.find("JOIN", 0);
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
                    join = input;
                    input = "";
                    return join;
                }

                join = input.substr(0, pos);
                input.erase(0, pos);

                return join;
            }

            join = input.substr(0, pos);
            input.erase(0, pos);

            return join;
        }

        join = input.substr(0, pos);
        input.erase(0, pos);

        return join;
    }

    join = input.substr(0, pos);
    input.erase(0, pos);

    return join;
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


//Formats an OrderBy statement
string orderByFormat(string s) {
    s.erase(0, 8);
    size_t pos = s.find_first_not_of(" ");
    if (pos != string::npos) {
        s.erase(0, pos);
    }
    s.erase(remove(s.begin(), s.end(), ';'), s.end());
    s = "Order By: " + s;
    return s;
}

//Formats a project statement for the canonical tree
string projectCanon(string s) {
    s.erase(0, 6);
    size_t pos = s.find_first_not_of(" ");
    if (pos != string::npos) {
        s.erase(0, pos);
    }
    s.erase(remove(s.begin(), s.end(), ';'), s.end());
    s = "Project: " + s;
    return s;
}

//Formats a Having statement
string havingFormat(string s) {
    s.erase(0, 6);
    size_t pos = s.find_first_not_of(" ");
    if (pos != string::npos) {
        s.erase(0, pos);
    }
    s.erase(remove(s.begin(), s.end(), ';'), s.end());
    s = "Having: " + s;
    return s;
}

//Formats a GroupBy statement
string groupByFormat(string s, string t) {
    s.erase(0, 8);
    t.erase(0, 9);
    size_t pos = s.find_first_not_of(" ");
    if (pos != string::npos) {
        s.erase(0, pos);
    }
    pos = t.find_first_not_of(" ");
    if (pos != string::npos) {
        t.erase(0, pos);
    }
    s.erase(remove(s.begin(), s.end(), ';'), s.end());
    s = "Group By: " + s;

    vector<string> agg = { "SUM", "COUNT", "AVG", "MIN", "MAX" };
    string seg;

    pos = t.find_first_of(",");
    
    while (pos != string::npos) {
        seg = t.substr(0, pos);
        t.erase(0, pos + 1);
        pos = seg.find_first_not_of(" ");
        if (pos != string::npos) {
            seg.erase(0, pos);
        }
        for (int i = 0; i < agg.size(); i++) {
            pos = seg.find(agg[i]);
            if (pos != string::npos) {
                s += ", " + seg;
                break;
            }
        }
        pos = t.find_first_of(",");
    }
    for (int i = 0; i < agg.size(); i++) {
        pos = t.find(agg[i]);
        pos = t.find_first_not_of(" ");
        if (pos != string::npos) {
            t.erase(0, pos);
        }
        if (pos != string::npos) {
            s += ", " + t;
            break;
        }
    }    

    return s;
}

//Formats a Where statement in a canon select statement
string selectCanonFormat(string s, string j) {
    s.erase(0, 5);
    size_t pos = s.find_first_not_of(" ");
    if (pos != string::npos) {
        s.erase(0, pos);
    }
    s.erase(remove(s.begin(), s.end(), ';'), s.end());
    s = "Select: " + s;

    pos = j.find("ON");
    string seg;
    while (pos != string::npos) {
        j.erase(0, pos + 2);
        pos = j.find("JOIN");
        if (pos != string::npos) {
            seg = j.substr(0, pos);
            pos = seg.find_first_not_of(" ");
            if (pos != string::npos) {
                seg.erase(0, pos);
            }
            s += "AND " + seg;
        }
        else {
            pos = j.find_first_not_of(" ");
            if (pos != string::npos) {
                j.erase(0, pos);
            }
            s += "AND " + j;
        }
        pos = j.find("ON");
    }

    return s;
}

//Returns the canonical nodes
vector<string> tableNodeGen(string s, string j, string sel) {
    vector<string> nodes;
    vector<int> nodesVal;
    
    s.erase(0, 4);
    size_t pos = s.find_first_not_of(" ");
    if (pos != string::npos) {
        s.erase(0, pos);
    }
    s.erase(remove(s.begin(), s.end(), ';'), s.end());

    pos = j.find("JOIN");
    string seg;
    while (pos != string::npos) {
        j.erase(0, 4);
        pos = j.find_first_not_of(" ");
        if (pos != string::npos) {
            j.erase(0, pos);
        }
        pos = j.find("ON");
        seg = j.substr(0, pos);
        s += ", " + seg;

        pos = j.find("JOIN");
        j.erase(0, pos);
    }

    pos = s.find_first_of(",");
    while (pos != string::npos) {
        seg = s.substr(0, pos);
        nodes.push_back(seg);
        nodesVal.push_back(0);
        s.erase(0, pos + 1);
        pos = s.find_first_of(",");
    }
    nodes.push_back(s);
    nodesVal.push_back(0);

    vector<string> args;

    sel.erase(0, 8);
    pos = sel.find_first_not_of(" ");
    if (pos != string::npos) {
        sel.erase(0, pos);
    }

    pos = sel.find("AND");
    while (pos != string::npos) {
        seg = sel.substr(0, pos);
        args.push_back(seg);
        sel.erase(0, pos + 3);
        pos = sel.find("AND");
    }
    args.push_back(sel);

    vector<string> compares;

    for (int i = 0; i < args.size(); i++) {
        pos = args[i].find("=");
        if (pos != string::npos) {
            pos = args[i].find(">");
            if (pos == string::npos) {
                pos = args[i].find("<");
                if (pos == string::npos) {
                    pos = args[i].find_first_not_of(" ");
                    if (pos != string::npos) {
                        args[i].erase(0, pos);
                    }
                    compares.push_back(args[i]);
                }
            }
        }
    }

    //Gets the weights of the canonical tables to know which tables should be placed further down the tree
    for (int i = 0; i < compares.size(); i++) {
        pos = compares[i].find("=");
        string left = compares[i].substr(0, pos);
        string right = compares[i].substr(pos + 1, compares[i].size());
        pos = right.find_first_not_of(" ");
        if (pos != string::npos) {
            right.erase(0, pos);
        }
        pos = left.find(".");
        left = left.substr(0, pos);
        pos = right.find(".");
        right = right.substr(0, pos);
        left.erase(remove(left.begin(), left.end(), ' '), left.end());
        right.erase(remove(right.begin(), right.end(), ' '), right.end());
        bool lTrue = false;
        bool rTrue = false;
        int ll = -1;
        int lr = -1;
        int rl = -1;
        int rr = -1;
        for (int j = 0; j < nodes.size(); j++) {
            pos = nodes[j].find_first_not_of(" ");
            if (pos != string::npos) {
                nodes[j].erase(0, pos);
            }
            pos = nodes[j].find(" ");
            string leftT = nodes[j].substr(0, pos);
            string rightT = nodes[j].substr(pos+1, nodes[j].size());
            leftT.erase(remove(leftT.begin(), leftT.end(), ' '), leftT.end());
            rightT.erase(remove(rightT.begin(), rightT.end(), ' '), rightT.end());
            if (left == leftT) {
                lTrue = true;
                ll = j;
            }
            if (left == rightT) {
                lTrue = true;
                lr = j;
            }
            if (right == leftT) {
                rTrue = true;
                rl = j;
            }
            if (right == rightT) {
                rTrue = true;
                rr = j;
            }
        }
        if (rTrue && lTrue) {
            if (ll > -1) {
                nodesVal[ll]++;
            }
            if (lr > -1) {
                nodesVal[lr]++;
            }
            if (rl > -1) {
                nodesVal[rl]++;
            }
            if (rr > -1) {
                nodesVal[rr]++;
            }
        }
    }

    //Pairs the weights and nodes
    vector<pair<int, string>> weights;
    for (size_t i = 0; i < nodes.size(); ++i) {
        weights.emplace_back(nodesVal[i], nodes[i]);
    }

    sort(weights.begin(), weights.end(),
        [](const auto& a, const auto& b) {
            return a.first > b.first; //descending order
        });

    vector<string> organizedNodes;
    for (const auto& p : weights) {
        organizedNodes.push_back(p.second);
    }

    return organizedNodes;
}


































int main() {
    //Will open the input file we need to optimize
    ifstream inputFile("input.txt");
    string line;

    //Creates and opens an output file
    ofstream outputFile1("output1.txt");
    ofstream outputFile2("output2.txt");
    ofstream outputFile3("output3.txt");
    ofstream outputFile4("output4.txt");
    ofstream outputFile5("output5.txt");

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

    //Schema
    vector<vector<string>> tables;
    tables = validTableName(schema);

    //Seperate out the parts of the query
    string select = selectStatement(query);
    string from = fromStatement(query);
    string join = joinStatement(query);
    string where = whereStatement(query);
    string groupBy = groupByStatement(query);
    string having = havingStatement(query);
    string orderBy = query;

    //Initiates a vector of vectors to hold the query tree components as we optimize it
    vector<vector<string>> queryTree;

    //Push some parts to the query tree
    if (orderBy.size() > 0) {
        orderBy = orderByFormat(orderBy);
        vector<string> orderByArg = { orderBy };
        queryTree.push_back(orderByArg);
    }
    if (select.size() > 0) {
        select = projectCanon(select);
        vector<string> selectArg = { select };
        queryTree.push_back(selectArg);
    }
    if (having.size() > 0) {
        having = havingFormat(having);
        vector<string> havingArg = { having };
        queryTree.push_back(havingArg);
    }
    if (groupBy.size() > 0) {
        groupBy = groupByFormat(groupBy, select);
        vector<string> groupByArg = { groupBy };
        queryTree.push_back(groupByArg);
    }
    if (where.size() > 0) {
        where = selectCanonFormat(where, join);
        vector<string> whereArg = { where };
        queryTree.push_back(whereArg);
    }
    if (from.size() > 0) {
        vector<string> tableNodes = tableNodeGen(from, join, where);
        queryTree.push_back(tableNodes);
    }

    
    for (int i = 0; i < queryTree.size(); i++) {
        for (int j = 0; j < queryTree[i].size(); j++) {
            outputFile1 << queryTree[i][j] << " ";
        }
        outputFile1 << "\n";
    }
    
    

    //Close the input and output file
    inputFile.close();
    outputFile1.close();
    outputFile2.close();
    outputFile3.close();
    outputFile4.close();
    outputFile5.close();

	return 0;
}