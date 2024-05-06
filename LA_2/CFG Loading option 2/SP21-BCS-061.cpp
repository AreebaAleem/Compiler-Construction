#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

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

void deleteRule(Production* production, Rule* ruleToDelete) {
    if (production && production->rule_list) {
        if (production->rule_list == ruleToDelete) {
            Rule* temp = production->rule_list;
            production->rule_list = production->rule_list->next;
            delete temp;
        } else {
            Rule* temp = production->rule_list;
            while (temp->next != nullptr && temp->next != ruleToDelete) {
                temp = temp->next;
            }
            if (temp->next == ruleToDelete) {
                Rule* nextRule = temp->next->next;
                delete temp->next;
                temp->next = nextRule;
            }
        }
    }
}

    Production* getStartCFG() {
        return startcfg;
    }

    void displayCFG() {
        Production* prodTemp = startcfg;
        int index = 0;
        while (prodTemp) {
            cout << "Production " << index << " :     " << prodTemp->name<< " -> " ;
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
};

void readGrammer(string filename, CFGList& cfgList){
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

bool allSymbolsAreNullable(const string& rule, const std::set<string>& nullable) {
    std::istringstream iss(rule);
    string symbol;
    while (iss >> symbol) {
        if (nullable.find(symbol) == nullable.end()) {
            return false;
        }
    }
    return true;
}

void removeNullProductions(CFGList& cfgList) {
    std::set<string> nullable;
    bool changed;
    Production* prodTemp;
    do {
        changed = false;
        prodTemp = cfgList.getStartCFG();
        while (prodTemp) {
            Rule* ruleTemp = prodTemp->rule_list;
            while (ruleTemp) {
                if (ruleTemp->rule == "ε") {
                    if (nullable.insert(prodTemp->name).second) {
                        changed = true;
                    }
                    cfgList.deleteRule(prodTemp, ruleTemp);
                    break;
                } else {
                    bool allNullable = true;
                    for (char symbol : ruleTemp->rule) {
                        string sym(1, symbol);
                        if (nullable.find(sym) == nullable.end()) {
                            allNullable = false;
                            break;
                        }
                    }
                    if (allNullable) {
                        cfgList.deleteRule(prodTemp, ruleTemp);
                        changed = true;
                        break;
                    }
                }
                ruleTemp = ruleTemp->next;
            }
            prodTemp = prodTemp->next;
        }
    } while (changed);
}

void removeUnitProductions(CFGList& cfgList) {
    Production* prodTemp = cfgList.getStartCFG();
    while (prodTemp) {
        Rule* ruleTemp = prodTemp->rule_list;
        while (ruleTemp) {
            if (ruleTemp->rule.length() == 1 && isupper(ruleTemp->rule[0])) {
                string unitSymbol = ruleTemp->rule;
                Production* unitProduction = cfgList.getStartCFG();
                while (unitProduction) {
                    if (unitProduction->name == unitSymbol) {
                        Rule* unitRule = unitProduction->rule_list;
                        while (unitRule) {
                            cfgList.addRule(prodTemp, unitRule->rule);
                            unitRule = unitRule->next;
                        }
                Rule* temp = ruleTemp->next;
                cfgList.deleteRule(prodTemp, ruleTemp);
                ruleTemp = temp;
                        break;
                    }
                    unitProduction = unitProduction->next;
                }
                break;
            }
            ruleTemp = ruleTemp->next;
        }
        prodTemp = prodTemp->next;
    }
}

int main() {
    CFGList cfgList;

    readGrammer("test.txt",cfgList);

    // Displaying the initial CFG
    cout << "\nInitial Context-Free Grammar (CFG):" << endl;
    cfgList.displayCFG();

    // Delete a rule and a production
    // cfgList.deleteRule(startProduction, 0);
    // cfgList.deleteProduction(1);

    // Displaying the updated CFG
    // cout << "\nUpdated Context-Free Grammar (CFG):" << endl;
    // cfgList.displayCFG();

    // Removing null productions
    removeNullProductions(cfgList);

    // Displaying the CFG after removing null productions
    cout << "\nContext-Free Grammar (CFG) after removing null productions:" << endl;
    cfgList.displayCFG();

    // Removing null productions
    removeUnitProductions(cfgList);

    // Displaying the CFG after removing null productions
    cout << "\nContext-Free Grammar (CFG) after removing unit productions:" << endl;
    cfgList.displayCFG();

    return 0;
}
