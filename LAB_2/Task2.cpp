#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Node {
public:
    string data;
    Node* next;

    Node(string value) : data(value), next(nullptr) {}
};

class LinkedList {
private:
    Node* First;
    Node* Last;
    Node** hashArray;
    int size;

public:
    LinkedList(int tableSize) : First(nullptr), Last(nullptr), size(tableSize) {
        hashArray = new Node*[size]();
        for (int i = 0; i < size; ++i) {
            hashArray[i] = nullptr;
        }
    }

    ~LinkedList() {
        for (int i = 0; i < size; ++i) {
            Node* current = hashArray[i];
            while (current != nullptr) {
                Node* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] hashArray;
    }

    int hashFunction(int value) {
        return value % size;
    }

void insert(int value, string word) {
    int key = hashFunction(value);
    Node* newNode = new Node(word);
    if (hashArray[key] == nullptr) {
        hashArray[key] = newNode;
    } else {
        // Traverse to the end of the linked list and append the new node
        Node* current = hashArray[key];
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}


    string search(int value) {
        int key = hashFunction(value);
        Node* current = hashArray[key];
        while (current != nullptr) {
            // Assuming that value matches the key in the linked list
            return current->data;
        }
        return "Not found";
    }
};

int main() {
    LinkedList hash(7);

    // Prompt user to enter a paragraph
    cout << "Enter a paragraph: ";
    string paragraph;
    getline(cin, paragraph);

    // Tokenize paragraph into words
    stringstream ss(paragraph);
    string word;
    while (ss >> word) {
        // Convert word to hash value
        int hashValue = 0;
        for (char ch : word) {
            hashValue += ch;
        }
        // Insert word into hash table
        hash.insert(hashValue, word);
    }

    // Asking user to enter a value to search
    int searchValue;
    cout << "Enter a value to search: ";
    cin >> searchValue;

    // Searching for the user-provided value
    string result = hash.search(searchValue);
    if (result != "Not found") {
        cout << "Value " << searchValue << " found at key: " << hash.hashFunction(searchValue) << ", Word: " << result << endl;
    } else {
        cout << "Value " << searchValue << " not found" << endl;
    }

    return 0;
}