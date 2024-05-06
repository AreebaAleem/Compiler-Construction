//create a link list and pass paragraph and it should treat each word as a node and print it
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

struct node {
    string info;
    node *next;
};

struct node *List = NULL;

void insertion(string paragraph) {
    stringstream ss(paragraph);
    string word;

    while (ss >> word) {
        node *p = new node;
        p->info = word;
        p->next = NULL;

        if (List == NULL) {
            List = p;
        } else {
            node *temp = List;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = p;
        }
    }
}

void display() {
    node *ptr = List;
    while (ptr != NULL) {
        cout << ptr->info << endl;
        ptr = ptr->next;
    }
}

int main() {
    cout << "Enter a paragraph: ";
    string paragraph;
    getline(cin, paragraph);

    insertion(paragraph);
    display();

    while (List != NULL) {
        node *temp = List;
        List = List->next;
        delete temp;
    }

    return 0;
}
