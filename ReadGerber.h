#pragma once
/**
	 header file:ReadGerber.h with implementation file =ReadGerber.cpp
	 Purpose: process the gerber file to create image from the gerber instructions
	 @author SBZ
	 @version 4.0 08/26/2016
*/

#include<iostream>
#include<vector>
class ReadGerber {
public:
	ReadGerber(std::string directory);
	/**
		checks for instruction command D01,D02 and D03.
		@param takes a string (the currentLine of the Open file).
		@return 1 for D01, 2 for D02,3 for D03, 0 for no instruction like D01,D02 or D03.
	*/
	int checkInstruction(std::string line);
	/**
		finds current X and Y coordinates described in gerber file.
		@param line=current line of the file, currentX_1=last X coordinate, currentY_1=last Y coordinate .
		@return XY as reference, X=*(XY+0), Y=*(XY+1).
	*/
	double* findXY(std::string line, double currentX_1, double currentY_1);

	/**
		converts cartesian coordinate (X/Y)
		@param value=X coordinate or Y coordinate
		@return Integer pixel value, given dpi=1693;
	*/
	int convertToPixel(double value);

	/**
		converts cartesian coordinate (X/Y).
		@param value=X coordinate or Y coordinate, dpi= any Integer for custom dpi setting.
		@return Integer pixel value.
	*/
	int convertToPixel(double value, int dpi);


	/**
		getters for the width of image, calculated from gerber file. It is normally used for image size optimization.
	*/
	double getDimensionWidth();

	/**
		getters for the height of image, calculated from gerber file. It is normally used for image size optimization.
	*/
	double getDimensionHeight();

	/**
		shifts XY coordinates to a specific origin initialiazed in X0 and Y0 in the constructor
		@param x=X coordinate and y=Y coordinate returned from findXY() method
		@return shifted XY as reference, X=*(XY+0), Y=*(XY+1)
	*/
	double* shiftXYToOrigin(double x, double y);


	void setCurrentAperture(std::string aperture);
	std::string getCurrentAperture();
	std::vector<std::string> apertureName;
	std::vector<char> apertureType;
	std::vector<std::string> apertureSize;

private:
	/**
		Sets format described in the file for gerber processing, used internally by the constructor itself.
		@param directory=gerber file directory.

	*/
	void setFormatSpecication(std::string directory);
	/**
		converts the string like X coordinate to it corresponding decimal following the instructions:set by setFormatSepciation method: found in gerber file  
		used internally inside findXY(line, currentX_1, currentY_1).
		@param takes the Y coordinate as std::string.
	*/
	double convertToDecimalX(std::string x);

	/**
	converts the string like Y coordinate to it corresponding decimal following the instructions:set by setFormatSepciation method: found in gerber file
	used internally inside findXY(line, currentX_1, currentY_1).
	@param takes the Y coordinate as std::string.
	*/

	double convertToDecimalY(std::string y);

	/**
		sets the dimension of the image height and width, used internally by the constructor itself.
	*/
	void setDimension();

	/**
		stores aperture information in different vector<std::string> apertureName, vector<std::string> apertureType, vector<std::string> apertureSize
		@return (bool)true if storage is successful, (bool)false if fail  
	*/
	bool storeAperture();

	int leadingZero = 0, coordinateSystem = 0, decimalXNumberOfDigits = 0, decimalYNumberOfDigits = 0, integerXNumberOfDigits = 0, integerYNumberOfDigits = 0, unitInch = 0;
	std::string directory;
	double width, height,X0,Y0,Xmin,Ymin;
	std::string currentAperture;
};