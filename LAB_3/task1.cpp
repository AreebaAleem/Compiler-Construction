#include<iostream>
#include<fstream>
using namespace std;

bool isAlphaNumeric(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9');
}

bool isKeyword(const string& word) {
    return (word == "int" || word == "float" || word == "double" || word == "char" || word == "bool");
}

void generateToken(const string& line) {
    string lexeme = "";
    for (char c : line) {
        if (isAlphaNumeric(c) || c == '_') {
            lexeme += c;
        } else {
            if (!lexeme.empty()) {
                if (isKeyword(lexeme)) {
                    cout << "Keyword: " << lexeme << endl;
                } else {
                    cout << "Identifier: " << lexeme << endl;
                }
                lexeme = "";
            }
            if (c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',' || c == '=') {
                cout << "Punctuation: " << c << endl;
            }
        }
    }
}

int main() {
    ifstream MyReadFile("input.txt");
    string line;
    while (getline(MyReadFile, line)) {
        generateToken(line);
    }
    MyReadFile.close();
    return 0;
}
