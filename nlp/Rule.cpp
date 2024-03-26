#include <vector>
#include <string>
#include <sstream>

class Rule {

public:

    Category mother;
    vector<Category> daughters;



    string reformat(string line) {
        for (int i = 0; i < line.size(); i++) {

            if (line[i] == ',') {
                line[i] = ' ';
            }
        }

        return line;
    }

    Rule() {

        Category mother;
        vector<Category> daughters;

    }

    Rule(string line) {

        line = reformat(line);

        stringstream ss(line);
        string word;

        this->daughters = vector<Category>(0);
        bool divider_found = false;

        while (ss >> word) {
            if (word == "-->") {
                divider_found = true;
            }
            else {
                if (!divider_found) {
                    Category c(word, false);
                    this->mother = c;
                    cout << word << " is a terminal: " << false << endl;
                }
                else {
                    bool terminal = false;
                    if ( word[0] == '[') {
                        terminal = true;
                        string temp = "";

                        //get every letter bar the [ and ]
                        for (int i = 1; i < word.size() -1; i++) {
                            temp += word[i];
                        }
                        word = temp;
                    }
                    
                    Category c(word, terminal);
                    cout << "is " << word << " a terminal: " << terminal << endl;
                    this->daughters.push_back(c);

                }


            }
        }




    }

    

};
