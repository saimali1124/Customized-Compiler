#include "lexer.h"
#include <cctype>
using namespace std;
//for printing tokens names, Same as the enum defined in lexer.h
string reserved[] = {
    "END_OF_FILE",
    "ID",
    "NUM",
    "RO",
    "INPUT",
    "OUTPUT",
    "STR",
    "CMT",
    "ASSIGNMENT",
    "^",
    "markazi",
    "kaam",
    "karo",
    "rakho",
    "jab",
    "tak",
    "bas",
    "agar",
    "to",
    "warna",
    "phir",
    "dekhao",
    "lo",
    "chalao",
    "wapas",
    "bhaijo",
    "adad",
    "khali",
    "khatam",
    "'@'",
    "':'",
    "';'",
    "'|'",
    "'`'",
    "'('",
    "')'",
    "'+'",
    "'-'",
    "'*'",
    "'/'",
    "'%'",
    "'\\n'"
};

string keywords[] = {
    "\"markazi",
    "\"kaam",
    "\"karo",
    "\"rakho",
    "\"jab",
    "\"tak",
    "\"bas",
    "\"agar",
    "\"to",
    "\"warna",
    "\"phir",
    "\"dekhao",
    "\"lo",
    "\"chalao",
    "\"wapas",
    "\"bhaijo",
    "\"adad",
    "\"khali",
    "\"khatam"
};

token::token()
{
    this->lexeme = "";
    this->tokenType = TokenType::ERROR;
}
token::token(string lexeme, TokenType tokenType)
{
    this->lexeme = lexeme;
    this->tokenType = tokenType;
}
void token::Print()
{
    cout << "{" << lexeme << " , "
        << reserved[(int)tokenType] << "}\n";
}
/////////////////////////////////////////////////////////////////////////////
int lexer::getCurrentPointer()
{
    return index;
}
void lexer::setCurrentPointer(int pos)
{
    if (pos >= 0 && pos < tokens.size())
        index = pos;
    else
        index = 0;
}
void lexer::Tokenize()//function that tokenizes your input stream
{
    vector<char>::iterator it = stream.begin();
	//your implementation goes here
    
    int state = 0;
    string lexeme = "";
    TokenType tok;


    for (it = stream.begin(); it != stream.end(); it++)
    {
        switch (state) //states of DFA
        {
        case 0:
             //store the cahracters in a string to later put it in a token.
            if(isalpha(*it) || *it == '_'){
                lexeme += "\"";
                lexeme += *it;
                state = 4;
            }
            else if(isdigit(*it)){ 
                lexeme += *it;
                state = 5;
            }
            else if(*it == '>'){
                state = 7;
            }
            else if(*it == '<'){
                state = 8;
            }
            else if(*it == '`'){
                lexeme += *it;
                state = 6;
            }
            else if(*it == ';'){
                lexeme = '^';
                tok = TokenType::specialsemi;
                tokens.push_back(token(string(lexeme), tok));
                lexeme = "";
                *it++;
                if(*it != '\n'){
                    *it--;
                    state = 3;
                }
                if(*it == '\n'){
                    lexeme ="^";
                    tok = TokenType::END_OF_LINE;
                    tokens.push_back(token(string(lexeme), tok));
                    lexeme ="";
                }
            }
            else if(*it == '='){
                state = 2;
                lexeme += "EQ";
                tok = TokenType::RO;
            }
            else if(*it == ':'){
                state = 9;
            }
            else if(*it == '+')
            {
                lexeme ="^";
                tok = TokenType::plus;
                tokens.push_back(token(string(lexeme), tok));
                lexeme ="";
            }
            else if(*it == '-')
            {
                lexeme ="^";
                tok = TokenType::minus;
                tokens.push_back(token(string(lexeme), tok));
                lexeme ="";
            }
            else if(*it == '*')
            {
                lexeme ="^";
                tok = TokenType::multiply;
                tokens.push_back(token(string(lexeme), tok));
                lexeme ="";
            }
            else if(*it == '/')
            {
                lexeme ="^";
                tok = TokenType::divide;
                tokens.push_back(token(string(lexeme), tok));
                lexeme ="";
            }
            else if(*it == '%')
            {
                lexeme ="^";
                tok = TokenType::modulus;
                tokens.push_back(token(string(lexeme), tok));
                lexeme ="";
            }
            else if(*it == '|')
            {
                lexeme ="^";
                tok = TokenType::specialpipe;
                tokens.push_back(token(string(lexeme), tok));
                lexeme ="";
            }
            else if(*it == '@')
            {
                lexeme ="^";
                tok = TokenType::special1;
                tokens.push_back(token(string(lexeme), tok));
                lexeme ="";
            }
            else if(*it == '(')
            {
                lexeme ="^";
                tok = TokenType::specialopenbracket;
                tokens.push_back(token(string(lexeme), tok));
                lexeme ="";
            }
            else if(*it == ')')
            {
                lexeme ="^";
                tok = TokenType::specialclosebracket;
                tokens.push_back(token(string(lexeme), tok));
                lexeme ="";
            }
            else if(*it == '\n')
            {
                lexeme ="^";
                tok = TokenType::END_OF_LINE;
                tokens.push_back(token(string(lexeme), tok));
                lexeme ="";
            }
            break;

        case 1: //Final state with 1 extra other character
            tokens.push_back(token(string(lexeme),tok));
            lexeme = "";
            state = 0;
            *it--;
            *it--;
            break;

        case 2: //Final state
            tokens.push_back(token(string(lexeme),tok));
            lexeme = "";
            state = 0;
            *it--;
            break;

        case 3: //Comment state
            if(*it == '\n'){
                if(lexeme!="")
                {
                    state = 1;
                    tok = TokenType::CMT;
                }
            }
            else{
                lexeme += *it;
            }
            break;

        case 4: //Identifier and Keywords state

            
            if(isalpha(*it) || *it == '_' || isdigit(*it)){
                lexeme += *it;
            }
            else{
                
                for(int i=0; i< 19 ; i++)
                {
                    int x = lexeme.compare(keywords[i]);
                    if(x==0)
                    {
                        if(lexeme.compare("\"markazi")==0)
                        {
                            tok= TokenType::markazi;
                        }
                        else if(lexeme.compare("\"kaam")==0)
                        {
                            tok= TokenType::kaam;
                        }
                        else if(lexeme.compare("\"karo")==0)
                        {
                            tok= TokenType::karo;
                        }
                        else if(lexeme.compare("\"rakho")==0)
                        {
                            tok= TokenType::rakho;
                        }
                        else if(lexeme.compare("\"jab")==0)
                        {
                            tok= TokenType::jab;
                        }
                        else if(lexeme.compare("\"tak")==0)
                        {
                            tok= TokenType::tak;
                        }
                        else if(lexeme.compare("\"bas")==0)
                        {
                            tok= TokenType::bas;
                        }
                        else if(lexeme.compare("\"agar")==0)
                        {
                            tok= TokenType::agar;
                        }
                        else if(lexeme.compare("\"to")==0)
                        {
                            tok= TokenType::to;
                        }
                        else if(lexeme.compare("\"warna")==0)
                        {
                            tok= TokenType::warna;
                        }
                        else if(lexeme.compare("\"phir")==0)
                        {
                            tok= TokenType::phir;
                        }
                        else if(lexeme.compare("\"dekhao")==0)
                        {
                            tok= TokenType::dekhao;
                        }
                        else if(lexeme.compare("\"lo")==0)
                        {
                            tok= TokenType::lo;
                        }
                        else if(lexeme.compare("\"chalao")==0)
                        {
                            tok= TokenType::chalao;
                        }
                        else if(lexeme.compare("\"wapas")==0)
                        {
                            tok= TokenType::wapas;
                        }
                        else if(lexeme.compare("\"bhaijo")==0)
                        {
                            tok= TokenType:: bhaijo;
                        }
                        else if(lexeme.compare("\"adad")==0)
                        {
                            tok= TokenType::adad;
                        }
                        else if(lexeme.compare("\"khali")==0)
                        {
                            tok= TokenType::khali;
                        }
                        else if(lexeme.compare("\"khatam")==0)
                        {
                            tok= TokenType::khatam;
                        }
                        lexeme = "^";
                        state = 2;
                    }
                }
                if(state != 2) //if not a keyword, then its an identifier
                {
                    lexeme += "\"";
                    state = 1;
                    tok = TokenType::ID;
                }
                else{
                    state = 1;
                }
            }
            break;
        
        case 5: //Number state
            if(isdigit(*it)){
                cout<<state;
                lexeme += *it;
            }
            else{
                state = 1;
                tok = TokenType::NUM;
            }
            break;

        case 6: //String state
            if(*it == '`'){
                lexeme += *it;
                state = 2;
                tok = TokenType::STR;
            }
            else{
                lexeme += *it;
            }
            break;

        case 7: //Input and RO state
            if(*it == '='){
                state = 2;
                lexeme = "GE";
                tok = TokenType::RO;
            }
            else if(*it == '>'){
                state = 2;
                lexeme = "^";
                tok = TokenType::INPUT;
            }
            else {
                state = 1;
                lexeme = "GT";
                tok = TokenType::RO;
            }
            break;

        case 8: //Output and RO state
            if(*it == '='){
                state = 2;
                lexeme = "LE";
                tok = TokenType::RO;
            }
            else if(*it == '>'){
                state = 2;
                lexeme = "NE";
                tok = TokenType::RO;
            }
            else if(*it == '<'){
                state = 2;
                lexeme = "^";
                tok = TokenType::OUTPUT;
            }
            else {
                state = 1;
                lexeme = "LT";
                tok = TokenType::RO;
            }
            break;

        case 9: //Assignment state
            if(*it == '='){
                lexeme += "^";
                tok = TokenType::ASSIGNMENT;
                state = 2;
            }
            else{
                tok = TokenType::specialcolon;
                state = 1;
            }
            break;


        default:
            break;
        }
    }
	//push_back EOF token at end to identify End of File
    tokens.push_back(token(string(""), TokenType::END_OF_FILE));
}
lexer::lexer(const char filename[])
{
    //constructors: takes file name as an argument and store all
    //the contents of file into the class varible stream
    ifstream fin(filename);
    if (!fin) //if file not found
    {
        cout << "file not found" << endl;
    }
    else
    {
        char byte = 0;
        while (fin.get(byte))
        { //store file contents into stream
            if (byte != '\r')
                stream.push_back(byte);
        }
        stream.push_back(' ');//dummy spaces appended at the end
        stream.push_back(' ');
        //Tokenize function to get all the (token,lexeme) pairs
        Tokenize();
        //assigning tokens to iterator::pointer
        index = 0;
    }
}
lexer::lexer()
{
    index = -1;
}
void lexer::printRaw()
{
    //helper function to print the content of file
    vector<char>::iterator it = stream.begin();
    for (it = stream.begin(); it != stream.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
}
token lexer::getNextToken()
{
    //this function will return single (token,lexeme) pair on each call
    //this is an iterator which is pointing to the beginning of vector of tokens
    token _token;
    if (index == tokens.size())
    {                       // return end of file if
        _token.lexeme = ""; // index is too large
        _token.tokenType = TokenType::END_OF_FILE;
    }
    else
    {
        _token = tokens[index];
        index = index + 1;
    }
    return _token;
}
void lexer::resetPointer()
{
    index = 0;
}
token lexer::peek(int howFar)
{
    if (howFar <= 0)
    { // peeking backward or in place is not allowed
        cout << "LexicalAnalyzer:peek:Error: non positive argument\n";
        exit(-1);
    }

    int peekIndex = index + howFar - 1;
    if (peekIndex > (tokens.size() - 1))
    {                                                 // if peeking too far
        return token("", TokenType::END_OF_FILE); // return END_OF_FILE
    }
    else
        return tokens[peekIndex];
}