#include <iostream>
#include <fstream>
using namespace std;

const int asciSize = 128;

int TT[31][asciSize] = {-1};

void initializeTT() {

    TT[0]['i'] = 1;
    TT[0]['w'] = 4;
    TT[0]['$'] = 9;
    TT[0]['d'] = 13;
    TT[0]['-'] = 12;
    TT[0]['\''] = 15;
    TT[0]['='] = 18;
    TT[0][';'] = 22;
    TT[0]['('] = 23;
    TT[0]['/'] = 27;
    TT[0]['{'] = 25;

    TT[1]['f'] = 2;

    TT[2]['l'] = 3;
    TT[2]['d'] = 3;

    for (int i = 0; i < asciSize; i++) {
        TT[3][i] = 3;
    }

    TT[4]['h'] = 5;

    TT[5]['i'] = 6;

    TT[6]['l'] = 7;

    TT[7]['e'] = 8;

    TT[8]['l'] = 3;
    TT[8]['d'] = 3;

    TT[9]['n'] = 10;

    TT[10]['l'] = 10;
    TT[10]['d'] = 10;
    TT[10]['_'] = 10;
    TT[10]['!'] = 11;
    TT[10]['{'] = 11;
    TT[10]['}'] = 11;

    for (int i = 0; i < asciSize; i++) {
        TT[11][i] = 0;
    }

    TT[12]['e'] = 13;

    TT[13]['l'] = 13;
    TT[13]['d'] = 14;

    for (int i = 0; i < asciSize; i++) {
        TT[14][i] = 0;
    }

    TT[15]['\\'] = 17;
    TT[15]['\''] = 16;

    for (int i = 0; i < asciSize; i++) {
        TT[16][i] = 0;
    }

    TT[17]['\''] = 15;

    TT[18]['='] = 20;
    TT[18]['l'] = 19;
    TT[18]['d'] = 19;

    for (int i = 0; i < asciSize; i++) {
        TT[19][i] = 0;
    }

    for (int i = 0; i < asciSize; i++) {
        TT[20][i] = 0;
    }

    for (int i = 0; i < asciSize; i++) {
        TT[21][i] = 0;
    }

    for (int i = 0; i < asciSize; i++) {
        TT[22][i] = 0;
    }

    TT[23]['('] = 23;
    TT[23][')'] = 23;

    for (int i = 0; i < asciSize; i++) {
        TT[24][i] = 0;
    }

    TT[25]['{'] = 25;
    TT[25]['}'] = 26;

    for (int i = 0; i < asciSize; i++) {
        TT[26][i] = 0;
    }

    TT[27]['*'] = 28;

    TT[28]['*'] = 28;
    TT[28]['/'] = 29;

    TT[29]['*'] = 28;
    TT[29]['/'] = 30;

    for (int i = 0; i < asciSize; i++) {
        TT[30][i] = 0;
    }
}

void genTok(const string& lex, ofstream& outputFile) {
    if (!lex.empty()) {
        outputFile << "Token: ";
        if (lex == "if" || lex == "while") {
            outputFile << "[Keyword] " << lex << endl;
        } else if (lex == "=" || lex == "==" || lex == "!=") {
            outputFile << "[Operator] " << lex << endl;
        } else if (lex == ";" || lex == "(" || lex == ")" || lex == "{" || lex == "}") {
            outputFile << "[Punctuation] " << lex << endl;
        } else if (lex[0] == '$') {
            outputFile << "[Identifier] " << lex << endl;
        } else if (lex[0] == '\'') {
            outputFile << "[Literal] " << lex << endl;
        } else if (lex[0] == '/' && lex.size() > 1 && lex[1] == '*') {
            outputFile << "[Ignored Comment] " << lex << endl;
        } else {
            outputFile << "[Literal] " << lex << endl;
        }
    }
}

int main() {
    initializeTT();

    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");

    string token;
    char ch;
    bool inString = false;

    while (inputFile.get(ch)) {
        if (ch == '\'') {
            inString = !inString;
            if (!inString) {
                genTok(token, outputFile);
                token.clear();
            }
            continue;
        }
        if (!inString && (isspace(ch) || ispunct(ch))) {
            genTok(token, outputFile);
            token.clear();
            if (ch != ' ' && ch != '\'') {
                outputFile << "Token: ";
                if (ch == '$') {
                    outputFile << "[Identifier] ";
                } else if (ch == '=') {
                    outputFile << "[Operator] ";
                } else if (ch == ';' || ch == '(' || ch == ')' || ch == '{' || ch == '}') {
                    outputFile << "[Punctuation] ";
                }
                outputFile << ch << endl;
            }
        } else {
            token += ch;
        }
    }

    genTok(token, outputFile);

    inputFile.close();
    outputFile.close();

    return 0;
}
