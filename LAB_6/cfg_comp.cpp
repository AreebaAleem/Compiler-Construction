#include <iostream>
#include <string>
#include <limits>

struct Rule {
    std::string r;
    Rule* next;
};

struct Production {
    std::string name;
    Production* next;
    Rule* first;
    Rule* last;
};

void insertProduction(Production*& head) {
    std::string input;
    char choice;

    do {
        Production* newProduction = new Production;

        std::cout << "Enter the production in the format A-->Ab|dB|e: ";
        std::getline(std::cin, input);

        size_t arrowPos = input.find("-->");
        if (arrowPos != std::string::npos) {
            newProduction->name = input.substr(0, arrowPos);
            input.erase(0, arrowPos + 3);
        }

        size_t pipePos = 0;
        Rule* previousRule = nullptr;
        while ((pipePos = input.find("|")) != std::string::npos) {
            Rule* newRule = new Rule;
            newRule->r = input.substr(0, pipePos);
            newRule->next = nullptr;

            if (newProduction->first == nullptr) {
                newProduction->first = newRule;
                newProduction->last = newRule;
            } else {
                newProduction->last->next = newRule;
                newProduction->last = newRule;
            }

            input.erase(0, pipePos + 1);
        }

        if (!input.empty()) {
            Rule* lastRule = new Rule;
            lastRule->r = input;
            lastRule->next = nullptr;
            if (newProduction->first == nullptr) {
                newProduction->first = lastRule;
                newProduction->last = lastRule;
            } else {
                newProduction->last->next = lastRule;
                newProduction->last = lastRule;
            }
        }

        newProduction->next = nullptr;

        if (head == nullptr) {
            head = newProduction;
        } else {
            Production* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newProduction;
        }

        std::cout << "Do you want to add another production? (y/n): ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (choice == 'y' || choice == 'Y');
}


void insertRule(Production*& head) {
    char choice;

    std::cout << "Do you want to add a rule (r) or a production (p)? (r/p): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 'p' || choice == 'P') {
        insertProduction(head);
    } else if (choice == 'r' || choice == 'R') {
        std::string productionName;
        std::cout << "Enter the name of the production: ";
        std::getline(std::cin, productionName);

        Production* current = head;
        bool productionFound = false;
        while (current != nullptr) {
            if (current->name == productionName) {
                productionFound = true;
                break;
            }
            current = current->next;
        }

        if (productionFound) {
            Rule* newRule = new Rule;
            std::cout << "Enter the rule: ";
            std::getline(std::cin, newRule->r);
            newRule->next = nullptr;

            if (current->first == nullptr) {
                current->first = newRule;
                current->last = newRule;
            } else {
                current->last->next = newRule;
                current->last = newRule; // Update the last pointer
            }

            std::cout << "Rule added to production " << productionName << std::endl;
        } else {
            std::cout << "Production " << productionName << " not found. Please enter a valid production." << std::endl;
        }
    } else {
        std::cout << "Invalid choice. Please enter 'r' to add a rule or 'p' to add a production." << std::endl;
    }
}


void displayProductions(Production* head) {
    std::cout << "Productions:\n";
    Production* current = head;
    while (current != nullptr) {
        std::cout << current->name << " --> ";
        Rule* rule = current->first;
        while (rule != nullptr) {
            std::cout << rule->r << "|";
            rule = rule->next;
        }
        std::cout << std::endl;
        current = current->next;
    }
}
void deleteProduction(Production*& head) {
    std::string productionName;
    std::cout << "Enter the name of the production you want to delete: ";
    std::getline(std::cin, productionName);

    Production* current = head;
    Production* prev = nullptr;
    bool productionFound = false;

    while (current != nullptr) {
        if (current->name == productionName) {
            productionFound = true;
            break;
        }
        prev = current;
        current = current->next;
    }

    if (productionFound) {
        // Remove the production from the linked list
        if (prev != nullptr) {
            prev->next = current->next;
        } else {
            head = current->next;
        }
        delete current; // Free memory
        std::cout << "Production " << productionName << " deleted successfully." << std::endl;
    } else {
        std::cout << "Production " << productionName << " not found. Cannot delete." << std::endl;
    }
}
void deleteRule(Production* head) {
    std::string productionName;
    std::cout << "Enter the name of the production from which you want to delete a rule: ";
    std::getline(std::cin, productionName);

    Production* current = head;
    bool productionFound = false;

    // Find the production
    while (current != nullptr) {
        if (current->name == productionName) {
            productionFound = true;
            break;
        }
        current = current->next;
    }

    if (productionFound) {
        std::string ruleToDelete;
        std::cout << "Enter the rule you want to delete: ";
        std::getline(std::cin, ruleToDelete);

        Rule* prevRule = nullptr;
        Rule* currentRule = current->first;
        bool ruleFound = false;

        // Find the rule in the production
        while (currentRule != nullptr) {
            if (currentRule->r == ruleToDelete) {
                ruleFound = true;
                break;
            }
            prevRule = currentRule;
            currentRule = currentRule->next;
        }

        if (ruleFound) {
            // Remove the rule from the production
            if (prevRule != nullptr) {
                prevRule->next = currentRule->next;
            } else {
                current->first = currentRule->next;
            }
            if (current->last == currentRule) {
                current->last = prevRule;
            }
            delete currentRule; // Free memory
            std::cout << "Rule '" << ruleToDelete << "' deleted from production '" << productionName << "' successfully." << std::endl;
        } else {
            std::cout << "Rule '" << ruleToDelete << "' not found in production '" << productionName << "'. Cannot delete." << std::endl;
        }
    } else {
        std::cout << "Production '" << productionName << "' not found. Cannot delete rule." << std::endl;
    }
}

int main() {
    Production* head = nullptr;
    insertProduction(head);
    insertRule(head);
    displayProductions(head);
    deleteProduction(head);
    displayProductions(head);
    deleteRule(head);
    displayProductions(head);
    return 0;
}