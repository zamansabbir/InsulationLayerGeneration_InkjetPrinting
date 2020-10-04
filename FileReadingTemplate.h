#include<fstream>
#include<string>
#include<iostream>

using namespace std;

int fileRead(){
	string line,fileDirectory;
	ifstream file(fileDirectory);
	if(file.is_open()){
		while(getline(file,line)){
			
		}
		file.close();
	}
	
	return 0;
}
