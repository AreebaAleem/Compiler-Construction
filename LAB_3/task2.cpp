#include <iostream>
#include <fstream>
using namespace std;

int main() {
    const int bufferSize = 30;
    char buffer1[bufferSize];
    char buffer2[bufferSize];
    char* ptr1 = buffer1;
    char lex[20];
    char ch;
    int i;
    int state = 0;
    int li=0;

    ifstream inputFile("input.txt");

    while (!inputFile.eof()) {
        inputFile.read(ptr1, bufferSize);
        string input(ptr1, inputFile.gcount());
        i = 0;
        for (char ch : input) {
        switch (state) {
            case 0:
                if (ch == '_' || isalpha(ch)) {
                    state = 1;
                    lex[li] = ch;
                    li++;
                }
                break;
            case 1:
                if (ch == '_' || isalnum(ch) || isalpha(ch)) {
                    state = 1;
                    lex[li] = ch;
                    li++;
                } else {
                    state = 2;
                }
                break;
            case 2:
                lex[li] = '\0';
                cout <<"Valid: " <<"<id> " << lex << endl;
                state = 0;
                i = i--;
                li = 0;
                break;
    }
    i++;
    if(i == 30){
        break;
    }
    }
        ptr1 = (ptr1 == buffer1) ? buffer2 : buffer1;
        cout << "------------------"<<endl;
    }

    inputFile.close();
    return 0;
}