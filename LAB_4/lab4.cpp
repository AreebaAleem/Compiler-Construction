#include <iostream>
#include <cstring>

enum COL{S=0,H,NH,A,R,T};
enum B{F=0,T};
enum TOK{N-0,ID,PUN,OP,LIT,KEY};

int TT[5][6]=
{
    {1,-1,-1,F,F,N},
    {-1,2,-1,F,F,N},
    {-1,3,2,F,F,N},
    {-1,3,2,F,F,N},
    {4,0,0,T,F,N}
};

int getcharT(char ch){
    return 0;
}

void genTok(const char* lex, int tokenType) {
    std::cout << "Generated token: " << lex << ", Type: " << tokenType << std::endl;
}

int main(){
char ch;
char lex[20];
int li=0;
int bi=0;
int state=0;
char buff[4096];
while(ch=buff[bi]!=EOF)
{
    if(!TT[state][A])
    {
        state=TT[state][getCharT(ch)]
        lex[li]=ch;
        li++;
    }
    else
    {
        lex[li]='\0';
    }
}

genTok(lex,TT[state][T]);
if(TT[state][R])
{
    bi--;
    li=0;
    state=0;
}
}