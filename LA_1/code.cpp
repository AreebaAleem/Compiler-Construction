#include <iostream>
#include <fstream>
using namespace std;

enum COL{S=0,SLASH,STAR,NOT_STAR,A,R,Token};
enum B{F=0,T};
enum TOK{N=0,ID,PUN,OP,LIT,KEY,COMMENT};

int TT[5][7]=
{
    {0,1,-1,-1,F,F,N},
    {1,-1,2,-1,F,F,N},
    {2,-1,3,2,F,F,N},
    {3,-1,3,2,F,F,N},
    {4,4,0,0,T,F,N}
};

int getcharT(char ch){
    if(ch == '/')
        return SLASH;
    else if(ch == '*')
        return STAR;
    else
        return NOT_STAR;
}

void genTok(const char* lex, int tokenType) {
    std::cout << "Generated token: " << lex << ", Type: " << tokenType << std::endl;
}

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
                    if (ch == '/' || ch == '*') {
                        state = TT[state][getcharT(ch)];
                        lex[li] = ch;
                        li++;
                    }
                    break;
                case 1:
                    if (ch == '*' || ch == '/') {
                        state = TT[state][getcharT(ch)];
                        lex[li] = ch;
                        li++;
                    } else {
                        state = 0;
                        i = i--;
                        li = 0;
                    }
                    break;
                case 2:
                    if (ch != '*') {
                        state = TT[state][getcharT(ch)];
                        lex[li] = ch;
                        li++;
                    } else {
                        state = 3;
                    }
                    break;
                case 3:
                    if (ch == '/') {
                        state = 4;
                    } else {
                        state = 2;
                    }
                    break;
                case 4:
                    lex[li] = '\0';
                    cout <<"Ignored Comment: " << lex << endl;
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