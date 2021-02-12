#include<iostream>
#include<conio.h>
#include"DrawPCB.h"
#include"ReadGerber.h"
#include<string>
#include<sstream>

using namespace std;
int main() {


	DrawPCB *d = new DrawPCB("C:\\Release\\BPM_TOP.art");
	//cout << "Enter directory of art file: ";
	//std::string fileLocation = "";
	//getline(cin,fileLocation);
	//DrawPCB *d = new DrawPCB(fileLocation);
	d->draw();
	d->saveImage();
	delete d;

	cout << "done";
	_getch();
	return 0;
}