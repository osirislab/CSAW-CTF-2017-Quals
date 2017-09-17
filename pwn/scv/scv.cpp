#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

//This is no no but whatever.....

using namespace std;


 
int main(){
	
	setvbuf(stdout, NULL, _IONBF, 0);	
	setvbuf(stdin, NULL, _IONBF, 0);

	int op = 0;
	int eflag = 1;
	int length = 0;
	
	char buffer[168];
	
	while(eflag){
		cout << "-------------------------" << endl;	
		cout << "[*]SCV GOOD TO GO,SIR...." << endl;
		cout << "-------------------------" << endl;	
		cout << "1.FEED SCV...." << endl;
		cout << "2.REVIEW THE FOOD...." << endl;
		cout << "3.MINE MINERALS...." << endl;
		cout << "-------------------------" << endl;	
		cout << ">>";
		cin >> op;

		switch(op){
			case 1:
				cout << "-------------------------" << endl;	
				cout << "[*]SCV IS ALWAYS HUNGRY....." << endl;
				cout << "-------------------------" << endl;	
				cout << "[*]GIVE HIM SOME FOOD......." << endl;
				cout << "-------------------------" << endl;	
				cout << ">>";

				length = read(0,buffer,248);
				break;
			case 2:
				cout << "-------------------------" << endl;	
				cout << "[*]REVIEW THE FOOD..........." << endl;
				cout << "-------------------------" << endl;	
				cout << "[*]PLEASE TREAT HIM WELL....." << endl;
				cout << "-------------------------" << endl;	
	
				printf("%s\n",buffer);
				break;
			case 3:
				eflag = 0;
				cout << "[*]BYE ~ TIME TO MINE MIENRALS..." << endl;
				break;
			default:
				cout << "[*]DO NOT HURT MY SCV...." << endl;
				break;
		}	 
	}

}
