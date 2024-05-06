#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Rule {
    string rule;
    Rule* next;
    Rule* prev;

    Rule(const string& r) : rule(r), next(nullptr), prev(nullptr) {}
};

struct Production {
    string name;
    Rule* rule_list;
    Production* next;
    Production* prev;

    Production(const string& n) : name(n), rule_list(nullptr), next(nullptr), prev(nullptr) {}
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

    bool isNullProduction(Production* production) {
        if (!production) return false;
        Rule* rule = production->rule_list;
        while (rule) {
            if (rule->rule == "ε") {
                return true;
            }
            rule = rule->next;
        }
        return false;
    }

void removeNullProductions() {
    Production* prod = startcfg;
    while (prod) {
        if (isNullProduction(prod)) {
            cout << "Removing null production from " << prod->name << endl;

            // Remove the null production rule
            Rule* rule = prod->rule_list;
            Rule* prev = nullptr;
            while (rule) {
                if (rule->rule == "ε") {
                    // Check if this rule is a standalone null production or part of another rule
                    if (prod->name.length() == 1) {
                        // Standalone null production, remove it directly
                        if (prev) {
                            prev->next = rule->next;
                        } else {
                            prod->rule_list = rule->next;
                        }
                        delete rule;
                    } else {
                        // Null production is part of another rule, update the rule
                        Production* otherProd = startcfg;
                        while (otherProd) {
                            Rule* otherRule = otherProd->rule_list;
                            while (otherRule) {
                                size_t pos = otherRule->rule.find(prod->name);
                                while (pos != string::npos) {
                                    // Update the rule to remove the null production
                                    otherRule->rule.erase(pos, prod->name.length());
                                    pos = otherRule->rule.find(prod->name, pos);
                                }
                                otherRule = otherRule->next;
                            }
                            otherProd = otherProd->next;
                        }
                        // Remove the null production rule
                        if (prev) {
                            prev->next = rule->next;
                        } else {
                            prod->rule_list = rule->next;
                        }
                        delete rule;
                    }
                    break;
                }
                prev = rule;
                rule = rule->next;
            }
        }
        prod = prod->next;
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

    // Remove null productions
    cfgList.removeNullProductions();

    // Displaying the updated CFG
    cout << "\nUpdated Context-Free Grammar (CFG):" << endl;
    cfgList.displayCFG();

    return 0;
}
