#include "consoleColor.h"

namespace color {
	//interpolation arrays
	const int sizeOfInterpolationArray = 8;
	const char interpolationCharArr[sizeOfInterpolationArray] = { ' ' , 176 , 177 , 178 , 178 , 177 , 176 , ' ' };
	const char interpolationColArr[sizeOfInterpolationArray] = { 0  , 0   , 0   , 1   , 0   , 1   , 1   , 1 };

	//base color functions
	baseColors intensify(baseColors a) {
		return (baseColors)((unsigned char)a + 8);
	}

	baseColors darken(baseColors a) {
		return (baseColors)((unsigned char)a - 8);
	}

	unsigned char getCol(baseColors background, baseColors foreground) {
		return ((unsigned char)background * 16 + (unsigned char)foreground);
	}

	CHAR_INFO interpolate(baseColors a, baseColors b, float ratio) {
		baseColors arr[2] = { a,b };
		int index = ratio * sizeOfInterpolationArray;
		if (index >= sizeOfInterpolationArray)index = sizeOfInterpolationArray - 1;
		CHAR_INFO rval;
		rval.Char.AsciiChar = interpolationCharArr[index];
		rval.Attributes = getCol(arr[interpolationColArr[index]], arr[1 - interpolationColArr[index]]);
		return rval;
	}

	//CHAR_INFO functioons
	baseColors getBackground(CHAR_INFO a) {
		return (baseColors)(a.Attributes / 16);
	}

	baseColors getForeground(CHAR_INFO a) {
		return (baseColors)(a.Attributes - (unsigned char)getBackground(a) * 16);
	}

	baseColors getDominant(CHAR_INFO a) {
		if (a.Char.AsciiChar == (char)178)return getForeground(a);
		else return getBackground(a);
	}

	CHAR_INFO interpolate(CHAR_INFO a, CHAR_INFO b, float ratio) {
		int index = ratio * sizeOfInterpolationArray;
		if (index == 0)return a;
		else if (index >= sizeOfInterpolationArray - 1)return b;
		else if ((int)getDominant(a) == (int)getDominant(b)) {
			if (ratio < 0.5)return a;
			else return b;
		}
		else return interpolate(getDominant(a), getDominant(b), ratio);
	}

	CHAR_INFO interpolate(std::vector<CHAR_INFO> list, float ratio) {
		CHAR_INFO errChar;
		errChar.Attributes = getCol(baseColors::lPink, baseColors::dBlue);
		errChar.Char.AsciiChar = '#';
		if (list.size() == 1)return list[0];
		if (list.size() == 0)return errChar;
		int interval = ratio * (list.size() - 1);
		if (interval == list.size() - 1)return list[interval];
		float sizeOfinterval = 1.0f / (list.size() - 1);
		float newRatio = (ratio - interval * sizeOfinterval) / sizeOfinterval;
		return interpolate(list[interval], list[interval + 1], newRatio);
	}

	CHAR_INFO getColor(float r, float g, float b) {
		float min = 1;
		//get min
		if (r < min)min = r;
		if (g < min)min = g;
		if (b < min)min = b;
		float w = min;//greyscale
		r -= min;
		g -= min;
		b -= min;
		min = 1;
		baseColors primaryCol = baseColors::black;
		baseColors secondaryCol = baseColors::black;
		float primaryColVal = 0, secondaryColVal = 0;
		if (r == 0) {
			secondaryCol = baseColors::dAqua;
			if (g < min)min = g;
			if (b < min)min = b;
			g -= min;
			b -= min;
			secondaryColVal = min;
			if (g == 0) {
				primaryCol = baseColors::dBlue;
				primaryColVal = b;
			}
			else {
				primaryCol = baseColors::dGreen;
				primaryColVal = g;
			}
		}
		else if (g == 0) {
			secondaryCol = baseColors::dPink;
			if (r < min)min = r;
			if (b < min)min = b;
			r -= min;
			b -= min;
			secondaryColVal = min;
			if (r == 0) {
				primaryCol = baseColors::dBlue;
				primaryColVal = b;
			}
			else {
				primaryCol = baseColors::dRed;
				primaryColVal = r;
			}
		}
		else {
			secondaryCol = baseColors::dYellow;
			if (r < min)min = r;
			if (g < min)min = g;
			r -= min;
			g -= min;
			secondaryColVal = min;
			if (r == 0) {
				primaryCol = baseColors::dGreen;
				primaryColVal = g;
			}
			else {
				primaryCol = baseColors::dRed;
				primaryColVal = r;
			}
		}

		CHAR_INFO primaryColChar;
		CHAR_INFO secondaryColChar;
		CHAR_INFO whiteColChar;

		std::vector<CHAR_INFO> list;
		CHAR_INFO temp;
		temp.Char.AsciiChar = ' ';

		temp.Attributes = getCol(baseColors::black, baseColors::black);
		list.push_back(temp);
		temp.Attributes = getCol(primaryCol, baseColors::black);
		list.push_back(temp);
		temp.Attributes = getCol(intensify(primaryCol), baseColors::black);
		list.push_back(temp);
		primaryColChar = interpolate(list, primaryColVal);
		temp.Attributes = getCol(secondaryCol, baseColors::black);
		list[1] = temp;
		temp.Attributes = getCol(intensify(secondaryCol), baseColors::black);
		list[2] = temp;
		secondaryColChar = interpolate(list, secondaryColVal);
		temp.Attributes = getCol(baseColors::dGrey, baseColors::black);
		list[1] = temp;
		temp.Attributes = getCol(baseColors::lGrey, baseColors::black);
		list[2] = temp;
		temp.Attributes = getCol(baseColors::white, baseColors::black);
		list.push_back(temp);
		whiteColChar = interpolate(list, w);
		CHAR_INFO rval;
		rval.Attributes = 0;
		rval.Char.AsciiChar = ' ';
		if (primaryColVal + secondaryColVal != 0) {
			rval = interpolate(primaryColChar, secondaryColChar, secondaryColVal / (primaryColVal + secondaryColVal));

		}
		if (primaryColVal + secondaryColVal + w != 0) {
			rval = interpolate(rval, whiteColChar, w / (primaryColVal + secondaryColVal + w));
		}
		else {
			rval.Attributes = 0;
		}
		return rval;
	}
	
	CHAR_INFO getColorWithIntensity(float r, float g, float b, bool addWhite) {
		int max = 1;
		float ratio = 1;
		if (r > 1 || g > 1 || b > 1) {
			max = r;
			if (g > max)max = g;
			if (b > max)max = b;

			float sq = r * r + g * g + b * b;
			sq = sqrt(sq);
			ratio = 1 / float(max);
			if (!addWhite) { sq = max; ratio = 1; }
			r /= sq;
			g /= sq;
			b /= sq;
		}
		CHAR_INFO white;
		white.Char.AsciiChar = ' ';
		white.Attributes = 15 * 16;
		return(interpolate(white, getColor(r, g, b), ratio * 10));
	}

}