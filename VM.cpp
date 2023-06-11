#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include "VM.h"

using namespace std;

ifstream file("TAC.txt");

VMTokens::VMTokens()
{
    ;
}

void VMTokens::TAC_to_quad(string quad[100][4], string symbol_table[100][6]) {

    if(file.is_open())
    {
        for (string line; getline(file, line); ) 
        {
            stringstream fin2(line);
            string oper ="", arg1="", arg2="", result="", ignor="";

            int line_no;
            fin2 >> line_no; 
            getline(fin2, line);

            stringstream fin(line);

            if (line.find("if") != string::npos) {
                fin >> ignor >> arg1 >> oper >> arg2 >> ignor >> result;
                for(int j=0 ; j<100; j++)
                {
                    if(symbol_table[j][0] == arg1 || symbol_table[j][0] == ("\"" + arg1 + "\""))
                    {
                        arg1= "&" + symbol_table[j][4];
                    }
                }
                for(int j=0 ; j<100; j++)
                {
                    if(symbol_table[j][0] == arg2 || symbol_table[j][0] == ("\"" + arg2 + "\""))
                    {
                        arg2= "&" + symbol_table[j][4];
                    }
                }
                if(oper==">")
                {
                    oper= "GT";
                }
                else if(oper=="<")
                {
                    oper= "LT";
                }
                else if(oper==">=")
                {
                    oper= "GE";
                }
                else if(oper=="<=")
                {
                    oper= "LE";
                }
                else if(oper=="==")
                {
                    oper= "EQ";
                }
            } else if (line.find("goto") != string::npos) {

                fin >> oper >> result;
                arg1 = " ";
                arg2 = " "; 
            } else if (line.find("in") != string::npos) {
                fin >> oper >> result;
                arg1 = " ";
                arg2 = " "; 
                for(int j=0 ; j<100; j++)
                {
                    if(symbol_table[j][0] == result || symbol_table[j][0] == ("\"" + result+ "\""))
                    {
                        result= "&" + symbol_table[j][4];
                    }
                }
            }  else if (line.find("out") != string::npos) {
                string full;
                fin >> oper >> result;
                arg1 = " ";
                arg2 = " "; 
                while(fin >> full) {
                    result += " " + full;
                }
                for(int j=0 ; j<100; j++)
                {
                    if(symbol_table[j][0] == result || symbol_table[j][0] == ("\"" + result+ "\""))
                    {
                        result= "&" + symbol_table[j][4];
                    }
                }
            } else if(line.find("=") != string::npos) {
                fin >> result >> oper >> arg1;
                if (oper == "=") {
                    if (line.find("+") != string::npos || line.find("-") != string::npos || line.find("*") != string::npos || line.find("/") != string::npos) {
                    
                        fin >> oper; 
                        fin >> arg2;
                    
                    } else {
                        arg2 = " "; 
                    }
                    for(int j=0 ; j<100; j++)
                    {
                        if(symbol_table[j][0] == arg1 || symbol_table[j][0] == ("\"" + arg1+ "\""))
                        {
                            arg1= "&" + symbol_table[j][4];
                        }
                    }
                    for(int j=0 ; j<100; j++)
                    {
                        if(symbol_table[j][0] == arg2 || symbol_table[j][0] == ("\"" + arg2+ "\""))
                        {
                            arg2= "&" + symbol_table[j][4];
                        }
                    }
                    for(int j=0 ; j<100; j++)
                    {
                        if(symbol_table[j][0] == result || symbol_table[j][0] == ("\"" + result + "\""))
                        {
                            result = "&" + symbol_table[j][4];
                        }
                    }
                }
            } else {
                fin >> oper >> arg1 >> arg2 >> result;
                arg2 = arg2.substr(0, arg2.length() - 1);

                for (int j = 0; j < 100; j++)
                {
                    if (symbol_table[j][0] == result || symbol_table[j][0] == ("\"" + result + "\""))
                    {
                        result = "&" + symbol_table[j][4];
                    }
                }
            }
            quad[line_no - 1][0] = oper;
            quad[line_no - 1][1] = arg1;
            quad[line_no - 1][2] = arg2;
            quad[line_no - 1][3] = result;
        }
        file.close();
    }
    else
    {
        cout<<"File opening error"<<endl;
    }
    
}

void VMTokens::execute_TAC(string quad[100][4], int ds[200], int startingLine) {
    for (int pc = startingLine-1; pc < 100; pc+=1) {
        //cout<<"TAC Line being executed: "<<pc+1<<endl;
        int i, j, k;
        bool i_const = false, j_const = false;
        if(quad[pc][0] == "")
        {
            break;
        }
        else if (quad[pc][0] == "+") {
            if (!quad[pc][1].empty() && quad[pc][1].find("&") != string::npos) {
                i = stoi(quad[pc][1].substr(1)); 
            } else if (!quad[pc][1].empty()) {
                i = stoi(quad[pc][1]);
                i_const = true;
            }

            if (!quad[pc][2].empty() && quad[pc][2].find("&") != string::npos) {
                j = stoi(quad[pc][2].substr(1)); 
            } else if (!quad[pc][2].empty()) {
                j = stoi(quad[pc][2]);
                j_const = true;
            }

            if (!quad[pc][3].empty() && quad[pc][3].find("&") != string::npos) {
                k = stoi(quad[pc][3].substr(1)); 
            } else if (!quad[pc][3].empty()) {
                k = stoi(quad[pc][3]);
            }

            if (i_const && j_const){
                ds[k] = i + j;
            } else if (i_const && !j_const) {
                ds[k] = i + ds[j];
            } else if (!i_const && j_const) {
                ds[k] = ds[i] + j;
            } else {
                ds[k] = ds[i] + ds[j];
            }

            i_const = false, j_const = false;
        }else if (quad[pc][0] == "-") {
            if (!quad[pc][1].empty() && quad[pc][1].find("&") != string::npos) {
                i = stoi(quad[pc][1].substr(1)); 
            } else if (!quad[pc][1].empty()) {
                i = stoi(quad[pc][1]);
                i_const = true;
            }

            if (!quad[pc][2].empty() && quad[pc][2].find("&") != string::npos) {
                j = stoi(quad[pc][2].substr(1)); 
            } else if (!quad[pc][2].empty()) {
                j = stoi(quad[pc][2]);
                j_const = true;
            }

            if (!quad[pc][3].empty() && quad[pc][3].find("&") != string::npos) {
                k = stoi(quad[pc][3].substr(1)); 
            } else if (!quad[pc][3].empty()) {
                k = stoi(quad[pc][3]);
            } 

            if (i_const && j_const){
                ds[k] = i - j;
            } else if (i_const && !j_const) {
                ds[k] = i - ds[j];
            } else if (!i_const && j_const) {
                ds[k] = ds[i] - j;
            } else {
                ds[k] = ds[i] - ds[j];
            }

            i_const = false, j_const = false;
        } 
        else if (quad[pc][0] == "/") {
            if (!quad[pc][1].empty() && quad[pc][1].find("&") != string::npos) {
                i = stoi(quad[pc][1].substr(1)); 
            } else if (!quad[pc][1].empty()) {
                i = stoi(quad[pc][1]);
                i_const = true;
            }

            if (!quad[pc][2].empty() && quad[pc][2].find("&") != string::npos) {
                j = stoi(quad[pc][2].substr(1)); 
            } else if (!quad[pc][2].empty()) {
                j = stoi(quad[pc][2]);
                j_const = true;
            }

            if (!quad[pc][3].empty() && quad[pc][3].find("&") != string::npos) {
                k = stoi(quad[pc][3].substr(1)); 
            } else if (!quad[pc][3].empty()) {
                k = stoi(quad[pc][3]);
            }
            
            if (i_const && j_const){
                ds[k] = i / j;
            } else if (i_const && !j_const) {
                ds[k] = i / ds[j];
            } else if (!i_const && j_const) {
                ds[k] = ds[i] / j;
            } else {
                ds[k] = ds[i] / ds[j];
            }

            i_const = false, j_const = false;
        }
        if (quad[pc][0] == "*") {
            if (!quad[pc][1].empty() && quad[pc][1].find("&") != string::npos) {
                i = stoi(quad[pc][1].substr(1)); 
            } else if (!quad[pc][1].empty()) {
                i = stoi(quad[pc][1]);
                i_const = true;
            }

            if (!quad[pc][2].empty() && quad[pc][2].find("&") != string::npos) {
                j = stoi(quad[pc][2].substr(1)); 
            } else if (!quad[pc][2].empty()) {
                j = stoi(quad[pc][2]);
                j_const = true;
            }

            if (!quad[pc][3].empty() && quad[pc][3].find("&") != string::npos) {
                k = stoi(quad[pc][3].substr(1)); 
            } else if (!quad[pc][3].empty()) {
                k = stoi(quad[pc][3]);
            }
            
            if (i_const && j_const){
                ds[k] = i * j;
            } else if (i_const && !j_const) {
                ds[k] = i * ds[j];
            } else if (!i_const && j_const) {
                ds[k] = ds[i] * j;
            } else {
                ds[k] = ds[i] * ds[j];
            }

            i_const = false, j_const = false;
        }   
        else if (quad[pc][0] == "goto") {
            k = stoi(quad[pc][3]); 
            pc = k - 2;
        } 
        else if (quad[pc][0] == "GE") {
            if (!quad[pc][1].empty() && quad[pc][1].find("&") != string::npos) {
                i = stoi(quad[pc][1].substr(1)); 
            } else if (!quad[pc][1].empty()) {
                i = stoi(quad[pc][1]);
            }

            if (!quad[pc][2].empty() && quad[pc][2].find("&") != string::npos) {
                j = stoi(quad[pc][2].substr(1)); 
            } else if (!quad[pc][2].empty()) {
                j = stoi(quad[pc][2]);
            }
            k = stoi(quad[pc][3]); 
            if (ds[i] >= ds[j])
            {
                pc = k - 2;
            } 
        } 
        else if (quad[pc][0] == "in") {
            if (!quad[pc][3].empty() && quad[pc][3].find("&") != string::npos) {
                k = stoi(quad[pc][3].substr(1)); 
            } else if (!quad[pc][3].empty()) {
                k = stoi(quad[pc][3]);
            }
            cin >> ds[k];
        } 
        else if (quad[pc][0] == "out") {
            if (!quad[pc][3].empty() && quad[pc][3].find("&") != string::npos) {
                k = stoi(quad[pc][3].substr(1)); 
                cout << ds[k] << endl;
            } else if (!quad[pc][3].empty()) {
                cout<<quad[pc][3]<<endl;
            }
            
        } 
        else if (quad[pc][0] == "=") {
            bool num=false;
            if (!quad[pc][1].empty() && quad[pc][1].find("&") != string::npos) {
                quad[pc][1].erase(remove_if(quad[pc][1].begin(), quad[pc][1].end(), ::isspace), quad[pc][1].end());
                i = stoi(quad[pc][1].substr(1)); 
            } else if (!quad[pc][1].empty()) {
                quad[pc][1].erase(remove_if(quad[pc][1].begin(), quad[pc][1].end(), ::isspace), quad[pc][1].end());
                i = stoi(quad[pc][1]);
                num=true;
            }

            if (!quad[pc][3].empty() && quad[pc][3].find("&") != string::npos) {
                quad[pc][3].erase(remove_if(quad[pc][3].begin(), quad[pc][3].end(), ::isspace), quad[pc][3].end());
                k = stoi(quad[pc][3].substr(1)); 
            } else if (!quad[pc][3].empty()) {
                quad[pc][3].erase(remove_if(quad[pc][3].begin(), quad[pc][3].end(), ::isspace), quad[pc][3].end());
                k = stoi(quad[pc][3]);
            }
            if(!num){
                ds[k] = ds[i];
            }
            else if(num==true)
            {
                ds[k]=i;
            }
             
        }
        else if (quad[pc][0] == "GT") {
            if (!quad[pc][1].empty() && quad[pc][1].find("&") != string::npos) {
                i = stoi(quad[pc][1].substr(1)); 
            } else if (!quad[pc][1].empty()) {
                i = stoi(quad[pc][1]);
            }

            if (!quad[pc][2].empty() && quad[pc][2].find("&") != string::npos) {
                j = stoi(quad[pc][2].substr(1)); 
            } else if (!quad[pc][2].empty()) {
                j = stoi(quad[pc][2]);
            }
            k = stoi(quad[pc][3]); 
            if (ds[i] > ds[j])
            {
                pc = k - 2;
            }  
        }
        else if (quad[pc][0] == "LE") {
            if (!quad[pc][1].empty() && quad[pc][1].find("&") != string::npos) {
                i = stoi(quad[pc][1].substr(1)); 
            } else if (!quad[pc][1].empty()) {
                i = stoi(quad[pc][1]);
            }

            if (!quad[pc][2].empty() && quad[pc][2].find("&") != string::npos) {
                j = stoi(quad[pc][2].substr(1)); 
            } else if (!quad[pc][2].empty()) {
                j = stoi(quad[pc][2]);
            }
            k = stoi(quad[pc][3]); 
            if (ds[i] <= ds[j])
            {
                pc = k - 2;
            } 
        }
        else if (quad[pc][0] == "LT") {
            if (!quad[pc][1].empty() && quad[pc][1].find("&") != string::npos) {
                i = stoi(quad[pc][1].substr(1)); 
            } else if (!quad[pc][1].empty()) {
                i = stoi(quad[pc][1]);
            }

            if (!quad[pc][2].empty() && quad[pc][2].find("&") != string::npos) {
                j = stoi(quad[pc][2].substr(1)); 
            } else if (!quad[pc][2].empty()) {
                j = stoi(quad[pc][2]);
            }
            k = stoi(quad[pc][3]);  
            if (ds[i] < ds[j])
            {
                pc = k - 2;
            } 
        }
        else if (quad[pc][0] == "EQ") {
            if (!quad[pc][1].empty() && quad[pc][1].find("&") != string::npos) {
                i = stoi(quad[pc][1].substr(1)); 
            } else if (!quad[pc][1].empty()) {
                i = stoi(quad[pc][1]);
            }

            if (!quad[pc][2].empty() && quad[pc][2].find("&") != string::npos) {
                j = stoi(quad[pc][2].substr(1)); 
            } else if (!quad[pc][2].empty()) {
                j = stoi(quad[pc][2]);
            }
            k = stoi(quad[pc][3]); 
            if (ds[i] == ds[j])
            {
                pc = k - 2;
            } 
        }
    }
}
