#include "DrawPCB.h"
#include<fstream>
#include<string>
#include"DrawPCB.h"
#include<algorithm>
#include<stdlib.h>
#include<math.h>
using namespace std;
DrawPCB::DrawPCB(string directory) {
	this->fileDirectory = directory;
	this->read = new ReadGerber(this->fileDirectory);
	this->image = cv::Mat(read->convertToPixel(read->getDimensionHeight()), read->convertToPixel(read->getDimensionWidth()),CV_8UC1,cv::Scalar(255));
	//cv::imshow("x",image);
	//cv::waitKey(0);
	this->XY = new double();
	*(XY + 0) = 0.0;
	*(XY + 1) = 0.0;
	this->currentXY_1 = new double();
	this->solidTrace = false;
	this->clockWise = false;
	this->height = read->getDimensionHeight();
	x = 0.0;
	y = 0.0;
	this->armThickness = 5;
	this->directionChoice = 1;
	this->armLength = 13;
	this->hypotenuse= sqrt(2 * armLength*armLength);
}
bool DrawPCB::draw() {
	double *tempXY = new double();
	*(tempXY+0)=0;
	*(tempXY + 1) = 0;
	

		string line;
		static bool crossCheck = true;
	//fileDirectory = "C:\\Release\\SIMPLE_TOP1.art";
		ifstream file(fileDirectory);
		if (file.is_open()) {
			while (getline(file, line)) {
				if (line.find("M02") != string::npos)
					break;
				else if (crossCheck & (line.find("G04") != string::npos))
					continue;
				else if (crossCheck & (line.find("%") != string::npos))
					continue;
				else{
					crossCheck = false;
					int c = read->checkInstruction(line);
					switch (c) {
					
					case 1: {/**"D01"
						   find the coordinates from the gerber, shift them to the new origin(20,20)(default)
						   */
						tempXY = read->findXY(line, currentX_1, currentY_1);
						XY = read->shiftXYToOrigin(*(tempXY + 0), *(tempXY + 1));
						x = 0.0;
						y = 0.0;
						x = *(XY + 0); y = *(XY + 1);
						currentXY_1 = read->shiftXYToOrigin(currentX_1, currentY_1);
						if (solidTrace)
							performD01Solid();
						else
							performD01Mesh();
						break;
					}
					case 2: {/**"D02"
						   find the coordinates from the gerber, shift them to the new origin(20,20)(default)
						   */
						tempXY = read->findXY(line, currentX_1, currentY_1);
						XY = read->shiftXYToOrigin(*(tempXY + 0), *(tempXY + 1));
						currentXY_1 = read->shiftXYToOrigin(currentX_1, currentY_1);

				//		performD02();
						break;
					}
					case 3: {/**"D03"
						   find the coordinates from the gerber, shift them to the new origin(20,20)(default)
						   */
						tempXY = read->findXY(line, currentX_1, currentY_1);
						XY = read->shiftXYToOrigin(*(tempXY + 0), *(tempXY + 1));
						performD03();
						break;
					}
					default: {//"No Instruction"
						if (line.find("G54") != string::npos) {

							read->setCurrentAperture(line.substr(3, line.size() - 4));
							break;
						}
						if ((line.find("G02") != string::npos) | (line.find("G03") != string::npos)) {
							this->interpolationTypeLinear = false;
							if (line.find("G02") != string::npos)
								this->clockWise = true;
							else if (line.find("G03") != string::npos)
								this->clockWise = false;
							break;
						}

						break;
					}
					}

					currentX_1 = *(tempXY + 0);
					currentY_1 = *(tempXY + 1);
					//delete tempXY;
				}
			}
			file.close();
		}
//		delete tempXY;
		delete read;
	return false;
}
char DrawPCB::setApertureSizeToMemory(string aName) {
	char r = 0;
	int pos = find((read->apertureName).begin(),(read->apertureName).end(), aName) - (read->apertureName).begin();
	//double x;
	if (pos<(int)(read->apertureName).size())
		switch ((read->apertureType).at(pos)) {
		case 'C': {
			*(size + 0) = atof((read->apertureSize).at(pos).c_str());
			*(size + 1) = 0.0;
			r = 1;
			break;
		}
		case 'R': {
			string x = (read->apertureSize).at(pos);
			*(size + 0) = atof(x.substr(0, x.find("X")).c_str());
			*(size + 1) = atof(x.substr(x.find("X") + 1, string::npos).c_str());
			r = 2;
			break;
		}
		case 'o': {
			string x = (read->apertureSize).at(pos);
			*(size + 0) = atof(x.substr(0, x.find("X")).c_str());
			*(size + 1) = atof(x.substr(x.find("X") + 1, string::npos).c_str());
			r = 3;
			break;
		}
		default: {
			r = 0;
			break; }

		}
	return r;
}



void DrawPCB::performD01Solid() {
	if ((this->interpolationTypeLinear == true)) {
		char a= setApertureSizeToMemory(read->getCurrentAperture());
		if (a>0) {
			long point1[2] = {this->read->convertToPixel(*(currentXY_1+0)),this->read->convertToPixel(*(currentXY_1 + 1))};

			//long point2[2] = {this->read->convertToPixel(*(XY+0)),this->read->convertToPixel(*(XY+1))};
			long point2[2] = { this->read->convertToPixel(this->x),this->read->convertToPixel(this->y) };
			//cout << "(" << *(XY + 0) << "," << *(XY + 1) << ")---(" << *(currentXY_1 + 0) << "," << *(currentXY_1 + 1) << ")" << endl;
			cout << "(" << point1[0] << "," << point1[1] << ")(" << point2[0] << "," << point2[1] << ") ";
			this->drawTheLine(point1,point2,this->read->convertToPixel(*(size+0)));
		}
	}
}
void DrawPCB::performD02() {
}
void DrawPCB::performD03() {
	if ((this->interpolationTypeLinear == true)) {
		int a = (int)setApertureSizeToMemory(read->getCurrentAperture());
		if (a>0) {
			switch ((int)a)
			{
			case 1: {
				long center[2] = {this->read->convertToPixel(*(XY + 0)),this->read->convertToPixel(*(XY + 1))};
				fillTheCircle(center,this->read->convertToPixel(*(size+0)/2));
				break;
			}
			case 2: {
				long point1[2] = { this->read->convertToPixel(*(XY + 0)),this->read->convertToPixel(*(XY + 1) + *(size + 1) / 2) };
				long point2[2] = { this->read->convertToPixel(*(XY + 0) + *(size + 0)),this->read->convertToPixel(*(XY + 1) - *(size + 1) / 2) };
				fillTheRectangle(point1, point2);
				break;
			}
			case 3: {
				
				break;
			}
			default:
				break;
			}
		}
	}
}


void DrawPCB::drawTheLine(long point1[], long point2[],int thickness) {
	point1[1] = this->read->convertToPixel(this->height) - point1[1];
	point2[1] = this->read->convertToPixel(this->height) - point2[1];
	cv::line(image, cv::Point(point1[0], point1[1]), cv::Point(point2[0], point2[1]), cv::Scalar(0),thickness,8,0);
}
void DrawPCB::drawTheCircle(long center[], long radius) {
	center[1] = this->read->convertToPixel(this->height) - center[1];
	cv::circle(image,cv::Point(center[0],center[1]),radius,cv::Scalar(255),1,8,0);
}
void DrawPCB::fillTheCircle(long center[], long radius) {
	center[1] = this->read->convertToPixel(this->height) - center[1];
	cv::circle(image, cv::Point(center[0], center[1]), radius, cv::Scalar(0), -1, 8, 0);
}
void DrawPCB::drawTheRectangle(long point1[], long point2[]) {
	point1[1] = this->read->convertToPixel(this->height) - point1[1];
	point2[1] = this->read->convertToPixel(this->height) - point2[1];
	cv::rectangle(image,cv::Point(point1[0],point1[1]),cv::Point(point2[0],point2[1]),cv::Scalar(0),1,8,0);
}
void DrawPCB::fillTheRectangle(long point1[], long point2[]) {
	point1[1] = this->read->convertToPixel(this->height) - point1[1];
	point2[1] = this->read->convertToPixel(this->height) - point2[1];
	cv::rectangle(image, cv::Point(point1[0], point1[1]), cv::Point(point2[0], point2[1]), cv::Scalar(0), -1, 8, 0);
}

void DrawPCB::saveImage() {
	//cv::line(image, cv::Point(100, 30), cv::Point(30, 30), cv::Scalar(0), 1, 8, 0);
	std::string directory = this->findFilePath();
	std::string fileName = this->findFileName();
	std::string saveFile = "";
	if(this->solidTrace)
		saveFile = directory + "\\" + fileName + ".bmp";
	else
	 saveFile = directory + "\\" + fileName+"Mesh.bmp";
	cv::imwrite(saveFile, this->image);
	//cv::imshow(fileName,this->image);
	//cv::waitKey(0);
}
std::string DrawPCB::findFileName() {


	size_t found1 = this->fileDirectory.find_last_of("/\\");
	size_t found2 = this->fileDirectory.find_last_of(".");
	int length = found2 - found1-1;
	string fileName = this->fileDirectory.substr(found1 + 1,length);
	return fileName;
}
std::string DrawPCB::findFilePath() {
//	cout << this->fileDirectory;
	size_t found = this->fileDirectory.find_last_of("/\\");
	string fileLocation = this->fileDirectory.substr(0, found);
	return fileLocation;

}

void DrawPCB::performD01Mesh() {
	if ((this->interpolationTypeLinear == true)) {
		char a = setApertureSizeToMemory(read->getCurrentAperture());
		if (a > 0) {
			long pointx[2] = { this->read->convertToPixel(*(currentXY_1 + 0)),this->read->convertToPixel(*(currentXY_1 + 1)) };
			long pointy[2] = { this->read->convertToPixel(this->x),this->read->convertToPixel(this->y) };
			double distance = sqrt((pointx[0] - pointy[0])*(pointx[0] - pointy[0]) + (pointx[1] - pointy[1])*(pointx[1] - pointy[1]));
			long recursion = (long)distance / hypotenuse;
			if (recursion == 0)
				recursion = 2;
			//long recursion = 2;
			this->directionChoice = this->checkLineAllignment(pointx, pointy);
		//	cout << "(" << pointx[0] << "," << pointx[1] << ")(" << pointy[0] << "," << pointy[1] << ") ";
			//this->drawTheLine(point1, point2, this->read->convertToPixel(*(size + 0)));
			switch (directionChoice) {
			case 1: {
				long xStartPoint = 0, yStartPoint = 0;
				//	int numberOfDiammonds = this->read->convertToPixel(*(size + 0)) / this->hypotenuse;
				if (pointx[0] > pointy[0]) {
					xStartPoint = pointy[0];
					yStartPoint = this->read->convertToPixel(this->height) - pointy[1];


					//cout << "(" << startPoint[0] << ", " << startPoint[1] << " )" << endl;

				}
				else
				{
					xStartPoint = pointx[0];
					yStartPoint = this->read->convertToPixel(this->height) - pointx[1];
					//this->drawMesh(point1, recursion);
				}
			//	cout << "(" << xStartPoint << "," << yStartPoint << ")"<<endl ;
			this->drawMesh(xStartPoint, yStartPoint, recursion);
				//cv::circle(image, cv::Point(xStartPoint, yStartPoint), 3, cv::Scalar(0), -1, 8, 0);
				//cv::circle(image, cv::Point(pointx[0], pointx[1]), 5, cv::Scalar(0), 1, 8, 0);
				//cv::circle(image, cv::Point(pointy[0], pointy[1]), 10, cv::Scalar(0), 1, 8, 0);




				break;
			}
			case 2: {
				//long startPoint[2] = { 0,0 };
				long xStartPoint = 0, yStartPoint = 0;
				pointx[1] = this->read->convertToPixel(this->height) - pointx[1];
				pointy[1]= this->read->convertToPixel(this->height) - pointy[1];
				if (pointx[1] > pointy[1]) {
					xStartPoint = pointy[0];
					yStartPoint = pointy[1];
					//yStartPoint = pointy[1];
				}
				else
				{
					xStartPoint = pointx[0];
					yStartPoint = pointx[1];
					//yStartPoint =  pointx[1];
				}
			//	cout << "(" << pointx[0] << "," << pointx[1] << ")" << "(" << pointy[0] << "," << pointy[1] << ")";
				//cout << "(" << xStartPoint << "," << yStartPoint << ")"<<endl ;
				this->drawMesh(xStartPoint, yStartPoint, recursion);
				//cv::circle(image, cv::Point(xStartPoint, yStartPoint), 3, cv::Scalar(0), -1, 8, 0);
				break;
			}
			case 3: {
				//long startPoint[2] = { 0,0 };
				pointx[1] = this->read->convertToPixel(this->height) - pointx[1];
				pointy[1] = this->read->convertToPixel(this->height) - pointy[1];
				long xStartPoint = 0, yStartPoint = 0;
				if (pointx[0] > pointy[0]) {
					xStartPoint = pointx[0];
					yStartPoint = pointx[1];
				}
				else
				{
					xStartPoint = pointy[0];
					yStartPoint = pointy[1];
				}
				cout << "(" << pointx[0] << "," << pointx[1] << ")" << "(" << pointy[0] << "," << pointy[1] << ")";
				cout << "(" << xStartPoint << "," << yStartPoint << ")recursion"<<recursion<<endl ;
				//this->drawMesh(xStartPoint, yStartPoint, recursion);
				break;
			}
			case 4: {
				//long startPoint[2] = { 0,0 };
				long xStartPoint = 0, yStartPoint = 0;
				if (pointx[0] > pointy[0]) {
					xStartPoint = pointx[0];
					yStartPoint = this->read->convertToPixel(this->height) - pointx[1];
				}
				else
				{
					xStartPoint = pointy[0];
					yStartPoint = this->read->convertToPixel(this->height) - pointy[1];
				}
			//	cout << "(" << xStartPoint << "," << yStartPoint << ")" <<endl;
				//	this->drawMesh(xStartPoint, yStartPoint, recursion);
				break;
			}
			default: break;
			}
		}
	}
	
}

void DrawPCB::drawMesh(long x__,long y__, int n) {
	if (n == 0)
		return;

	else {
		switch (this->directionChoice) {
		case 1: { //horizontal case
			long x2 = (long)round(x__ + armLength*cos(deg2Radian(45)));
			long y2 = (long)round(y__ + armLength*sin(deg2Radian(45)));
			cv::line(image, cv::Point(x__,  y__),cv:: Point(x2, y2), cv::Scalar(0),armThickness, 8, 0);

			long x3 = (long)round(x__+ armLength*cos(deg2Radian(-45)));
			long y3 = (long)round(y__ + armLength*sin(deg2Radian(-45)));
			cv::line(image, cv::Point(x__, y__), cv::Point(x3, y3), cv::Scalar(0), armThickness, 8, 0);

			long x4 = (long)round(x3 + armLength*cos(deg2Radian(45)));
			long y4 = (long)round(y3 + armLength*sin(deg2Radian(45)));

			cv::line(image,cv:: Point(x2, y2), cv::Point(x4, y4), cv::Scalar(0),armThickness, 8, 0);
			cv::line(image, cv::Point(x3, y3), cv::Point(x4, y4), cv::Scalar(0), armThickness, 8, 0);
			//long point[2] = { x4,y4 };
			drawMesh(x4,y4, --n);
			break;
		}
		case 2: {

			//vertical case
			long x2 = (long)round(x__ + armLength*cos(deg2Radian(45)));
			long y2 = (long)round(y__ + armLength*sin(deg2Radian(45)));
			cv::line(image, cv::Point(x__, y__), cv::Point(x2, y2), cv::Scalar(0), armThickness, 8, 0);
			//cv::circle(image,cv::Point(x__,y__),2,cv::Scalar(0),-1,8,0);
			long x3 = (long)round(x__ + armLength*cos(deg2Radian(135)));
			long y3 = (long)round(y__ + armLength*sin(deg2Radian(135)));
			cv::line(image, cv::Point(x__, y__), cv::Point(x3, y3), cv::Scalar(0), armThickness, 8, 0);
			//circle(image, Point(x3, y3), 2, Scalar(0), -1, 8, 0);
			long x4 = (long)round(x3 + armLength*cos(deg2Radian(45)));
			long y4 = (long)round(y3 + armLength*sin(deg2Radian(45)));

			cv::line(image, cv::Point(x2, y2), cv::Point(x4, y4), cv::Scalar(0), armThickness, 8, 0);
			//circle(image, Point(x4, y4), 2, Scalar(0), -1, 8, 0);
			cv::line(image, cv::Point(x3, y3), cv::Point(x4, y4), cv::Scalar(0), armThickness, 8, 0);
			//long point[2] = { x4,y4 };
			drawMesh(x4,y4, --n);
			break;
		}
		case 3: {
			//inclined left case
		

			long x2 = (long)round(x__ + armLength*cos(deg2Radian((45 + angle))));
			long y2 = (long)round(y__ - armLength*sin(deg2Radian((45 + angle))));

			//circle(image,Point(x2,y2),2,Scalar(0),1,8,0);

			long x3 = (long)round(x__ + armLength*cos(deg2Radian((135 + angle)))); // -
			long y3 = (long)round(y__ + armLength*sin(deg2Radian((135 + angle))));

			//circle(image, Point(x3, y3), 5, Scalar(0), 1, 8, 0);
			long x4 = (long)round(x3 + armLength*cos(deg2Radian((45 + angle))));
			long y4 = (long)round(y3 - armLength*sin(deg2Radian((45 + angle))));
			//if (y4 <= pointLimit[1])
				//break;
			//else {
				cv::line(image, cv::Point(x__, y__), cv::Point(x2, y2), cv::Scalar(0), armThickness, 8, 0);
				cv::line(image, cv::Point(x__, y__), cv::Point(x3, y3), cv::Scalar(0), armThickness, 8, 0);
				cv::line(image, cv::Point(x2, y2), cv::Point(x4, y4), cv::Scalar(0), armThickness, 8, 0);
				circle(image, cv::Point(x4, y4), 5, cv::Scalar(0), -1, 8, 0);
				cv::line(image, cv::Point(x3, y3), cv::Point(x4, y4), cv::Scalar(0), armThickness, 8, 0);
				//long point[2] = { x4,y4 };
				drawMesh(x4,y4, --n);
			//}
			
			
			break;
		}
		case 4: {
			long x2 = (long)round(x__ + armLength*cos(deg2Radian((45 - angle))));
			long y2 = (long)round(y__ - armLength*sin(deg2Radian((45 - angle))));
			long x3 = (long)round(x__ - armLength*cos(deg2Radian((135 - angle))));
			long y3 = (long)round(y__ + armLength*sin(deg2Radian((135 - angle))));
			long x4 = (long)round(x3 + armLength*cos(deg2Radian((45 - angle))));
			long y4 = (long)round(y3 - armLength*sin(deg2Radian((45 - angle))));
			line(image, cv::Point(x__, y__), cv::Point(x2, y2), cv::Scalar(0), armThickness, 8, 0);
			line(image, cv::Point(x__, y__), cv::Point(x3, y3), cv::Scalar(0), armThickness, 8, 0);
			line(image, cv::Point(x2, y2), cv::Point(x4, y4), cv::Scalar(0), armThickness, 8, 0);
			//circle(image, Point(x4, y4), 5, Scalar(0), 1, 8, 0);
			line(image, cv::Point(x3, y3), cv::Point(x4, y4), cv::Scalar(0), armThickness, 8, 0);
			//long point[2] = { x4,y4 };
			drawMesh(x4,y4, --n);
			break;
		}
		default:
			break;
		}
	}


}
double DrawPCB::deg2Radian(double degree) {
	return (3.14159265*degree / 180);
}

int DrawPCB::checkLineAllignment(long point1[],long point2[]){
	// horizontal case
	if (point1[1] == point2[1]) {
		this->angle = 0.0;
		return 1;
	}
	//vertical case
	else if (point1[0] == point2[0]) {
		this->angle = 0.0;
		return 2;
	}
	//tilted 
	else {
		
		this->angle= atan((double)(point1[1] - point2[1]) / (point1[0] - point2[0])) * 180 / (3.14159265);
		//tileted left, slope positive
		if (this->angle > 0)
			return 3;
		//tilted right, slope negative
		else if (this->angle < 0)
			return 4;
		else
			return 0;
	}
		
}
