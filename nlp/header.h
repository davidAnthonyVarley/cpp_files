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
    Category() {}
};



class Rule {

public:

    Category mother;
    vector<Category> daughters;
    bool exists; //used to return that a rule that doesn't exist when parsing



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

    Rule(bool exist) {
        this->exists = false;
    }

    Rule(string line) { //0

        line = reformat(line);
        stringstream ss(line);
        string word;

        this->exists = true;

        this->daughters = vector<Category>(0);
        bool divider_found = false;

        while (ss >> word) { //1
            if (word == "-->") { //1.1
                divider_found = true;
            } //1.1 end
            else { //1.2
                if (!divider_found) { //1.2.1
                    Category c(word, false);
                    this->mother = c;
                    cout << word << " is a terminal: " << false << endl;
                } //1.2.1 end
                else { //1.2.2
                    bool terminal = false;
                    if ( word[0] == '[') { //1.2.2.1
                        terminal = true;
                        string temp = "";

                        //get every letter bar the [ and ]
                        for (int i = 1; i < word.size() -1; i++) { //1.2.2.1.1
                            temp += word[i];
                        } //1.2.2.1.1 end

                        word = temp;
                    } //1.2.2.1 end

                    Category c(word, terminal);
                    cout << "is " << word << " a terminal: " << terminal << endl;
                    this->daughters.push_back(c);
                } //1.2.2.end
            } //1.2 end
        } //1 end
    } //0 end



    string tostring() {

        string s = "";

            cout << this->mother.label << " --> ";
            s += this->mother.label + " --> ";

            for (Category d : this->daughters) {
                cout << d.label << " ";
                s += d.label + " ";
            }

            cout << endl;

        return s + "###########################";
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

    // bool compare(string s, Category c) {
        
    //     for (Category d : )

    // }

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
        return Rule(false);
    }

    bool rule_has_been_used(int index) {
        if (this->used_rules[index]==1) {
            return true;
        }
        //else
        return false;
    }
};


class BottomUpParser {

    public:

    vector<Rule> rules;
    vector<string> input;
    vector<Category> PDA;
    Rule initial;
    vector<int> used_rules;
    

    BottomUpParser(string filepath) {
        this->rules = vector<Rule>(0);
        this->PDA = vector<Category>(0);
        this->make_rules(filepath);
    }
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

    bool parse_input(string in) {
        cout << endl;
        cout << endl;
        //cout << endl << endl;

        this->print_rules();

        format_input(in);
        this->used_rules = vector<int>(rules.size());

        cout << endl;
        cout << endl;
        //cout << "will attempt to parse input now" << endl;
        
        for (int i = 0; i < this->input.size(); i++) {

            Category current(this->input[i], true);
            push(current);
            vector<Category> holder{ current};

            bool failed_to_reduce = false;
            cout << "/" << endl;
            cout << "/" << endl;
            cout << "Pushing '" << current.label << "' onto stack " << endl;
            cout << "/" <<  endl;
            cout << "/" <<  endl;

            while (!failed_to_reduce) {
                Rule next = find_mother(holder);
                if (!next.exists) {

                    //cout << "Couldn't find rule with '" << current.label << "' as the daughter " << endl;
                    failed_to_reduce = true;
                }
                else {
                    cout << "Reducing '" << current.label << "' to '" << next.mother.label <<"'" << endl;
                    pop();
                    //cout << "pop is fine" << endl;
                    push(next.mother);
                    //cout << "push is fine" << endl;
                    holder[0] = next.mother;
                    //cout << "arr index is fine" << endl;
                }

                attempt_to_parse_forest();
                cout << endl;
            }

            //can't reduce current word anymore, add next word from input

        }

        cout << "checking success" << endl;
        cout << endl;



        return check_success();//result;
    }

    bool check_success() {

        cout << "the stack generated by the BU parser, before checking if it is valid: ";
        print_PDA();

        Category last = pop();
  
        cout << "if ( " << last.label << " == " << this->initial.mother.label << ")" << endl;
        if ( last.label == this->initial.mother.label) {
            cout << "SUCCESS " << endl;
            return true;
        }
        else {
            cout << "failed " << endl;
            return false;
        }
    }


    void print_rules() {

        for (Rule r : this->rules) {
            cout << r.mother.label << " --> ";
            for (Category d : r.daughters) {
                cout << d.label << " ";
            }
            cout << endl;
        }

    }

    void print_PDA() {

        for ( int i = 0; i < PDA.size(); i++) {
            cout << PDA[i].label << " ";
        }
        cout << endl;
    }


    void attempt_to_parse_forest() {

        for (int i = 0; i < PDA.size(); i++) {
            vector<vector<Category>> daughters_already_pushed_back_on{};
            
            for (int j = i; j < PDA.size(); j++) {

                //cout << "i, j: " << i << ", " << j << endl;
                //cout << "stack size: " << PDA.size() << endl;

                int len = j - i + 1;
                //cout << "daughters size: " << len << endl;
                vector<Category> daughters = vector<Category>(len);
          
                string labels = "";

                Rule r(false);
                    for (int k = daughters.size() -1 ; k >= 0; k--) {
                        daughters[k] = pop();
                        labels+= daughters[k].label;

                        if (k!=0) {
                            labels+= " ";
                        }
                    }

                cout << "Try to parse: ";
                
                for (Category c : daughters) {
                    cout << c.label << " ";
                }
                cout << endl;

                r = can_be_built_up(daughters);
                //cout << "r could be built up: " << r.exists << endl;

                if (r.exists) {

                    //cout << "pushing mother on " << endl;
                    push(r.mother);
                    cout << r.mother.label << " was parsed from ";// << labels << endl;
                    for (Category c : daughters) {
                        cout << c.label << " ";
                    }
                    cout << endl;

                    //set i == 0, try to parse again
                    //i = 0;
                    //j = PDA.size();
                }
                else {
                    //cout << "1" << endl;

                    if (!has_already_been_pushed_back(daughters_already_pushed_back_on, daughters)) {
                        //cout << "pushing daughters on after they couldn't be built up no mo: " << endl;
                        push(daughters);
                    }
                    else {

                       // cout << labels << " was already pushed on in some form" << endl;
                        print_PDA();

                    }       

                    //cout << "2" << endl;
                }
            }
        }


        //cout << " parsing loops have finsihed" << endl;
        cout << "finishing parse attempt" << endl;
    }

    bool has_already_been_pushed_back(vector<vector<Category>> daughters_already_pushed_back_on, vector<Category> search_for) {

        //cout << "in1" << endl;

        for (int i = 0; i < daughters_already_pushed_back_on.size(); i++) {

            //cout << "in2" << endl;
            bool identical = true;
            bool accessed = false;

            //cout << "already pb size: " << daughters_already_pushed_back_on[i].size() << endl;
            //cout << "search for size: " << search_for.size() << endl;
            
            for (int j = 0; j < daughters_already_pushed_back_on[i].size() && (j < search_for.size()); j++) {

                accessed = true;
                //cout << "in3" << endl;
                try {
                    if (daughters_already_pushed_back_on[i][j].label != search_for[j].label) {

                        identical = false;
                
                    }
                }
                catch (const exception& e){

                    //cout << "exception time" << endl;
                    //cout << "exception: " << e.what() << endl;
                    identical = false;
                }
            }
            //cout << "out3" << endl;

            if (identical && accessed) {
                return true;
            }

            
        }

        //cout << "this (set of ) daughter(s) hasn't been examined yet" << endl;

        return false;
    }

    Rule can_be_built_up(vector<Category> cats) {

        for (Rule r : this->rules) {
            bool identical_daughters = true;

            for (int i = 0; i < cats.size(); i++) {
                if (r.daughters[i].label != cats[i].label ) {
                    identical_daughters = false; 
                    i = cats.size();
                }
            }

            if (identical_daughters && ( cats.size() == r.daughters.size()) ) {
                    return r;
            }
        }

        return Rule(false);
    }

    Category pop() {

        Category popped = PDA[PDA.size() -1];
        PDA.pop_back();
        return popped;
    }

    void push(Category c) {

        PDA.push_back(c);
    }

    void push(vector<Category> c) {

        for (Category cat : c) {
            PDA.push_back(cat);
        }
    }


    Rule find_mother(vector<Category> daughters) {

        for (int i = 0; i < rules.size(); i++) {

            Rule r = rules[i];
            bool is_mother = true;

            for (int j = 0; j< r.daughters.size(); j++) {

                 if (r.daughters[j].label != daughters[j].label) {
                    is_mother = false;
                }
            }
            bool same_size = r.daughters.size() == daughters.size();

            if (!rule_has_been_used(i) && is_mother && same_size) {
                    this->used_rules[i] = 1;
                    return r;
                }
            }
        

        for (int i = 0; i < rules.size(); i++) {

            //cout << "searching for " << daughters[0].label << endl;

            Rule r = rules[i];

            //cout << "rule: " << r.tostring() << endl;
            bool is_mother = true;

            for (int j = 0; j< r.daughters.size(); j++) {

                 if (r.daughters[j].label != daughters[j].label) {
                    is_mother = false;
                }
            }
            bool same_size = r.daughters.size() == daughters.size();

                if (is_mother && same_size) {
                    this->used_rules[i] = 1;
                    return r;
                }
            }
        

        //else
        return Rule(false);
    }

    bool rule_has_been_used(int index) {
        if (this->used_rules[index]==1) {
            return true;
        }
        //else
        return false;
    }


 
};
