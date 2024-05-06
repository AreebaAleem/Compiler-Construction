#include <iostream>
#include <string>
#include <sstream>

int hashFunction(const std::string& word);

class Node {
public:
    std::string data;
    Node* next;

    Node(const std::string& data) {
        this->data = data;
        this->next = nullptr;
    }
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList() {
        head = nullptr;
    }

    void insertion(const std::string& data) {
        Node* new_node = new Node(data);
        if (head == nullptr) {
            head = new_node;
        } else {
            Node* last_node = head;
            while (last_node->next != nullptr) {
                last_node = last_node->next;
            }
            last_node->next = new_node;
        }
    }

    std::pair<int, int> search(const std::string& data) {
        int count = 0;
        int index = -1; // Initialize index as -1 to indicate not found
        Node* current_node = head;
        while (current_node != nullptr) {
            if (current_node->data == data) {
                count++;
                // Save the index when the word is found
                index = hashFunction(data);
            }
            current_node = current_node->next;
        }
        return {count, index};
    }

    void display() {
        Node* current_node = head;
        while (current_node != nullptr) {
            std::cout << current_node->data << " -> ";
            current_node = current_node->next;
        }
        std::cout << "nullptr" << std::endl;
    }

    ~LinkedList() {
        Node* current_node = head;
        while (current_node != nullptr) {
            Node* next_node = current_node->next;
            delete current_node;
            current_node = next_node;
        }
    }
};

int hashFunction(const std::string& word) {
    int sum = 0;
    for (char c : word) {
        sum += static_cast<int>(c);
    }
    return (sum % 10);
}

int main() {
    LinkedList array_of_linked_lists[10];

    std::string sentence;
    std::cout << "Enter a sentence: ";
    std::getline(std::cin, sentence);

    std::istringstream iss(sentence);
    std::string word;

    while (iss >> word) {
        int hash_value = hashFunction(word);
        array_of_linked_lists[hash_value].insertion(word);
    }

    std::string search_word;
    std::cout << "Enter a word to search: ";
    std::cin >> search_word;

    auto result = array_of_linked_lists[hashFunction(search_word)].search(search_word);
    int count = result.first;
    int index = result.second;

    if (count > 0) {
        std::cout << "The word \"" << search_word << "\" appears " << count << " times in the sentence." << std::endl;
        std::cout << "It is stored at key (hash value) " << index << " in the hash table." << std::endl;
    } else {
        std::cout << "The word \"" << search_word << "\" does not appear in the sentence." << std::endl;
    }

    return 0;
}
