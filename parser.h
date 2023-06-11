#ifndef _PARSER_H_
#define _PARSER_H_
#include "lexer.h"
//for future assignments leave it as it is
class parser
{
    lexer _lexer;
public:
    static int starting_line_number;
    void syntax_error();
    void incLine();
    void checkNewLine();
    token expect(TokenType expected_type);
    parser(const char filename[]);
    void readAndPrintAllInput();
    void resetPointer();
    void printSymbolTable();
    void printTabs(int t);
    void printTAC(string s);
    string Temp();
    /*Terminal functions goes here use peek and expect*/
    /*use TokenType:: for token names for example
        expect(TokenType::ASSIGN);   //example function call
      */
	//all your parser function goes here
    void Start();
    void Starting();
    void Function();
    string Datatype();
    void Parameters();
    void Param();
    void Statements();
    void Statement();
    void Declare();
    void DeclareType();
    void AssignToExpression();
    void Assigned();
    void FunctionCall();
    void Argument();
    void K();
    void ZZ();
    void CommentSection();
    void Comment();
    void If();
    void ElseIf();
    void Else();
    void Return();
    void Output();
    void Input();
    void Inp();
    void Loop();
    void Expression();
    void R();
    void E();
    void X();
    /// @param val : this value is to distinguish between Z1 and Z2 .i.e which variable should be updated 
    void Z(bool val);
    void J();
    void H();
    token Funcname();
};
#endif