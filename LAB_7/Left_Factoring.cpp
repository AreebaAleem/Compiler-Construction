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

void resolveLeftFactoring() {
    Production* prod = startcfg;
    while (prod) {
        Rule* ruleA = prod->rule_list;
        while (ruleA && ruleA->next) {
            Rule* ruleB = ruleA->next;
            while (ruleB) {
                string prefix = commonPrefix(ruleA->rule, ruleB->rule);
                if (!prefix.empty() && prefix.size() > 0) {
                    // Left factoring needed
                    // Create new production for factored rules
                    Production* newProduction = new Production(prod->name + "'");
                    // Move rules with common prefix to the new production
                    moveRulesWithPrefix(prod, newProduction, prefix);
                    // Add the new production after the current production
                    newProduction->next = prod->next;
                    prod->next = newProduction;
                    // Remove the common prefix from original rules
                    removePrefixFromRules(prod, prefix);
                    // Add the common prefix and new production to the original production
                    Rule* prefixRule = new Rule(prefix + newProduction->name);
                    prefixRule->next = prod->rule_list;
                    prod->rule_list = prefixRule;
                    // Display changes
                    cout << "Left factoring applied in production '" << prod->name << "' for rules starting with '" << prefix << "'" << endl;
                    return; // Stop at the first occurrence of left factoring
                }
                ruleB = ruleB->next;
            }
            ruleA = ruleA->next;
        }
        prod = prod->next;
    }
}



void moveRulesWithPrefix(Production* source, Production* destination, const string& prefix) {
    Rule* rule = source->rule_list;
    Rule* prev = nullptr;
    bool prefixFound = false; // Flag to track if the prefix has been found in the original rules
    while (rule) {
        if (rule->rule.substr(0, prefix.size()) == prefix) {
            // Move this rule to the destination production
            if (!destination->rule_list) {
                destination->rule_list = new Rule(rule->rule.substr(prefix.size())); // Remove the prefix
            } else {
                Rule* destRule = destination->rule_list;
                while (destRule->next) {
                    destRule = destRule->next;
                }
                destRule->next = new Rule(rule->rule.substr(prefix.size()));
            }
            // Remove this rule from the source production
            if (!prev) {
                source->rule_list = rule->next;
                delete rule;
                rule = source->rule_list;
            } else {
                prev->next = rule->next;
                delete rule;
                rule = prev->next;
            }
            // Set the flag to indicate that the prefix has been found
            prefixFound = true;
        } else {
            prev = rule;
            rule = rule->next;
        }
    }
    // If the prefix was found in the original rules, modify the first rule to include the prefix and the new production
    if (prefixFound) {
        Rule* firstRule = source->rule_list;
        if (firstRule) {
            firstRule->rule = prefix + destination->name;
        }
    }
}





void removePrefixFromRules(Production* prod, const string& prefix) {
    Rule* rule = prod->rule_list;
    while (rule) {
        if (rule->rule.substr(0, prefix.size()) == prefix) {
            rule->rule = rule->rule.substr(prefix.size()); // Remove the prefix
        }
        rule = rule->next;
    }
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


    cfgList.resolveLeftFactoring();
    
    // Displaying the updated CFG
    cout << "Updated Context-Free Grammar (CFG):" << endl;
    cfgList.displayCFG();

    return 0;
}
