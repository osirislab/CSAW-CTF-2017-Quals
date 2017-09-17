#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define MAX_SIZE 100
#define INPUT 0
#define OUTPUT 1

using namespace std;

char*ptr[MAX_SIZE];
int global_index = 0;

//option 1:Make
void Make(){

	int user_size = 0;
	
	cout << "[*]SPECIFY THE SIZE OF ZEALOT" << endl;
	cout << ">>";
	
	cin >> user_size;
	
	char*local = (char *)malloc(sizeof(char)*user_size);

	cout << "[*]GIVE SOME SKILLS TO ZEALOT" << endl;
	cout << ">>";
	
	int length = read(0,local,user_size);
	ptr[global_index++] = local;			
		
}
	

//option 2:Break
void Destroy(){
	
	int user_index = 0;

	cout << "[*]WHICH ONE DO YOU WANT TO DESTROY?" << endl;
	cout << ">>";

	cin >> user_index;

	cout << "[*]BREAKING...." << endl;
	free(ptr[user_index]);
	//ptr[user_index] = NULL;
	cout << "[*]SUCCESSFUL!" << endl;
		
}

//option 3:Fix
void Fix(){

	int user_index = 0;
	int user_size = 0;

	cout << "[*]WHCIH ONE DO YOU WANT TO FIX ?" << endl;
	cout << ">>";

	cin >> user_index;
	
	if(ptr[user_index] != NULL){

		cout << "[*]SPECIFY THE SIZE OF ZEALOT" << endl;
		cout << ">>";

		cin >> user_size;

		cout << "[*]GIVE SOME SKILLS TO ZEALOT" << endl;
		cout << ">>";

		int read_length = read(0,ptr[user_index],user_size);

		cout << "[*]FIXED ZEALOT NUMBER:" << user_index << endl;
		cout << "[*]FIXED ZEALOT SIZE:" << user_size << endl;


	}else{
		cout << "[*]HMMM...." << endl;

	}

}

//option 4:Display
void Display(){

	int user_index = 0; 
	cout << "[*]WHICH ONE DO YOU WANT TO SEE?" << endl;
	cout << ">>";

	cin >> user_index;
	
	cout << "[*]SHOWING...." << endl;
	write(1,ptr[user_index],sizeof(ptr[user_index]));	


}

int main(){

	//stuff....	
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
	
	int eflag = 1;
	int select = 0;

	cout << "|-------------------------------|" << endl;
	cout << "|AUIR AUIR AUIR AUIR AUIR AUIR A|" << endl;

	while(eflag){

		cout << "|-------------------------------|" << endl;
		cout << "[1]MAKE ZEALOTS" << endl;
		cout << "[2]DESTROY ZEALOTS" << endl;
		cout << "[3]FIX ZEALOTS" << endl;
		cout << "[4]DISPLAY SKILLS" << endl;
		cout << "[5]GO HOME" << endl;
		cout << "|-------------------------------|" << endl;
		cout << ">>";


		cin >> select;

		switch(select){
			case 1:
				Make();
				break;
			case 2:
				Destroy();
				break;
			case 3:
				Fix();
				break;
			case 4:
				Display();
				break;
			case 5:
				eflag = 0;
				cout << "[*]NOOBS CAN'T PROTECT AUIR...." << endl;
				break;
			default:
				cout << "[*]WRONG OPTION...." << endl;

		}

	}	

}
