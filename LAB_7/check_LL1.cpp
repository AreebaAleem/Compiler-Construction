#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Rule {
    string rule;
    Rule* next;

    Rule(const string& r) : rule(r), next(nullptr) {}
};

struct Production {
    string name;
    Rule* rule_list;
    Production* next;

    Production(const string& n) : name(n), rule_list(nullptr), next(nullptr) {}
};

class CFGList {
private:
    Production* startcfg;

public:
    CFGList() : startcfg(nullptr) {}

    void addRule(Production* production, const string& rule) {
        Rule* newRule = new Rule(rule);
        if (!production->rule_list) {
            production->rule_list = newRule;
        } else {
            Rule* temp = production->rule_list;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newRule;
        }
    }

    void addProduction(const string& name) {
        Production* currentProduction = new Production(name);
        if (!startcfg) {
            startcfg = currentProduction;
        } else {
            Production* temp = startcfg;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = currentProduction;
        }
    }

    void deleteProduction(int index) {
        if (index == 0 && startcfg) {
            Production* temp = startcfg;
            startcfg = startcfg->next;
            delete temp;
        } else {
            Production* temp = startcfg;
            for (int i = 0; temp != nullptr && i < index - 1; ++i) {
                temp = temp->next;
            }
            if (temp == nullptr || temp->next == nullptr) {
                return;
            }
            Production* nextProd = temp->next->next;
            delete temp->next;
            temp->next = nextProd;
        }
    }

    void deleteRule(Production* production, int index) {
        if (index == 0 && production && production->rule_list) {
            Rule* temp = production->rule_list;
            production->rule_list = production->rule_list->next;
            delete temp;
        } else {
            Rule* temp = production->rule_list;
            for (int i = 0; temp != nullptr && i < index - 1; ++i) {
                temp = temp->next;
            }
            if (temp == nullptr || temp->next == nullptr) {
                return;
            }
            Rule* nextRule = temp->next->next;
            delete temp->next;
            temp->next = nextRule;
        }
    }

    Production* getStartCFG() {
        return startcfg;
    }

    void displayCFG() {
        Production* prodTemp = startcfg;
        int index = 0;
        while (prodTemp) {
            cout << "Production " << index << " (" << prodTemp->name << "): ";
            Rule* ruleTemp = prodTemp->rule_list;
            while (ruleTemp) {
                cout << ruleTemp->rule << " | ";
                ruleTemp = ruleTemp->next;
            }
            cout << endl;
            prodTemp = prodTemp->next;
            ++index;
        }
    }

    // Function to check if left factoring is needed
    bool needsLeftFactoring() {
        Production* prod = startcfg;
        while (prod) {
            Rule* ruleA = prod->rule_list;
            while (ruleA && ruleA->next) {
                Rule* ruleB = ruleA->next;
                while (ruleB) {
                    string prefix = commonPrefix(ruleA->rule, ruleB->rule);
                    if (!prefix.empty() && prefix.size() > 0) {
                        cout << "Left factoring needed in production '" << prod->name << "' for rules starting with '" << prefix << "'" << endl;
                        return true; // if a common prefix is found
                    }
                    ruleB = ruleB->next;
                }
                ruleA = ruleA->next;
            }
            prod = prod->next;
        }
        return false;
    }

    // Helper function to find common prefix of two strings
    string commonPrefix(const string& a, const string& b) {
        size_t min_length = min(a.size(), b.size());
        for (size_t i = 0; i < min_length; ++i) {
            if (a[i] != b[i]) {
                return a.substr(0, i);
            }
        }
        return a.substr(0, min_length);
    }
};

void readGrammar(string filename, CFGList& cfgList){
    ifstream file;
    string line;
    file.open(filename);

    bool first = true;

    while (!file.eof()){        
        getline(file, line);
        
        stringstream lineAsStream(line);
    
        string head;
        getline(lineAsStream, head, '=');

        cfgList.addProduction(head);
        Production* nextProduction;
        if(first){
            nextProduction = cfgList.getStartCFG();
            first = false;
        }else{
            nextProduction = nextProduction->next;
        }
    
        string rule;
        while(getline(lineAsStream, rule, '|')){            
            cfgList.addRule(nextProduction, rule);
        }
    }
}

int main() {
    CFGList cfgList;

    readGrammar("test.txt", cfgList);    

    // Displaying the initial CFG
    cout << "Initial Context-Free Grammar (CFG):" << endl;
    cfgList.displayCFG();

    // Check for left factoring
    if (cfgList.needsLeftFactoring()) {
        cout << "The CFG needs left factoring." << endl;
    } else {
        cout << "The CFG does not need left factoring." << endl;
    }

    return 0;
}
