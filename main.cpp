#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "parser.h"
#include "VM.h"
using namespace std;

void read_symbol_table(string symbol_table[100][6]);
int ds[200]={};
ifstream symbol_file("symbol_table.txt");

int main(int argc, char* argv[])
{
	//asking for file name as command line arguments
	if (argc == 2)
	{
		lexer _lexer(argv[1]);
		token t;
		t = _lexer.getNextToken();
		while (t.tokenType != TokenType::END_OF_FILE)
		{
			t.Print();
			t = _lexer.getNextToken();
		}
		parser p(argv[1]);
		ofstream f;
		f.open("TAC.txt");
		f << "";
		p.Start();
		p.printSymbolTable();
		string symbol_table[100][6];
		read_symbol_table(symbol_table);
		// for (int i = 0; i < 100; ++i) {
		// 	if (!symbol_table[i][0].empty()) {
		// 		std::cout << symbol_table[i][0] << " - " << symbol_table[i][1] << " - " << symbol_table[i][2] << " - " << symbol_table[i][3] << " - " << symbol_table[i][4] << " - " << symbol_table[i][5] << "\n";
		// 	}
    	// }
		VMTokens VM;
		string quad[100][4];
		VM.TAC_to_quad(quad, symbol_table);
		
		//print quad
		// for (int i = 0; i < 100; ++i) {
		// 	if (!quad[i][0].empty()) {
		// 		cout << i << "  " << "(" << quad[i][0] << ", " << quad[i][1] << ", " << quad[i][2] << ", " << quad[i][3] << ")\n";
		// 	}
    	// }
		
		VM.execute_TAC(quad, ds, parser::starting_line_number);
	}
	else if (argc > 2)
	{ //argument limit exceeds
		cout << "Too many arguments" << endl;
	}
	else //if file name is'nt given
	{
		cout << "Please provide a file name" << endl;
	}
	return 0;
}

void read_symbol_table(string symbol_table[100][6]) {

	if(symbol_file.is_open())
	{
		
		string id, type, dataType, initialValue, address, context, line;
		for(int table_line=0; getline(symbol_file, line); table_line+=1) {
			stringstream fin(line);
			char dash= '-';

			getline(fin, id, dash); getline(fin, type, dash); getline(fin, dataType, dash); getline(fin, initialValue, dash); getline(fin, address, dash); getline(fin, context);

			auto frwd = [](unsigned char c) 
			{ 
				return !isspace(c); 
			};
			id.erase(id.begin(), find_if(id.begin(), id.end(), frwd));
			auto rvrs = [](unsigned char c) 
			{ 
				return !isspace(c); 
			};
			id.erase(find_if(id.rbegin(), id.rend(), rvrs).base(), id.end());

			symbol_table[table_line][0] = id;
			symbol_table[table_line][1] = type;
			symbol_table[table_line][2] = dataType;
			symbol_table[table_line][3] = initialValue;
			symbol_table[table_line][4] = address;
			symbol_table[table_line][5] = context;
		}
		symbol_file.close();
	}
	else{
		cout<<"File opening error"<<endl;
	}
}