// basic file operations
#include <iostream>
#include "c_LevelOne.h"
#include <fstream>
#include <string>
using namespace std;

void SaveData(void)
{
	int a = 36;
	string test = "hello";
	char test2 = 'c';
	ofstream myfile;
	myfile.open("example.txt");
	myfile << "Writing this to a file." << endl;
	myfile << a << "\n"; // prints out "36"
	myfile << test << "\n"; // prints out "hello"
	myfile << test2 << "\n"; // prints out "c"
	myfile.close();
}