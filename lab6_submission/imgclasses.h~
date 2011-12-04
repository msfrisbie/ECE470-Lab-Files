/*
	Copyright (C) 2011 by Matt Frisbie

	This file is part of an implementation for ECE470 Lab 5&6 at the University of Illinois Champaign-Urbana.

	This implementation is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This implementation is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this implementation.  If not, see <http://www.gnu.org/licenses/>.
*/

// imgclasses.h

#ifndef IMGCLASSES_H
#define IMGCLASSES_H

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>

#include "labs.h"
#include "console.h"
#include "lablib.h"
#include "imgclasses.h"
#include "blockclass.h"

//using namespace std;

class image {
	public:

	int height, width;
	std::vector< std::vector<int> > img_array;
	std::vector<block*> block_array;
	QRgb *pfirstassociatedrgb;

	image(QImage *threshed, QImage *associated);
	~image();

	void calculateCentroid(int tag);
	void writeToFile();
	void writeToTerminal();
	void findAndTagBlocks();
	void tagBlock(int x,int y,int tag, block *b);
	void setCentroid(block *b);
	void filterImage();
	bool checkThreshTag(int tag);
	void drawGrid();
	void drawPixel(int x, int y);
	void drawBlackWhite();
	void drawCross(int x, int y);
	void printBlocksToConsole();
	void drawNumber(int x, int y, int num);

};


#endif

