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
    Production* prev; // Add a prev pointer

    Production(const string& n) : name(n), rule_list(nullptr), next(nullptr), prev(nullptr) {}
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
            currentProduction->prev = temp; // Set the prev pointer
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

    void removeNullProductions() {
        for (Production* prod = startcfg; prod != nullptr; prod = prod->next) {
            Rule* rule = prod->rule_list;
            while (rule != nullptr) {
                if (rule->rule.empty()) {
                    // Null production found, remove it
                    Rule* temp = rule;
                    rule = rule->next;
                    delete temp;
                } else {
                    rule = rule->next;
                }
            }
        }
    }

    void reverseTraverseAndRemoveNull() {
        Production* prod = startcfg;
        while (prod != nullptr) {
            Production* prevProd = prod->prev;
            if (prevProd != nullptr) {
                // Remove null productions before traversing the previous production
                removeNullProductions();
                cout << "Updated CFG after removing null productions before production: " << prevProd->name << endl;
                displayCFG();
            }
            prod = prevProd;
        }
    }
};

void readGrammar(string filename, CFGList& cfgList) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream lineAsStream(line);
        string head;
        getline(lineAsStream, head, '=');

        cfgList.addProduction(head);

        string rule;
        while (getline(lineAsStream, rule, '|')) {
            cfgList.addRule(cfgList.getStartCFG(), rule);
        }
    }
}

int main() {
    CFGList cfgList;

    readGrammar("text.txt", cfgList);

    // Displaying the initial CFG
    cout << "Initial Context-Free Grammar (CFG):" << endl;
    cfgList.displayCFG();

    // Reverse traverse and remove null productions
    cfgList.reverseTraverseAndRemoveNull();

    return 0;
}
