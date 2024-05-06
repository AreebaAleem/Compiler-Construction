#include <iostream>
#include <string>

using namespace std;

const int asciSize = 128;

int TT[5][asciSize] = {-1};

void initializeTT() {
    for (int i = 0; i < asciSize; i++) {
        TT[0][i] = 0;
        TT[1][i] = 1;
        TT[2][i] = 2;
        TT[3][i] = 3;
        TT[4][i] = 4;
    }
    TT[0]['<'] = 1;
    TT[0]['>'] = 1;
    TT[1]['='] = 2;
    TT[0]['!'] = 3;
    TT[3]['='] = 2;
}

int main() {
    initializeTT();
    string code = "< <= > >= != == << <> == >> ><";
    int state = 0;
    string lex;

    for (char ch : code) {
        if (ch == ' ') {
            if ((state == 1 || state == 2) && lex != "<>" && lex != "<<" && lex != ">>" && lex != "><") {
                cout << "Operator: " << lex << endl;
            }
            state = 0;
            lex = "";
        } else {
            state = TT[state][ch];
            lex += ch;
        }
    }
    if ((state == 1 || state == 2) && lex != "<>" && lex != "<<" && lex != ">>" && lex != "><") {
        cout << "Operator: " << lex << endl;
    }

    return 0;
}
