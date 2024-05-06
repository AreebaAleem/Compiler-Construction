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
        bool leftRecursive = false;
        while (prod) {
            Rule* rule = prod->rule_list;
            while (rule) {
                if (rule->rule.find(prod->name) == 0) {
                    leftRecursive = true;
                    break;
                }
                rule = rule->next;
            }
            prod = prod->next;
        }
        if (leftRecursive) {
            cout << "Grammar is left recursive." << endl;
            return true;
        } else {
            cout << "Grammar is not left recursive." << endl;
            return false;
        }
    }

void eliminateLeftRecursion() {
    Production* currentProd = startcfg;
    while (currentProd) {
        Rule* currentRule = currentProd->rule_list;
        Rule* directLeftRecursion = nullptr;
        Rule* prevNonLeftRecursion = nullptr;
        Rule* lastNonLeftRecursion = nullptr;

        // Identify and isolate left-recursive rules
        while (currentRule) {
            if (currentRule->rule.find(currentProd->name) == 0) {
                // Left recursive rule found
                if (!directLeftRecursion) {
                    // First left recursive rule encountered in this production
                    directLeftRecursion = currentRule;
                }
            } else {
                // Non-left recursive rule
                if (!lastNonLeftRecursion) {
                    // First non-left recursive rule encountered
                    prevNonLeftRecursion = currentRule;
                } else {
                    lastNonLeftRecursion->next = currentRule;
                }
                lastNonLeftRecursion = currentRule;
            }
            currentRule = currentRule->next;
        }

        if (directLeftRecursion) {
            // Create a new production to handle left-recursive rules
            Production* newProd = new Production(currentProd->name + "'");
            newProd->next = currentProd->next;
            currentProd->next = newProd;

            // Modify the existing production to remove left recursion
            currentProd->rule_list = prevNonLeftRecursion ? prevNonLeftRecursion->next : nullptr;

            // Append the left-recursive rules to the new production
            directLeftRecursion->rule.erase(0, currentProd->name.length()); // Remove left recursion prefix
            newProd->rule_list = directLeftRecursion->next;
            directLeftRecursion->next = nullptr; // Remove from original production

            // Add epsilon rule to the new production
            addRule(newProd, "epsilon");

            // Add a rule for each non-left recursive production pointing to the new production
            Production* tempProd = startcfg;
            while (tempProd) {
                if (tempProd != currentProd) {
                    addRule(tempProd, newProd->name);
                }
                tempProd = tempProd->next;
            }
        }

        currentProd = currentProd->next;
    }
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

    // Check for left recursion and eliminate if found
    if (cfgList.isLeftRecursive()) {
        cout << "\nLeft recursion detected. Eliminating..." << endl;
        cfgList.eliminateLeftRecursion();
        cout << "\nContext-Free Grammar after left recursion elimination:" << endl;
        cfgList.displayCFG();
    } else {
        cout << "\nNo left recursion detected." << endl;
    }

    return 0;
}
