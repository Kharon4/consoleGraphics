#pragma once

#include <Windows.h>
#include <vector>

namespace color {
	enum class baseColors {
		black	= 0,
		dBlue	= 1, 
		dGreen	= 2, 
		dAqua	= 3, 
		dRed	= 4, 
		dPink	= 5, 
		dYellow = 6, 
		lGrey	= 7, 
		dGrey	= 8, 
		lBlue	= 9, 
		lGreen	= 10, 
		lAqua	= 11, 
		lRed	= 12, 
		lPink	= 13, 
		lYellow = 14, 
		white	= 15
	};

	//base color functions

	baseColors intensify(baseColors a);
	baseColors darken(baseColors a);

	unsigned char getCol(baseColors background, baseColors foreground);

	CHAR_INFO interpolate(baseColors a, baseColors b, float ratio);

	//CHAR_INFO functioons
	baseColors getBackground(CHAR_INFO a);
	baseColors getForeground(CHAR_INFO a);
	baseColors getDominant(CHAR_INFO a);

	CHAR_INFO interpolate(CHAR_INFO a, CHAR_INFO b, float ratio);
	CHAR_INFO interpolate(std::vector<CHAR_INFO> list, float ratio);

	CHAR_INFO getColor(float r, float g, float b);
	CHAR_INFO getColorWithIntensity(float r, float g, float b, bool addWhite = true);
}