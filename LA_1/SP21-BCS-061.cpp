#include <iostream>
#include <fstream>
using namespace std;

const int asciSize = 128;

int TT[5][asciSize] = {-1};

void initializeTT() {
    for (int i = 0; i < asciSize; i++) {
        TT[0][i] = 0;
        TT[1][i] = 1;
        TT[2][i] = 2;
        TT[3][i] = 2;
        TT[4][i] = 0;
    }
    TT[0]['/'] = 1;
    TT[1]['*'] = 2;
    TT[2]['*'] = 3;
    TT[3]['*'] = 3;
    TT[3]['/'] = 3;
    TT[4]['/'] = 0;
}

int main() {
    initializeTT();
    const int bufferSize = 30;
    char buffer1[bufferSize];
    char buffer2[bufferSize];
    char* ptr1 = buffer1;
    char lex[20];
    char ch;
    int i;
    int state = 0;
    int li=0;
    bool notComment = false;

    ifstream inputFile("input.txt");

    while (!inputFile.eof()) {
        inputFile.read(ptr1, bufferSize);
        string input(ptr1, inputFile.gcount());
        i = 0;
        for (char ch : input) {
            if (state == 0) {
                if (ch == '/' || ch == '*') {
                    state = TT[state][ch];
                    lex[li] = ch;
                    li++;
                    notComment = false;
                } else {
                    if (!notComment) {
                        cout << "--------Not a Comment----------"<<endl;
                        notComment = true;
                    }
                }
            } else if (state == 1) {
                if (ch == '*' || ch == '/') {
                    state = TT[state][ch];
                    lex[li] = ch;
                    li++;
                } else {
                    state = 0;
                    li = 0;
                }
            } else if (state == 2) {
                if (ch != '*') {
                    state = TT[state][ch];
                    lex[li] = ch;
                    li++;
                } else {
                    state = 3;
                    lex[li] = ch;
                    li++;
                }
            } else if (state == 3) {
                if (ch == '/') {
                    state = 4;
                    lex[li] = ch;
                    li++;
                } else {
                    state = 2;
                }
            } else if (state == 4) {
                lex[li] = '\0';
                cout <<"Ignored Comment: " << lex << endl;
                state = 0;
                li = 0;
            }
            i++;
            if(i == 30){
                break;
            }
        }
        ptr1 = (ptr1 == buffer1) ? buffer2 : buffer1;
    }

    if (notComment) {
    cout << "--------Not a Comment----------"<<endl;
}

    inputFile.close();
    return 0;
}