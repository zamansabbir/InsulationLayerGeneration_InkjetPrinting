#pragma once
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "ReadGerber.h"
class DrawPCB{
public:
	DrawPCB(std::string directory);
	bool draw();
	void saveImage();
	double currentX_1 = 0.0, currentY_1 = 0.0;
	double *XY ;
	double *currentXY_1;

	bool interpolationTypeLinear = true;
	ReadGerber* read;
	std::string fileDirectory;
	float* size=new float();
private: 
	/**
		draw a line on image array using bresenham algorithm, 
		@param point1[]= from (x,y) point2[]=To (x,y), thickness. thickness should be converted to pixels prior to calling this function

	*/
	void drawTheLine(long point1[], long point2[],int thickness);

	/**
	draw a circle on cv::InputOutput image array 
	@param center[]= (x,y) ,radius. radius should be converted to pixels prior to calling this function

	*/
	void drawTheCircle(long center[],long radius);

	/**
	draw a filled circle  on cv::InputOutput image array
	@param center[]= (x,y) ,radius. radius should be converted to pixels prior to calling this function

	*/
	void fillTheCircle(long center[], long radius);

	/**
	draw a rectangle on cv::InputOutput image array
	@param point1[]= (x,y): Top Left Corner ,point2[]=(x,y): Bottom Right Corner.

	*/
	void drawTheRectangle(long point1[], long point2[]);

	/**
	draw a filled rectangle on cv::InputOutput image array
	@param point1[]= (x,y): Top Left Corner ,point2[]=(x,y): Bottom Right Corner.

	*/
	void fillTheRectangle(long point1[],long point2[]);
	

	/**
		perform instruction directed by D01, follows the interpolation.
	*/
	void performD01Solid();
	
	/**
		perform diamond shape look alike mesh instead of solid trace
	*/
	void performD01Mesh();

	/**
	perform instruction directed by D02, follows the interpolation. actually It does nothing
	*/
	void performD02();

	/**
	perform instruction directed by D03, follows the interpolation type. Draws pads and vias on the cv::InputOutput Image array 
	*/
	void performD03();

	/**
		stores the apertureSize to float type dynamic memory(pointed by *size), If apertureType='C', stores *(size+0)=diameter, *(size+1)=0.0
		if apertureType='R', stores *(size+0)=X Size, *(size+1)=Y Size; if apertureType='o', stores *(size+0)=X Size; *(size+1)=Y Size
		@param aName="apertureName"
		@returen 1 if apertureType='C', 2 if apertureType='R', 3 if apertureType='o' and 0 if no apertureType matched, return is done as char 
		to utilize 8 bit memory only, typecast to int type is required for mathematical operation
	*/
	char setApertureSizeToMemory(std::string aName); 

	/**
	finds the fileName with extension from the directory string using member this->directory
	@return the file name as string
	*/

	std::string findFileName();

	/**
	finds the file Directory or the location from the directory string using member this->directory
	@return the file location as string
	*/

	std::string findFilePath();
	
	/**
		internal use only
	*/
	void drawMesh(long x__,long y__, int n);
	double deg2Radian(double degree);
	/**
		check if the line is vertical, horizontla or tilted, sets the angle if tilted to left or right, angle is then used inside drawMesh method for case 3 and 4 as directionChoice;
		@param: point1[2]=starting point point2[2]=ending point of the line
		@return 1 if horizontal, 2 if vertical, 3 if tilted left(angled with X axis greater than +90 degree), 4 if tilted right(angled with X axis less than +90 degree) 
	*/
	int checkLineAllignment(long point1[],long point2[]);
	cv::Mat image;
	double height;
	double x, y,angle, hypotenuse;
	bool solidTrace, clockWise;
	int armThickness,directionChoice,armLength;
};