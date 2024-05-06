#pragma once
#include <string>

//CFG symbols {Terminals and Non-Terminals}
struct Symbol
{
	char s;
	bool terminal;
};

//Grammer Rule
struct Rule
{
	Symbol rule[10];
	Rule * next;
	Rule * prev;
};

//Grammer Rule List
struct RList					
{
	Symbol s;
	Rule * head;
	Rule * last;
	RList * next_rlist;
	RList * prev_rlist;
};

