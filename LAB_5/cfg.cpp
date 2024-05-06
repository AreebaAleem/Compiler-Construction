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

    Production* getLastProduction() {
        if (!startcfg) return nullptr;
        Production* temp = startcfg;
        while (temp->next) {
            temp = temp->next;
        }
        return temp;
    }

public:
    CFGList() : startcfg(nullptr) {}

    void addRule(Production* production, const string& rule) {
        if (!production) return;
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

    void addRuleToLatestProduction(const string& rule) {
        Production* lastProd = getLastProduction();
        addRule(lastProd, rule);
    }

    void addProduction(const string& name) {
        Production* newProduction = new Production(name);
        if (!startcfg) {
            startcfg = newProduction;
        } else {
            Production* temp = startcfg;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newProduction;
        }
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

    bool isLeftRecursive() {
        Production* prod = startcfg;
        while (prod) {
            Rule* rule = prod->rule_list;
            while (rule) {
                if (rule->rule.find(prod->name) == 0) {
                    cout << "Grammar is left recursive." << endl;
                    return true;
                }
                rule = rule->next;
            }
            prod = prod->next;
        }
        cout << "Grammar is not left recursive." << endl;
        return false;
    }
};

void readGrammar(const string& filename, CFGList& cfgList) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream lineAsStream(line);
        string head;
        getline(lineAsStream, head, '=');
        cfgList.addProduction(head);

        string rule;
        while (getline(lineAsStream, rule, '|')) {
            cfgList.addRuleToLatestProduction(rule);
        }
    }
}

int main() {
    CFGList cfgList;

    readGrammar("text.txt", cfgList);

    // Displaying the initial CFG
    cout << "Initial Context-Free Grammar (CFG):" << endl;
    cfgList.displayCFG();

    // Delete a rule and a production
    // cfgList.deleteRule(startProduction, 0);
    // cfgList.deleteProduction(1);

    // Displaying the updated CFG
    // cout << "\nUpdated Context-Free Grammar (CFG):" << endl;
    // cfgList.displayCFG();

    // Check for left recursion
    cfgList.isLeftRecursive();

    return 0;
}

//left recursion