#include "kind.h"
#include "lexer.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>
// Use only the neeeded aspects of each namespace
using std::string;
using std::vector;
using std::endl;
using std::cerr;
using std::cin;
using std::cout;
using std::getline;
using ASM::Token;
using ASM::Lexer;
using std::map;

const string invalid = "ERROR, instruction not supported";
const string invalword = "ERROR, invalid .word instruction";
const string labelexists = "ERROR, label already exists";
const string labelnotfound = "ERROR, label not found";
const string invaljr = "ERROR, invalid jr instruction";
const string invaladd = "ERROR, invalid add instruction";
const string invalsub = "ERROR, invalid sub instruction";
const string invalslt = "ERROR, invalid slt instruction";
const string invalsltu = "ERROR, invalid sltu instruction";
const string ioutofrange = "ERROR, index out of range";
const string invallis = "ERROR,  invalid lis instruction";
const string invalmultdiv = "ERROR,  invalid mult or div instruction";

// List relocatables
vector<int> relocatables;
// List of Instructions
vector<int> Instructions;
// Number of Instructions
int numInst = 0;
// Symbol table for labels
map<string, int> dict;
// address current instruction is on
int PC = 0;

//erases whitespace
void erasewhitespace(vector<Token*> &Tokens)
{
	vector<Token*>::iterator vit = Tokens.begin();
	while (vit != Tokens.end())
	{
		Token *element = *vit;
		if (element->toString() == "WHITESPACE")
		{
			Tokens.erase(vit);
		}
		else {++vit;}
	}
}

// outputs binary
void output_word(int word) {
	char c;
	c = word >> 24;
	cout << c;
	c = word >> 16;
	cout << c;
	c = word >> 8;
	cout << c;
	c = word;
	cout << c;
}

// assembles tokens into instructions 
void assembler(vector<Token*> &Tokens)
{
	// Checks size of vector
	if (Tokens.size() == 2)
	{
		// checks if there is only one thing after.word
		if (Tokens[0]->toString() == "DOTWORD")
		{
			if (Tokens[1]->toString() == "ID")
			{
				map<string, int>::iterator mit = dict.find(Tokens[1]->getLexeme());
				// if entry is not in dictionary, id is an error
				if (mit != dict.end())
				{
					// pushes value of label to instructions
					Instructions.push_back(mit->second);
					relocatables.push_back((PC+12));
				}
				else {throw labelnotfound;}
			}
			// pushes denoted int to instructions
			else {
				//cout << Tokens[1]->toInt() << endl;
				Instructions.push_back(Tokens[1]->toInt());}
		}
		else 
		// checks for proper jr instruction
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "jr"))
		{
			// only carries out if next token is a register
			if (Tokens[1]->toString() == "REGISTER")
			{
				int s = Tokens[1]->toInt();
				int inst = (s << 21) | 8;
				Instructions.push_back(inst);
			}
			else
			{throw invaljr;}
		}
		else
		// checks for proper jalr instruction
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "jalr"))
		{
			// only carries out if next token is a register
			if (Tokens[1]->toString() == "REGISTER")
			{
				int s = Tokens[1]->toInt();
				int inst = (s << 21) | 9;
				Instructions.push_back(inst);
			}
			else
			{throw invaljr;}
		}
		else
		// checks for proper lis instruction
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "lis"))
		{
			// only carries out if next token is a register
			if (Tokens[1]->toString() == "REGISTER")
			{
				int d = Tokens[1]->toInt();
				int inst = (d << 11) | 20;
				Instructions.push_back(inst);
			}
			else
			{throw invallis;}
		}
		else
		// checks for proper mflo instruction
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "mflo"))
		{
			// only carries out if next token is a register
			if (Tokens[1]->toString() == "REGISTER")
			{
				int d = Tokens[1]->toInt();
				int inst = (d << 11) | 18;
				Instructions.push_back(inst);
			}
			else
			{throw invallis;}
		}
		else
		// checks for proper mfhi instruction
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "mfhi"))
		{
			// only carries out if next token is a register
			if (Tokens[1]->toString() == "REGISTER")
			{
				int d = Tokens[1]->toInt();
				int inst = (d << 11) | 16;
				Instructions.push_back(inst);
			}
			else
			{throw invallis;}
		}
		else
		{throw invalid;}
	}
	else
	if (Tokens.size() == 4)
	{
		// checks for mult instruction
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "mult"))
		{
			// only carries out if next 3 tokens are correct
			if ((Tokens[1]->toString() == "REGISTER") && (Tokens[2]->toString() == "COMMA") && (Tokens[3]->toString() == "REGISTER"))
			{
				int s = Tokens[1]->toInt();
				int t = Tokens[3]->toInt();
				int inst = (s << 21) | (t << 16) | 24;
				Instructions.push_back(inst);
			}
			else
			{throw invalmultdiv;}
		}
		else
		// checks for multu instruction
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "multu"))
		{
			// only carries out if next 3 tokens are correct
			if ((Tokens[1]->toString() == "REGISTER") && (Tokens[2]->toString() == "COMMA") && (Tokens[3]->toString() == "REGISTER"))
			{
				int s = Tokens[1]->toInt();
				int t = Tokens[3]->toInt();
				int inst = (s << 21) | (t << 16) | 25;
				Instructions.push_back(inst);
			}
			else
			{throw invalmultdiv;}
		}
		else
		// checks for div instruction
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "div"))
		{
			// only carries out if next 3 tokens are correct
			if ((Tokens[1]->toString() == "REGISTER") && (Tokens[2]->toString() == "COMMA") && (Tokens[3]->toString() == "REGISTER"))
			{
				int s = Tokens[1]->toInt();
				int t = Tokens[3]->toInt();
				int inst = (s << 21) | (t << 16) | 26;
				Instructions.push_back(inst);
			}
			else
			{throw invalmultdiv;}
		}
		else
		// checks for divu instruction
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "divu"))
		{
			// only carries out if next 3 tokens are correct
			if ((Tokens[1]->toString() == "REGISTER") && (Tokens[2]->toString() == "COMMA") && (Tokens[3]->toString() == "REGISTER"))
			{
				int s = Tokens[1]->toInt();
				int t = Tokens[3]->toInt();
				int inst = (s << 21) | (t << 16) | 27;
				Instructions.push_back(inst);
			}
			else
			{throw invalmultdiv;}
		}
		else
		{throw invalid;}
	}
	else
	if (Tokens.size() == 6)
	{
		// checks for add instruction
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "add"))
		{
			// only carries out if next 3 tokens are registers
			if ((Tokens[1]->toString() == "REGISTER") && (Tokens[2]->toString() == "COMMA") && (Tokens[3]->toString() == "REGISTER") 
				&& (Tokens[4]->toString() == "COMMA") && (Tokens[5]->toString() == "REGISTER"))
			{
				int d = Tokens[1]->toInt();
				int s = Tokens[3]->toInt();
				int t = Tokens[5]->toInt();
				int inst = (s << 21) | (t << 16) | (d << 11) | 32;
				Instructions.push_back(inst);
			}
			else
			{throw invaladd;}
		}
		else
		// checks for sub instruction
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "sub"))
		{
			// only carries out if next 3 tokens are registers
			if ((Tokens[1]->toString() == "REGISTER") && (Tokens[2]->toString() == "COMMA") && (Tokens[3]->toString() == "REGISTER") 
				&& (Tokens[4]->toString() == "COMMA") && (Tokens[5]->toString() == "REGISTER"))
			{
				int d = Tokens[1]->toInt();
				int s = Tokens[3]->toInt();
				int t = Tokens[5]->toInt();
				int inst = (s << 21) | (t << 16) | (d << 11) | 34;
				Instructions.push_back(inst);
			}
			else
			{throw invalsub;}
		}
		else
		// checks for slt instruction
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "slt"))
		{
			// only carries out if next 3 tokens are registers
			if ((Tokens[1]->toString() == "REGISTER") && (Tokens[2]->toString() == "COMMA") && (Tokens[3]->toString() == "REGISTER") 
				&& (Tokens[4]->toString() == "COMMA") && (Tokens[5]->toString() == "REGISTER"))
			{
				int d = Tokens[1]->toInt();
				int s = Tokens[3]->toInt();
				int t = Tokens[5]->toInt();
				int inst = (s << 21) | (t << 16) | (d << 11) | 42;
				Instructions.push_back(inst);
			}
			else
			{throw invalslt;}
		}
		else
		// checks for sltu instruction
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "sltu"))
		{
			// only carries out if next 3 tokens are registers
			if ((Tokens[1]->toString() == "REGISTER") && (Tokens[2]->toString() == "COMMA") && (Tokens[3]->toString() == "REGISTER") 
				&& (Tokens[4]->toString() == "COMMA") && (Tokens[5]->toString() == "REGISTER"))
			{
				int d = Tokens[1]->toInt();
				int s = Tokens[3]->toInt();
				int t = Tokens[5]->toInt();
				int inst = (s << 21) | (t << 16) | (d << 11) | 43;
				Instructions.push_back(inst);
			}
			else
			{throw invalsltu;}
		}
		else
		// checks for beq
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "beq"))
		{
			// only carries out if proper order of tokens
			if ((Tokens[1]->toString() == "REGISTER") && (Tokens[2]->toString() == "COMMA") && (Tokens[3]->toString() == "REGISTER") 
				&& (Tokens[4]->toString() == "COMMA"))
			{
				int i;
				if (Tokens[5]->toString() == "ID")
				{
					map<string, int>::iterator lit = dict.find(Tokens[5]->getLexeme());
					// if entry is not in dictionary, id is an error
					if (lit != dict.end())
					{
						int val = lit->second;
						i = (val - 12 - (PC + 4))/4;
					}
					else
					{throw labelnotfound;}
				}
				else
				{
					i = Tokens[5]->toInt();
				}
				int s = Tokens[1]->toInt();
				int t = Tokens[3]->toInt();
				if ((i >= -32768) && (i <= 32767))
				{
					if (i < 0)
					{
						int inst = (4 << 26) | (s << 21) | (t << 16) | (i & 0xffff);
						Instructions.push_back(inst);
					}
					else
					{
						int inst = (4 << 26) | (s << 21) | (t << 16) | i;
						Instructions.push_back(inst);
					}
				}
				else
				{throw ioutofrange;}
			}
			else
			{throw invalsltu;}
		}
		else
		// checks for bne
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "bne"))
		{
			// only carries out if proper order of tokens
			if ((Tokens[1]->toString() == "REGISTER") && (Tokens[2]->toString() == "COMMA") && (Tokens[3]->toString() == "REGISTER") 
				&& (Tokens[4]->toString() == "COMMA"))
			{
				int i;
				if (Tokens[5]->toString() == "ID")
				{
					map<string, int>::iterator lit = dict.find(Tokens[5]->getLexeme());
					// if entry is not in dictionary, id is an error
					if (lit != dict.end())
					{
						int val = lit->second;
						i = (val - 12 - (PC + 4))/4;
					}
					else
					{throw labelnotfound;}
				}
				else
				{
					i = Tokens[5]->toInt();
				}
				int s = Tokens[1]->toInt();
				int t = Tokens[3]->toInt();
				if ((i >= -32768) && (i <= 32767))
				{
					if (i < 0)
					{
						int inst = (5 << 26) | (s << 21) | (t << 16) | (i & 0xffff);
						Instructions.push_back(inst);
					}
					else
					{
						int inst = (5 << 26) | (s << 21) | (t << 16) | i;
						Instructions.push_back(inst);
					}
				}
				else
				{throw ioutofrange;}
			}
			else
			{throw invalsltu;}
		}
		else
		{throw invalid;} 
	}
	else
	if (Tokens.size() == 7)
	{
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "lw"))
		{
			// only carries out if proper order of tokens
			if ((Tokens[1]->toString() == "REGISTER") && (Tokens[2]->toString() == "COMMA") && (Tokens[4]->toString() == "LPAREN") 
				&& (Tokens[5]->toString() == "REGISTER") && (Tokens[6]->toString() == "RPAREN"))
			{
				int i = Tokens[3]->toInt();
				int s = Tokens[5]->toInt();
				int t = Tokens[1]->toInt();
				if ((i >= -32768) && (i <= 32767))
				{
					if (i < 0)
					{
						int inst = (35 << 26) | (s << 21) | (t << 16) | (i & 0xffff);
						Instructions.push_back(inst);
					}
					else
					{
						int inst = (35 << 26) | (s << 21) | (t << 16) | i;
						Instructions.push_back(inst);
					}
				}
				else
				{throw ioutofrange;}
			}
			else
			{throw invalsltu;}
		}
		else
		if ((Tokens[0]->toString() == "ID") && (Tokens[0]->getLexeme() == "sw"))
		{
			// only carries out if proper order of tokens
			if ((Tokens[1]->toString() == "REGISTER") && (Tokens[2]->toString() == "COMMA") && (Tokens[4]->toString() == "LPAREN") 
				&& (Tokens[5]->toString() == "REGISTER") && (Tokens[6]->toString() == "RPAREN"))
			{
				int i = Tokens[3]->toInt();
				int s = Tokens[5]->toInt();
				int t = Tokens[1]->toInt();
				if ((i >= -32768) && (i <= 32767))
				{
					if (i < 0)
					{
						int inst = (43 << 26) | (s << 21) | (t << 16) | (i & 0xffff);
						Instructions.push_back(inst);
					}
					else
					{
						int inst = (43 << 26) | (s << 21) | (t << 16) | i;
						Instructions.push_back(inst);
					}
				}
				else
				{throw ioutofrange;}
			}
			else
			{throw invalsltu;}
		}
		else
		{throw invalid;}
	}
	else
	{throw invalid;} 
}

//creats a symbol table for labes
void labelcreate(vector<Token*> &lines)
{
	// only goes through if the size of the lines is at least one token
	if (lines.size() >= 1)
	{
		if (lines[0]->toString() == "LABEL") 
		{

			std::vector<Token*>::iterator it = lines.begin();
			// iterates over all the tokens and finds all the labels
			while (it != lines.end())
			{
				Token *element = *it;
				// if not a label, it's an instruction and therefore no labels should come after and exits
				if (element->toString() != "LABEL")
				{
					// increments the number of instructions
					++numInst;
					break;
				}
				else
				{
					string entry = element->getLexeme();
					// erases last char in string
					entry.erase(entry.end() - 1);
					map<string, int>::iterator mit = dict.find(entry);
					// if entry is not in dictionary, adds it, otherwise throws an error
					if (mit == dict.end())
					{
						dict[entry] = (numInst * 4) + 12;
						lines.erase(it);
					}
					else {throw labelexists;}
				}
			}
		}else {++numInst;}
	}
}


int main(int argc, char* argv[]){
	// Nested vector representing lines of Tokens
	// Needs to be used here to cleanup in the case
	// of an exception
	vector< vector<Token*> > tokLines;
	try{
		// Create a MIPS recognizer to tokenize
		// the input lines
		Lexer lexer;
		// Tokenize each line of the input
		string line;
		while(getline(cin,line)){
			tokLines.push_back(lexer.scan(line));
		}

		// erase whitespace tokens
		// then build symbol table, doesn't care about errors
		vector<vector<Token*> >::iterator it;
		for(it = tokLines.begin(); it != tokLines.end(); ++it)
		{
			erasewhitespace(*it);
			labelcreate(*it);
		}

/*
		// prints tokens to standard error
		for(it = tokLines.begin(); it != tokLines.end(); ++it){
			vector<Token*>::iterator it2;
			for(it2 = it->begin(); it2 != it->end(); ++it2){
				cerr << "  Token: "  << *(*it2) << endl;
			}
		}
*/

		// converts tokens into binary instructions
		vector<vector<Token*> >::iterator mit;
		for(mit = tokLines.begin(); mit != tokLines.end(); ++mit){
			if (mit->size() >= 1)
			{
				assembler(*mit);
				PC += 4;
			}
		}

		map<string, int>::iterator mapit;
		int cookie = (4 << 26) | 2;
		int codelen = (PC) + 12;
		int filen = (relocatables.size()*8) + codelen;
		Instructions.insert(Instructions.begin(), codelen);
		Instructions.insert(Instructions.begin(), filen);
		Instructions.insert(Instructions.begin(), cookie);

		vector<int>::iterator relit;
		for(relit = relocatables.begin(); relit != relocatables.end(); ++ relit)
		{
			Instructions.push_back(1);
			Instructions.push_back(*relit);
		}
		// if code gets to this point, it is correct assembly
		// prints the symbol table to cerr
		/*map<string, int>::iterator mapit;
		for(mapit = dict.begin(); mapit != dict.end(); ++mapit)
		{
			cerr << mapit->first << " " << mapit->second <<endl;
		}*/

		// outputs the binary instructions
		for(vector<int>::iterator it = Instructions.begin(); it != Instructions.end(); ++it)
		{
			output_word(*it);
		}

	} catch(const string& msg){
		// If an exception occurs print the message and end the program
		cerr << msg << endl;
	}
	// Delete the Tokens that have been made
	vector<vector<Token*> >::iterator it;
	for(it = tokLines.begin(); it != tokLines.end(); ++it){
		vector<Token*>::iterator it2;
		for(it2 = it->begin(); it2 != it->end(); ++it2){
			delete *it2;
		}
	}
}
