#include "parser.h"
#include <fstream>
#include <algorithm>
#include <limits>

//* global variables
int line=1;
int tabs = 0;
int noOfParams =0;
int variableNum =0;
string IDname="";
int addressLine =0;
bool first = true;
int Elseif_i = 0;
int Else_i = 0;
int address = 0;
int initialValue = 0;
bool constOnRHS = 1;
string context = "global";
string temp = "";
string Z1 = "";
string Z2 = "";
string E1 = "";
string E2 = "";
bool assignment = false;
bool ifCondition = false;
string if_P1 = "";
string if_P2 = "";
int parser::starting_line_number = 0;

struct SymbolTable
{
    string idname;
    string type;
    string dataType;
    int initialValue;
    int address;
    string context;
};

//* global variable
vector<SymbolTable> symbolTable;

void parser::syntax_error()
{
    cout << "SYNTAX ERROR at line = ";
    cout<<line;
    exit(1);
}

void parser::incLine()
{
    line++;
}
token parser::expect(TokenType expected_type)
{
    token t = _lexer.getNextToken();
    if (t.tokenType != expected_type)
    {
        // cout << "Expected " << reserved1[(int)expected_type] << " Got: ";
        // t.Print();
        syntax_error();
    }
    return t;
}
parser::parser(const char filename[])
{
    _lexer = lexer(filename);
}
void parser::readAndPrintAllInput() // read and print allinputs (provided)
{
    token t;
    t = _lexer.getNextToken();
    while (t.tokenType != TokenType::END_OF_FILE)
    {
        t.Print();
        t = _lexer.getNextToken();
    }
}
void parser::resetPointer()
{
    _lexer.resetPointer();
}

/// @brief Printing the symbol Table in the file "symbol_table.txt"
void parser::printSymbolTable()
{
    ofstream f;
    f.open("symbol_table.txt");
    f<<"*ID - Type - Datatype - Initial Value - Address - Context*"<<endl<<endl;
    for (const auto &symboll : symbolTable)
    {
        f << symboll.idname << " - " << symboll.type << " - " << symboll.dataType << " - " << symboll.initialValue << " - " << symboll.address << " - " << symboll.context << endl;
    }
}

/// @brief Print "t" number of tabs
/// @param t : number of tabs to print
void parser::printTabs(int t)
{
    for(int i = 0; i < t; i++)
        cout<<'\t';
}

/// @brief Function to check if we encountered a new line token
void parser::checkNewLine()
{
    if(_lexer.peek(1).tokenType == TokenType::END_OF_LINE)
    {
        expect(TokenType::END_OF_LINE);
        incLine();
    }
}

/// @brief returns the lexeme for RO token to be printed in TAC file
/// @param lexeme : lexeme of the RO (token,lexeme) pair
string LexemeForRO(string lexeme)
{
    if(lexeme == "LT")
        return " < ";
    else if(lexeme == "GT")
        return " > ";
    else if(lexeme == "LE")
        return " <= ";
    else if(lexeme == "GE")
        return " >= ";
    else if(lexeme == "EQ")
        return " == ";
    else if(lexeme == "NE")
        return " <> ";
}

/// @brief Filling the blanks of goto lines
/// @param lineNumber : line number of the goto line
/// @param jumpToLine : number to print in place of blank
void BackPatch(int lineNumber, int jumpToLine)
{
    fstream file;
    file.open("TAC.txt", ios::in | ios::out);
    if (!file.is_open()) {
        cout << "Error: could not open file" << endl;
        return;
    }
    string line;
    int count = 1;
    vector<string> lines;
    while (getline(file, line)) {
        lines.push_back(line);
        count++;
    }
    line = to_string(jumpToLine);
    lines[lineNumber-1] += line;
    // cout<<lines[lineNumber-1]<<endl;
    file.close();
    ofstream file1;
    file1.open("TAC.txt");
    if(file1.is_open())
    {
        for (int i = 0; i < lines.size(); i++) {
            if(i == lines.size() - 1)
                file1 << lines[i];
            else
                file1 << lines[i] << endl;
        }
    }
    file1.close();
}

/// @brief Output the Three Address Code to file
/// @param s : The string to be printed in file
void parser::printTAC(string s)
{
    ofstream file;
    file.open("TAC.txt",ios::app);
    if(file.is_open())
    {
        char t = '"';

        if(first==true)
        {
            addressLine++;
            file << addressLine;
            file << "   ";
            first=false;
        }

        if(s[0]==t)
        {
            for(int i=1; i<s.size()-1; i++)
            {
                file << s[i];
            }
        }
        else{
            file << s;
        }

        if(s=="\n")
        {
            addressLine++;
            file << addressLine;
            file << "   ";
        }

        file.close();
    }
    else{
        cout<<"File error";
    }
}

/// @brief Generate a temporary variable to for TAC
string parser::Temp()
{ 
    variableNum++;
    return "t" + to_string(variableNum);
}

// function definitions of CFG
void parser::Start()
{
    if(_lexer.peek(1).tokenType == TokenType::END_OF_LINE)
    {
        expect(TokenType::END_OF_LINE);
        incLine();
    }
    ++tabs; printTabs(tabs); cout<<"Start"<<endl;
    CommentSection();
    Starting();
    --tabs;
}
void parser::Starting()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Starting"<<endl;
    if (_lexer.peek(1).tokenType == TokenType::kaam)
    {
        ++tabs; printTabs(tabs); cout<<"kaam"<<endl;--tabs;
        expect(TokenType::kaam);
        checkNewLine();
        Function();
        ++tabs; printTabs(tabs); cout<<"kaam"<<endl;--tabs;
        expect(TokenType::kaam);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"khatam"<<endl;--tabs;
        expect(TokenType::khatam);
        
        checkNewLine();
        Start();
    }
    else if(_lexer.peek(1).tokenType == TokenType::END_OF_FILE)
    {
        ++tabs; printTabs(tabs); cout<<"END_OF_FILE"<<endl;--tabs;
        expect(TokenType::END_OF_FILE);
        
        checkNewLine();
    }
    else{
        syntax_error();
    }
    --tabs;
}
void parser::CommentSection()
{
    
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"CommentSection"<<endl;
    if (_lexer.peek(1).tokenType == TokenType::specialsemi)
    {
        ++tabs; printTabs(tabs); cout<<";"<<endl; --tabs;
        expect(TokenType::specialsemi);
        
        checkNewLine();
        Comment();
        CommentSection();
    }
    else
    {
        ;
    }
    --tabs;
}
void parser::Comment()
{
    
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Comment"<<endl;
    if (_lexer.peek(1).tokenType == TokenType::CMT)
    {
        ++tabs; printTabs(tabs); cout<<"CMT"<<endl; --tabs;
        expect(TokenType::CMT);
        
        checkNewLine();
    }
    else
    {
        ;
    }
    --tabs;
}
void parser::Function()
{
    checkNewLine();

    ++tabs; printTabs(tabs); cout<<"Function"<<endl;
    token t = Funcname();
    ++tabs; printTabs(tabs); cout<<"@"<<endl; --tabs;
    expect(TokenType::special1);

    checkNewLine();
    string typpe = Datatype();

    if (t.lexeme != "No")
    {
        SymbolTable symbol;
        symbol.idname = t.lexeme;
        symbol.dataType = typpe;
        symbol.type = "func";
        symbol.address = line;
        symbol.initialValue = 0;
        symbol.context = "global";
        address = 0;
        context = symbol.idname;
        bool found = false;
        for (auto it = symbolTable.begin(); it != symbolTable.end(); it++)
        {
            if (it->idname == symbol.idname)
            {
                found = true;
            }
        }
        if (!found)
        {
            symbolTable.push_back(symbol);
        }
    }
    ++tabs; printTabs(tabs); cout<<"("<<endl; --tabs;
    expect(TokenType::specialopenbracket);
    checkNewLine();
    Parameters();
    ++tabs; printTabs(tabs); cout<<")"<<endl; --tabs;
    expect(TokenType::specialclosebracket);
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"karo"<<endl; --tabs;
    expect(TokenType::karo);
    checkNewLine();
    Statements();

    --tabs;
}
string parser::Datatype()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Datatype"<<endl;
    string type;
    if (_lexer.peek(1).tokenType == TokenType::adad)
    {
        ++tabs; printTabs(tabs); cout<<"adad"<<endl; --tabs;
        expect(TokenType::adad);
        checkNewLine();
        type = "adad";
    }
    else if (_lexer.peek(1).tokenType == TokenType::khali)
    {
        ++tabs; printTabs(tabs); cout<<"khali"<<endl; --tabs;
        expect(TokenType::khali);
        checkNewLine();
        type = "void";
    }
    else{
        syntax_error();
    }
    --tabs;
    return type;
}
void parser::Parameters()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Parameters"<<endl;
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        ++tabs; printTabs(tabs); cout<<"ID"<<endl; --tabs; 
        token t = expect(TokenType::ID);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"@"<<endl; --tabs; 
        expect(TokenType::special1);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"adad"<<endl; --tabs; 
        expect(TokenType::adad);
        checkNewLine();

        SymbolTable symbol;
        symbol.idname = t.lexeme;
        symbol.dataType = "adad";
        symbol.type = "var";
        symbol.address = address;
        symbol.initialValue = 0;
        symbol.context = context;
        bool found = false;
        for (auto it = symbolTable.begin(); it != symbolTable.end(); it++)
        {
            if (it->idname == symbol.idname)
            {
                found = true;
            }
        }
        if (!found)
        {
            address += 4;
            symbolTable.push_back(symbol);
        }

        Param();
    }
    else
    {
        ;
    }
    --tabs;
}
void parser::Param()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Param"<<endl;  
    if (_lexer.peek(1).tokenType == TokenType::specialpipe)
    {
        ++tabs; printTabs(tabs); cout<<"|"<<endl; --tabs; 
        expect(TokenType::specialpipe);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"ID"<<endl; --tabs; 
        token t = expect(TokenType::ID);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"@"<<endl; --tabs; 
        expect(TokenType::special1);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"adad"<<endl; --tabs; 
        expect(TokenType::adad);
        checkNewLine();

        SymbolTable symbol;
        symbol.idname = t.lexeme;
        symbol.dataType = "adad";
        symbol.type = "var";
        symbol.address = address;
        symbol.initialValue = 0;
        symbol.context = context;
        bool found = false;
        for (auto it = symbolTable.begin(); it != symbolTable.end(); it++)
        {
            if (it->idname == symbol.idname)
            {
                found = true;
            }
        }
        if (!found)
        {
            address += 4;
            symbolTable.push_back(symbol);
        }

        Param();
    }
    else
    {
        ;
    }
    --tabs;
}
void parser::Statements()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Statements"<<endl; 
    if (_lexer.peek(1).tokenType == TokenType::rakho)
    {
        Statement();
        Statements();
    }
    else if (_lexer.peek(1).tokenType == TokenType::agar)
    {
        Statement();
        Statements();
    }
    else if (_lexer.peek(1).tokenType == TokenType::lo)
    {
        Statement();
        Statements();
    }
    else if (_lexer.peek(1).tokenType == TokenType::dekhao)
    {
        Statement();
        Statements();
    }
    else if (_lexer.peek(1).tokenType == TokenType::wapas)
    {
        Statement();
        Statements();
    }
    else if (_lexer.peek(1).tokenType == TokenType::jab)
    {
        Statement();
        Statements();
    }
    else
    {
        ;
    }
    --tabs;
}
void parser::Statement()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Statement"<<endl; 
    if (_lexer.peek(1).tokenType == TokenType::rakho)
    {
        Declare();
        ++tabs; printTabs(tabs); cout<<";"<<endl; --tabs; 
        expect(TokenType::specialsemi);
        checkNewLine();
        Comment();
    }
    else if (_lexer.peek(1).tokenType == TokenType::agar)
    {
        If();
        CommentSection();
    }
    else if (_lexer.peek(1).tokenType == TokenType::lo)
    {
        Input();
        ++tabs; printTabs(tabs); cout<<";"<<endl; --tabs; 
        expect(TokenType::specialsemi);
        checkNewLine();
        Comment();
    }
    else if (_lexer.peek(1).tokenType == TokenType::dekhao)
    {
        Output();
        ++tabs; printTabs(tabs); cout<<";"<<endl; --tabs; 
        expect(TokenType::specialsemi);
        checkNewLine();
        Comment();
    }
    else if (_lexer.peek(1).tokenType == TokenType::wapas)
    {
        Return();
        ++tabs; printTabs(tabs); cout<<";"<<endl; --tabs; 
        expect(TokenType::specialsemi);
        checkNewLine();
        Comment();
    }
    else if (_lexer.peek(1).tokenType == TokenType::jab)
    {
        Loop();
        CommentSection();
    }
    else{
        syntax_error();
    }
    --tabs;
}
void parser::Declare()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Declare"<<endl; 
    if (_lexer.peek(1).tokenType == TokenType::rakho)
    {
        constOnRHS = true;
        initialValue = 0;
        ++tabs; printTabs(tabs); cout<<"rakho"<<endl; --tabs; 
        expect(TokenType::rakho);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"ID"<<endl; --tabs; 
        token t = expect(TokenType::ID);
        IDname= t.lexeme; //global
        checkNewLine();

        DeclareType();

        SymbolTable symbol;
        symbol.idname = t.lexeme;
        symbol.dataType = "adad";
        symbol.type = "var";
        symbol.address = address;
        if(constOnRHS == true)
            symbol.initialValue = initialValue;
        else
            symbol.initialValue = 0;
        symbol.context = context;
        bool found = false;
        for (auto it = symbolTable.begin(); it != symbolTable.end(); it++)
        {
            if (it->idname == symbol.idname)
            {
                found = true;
            }
        }
        if (!found)
        {
            address += 4;
            symbolTable.push_back(symbol);
        }
    }
    else{
        syntax_error();
    }
    --tabs;
}
void parser::DeclareType()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"DeclareType"<<endl; 
    if (_lexer.peek(1).tokenType == TokenType::special1)
    {
        ++tabs; printTabs(tabs); cout<<"@"<<endl; --tabs; 
        expect(TokenType::special1);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"adad"<<endl; --tabs; 
        expect(TokenType::adad);
        checkNewLine();
        AssignToExpression();
    }
    else if(_lexer.peek(1).tokenType == TokenType::ASSIGNMENT)
    {
        ++tabs; printTabs(tabs); cout<<":="<<endl; --tabs; 
        expect(TokenType::ASSIGNMENT);
        checkNewLine();
        Assigned();
    }
    else{
        syntax_error();
    }
    --tabs;
}
void parser::AssignToExpression()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"AssignToExpression"<<endl; 
    if (_lexer.peek(1).tokenType == TokenType::ASSIGNMENT)
    {
        ++tabs; printTabs(tabs); cout<<":="<<endl; --tabs; 
        expect(TokenType::ASSIGNMENT);
        checkNewLine();
        Assigned();
    }
    else
    {
        ;
    }
    --tabs;
}
void parser::Assigned()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Assigned"<<endl;
    if (_lexer.peek(1).tokenType == TokenType::chalao)
    {
        FunctionCall();
    }
    else
    {
        assignment = true;
        Expression();
        printTAC(IDname);  //Translation Scheme
        printTAC(" = ");  //Translation Scheme
        printTAC(Z1);
        printTAC("\n");  //Translation Scheme
        assignment = false;
    }
    --tabs;
}
void parser::FunctionCall()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"FunctionCall"<<endl;
    if(_lexer.peek(1).tokenType == TokenType::chalao)
    {
        ++tabs; printTabs(tabs); cout<<"chalao"<<endl; --tabs; 
        expect(TokenType::chalao);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"ID"<<endl; --tabs; 
        token t = expect(TokenType::ID);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"("<<endl; --tabs; 
        expect(TokenType::specialopenbracket);
        checkNewLine();
        Argument();
        printTAC("call "); //Translation Scheme
        printTAC(t.lexeme); //Translation Scheme
        printTAC(" "); //Translation Scheme
        printTAC(to_string(noOfParams)); //Translation Scheme
        printTAC(", "); //Translation Scheme
        string temp = Temp();

        // Storing in symbol table
        SymbolTable symbol;
        symbol.idname = temp;
        symbol.dataType = "adad";
        symbol.type = "var";
        symbol.address = address;
        if(constOnRHS == true)
            symbol.initialValue = initialValue;
        else
            symbol.initialValue = 0;
        symbol.context = context;
        bool found = false;
        for (auto it = symbolTable.begin(); it != symbolTable.end(); it++)
        {
            if (it->idname == symbol.idname)
            {
                found = true;
            }
        }
        if (!found)
        {
            address += 4;
            symbolTable.push_back(symbol);
        }
        printTAC(temp); //Translation Scheme
        printTAC("\n"); //Translation Scheme
        printTAC(IDname); //Translation Scheme
        IDname="";
        printTAC(" = "); //Translation Scheme
        printTAC(temp); //Translation Scheme
        printTAC("\n"); //Translation Scheme
        noOfParams=0;
        ++tabs; printTabs(tabs); cout<<")"<<endl; --tabs; 
        expect(TokenType::specialclosebracket);
        checkNewLine();
    }
    else{
        syntax_error();
    }
    --tabs;
}
void parser::Argument()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Argument"<<endl; 
    if (_lexer.peek(1).tokenType == TokenType::ID)
    { 
        ZZ();
        K();
    }
    else if (_lexer.peek(1).tokenType == TokenType::NUM)
    { 
        ZZ();
        K();
    }
    else
    {
        ;
    }
    --tabs;
}
void parser::K()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"K"<<endl; 
    if (_lexer.peek(1).tokenType == TokenType::specialpipe)
    {
        ++tabs; printTabs(tabs); cout<<"|"<<endl; --tabs; 
        expect(TokenType::specialpipe);
        checkNewLine();
        ZZ();
        K();
    }
    else
    {
        ;
    }
    --tabs;
}
void parser::ZZ()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"ZZ"<<endl; 
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        ++tabs; printTabs(tabs); cout<<"ID"<<endl; --tabs; 
        token t = expect(TokenType::ID);
        checkNewLine();
        noOfParams++;
        printTAC("param "); //Translation Scheme
        printTAC(t.lexeme); //Translation Scheme
        printTAC("\n"); //Translation Scheme
    }
    else if (_lexer.peek(1).tokenType == TokenType::NUM)
    {
        ++tabs; printTabs(tabs); cout<<"NUM"<<endl; --tabs; 
        token y = expect(TokenType::NUM);
        checkNewLine();
        noOfParams++;
        printTAC("param "); //Translation Scheme
        printTAC(y.lexeme); //Translation Scheme
        printTAC("\n"); //Translation Scheme
    }
    else
    {
        syntax_error();
    }
    --tabs;
}
void parser::If()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"If"<<endl; 
    if(_lexer.peek(1).tokenType == TokenType::agar)
    {
        ifCondition = true;
        ++tabs; printTabs(tabs); cout<<"agar"<<endl; --tabs; 
        expect(TokenType::agar);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"("<<endl; --tabs; 
        expect(TokenType::specialopenbracket);
        checkNewLine();
        int if_t = addressLine;
        Expression();
        if_P1 = Z1;
        ++tabs; printTabs(tabs); cout<<"RO"<<endl; --tabs;
        string dum4 = _lexer.peek(1).lexeme;
        expect(TokenType::RO);
        checkNewLine();
        Expression();
        if_P2 = Z1;
        int ln1 = addressLine;
        printTAC("if ");
        printTAC(if_P1);
        printTAC(LexemeForRO(dum4));
        printTAC(if_P2);
        ++tabs; printTabs(tabs); cout<<")"<<endl; --tabs; 
        expect(TokenType::specialclosebracket);
        printTAC(" goto ");  //Translation Scheme
        printTAC("\n");  //Translation Scheme
        printTAC("goto ");  //Translation Scheme
        int if_f = addressLine;
        printTAC("\n");  //Translation Scheme
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"to"<<endl; --tabs; 
        expect(TokenType::to);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"phir"<<endl; --tabs; 
        expect(TokenType::phir);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"karo"<<endl; --tabs; 
        expect(TokenType::karo);
        checkNewLine();
        BackPatch(ln1, addressLine);
        Statements();
        printTAC("goto ");  //Translation Scheme
        int if_e = addressLine;
        printTAC("\n");  //Translation Scheme
        Elseif_i = if_f;
        ElseIf();
        ++tabs; printTabs(tabs); cout<<"bas"<<endl; --tabs; 
        expect(TokenType::bas);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"karo"<<endl; --tabs; 
        expect(TokenType::karo);
        BackPatch(if_e, addressLine);
        checkNewLine();
        ifCondition = false;
    }
    else{
        syntax_error();
    }
    --tabs;
}
void parser::ElseIf()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"ElseIf"<<endl; 
    if (_lexer.peek(1).tokenType == TokenType::warna)
    {
        ifCondition = true;
        ++tabs; printTabs(tabs); cout<<"warna"<<endl; --tabs; 
        expect(TokenType::warna);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"agar"<<endl; --tabs; 
        expect(TokenType::agar);
        BackPatch(Elseif_i, addressLine);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"("<<endl; --tabs; 
        expect(TokenType::specialopenbracket);
        checkNewLine();
        int Elseif_t = addressLine;
        Expression();
        if_P1 = Z1;
        ++tabs; printTabs(tabs); cout<<"RO"<<endl; --tabs; 
        string dum5 = _lexer.peek(1).lexeme;
        expect(TokenType::RO);
        checkNewLine();
        Expression();
        if_P2 = Z1;
        printTAC("if ");
        printTAC(if_P1);
        printTAC(LexemeForRO(dum5));
        printTAC(if_P2);
        ++tabs; printTabs(tabs); cout<<")"<<endl; --tabs; 
        expect(TokenType::specialclosebracket);
        printTAC(" goto ");
        int ln3 = addressLine;
        printTAC("\n");
        printTAC("goto ");
        int Elseif_f = addressLine;
        printTAC("\n");
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"to"<<endl; --tabs; 
        expect(TokenType::to);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"phir"<<endl; --tabs; 
        expect(TokenType::phir);
        checkNewLine();
        BackPatch(ln3, addressLine);
        Statements();
        Else_i = Elseif_f;
        printTAC("goto ");
        int Elseif_e = addressLine;
        printTAC("\n");
        Else();
        BackPatch(Elseif_e, addressLine);
        ifCondition = false;
    }
    else
    {
        Else();
        BackPatch(Elseif_i, addressLine);
    }
    --tabs;
}
void parser::Else()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Else"<<endl; 
    if (_lexer.peek(1).tokenType == TokenType::warna)
    {
        ++tabs; printTabs(tabs); cout<<"warna"<<endl; --tabs; 
        expect(TokenType::warna);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"phir"<<endl; --tabs; 
        expect(TokenType::phir);
        BackPatch(Else_i, addressLine);
        checkNewLine();
        Statements();
    }
    else
    {
        ;
        BackPatch(Else_i, addressLine);
    }
    --tabs;
}
void parser::Return()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Return"<<endl;
    if(_lexer.peek(1).tokenType == TokenType::wapas)
    {
        ++tabs; printTabs(tabs); cout<<"wapas"<<endl; --tabs; 
        expect(TokenType::wapas);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"bhaijo"<<endl; --tabs; 
        expect(TokenType::bhaijo);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"NUM"<<endl; --tabs; 
        token t = expect(TokenType::NUM);
        printTAC("ret "); //Translation Scheme
        printTAC(t.lexeme); //Translation Scheme
        printTAC("\n"); //Translation Scheme
        checkNewLine();
    }
    else{
        syntax_error();
    }
    --tabs;
}
void parser::Output()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Output"<<endl; 
    if(_lexer.peek(1).tokenType == TokenType::dekhao)
    {
        ++tabs; printTabs(tabs); cout<<"dekhao"<<endl; --tabs; 
        expect(TokenType::dekhao);
        printTAC("out "); //Translation Scheme
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"<<"<<endl; --tabs; 
        expect(TokenType::OUTPUT);
        checkNewLine();
        J();
        H();
    }
    else{
        syntax_error();
    }
    --tabs;
}
void parser::Input()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Input"<<endl;
    if(_lexer.peek(1).tokenType == TokenType::lo)
    {
        ++tabs; printTabs(tabs); cout<<"lo"<<endl; --tabs; 
        expect(TokenType::lo);
        checkNewLine();
        Inp();
    }
    else{
        syntax_error();
    }
    --tabs;
}
void parser::Inp()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Inp"<<endl; 
    if (_lexer.peek(1).tokenType == TokenType::INPUT)
    {
        ++tabs; printTabs(tabs); cout<<">>"<<endl; --tabs; 
        printTAC("in "); //Translation Scheme
        expect(TokenType::INPUT);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"ID"<<endl; --tabs; 
        token t = expect(TokenType::ID);
        printTAC(t.lexeme); //Translation Scheme
        printTAC("\n"); //Translation Scheme
        checkNewLine();
    }
    else if (_lexer.peek(1).tokenType == TokenType::OUTPUT)
    {
        ++tabs; printTabs(tabs); cout<<"<<"<<endl; --tabs; 
        expect(TokenType::OUTPUT);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"STR"<<endl; --tabs; 
        token t = expect(TokenType::STR);
        printTAC("out "); //Translation Scheme
        printTAC(t.lexeme); //Translation Scheme
        printTAC("\n"); //Translation Scheme
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<">>"<<endl; --tabs; 
        expect(TokenType::INPUT);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"ID"<<endl; --tabs; 
        token y = expect(TokenType::ID);
        printTAC("in "); //Translation Scheme
        printTAC(y.lexeme); //Translation Scheme
        printTAC("\n"); //Translation Scheme
        checkNewLine();
    }
    else{
        syntax_error();
    }
    --tabs;
}
void parser::Loop()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Loop"<<endl; 
    if(_lexer.peek(1).tokenType == TokenType::jab)
    {
        ifCondition = true;
        ++tabs; printTabs(tabs); cout<<"jab"<<endl; --tabs; 
        expect(TokenType::jab);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"tak"<<endl; --tabs; 
        expect(TokenType::tak);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"("<<endl; --tabs; 
        expect(TokenType::specialopenbracket);
        checkNewLine();
        int Loop_t = addressLine;
        Expression();
        if_P1 = Z1;
        ++tabs; printTabs(tabs); cout<<"RO"<<endl; --tabs; 
        string dum3 = _lexer.peek(1).lexeme;
        expect(TokenType::RO);
        checkNewLine();
        Expression();
        if_P2 = Z1;
        int ln = addressLine;
        printTAC("if ");
        printTAC(if_P1);
        printTAC(LexemeForRO(dum3));
        printTAC(if_P2);
        ++tabs; printTabs(tabs); cout<<")"<<endl; --tabs; 
        expect(TokenType::specialclosebracket);
        printTAC(" goto ");
        printTAC("\n");
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"karo"<<endl; --tabs; 
        expect(TokenType::karo);
        checkNewLine();
        Statements();
        printTAC("goto ");
        string dum = to_string(Loop_t);
        printTAC(dum);
        printTAC("\n");
        BackPatch(ln, addressLine);
        ++tabs; printTabs(tabs); cout<<"bas"<<endl; --tabs; 
        expect(TokenType::bas);
        checkNewLine();
        ++tabs; printTabs(tabs); cout<<"karo"<<endl; --tabs; 
        expect(TokenType::karo);
        checkNewLine();
        ifCondition = false;
    }
    else{
        syntax_error();
    }
    --tabs;
}
void parser::Expression()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Expression"<<endl; 
    E();
    R();
    if(Z1[0] != 't' && assignment == false && ifCondition == false){
        printTAC(Z1);
    }
    --tabs;
}
void parser::R()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"R"<<endl;
    if (_lexer.peek(1).tokenType == TokenType::plus)
    {
        ++tabs; printTabs(tabs); cout<<"+"<<endl; --tabs; 
        expect(TokenType::plus);
        checkNewLine();
        constOnRHS = false;
        string dum1 = E1;
        E();
        E2 = E1;
        E1 = dum1;
        temp = Temp();

        // Storing in symbol table
        SymbolTable symbol;
        symbol.idname = temp;
        symbol.dataType = "adad";
        symbol.type = "var";
        symbol.address = address;
        if(constOnRHS == true)
            symbol.initialValue = initialValue;
        else
            symbol.initialValue = 0;
        symbol.context = context;
        bool found = false;
        for (auto it = symbolTable.begin(); it != symbolTable.end(); it++)
        {
            if (it->idname == symbol.idname)
            {
                found = true;
            }
        }
        if (!found)
        {
            address += 4;
            symbolTable.push_back(symbol);
        }

        string s = temp + " = " + E1 + " + " + E2;
        printTAC(s);
        E1 = temp;
        printTAC("\n");
        R();
    }
    else if (_lexer.peek(1).tokenType == TokenType::minus)
    {
        ++tabs; printTabs(tabs); cout<<"-"<<endl; --tabs; 
        expect(TokenType::minus);
        checkNewLine();
        constOnRHS = false;
        string dum1 = E1;
        E();
        E2 = E1;
        E1 = dum1;
        temp = Temp();

        // Storing in symbol table
        SymbolTable symbol;
        symbol.idname = temp;
        symbol.dataType = "adad";
        symbol.type = "var";
        symbol.address = address;
        if(constOnRHS == true)
            symbol.initialValue = initialValue;
        else
            symbol.initialValue = 0;
        symbol.context = context;
        bool found = false;
        for (auto it = symbolTable.begin(); it != symbolTable.end(); it++)
        {
            if (it->idname == symbol.idname)
            {
                found = true;
            }
        }
        if (!found)
        {
            address += 4;
            symbolTable.push_back(symbol);
        }

        string s = temp + " = " + E1 + " - " + E2;
        printTAC(s);
        E1 = temp;
        printTAC("\n");
        R();
    }
    else
    {
        Z1 = E1;
    }
    --tabs;
}
void parser::E()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"E"<<endl; 
    Z(1);
    X();
    E1 = Z1;
    --tabs;
}
void parser::X()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"X"<<endl;  
    if (_lexer.peek(1).tokenType == TokenType::multiply)
    {
        ++tabs; printTabs(tabs); cout<<"*"<<endl; --tabs; 
        expect(TokenType::multiply);
        checkNewLine();
        constOnRHS = false;
        Z(0);
        temp = Temp();

        // Storing in symbol table
        SymbolTable symbol;
        symbol.idname = temp;
        symbol.dataType = "adad";
        symbol.type = "var";
        symbol.address = address;
        if(constOnRHS == true)
            symbol.initialValue = initialValue;
        else
            symbol.initialValue = 0;
        symbol.context = context;
        bool found = false;
        for (auto it = symbolTable.begin(); it != symbolTable.end(); it++)
        {
            if (it->idname == symbol.idname)
            {
                found = true;
            }
        }
        if (!found)
        {
            address += 4;
            symbolTable.push_back(symbol);
        }

        string s = temp + " = " + Z1 + " * " + Z2;
        printTAC(s);
        Z1 = temp;
        printTAC("\n");
        X();
    }
    else if (_lexer.peek(1).tokenType == TokenType::divide)
    {
        ++tabs; printTabs(tabs); cout<<"/"<<endl; --tabs; 
        expect(TokenType::divide);
        checkNewLine();
        constOnRHS = false;
        Z(0);
        temp = Temp();

        // Storing in symbol table
        SymbolTable symbol;
        symbol.idname = temp;
        symbol.dataType = "adad";
        symbol.type = "var";
        symbol.address = address;
        if(constOnRHS == true)
            symbol.initialValue = initialValue;
        else
            symbol.initialValue = 0;
        symbol.context = context;
        bool found = false;
        for (auto it = symbolTable.begin(); it != symbolTable.end(); it++)
        {
            if (it->idname == symbol.idname)
            {
                found = true;
            }
        }
        if (!found)
        {
            address += 4;
            symbolTable.push_back(symbol);
        }

        string s = temp + " = " + Z1 + " / " + Z2;
        printTAC(s);
        Z1 = temp;
        printTAC("\n");
        X();
    }
    else if (_lexer.peek(1).tokenType == TokenType::modulus)
    {
        ++tabs; printTabs(tabs); cout<<"%"<<endl; --tabs; 
        expect(TokenType::modulus);
        checkNewLine();
        constOnRHS = false;
        Z(0);
        temp = Temp();

        // Storing in symbol table
        SymbolTable symbol;
        symbol.idname = temp;
        symbol.dataType = "adad";
        symbol.type = "var";
        symbol.address = address;
        if(constOnRHS == true)
            symbol.initialValue = initialValue;
        else
            symbol.initialValue = 0;
        symbol.context = context;
        bool found = false;
        for (auto it = symbolTable.begin(); it != symbolTable.end(); it++)
        {
            if (it->idname == symbol.idname)
            {
                found = true;
            }
        }
        if (!found)
        {
            address += 4;
            symbolTable.push_back(symbol);
        }

        string s = temp + " = " + Z1 + " % " + Z2;
        printTAC(s);
        Z1 = temp;
        printTAC("\n");
        X();
    }
    else
    {
    }
    --tabs;
}
void parser::Z(bool val)
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Z"<<endl; 
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        ++tabs; printTabs(tabs); cout<<"ID"<<endl; --tabs; 
        token t = expect(TokenType::ID);
        checkNewLine();
        if(val == 1)
          Z1 = t.lexeme[1];
        else
            Z2 = t.lexeme[1];
    }
    else if (_lexer.peek(1).tokenType == TokenType::NUM)
    {
        ++tabs; printTabs(tabs); cout<<"NUM"<<endl; --tabs; 
        token y = expect(TokenType::NUM);
        checkNewLine();
        initialValue = stoi(y.lexeme);
        if(val == 1)
          Z1 = y.lexeme;
        else
            Z2 = y.lexeme;
    }
    else
    {
        syntax_error();
    }
    --tabs;
}
void parser::J()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"J"<<endl;  
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        ++tabs; printTabs(tabs); cout<<"ID"<<endl; --tabs; 
        token t = expect(TokenType::ID);
        printTAC(t.lexeme); //Translation Scheme
        printTAC("\n"); //Translation Scheme
        checkNewLine();
    }
    else if (_lexer.peek(1).tokenType == TokenType::STR)
    {
        ++tabs; printTabs(tabs); cout<<"STR"<<endl; --tabs; 
        token y = expect(TokenType::STR);
        printTAC(y.lexeme); //Translation Scheme
        printTAC("\n"); //Translation Scheme
        checkNewLine();
    }
    else{
        syntax_error();
    }
    --tabs;
}
void parser::H()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"H"<<endl; 
    if (_lexer.peek(1).tokenType == TokenType::OUTPUT)
    {
        printTAC("out "); //Translation Scheme
        ++tabs; printTabs(tabs); cout<<"<<"<<endl; --tabs; 
        expect(TokenType::OUTPUT);
        checkNewLine();
        J();
        H();
    }
    else
    {
        ;
    }
    --tabs;
}
token parser::Funcname()
{
    checkNewLine();
    ++tabs; printTabs(tabs); cout<<"Funcname"<<endl; 
    token t;
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        ++tabs; printTabs(tabs); cout<<"ID"<<endl; --tabs; 
        t = expect(TokenType::ID);
        checkNewLine();
    }
    else if (_lexer.peek(1).tokenType == TokenType::markazi)
    {
        ++tabs; printTabs(tabs); cout<<"markazi"<<endl; --tabs; 
        t = expect(TokenType::markazi);
        parser::starting_line_number = addressLine + 1;
        t.lexeme = "\"markazi\"";
        checkNewLine();
    }
    else{
        syntax_error();
    }
    --tabs;
    return t;
}
// this function is for sample purposes only
/*
bool parser::statements()
{
    //statements-- > COLON LPAREN start RPAREN
    if (_lexer.peek(1).tokenType == TokenType::COLON)
    {
        expect(TokenType::COLON);
        if (_lexer.peek(1).tokenType == TokenType::LPAREN)
        {
            expect(TokenType::LPAREN);
            start();
            if (_lexer.peek(1).tokenType == TokenType::RPAREN)
            {
                expect(TokenType::RPAREN);
                return true;
            }
        }
    }
    return false;
}*/