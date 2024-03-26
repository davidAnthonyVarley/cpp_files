#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;


class Category {

    public:

    string label;
    bool is_terminal;

    Category(string label, bool is_terminal) {

        this->label = label;
        this->is_terminal = is_terminal;

    }

    Category() {

        this->label = "";
        this->is_terminal = false;
        
    } 

};


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


class TopDownParser {

    public:

    vector<Rule> rules;
    vector<string> input;
    vector<Category> PDA;
    Rule initial;

    vector<int> used_rules;

    

    TopDownParser(string filepath) {
        this->rules = vector<Rule>(0);
        this->PDA = vector<Category>(0);
        this->make_rules(filepath);
        
    }

    TopDownParser() {}

    void format_input(string line) {
        stringstream ss(line);
        string word;

        while (ss >> word) {
            this->input.push_back(word);
        }
    }

    void make_rules(string filepath) {

        ifstream f(filepath);
        string line;
        while (getline(f, line)) {
            Rule r(line);
            this->rules.push_back(r);
        }

        for (Rule r : this->rules) {
            if (r.mother.label == "s") {
                this->initial = r;
            }
        }
        
    }

    bool parse(string in) {
        format_input(in);
        this->used_rules = vector<int>(rules.size());
        expand(initial);

        //for testing
        cout << "the PDA stack generated: ";
        print_PDA();
        //check if the stack matches the input


        //for (int i = 0; i < input.size(); i++) {
        cout << "input size: " << input.size() << endl;
        for (int i = input.size() -1; i >= 0; i--) {

            Category popped = pop();
            cout << "Comparing " << input[i] << " and " << popped.label << endl;

            if ( input[i] != popped.label) {
                return false;
            }
        }
        cout << "Have gone through all elements in input. " << endl;

        bool result = (PDA.size() == 0);

        if (result ) {
            cout << "SUCCESS " << endl;
        }
        else {
            cout << "failed " << endl;
        }

        return ( PDA.size() == 0);
       

    }

    void expand(Rule r) {

        for (int i = 0; i < r.daughters.size(); i++) {

            Category daughter = r.daughters[i];

            if (!daughter.is_terminal) {
                Rule d = find_rule( daughter.label);
                cout << "expanding " << daughter.label << endl;
                expand(d);
            }
            else { // this is a terminal symbol
                //the back of PDA represents the top of the stack
                PDA.push_back(daughter);
            }
        }

    }

    void print_PDA() {

        for ( int i = 0; i < PDA.size(); i++) {
            cout << PDA[i].label << " ";
        }
        cout << endl;


    }

    void push(Category c) {
        PDA.push_back(c);
    }

    Category pop() {

        Category c = this->PDA[PDA.size() -1 ];
        this->PDA.pop_back();

        return c;

    }

    Rule find_rule(string s) {

        for (int i = 0; i < rules.size(); i++) {

            Rule r = rules[i];

            if (r.mother.label == s) {
                if (!rule_has_been_used(i)) {
                    this->used_rules[i] = 1;
                    return r;
                }
            }
        }

        for (Rule r: this->rules) {
            if (r.mother.label == s) {
                    return r;
            }
        }

        //else
        return Rule("ERROR");
    }



    bool rule_has_been_used(int index) {
        if (this->used_rules[index]==1) {
            return true;
        }
        //else
        return false;
    }




};



int main() {

    TopDownParser tdp("cgram");
    tdp.parse("the man hit the dog");
    

    
}