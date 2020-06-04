//Simple SIC assembler that supports bitmask relocation

#include<bits/stdc++.h>
#include<fstream>

using namespace std;

map<string , string> optab;
map<string , string> symtab;

unsigned int start;
string length;

string decToHex(unsigned int d){
	stringstream ss;
	ss << hex << uppercase << d;
	return ss.str();
}

unsigned int hexToDec(string h){
	stringstream ss;
	unsigned int d;
	ss << hex << h;
	ss >> d;
	return d;
}
void getOptab(void){
	fstream file("opcode.txt", ios::in);
	if(!file){ cout << "Opcode not found" << endl; return;}
	string line;
	string opcodestr;
	string opcode;
	stringstream s;
	while(getline(file, line)){
		s.str(line);
		s >> opcodestr >> opcode;
		//cout << opcodestr << " " << opcode << endl;
		//optab.insert(pair<string, string>(opcodestr, opcode));
		optab[opcodestr] = opcode;
		s.clear();
	}
	file.close();
}

string getOpcode(string str){
	map<string , string>::iterator it;
	it = optab.find(str);
	if(it != optab.end()){
		return it->second;
	}
	return "NULL";
}

string getSymbol(string str){
	map<string ,string>::iterator it;
	string res;
	it = symtab.find(str);
	if(it != symtab.end()){
		res = it->second;
	}
	return res;
}

void printSymtab(){
	map<string, string>::iterator it;
	for(it = symtab.begin(); it != symtab.end(); it++){
		cout << it->first << " " << it->second << "\n";
	}
}

void pass1(fstream &file){
	//Create the opcode table 
	getOptab();
	string opcode;
	//Location couter
	//symtab
	string line;
	string label, operand, value;
	string word[3];
	int count;
	stringstream s;
	unsigned int locctr;

	while(getline(file, line)){
		s.str(line);
		count = 0;	
		while(s >> word[count++]){}
		count--;
		if(count == 3){
			label = word[0];
			operand = word[1];
			value = word[2];
		}
		else if(count == 2){
			operand = word[0];
			value = word[1];
			label.clear();
		}
		else if(count == 1){
			operand = word[0];
			label.clear();
			value.clear(); 
		}
		s.clear();
		//cout << label << " " << operand << " " << value << endl;

		if(operand == "START"){
			start = hexToDec(value);
			locctr = start;
			continue;
			//cout << "loc : " << locctr << endl;
		}
		else if(operand == "END"){
			length = decToHex(locctr - start);
			continue;
		}
		else if(label == "."){continue;}
		else if(!label.empty()){//search in symtab
			map<string, string>::iterator it;
			it = symtab.find(label);
			if(it != symtab.end()){
				cout << "Duplicate Symtab\n";
			}
			else{
				symtab[label] = decToHex(locctr);
			}

		}

		if((opcode = getOpcode(operand)) != "NULL"){
			locctr += 3;
		}
		else if(operand == "WORD"){
			locctr += 3;
		}
		else if(operand == "RESW"){
			locctr += (3 * stoi(value));
		}
		else if(operand == "RESB"){
			locctr += stoi(value);
		}
		else if(operand == "BYTE"){
			//find size of const
		}
		else{
			cout << "Could not find opcode\n";
		}
			
		//cout << operand << " " << decToHex(locctr) << " " << locctr << endl;
	}	
}

void printText(string &Taddress, string &Tlength, stringstream &codeStream , unsigned int &locctr){
        Tlength = decToHex(locctr - hexToDec(Taddress));
	string Tcode;
	codeStream >> Tcode;
	codeStram.clear();
v
	cout << "T" << setw(6) << setfill('0') << Taddress;
	cout << setw(2) << setfill('0') << Tlength;
	cout << Tcode << "\n";
	Taddress.clear();
	Tcode.clear();
}

void pass2(fstream &file){
	fstream outFile("out.txt", ios::out|ios::trunc);
	string opcode;
	string line;
	string label, operand, value;
	string word[3];
	string Taddress, Tlength;
	int count;
	stringstream s;
	stringstream codeStream;
	unsigned int locctr;

	while(getline(file, line)){
		s.str(line);
		count = 0;	
		while(s >> word[count++]){}
		count--;
		if(count == 3){
			label = word[0];
			operand = word[1];
			value = word[2];
		}
		else if(count == 2){
			operand = word[0];
			value = word[1];
			label.clear();
		}
		else if(count == 1){
			operand = word[0];
			label.clear();
			value.clear(); 
		}
		s.clear();
		

		if(operand == "START"){
			locctr = start;
			string name = label;
			name.resize(6);
			cout << "H" << setw(6) << right << name;
			cout << setw(6) << setfill('0') << decToHex(start) << setw(6) << length << "\n";
			continue;
		}
		else if(operand == "END"){
			cout << "E" << setw(6) << setfill('0') << decToHex(start) << "\n";
			continue;
		}
		else if(label == "."){
			continue;
		}
		else if((opcode = getOpcode(operand)) != "NULL"){
			if(Taddress.empty()){
				Taddress = decToHex(locctr);
			}
			locctr += 3;

			if(!label.empty()){
			}
		}
		else if(operand == "WORD"){
			locctr += 3;
		}
		else if(operand == "RESW"){
			if(!Taddress.empty()){
			  printText(Taddress, Tlength, codeStream, locctr);
			}
			locctr += (3 * stoi(value));
		}
		else if(operand == "RESB"){
			locctr += stoi(value);
		}
		else if(operand == "BYTE"){
			//find size of const
		}
		else{
			cout << "Could not find opcode\n";
		}


	}
}

		
int main(){
	cout << "SIC Assembler" << endl;

	fstream file("test.asm", ios::in);
	try{
		if(!file){
			throw runtime_error("Could not find the File specified");
		}
	}
	catch(runtime_error e){
		cout << e.what();
	}
	pass1(file);
	//cout << "Start " << decToHex(start) << endl;
	//cout << "Lenght " << length << endl;
	//printSymtab();	
	
	file.clear();
	file.seekg(0, ios::beg);
	file.seekp(0, ios::beg);
	pass2(file);
	file.close();
	return 0;
}
