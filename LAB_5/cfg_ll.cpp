#include <iostream>
#include <string>

struct Rule {
    std::string content;
    Rule* next;

    Rule(const std::string& c) : content(c), next(nullptr) {}
};

struct Production {
    char name;
    Rule* firstRule;
    Production* next;

    Production(char n) : name(n), firstRule(nullptr), next(nullptr) {}
};

void insertProduction(Production*& head, char name) {
    Production* newProd = new Production(name);
    newProd->next = head;
    head = newProd;
}

void insertRule(Production* head, char prodName, const std::string& ruleContent) {
    Production* prod = head;
    while (prod && prod->name != prodName) {
        prod = prod->next;
    }
    if (prod) {
        Rule* newRule = new Rule(ruleContent);
        if (prod->firstRule == nullptr) {
            prod->firstRule = newRule;
        } else {
            Rule* lastRule = prod->firstRule;
            while (lastRule->next != nullptr) {
                lastRule = lastRule->next;
            }
            lastRule->next = newRule;
        }
    }
}


void deleteProduction(Production*& head, char name) {
    Production* prev = nullptr;
    Production* curr = head;
    while (curr && curr->name != name) {
        prev = curr;
        curr = curr->next;
    }
    if (curr) {
        while (curr->firstRule) {
            Rule* temp = curr->firstRule;
            curr->firstRule = curr->firstRule->next;
            delete temp;
        }
        if (prev) {
            prev->next = curr->next;
        } else {
            head = curr->next;
        }
        delete curr;
    }
}

void deleteRule(Production* head, char prodName, const std::string& ruleContent) {
    Production* prod = head;
    while (prod && prod->name != prodName) {
        prod = prod->next;
    }
    if (prod) {
        Rule* curr = prod->firstRule;
        Rule* prev = nullptr;
        while (curr && curr->content != ruleContent) {
            prev = curr;
            curr = curr->next;
        }
        if (curr) {
            if (prev) {
                prev->next = curr->next;
            } else {
                prod->firstRule = curr->next;
            }
            delete curr;
        }
    }
}

void printCFG(Production* head) {
    while (head) {
        std::cout << head->name << "->";
        Rule* rule = head->firstRule;
        if (rule) {
            while (rule) {
                std::cout << rule->content;
                if (rule->next) std::cout << "|";
                rule = rule->next;
            }
        } else {
            std::cout << "No rules";
        }
        std::cout << std::endl;
        head = head->next;
    }
}


int main() {
    Production* cfg = nullptr;

    char prodName;
    std::string ruleContent;

    std::cout << "Enter a production (enter '!' to stop):\n";
    while (std::cin >> prodName && prodName != '!') {
        insertProduction(cfg, prodName);

        std::cout << "Enter rules for production " << prodName << " (enter '!' to stop):\n";
        std::cin.ignore();
        std::getline(std::cin, ruleContent);
        while (ruleContent != "!") {
            insertRule(cfg, prodName, ruleContent);
            std::getline(std::cin, ruleContent);
        }

        std::cout << "Enter next production or '!' to stop:\n";
    }

    std::cout << "\nLoaded CFG:\n";
    printCFG(cfg);

    while (cfg) {
        deleteProduction(cfg, cfg->name);
    }

    return 0;
}
