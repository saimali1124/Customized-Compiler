#ifndef _LEXER_H_
#define _LEXER_H_
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

//all your tokens goes here
enum class TokenType
{
	END_OF_FILE = 0,
	ID = 1,
	NUM = 2,
	RO = 3,
	INPUT = 4,
	OUTPUT = 5,
	STR = 6,
	CMT = 7,
	ASSIGNMENT = 8,
	null = 9,
	markazi=10,
    kaam=11,
    karo=12,
    rakho=13,
    jab=14,
    tak=15,
    bas=16,
    agar=17,
    to=18,
    warna=19,
    phir=20,
    dekhao=21,
    lo=22,
    chalao=23,
    wapas=24,
    bhaijo=25,
    adad=26,
    khali=27,
    khatam=28,
	special1=29,
	specialcolon=30,
	specialsemi=31,
	specialpipe=32,
	specialtick=33,
	specialopenbracket=34,
	specialclosebracket=35,
	plus=36,
	minus=37,
	multiply=38,
	divide=39,
	modulus=40,
	END_OF_LINE=41,
	ERROR

};
//structure of a token 
struct token
{
	string lexeme;
	TokenType tokenType;//enum type
	//constructor
	token(string lexeme, TokenType tokenType);
	//constructor default
	token();
	void Print();
};
class lexer
{
	vector<char> stream;  //used for storing file sample_code.ol content
	vector<token> tokens; //vector to store all (tokens,lexeme) pairs
	void Tokenize();//populates tokens vector
	int index;

public:
	lexer();
	lexer(const char filename[]);
	void printRaw();//just print everything as it is present in file
	token getNextToken();//get next token
	void resetPointer();//reset pointer to start getting tokens from start
	int getCurrentPointer();//get where current pointer in tokens vector is
	void setCurrentPointer(int pos);//move current pointer to wherever
	token peek(int);//peek the next token
};

#endif // !_LEXER_H