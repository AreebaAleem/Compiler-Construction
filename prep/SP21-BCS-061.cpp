#include <iostream>
#include <fstream>
using namespace std;

const int asciSize = 128;

int TT[4][asciSize] = {-1};

void initializeTT() {
    for (int i = 0; i < asciSize; i++) {
        TT[0][i] = 0;
        TT[1][i] = 1;
        TT[2][i] = 2;
        TT[3][i] = 1;
    }
    TT[0]['"'] = 1;
    TT[0]['/'] = 2;
    TT[1]['"'] = 0;
    TT[1]['\\'] = 3;
    TT[2]['/'] = 2;
}

int main() {
    initializeTT();
    const int bufferSize = 50;
    char buffer1[bufferSize];
    char buffer2[bufferSize];
    char* ptr1 = buffer1;
    int state = 0;

    ifstream inputFile("input.cpp");
    ofstream outputFile("output.cpp");

    while (!inputFile.eof()) {
        inputFile.read(ptr1, bufferSize);
        string input(ptr1, inputFile.gcount());
        for (char ch : input) {
            if (state == 0 && (ch == '"' || ch == '/')) {
                state = TT[state][ch];
            } else if (state == 1) {
                if (ch == '"' || ch == '\\') {
                    state = TT[state][ch];
                    continue;
                }
            } else if (state == 2) {
                if (ch != '\n') {
                    continue;
                } else {
                    state = 0;
                }
            } else if (state == 3) {
                state = 1;
            }
            if (state == 0) {
                outputFile << ch;
            }
        }
        ptr1 = (ptr1 == buffer1) ? buffer2 : buffer1;
    }

    inputFile.close();
    outputFile.close();
    return 0;
}