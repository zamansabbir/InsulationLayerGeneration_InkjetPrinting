/**
header file:ReadGerber.h with implementation file =ReadGerber.cpp
Purpose: process the gerber file to create image from the gerber instructions
@author SBZ
@version 3.0 08/24/2016
*/

#include <string>
#include "ReadGerber.h"
#include <iostream>
#include<fstream>
#include<math.h>

using namespace std;
ReadGerber::ReadGerber(string directory) //constructor of class ReadGerber with directory as std::string
{
	this->X0 = 20;
	this-> Y0 = 20;
	this->directory = directory;
	setFormatSpecication(this->directory);
	setDimension();
}


int ReadGerber::checkInstruction(string line) {
	if (line.find("D01") != string::npos)
		//check for line
		return 1;

	else if (line.find("D02") != string::npos)
		//check for position shifting
		return 2;
	else if (line.find("D03") != string::npos)
		//check for flash
		return 3;
	else
		return 0;// no instruction for drawing, flashing or shifting
}

double* ReadGerber::findXY(string line, double currentX_1, double currentY_1) ///return a double* 
{
	static double a[2] = {};
	//string xyAsString[2] = {};
	if (line.find("X") != string::npos) //X is present
	{
		int Xpos = line.find("X");
		int Dpos = line.find("D");
		if (line.find("Y") != string::npos) // Y is there too
		{
			int Ypos = line.find("Y");
			a[0] = convertToDecimalX(line.substr(Xpos + 1, Ypos - Xpos - 1));
			a[1] = convertToDecimalY(line.substr(Ypos + 1, Dpos - Ypos - 1));
			return a;

		}
		else // Y is not there, replacing Y=currentY
		{
			a[0] = convertToDecimalX(line.substr(Xpos + 1, Dpos - Xpos - 1));
			a[1] = currentY_1;
			return a;
		}
	}
	else if (line.find("Y") != string::npos) {	// only Y is there
		int Ypos = line.find("Y");
		int Dpos = line.find("D");
		a[0] = currentX_1;
		a[1] = convertToDecimalY(line.substr(Ypos + 1, Dpos - Ypos - 1));
		return a;
	}
	else {
		a[0] = currentX_1;
		a[1] = currentY_1;
		return a;
	}


}
int ReadGerber::convertToPixel(double value, int dpi) {
	return round(value*dpi);
}

double* ReadGerber::shiftXYToOrigin(double x, double y) {
	static double b[2] = {};
	if (x > X0&&y > Y0) {
		b[0] = x - (Xmin - X0);
		b[1] = y - (Ymin - Y0);
		return b;
	}
	else {
		b[0] = x;
		b[1] = y;
		return b;
	}
}

int ReadGerber::convertToPixel(double value) {
	return round(value * 1693);
}

double ReadGerber::convertToDecimalX(string x) {
	int XintegerPart;
	double XdecimalPart;
	switch (leadingZero) {
	case 0:
		//Enter code for not leadingZero condition here
		break;
	case 1:
		XdecimalPart = (double)(atoi(x.substr(x.length() - decimalXNumberOfDigits, decimalXNumberOfDigits).c_str())) / pow(10, decimalXNumberOfDigits);
		XintegerPart = atoi(x.substr(0, x.length() - decimalXNumberOfDigits).c_str());

		break;
	}
	return XintegerPart + XdecimalPart;
}

double ReadGerber::convertToDecimalY(string y)
{
	int YintegerPart;
	double YdecimalPart;
	switch (leadingZero) {
	case 0:

		break;
	case 1:
		YdecimalPart = (double)(atoi(y.substr(y.length() - decimalYNumberOfDigits, decimalYNumberOfDigits).c_str())) / pow(10, decimalYNumberOfDigits);
		YintegerPart = atoi(y.substr(0, y.length() - decimalYNumberOfDigits).c_str());
		break;
	}
	return YintegerPart + YdecimalPart;

}
void ReadGerber::setDimension()
{
	double *XY = new double();
	double minimumX = 0.0, minimumY=0.0, maximumX = 0.0, maximumY=0.0, currentX_1 = 0.0, currentY_1 = 0.0;
	*(XY + 0) = 0;
	*(XY + 1) = 0;
	ifstream file(directory);
	std::string line;
	bool firsTimetXY = true, check = false;
	if (file.is_open()) {
		while (getline(file, line)) {
			int c = checkInstruction(line);
			switch (c) {
			case 1://"D01"
				XY = findXY(line, currentX_1, currentY_1);
				minimumX = (*(XY + 0) < minimumX) ? *(XY + 0) : minimumX;
				minimumY = (*(XY + 1) < minimumY) ? *(XY + 1) : minimumY;
				maximumX = (*(XY + 0) > maximumX) ? *(XY + 0) : maximumX;
				maximumY = (*(XY + 1) > maximumY) ? *(XY + 1) : maximumY;
				check = true;
				break;
			case 2://"D02"
				XY = findXY(line, currentX_1, currentY_1);
				minimumX = (*(XY + 0) < minimumX) ? *(XY + 0) : minimumX;
				minimumY = (*(XY + 1) < minimumY) ? *(XY + 1) : minimumY;
				maximumX = (*(XY + 0) > maximumX) ? *(XY + 0) : maximumX;
				maximumY = (*(XY + 1) > maximumY) ? *(XY + 1) : maximumY;
				check = true;
				//	cout << "X=" << read->convertToPixel(*(XY + 0)) << " ,Y=" << read->convertToPixel(*(XY + 1)) << endl;
				break;
			case 3://"D03"
				XY = findXY(line, currentX_1, currentY_1);
				minimumX = (*(XY + 0) < minimumX) ? *(XY + 0) : minimumX;
				minimumY = (*(XY + 1) < minimumY) ? *(XY + 1) : minimumY;
				maximumX = (*(XY + 0) > maximumX) ? *(XY + 0) : maximumX;
				maximumY = (*(XY + 1) > maximumY) ? *(XY + 1) : maximumY;
				check = true;
				//cout << "X=" << read->convertToPixel(*(XY + 0)) << " ,Y=" << read->convertToPixel(*(XY + 1)) << endl;
				break;
			default://"No Instruction"

				break;
			}

			if (firsTimetXY&check) {///Encountered for the first time, ignored later
				minimumX = *(XY + 0);//minimumX
				minimumY = *(XY + 1);//minimumY
				maximumX = *(XY + 0);//maximumX
				maximumY = *(XY + 1);//maximumY
				firsTimetXY = false;
			}

			currentX_1 = *(XY + 0);
			currentY_1 = *(XY + 1);


		}
		file.close();
	}
	this->width = maximumX - minimumX;
	this->height = maximumY - minimumY;
	this->Xmin = minimumX;
	this->Ymin = minimumY;
	
}
void ReadGerber::setFormatSpecication(std::string directory) {
	std::string line;
	ifstream file(directory);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (line.find("FS") != string::npos)
			{

				if (line.find("L") != string::npos)
					leadingZero = 1;
				if (line.find("I") != string::npos)
					coordinateSystem = 1;
				if (line.find("MO") != string::npos)
					unitInch = 1;
				integerXNumberOfDigits = (int)line[6] - 48;
				decimalXNumberOfDigits = (int)line[7] - 48;
				integerYNumberOfDigits = (int)line[9] - 48;
				decimalYNumberOfDigits = (int)line[10] - 48;
				break;
				//specification set, breaking from the loop
			}
		}
		file.close();
	}
}
double ReadGerber::getDimensionWidth() {
	return this->width;
}

double ReadGerber::getDimensionHeight() {
	return this->height;
}

