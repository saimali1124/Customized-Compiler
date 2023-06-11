#include <fstream>
#include <iostream>
#include <vector>
#define VM_H
using namespace std;
class VMTokens{
    private:
    
    public:
    VMTokens();
    void TAC_to_quad(string quad[100][4], string symbol_table[100][6]);
    void execute_TAC(string quad[100][4], int ds[100], int startingLine);
};