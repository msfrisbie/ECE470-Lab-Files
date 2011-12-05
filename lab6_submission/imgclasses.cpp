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


	DISCLAIMER FOR UIUC STUDENTS: 

	This code was submitted for a class grade in the Fall 2011 ECE470 course.  As such,
	it is expected that the school retains a repository of all submitted code, including
	these files, and actively checks against them in an effort to deter cheating by means
	of copying code from other students.  

	It is for your benefit to derive your own solution for the lab.  This solution is 
	publicly available in order to allow you to gain insight into alternate strategies
	surrounding the labs for this class.  I am in no way advocating using any part of this
	solution in your own implementation.  Should you decide to copy code from this solution, 
	I am in no way responsible for any consequences that may result from you being caught.  

	The student code regarding academic integrity can be found on the school website.  
*/

// imgclasses.cpp

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

using namespace std;

#define NUM_VALUES 256
#define IMG_HEIGHT 240
#define IMG_WIDTH 320
#define STACK_OVERFLOW_CAP 25000
#define BACKGROUND_NOISE_THRESH 200
// somehow, blocking out gargantuan objects crashed the webcam...
#define GARGANTUAN_OBJECT_THRESH 1500
// accounts for shadows from ambient sunlight
// 0 at dusk
// -6 during the day
#define LIGHTING_OFFSET -2
#define NUM_OBJ_OFFSET 0

int one_pixels[9][7] = {{0,0,0,1,1,0,0},
						{0,0,1,1,1,0,0},
						{0,0,1,1,1,0,0},
						{0,0,0,1,1,0,0},
						{0,0,0,1,1,0,0},
						{0,0,0,1,1,0,0},
						{0,0,0,1,1,0,0},
						{0,0,1,1,1,1,0},
						{0,0,1,1,1,1,0}};

int two_pixels[9][7] = {{0,1,1,1,1,0,0},
						{1,1,1,1,1,1,0},
						{1,0,0,0,1,1,0},
						{0,0,0,0,1,1,0},
						{0,0,0,1,1,0,0},
						{0,1,1,1,0,0,0},
						{1,1,1,0,0,0,0},
						{1,1,1,1,1,1,0},
						{1,1,1,1,1,1,0}};

int three_pixels[9][7]={{0,1,1,1,1,0,0},
						{1,1,1,1,1,1,0},
						{1,1,0,0,1,1,0},
						{0,0,0,0,1,1,0},
						{0,0,0,1,1,1,0},
						{0,0,0,0,1,1,0},
						{1,1,0,0,1,1,0},
						{1,1,1,1,1,1,0},
						{0,1,1,1,1,0,0}};

int four_pixels[9][7] ={{0,0,0,1,1,1,0},
						{0,0,1,1,1,1,0},
						{0,1,1,0,1,1,0},
						{1,1,0,0,1,1,0},
						{1,1,1,1,1,1,0},
						{1,1,1,1,1,1,0},
						{0,0,0,0,1,1,0},
						{0,0,0,1,1,1,1},
						{0,0,0,1,1,1,1}};

int five_pixels[9][7] ={{1,1,1,1,1,1,0},
						{1,1,1,1,1,1,0},
						{1,1,0,0,0,0,0},
						{1,1,1,1,1,0,0},
						{1,1,1,1,1,1,0},
						{0,0,0,0,1,1,0},
						{1,1,0,0,1,1,0},
						{1,1,1,1,1,1,0},
						{0,1,1,1,1,0,0}};

int six_pixels[9][7] = {{0,1,1,1,1,1,0},
						{1,1,1,1,1,1,0},
						{1,1,0,0,0,0,0},
						{1,1,1,1,1,0,0},
						{1,1,1,1,1,1,0},
						{1,1,0,0,1,1,0},
						{1,1,0,0,1,1,0},
						{1,1,1,1,1,1,0},
						{0,1,1,1,1,0,0}};

int seven_pixels[9][7]={{1,1,1,1,1,1,1},
						{1,1,1,1,1,1,1},
						{0,0,0,0,1,1,0},
						{0,0,0,1,1,0,0},
						{0,0,1,1,0,0,0},
						{0,0,1,1,0,0,0},
						{0,0,1,1,0,0,0},
						{0,0,1,1,0,0,0},
						{0,0,1,1,0,0,0}};

int eight_pixels[9][7]={{0,1,1,1,1,0,0},
						{1,1,1,1,1,1,0},
						{1,1,0,0,1,1,0},
						{1,1,0,0,1,1,0},
						{0,1,1,1,1,0,0},
						{1,1,0,0,1,1,0},
						{1,1,0,0,1,1,0},
						{1,1,1,1,1,1,0},
						{0,1,1,1,1,0,0}};

int nine_pixels[9][7]= {{0,1,1,1,1,1,0},
						{1,1,1,1,1,1,0},
						{1,1,0,0,1,1,0},
						{1,1,0,0,1,1,0},
						{1,1,1,1,1,1,0},
						{0,1,1,1,1,1,0},
						{0,0,0,0,1,1,0},
						{0,0,0,0,1,1,0},
						{0,0,0,0,1,1,0}};

image::image(QImage *threshed, QImage *associated) {

	int red;

	this->height = threshed->height();
	this->width = threshed->width();
	QRgb *p_first_thresh_rgb = (QRgb*)threshed->bits();
	this->pfirstassociatedrgb = (QRgb*)associated->bits();

	img_array.resize(height);
	for(int i = 0; i < height; i++) {
		img_array[i].resize(width);
	}

	for(int row=0; row<height; row++)
	{
		for(int col=0; col<width; col++)
		{
			red = qRed(*(p_first_thresh_rgb+row*width+col));			// note: r=g=b= {255 or 0}
			
			if (red==0x00 && row != 0) {
				img_array[height-row-1][width-col-1] = 1;
			}
			else {
				img_array[height-row-1][width-col-1] = 0;
			}
		}
	}

	this->findAndTagBlocks();

	this->filterImage();

	this->drawBlackWhite();
}


void image::findAndTagBlocks() {
	
	int tag = 1;
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			// found new image
			if(img_array[j][i] == 1) {

				tag++;
				
				block *newblock = new block(height,width);

				tagBlock(j,i,tag,newblock);

				newblock->block_tag = tag;		// this is the 'primary key'!

				if(newblock->block_size > BACKGROUND_NOISE_THRESH) {
					block_array.push_back(newblock);
					this->setCentroid(newblock);
				}
				else {
					tagBlock(j,i,0,newblock);	
					delete newblock;
					//tag--;
				}
			}
		}
	}
}

void image::tagBlock(int x,int y,int tag, block* b) {

	if((x == -1) || (y == -1) || (y == (width)) || (x == (height)))
		return;

	if(img_array[x][y] == 0)
		return;

	if(img_array[x][y] == 1) {

		b->block_size++;

		img_array[x][y] = tag;

		if(y<b->block_xmin)
			b->block_xmin = y;
		else if(y>b->block_xmax)
			b->block_xmax = y;
		if(x<b->block_ymin)
			b->block_ymin = x;
		else if(x>b->block_ymax)
			b->block_ymax = x;

		if(x<height-1)
			tagBlock(x+1,y,tag,b);
		if(y<width-1)
			tagBlock(x,y+1,tag,b);
		if(x>0)
			tagBlock(x-1,y,tag,b);
		if(y>0)
			tagBlock(x,y-1,tag,b);
	}
	else {
		return;
	}
	
}

void image::writeToFile() {

	ofstream myfile;
	myfile.open("out.txt");

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			myfile << img_array[i][j];
		}
		myfile << "\n";
	}

	myfile.close();
}

void image::writeToTerminal() {

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			cout << img_array[i][j];
		}
		cout << endl;
	}
}

void image::setCentroid(block *b) {
	
	int xsum = 0;
	int ysum = 0;
	for(int j=b->block_ymin;j<b->block_ymax+1;j++) {
		for(int i=b->block_xmin;i<b->block_xmax+1;i++) {
			if(img_array[j][i] == b->block_tag)
				xsum += j;
		}
	}
	for(int j=b->block_xmin;j<b->block_xmax+1;j++) {
		for(int i=b->block_ymin;i<b->block_ymax+1;i++) {
			if(img_array[i][j] == b->block_tag)
				ysum += j;
		}
	}

	b->centroid_x = (int)f_div(ysum,(b->block_size));
	b->centroid_y = (int)f_div(xsum,(b->block_size));

	b->setWorldCoordinates();

}

void image::drawGrid() {

	for(int row=0; row<height; row++)
	{
		for(int col=0; col<width; col++)
		{
			if(img_array[row][col] == 0) {
				if(row%10+10 == 10 || col%10+10 == 10 || row==0 || col==0 || row==IMG_HEIGHT-1 || col==IMG_WIDTH-1) {
					*(pfirstassociatedrgb+row*width+col)=qRgb(50,50,50);
				}
				if(row%50 == 0 || col%50 == 0 || row==0 || col==0 || row==IMG_HEIGHT-1 || col==IMG_WIDTH-1) {
					*(pfirstassociatedrgb+row*width+col)=qRgb(100,100,100);
				}
			}
		}
	}
}

void image::drawPixel(int col, int row) {

	*(pfirstassociatedrgb+row*width+col)=qRgb(255,0,0);
	
}

void image::drawBlackWhite() {

	for(int row=0; row<height; row++)
	{
		for(int col=0; col<width; col++)
		{
			if(img_array[row][col] == 0) {
				*(pfirstassociatedrgb+row*width+col)=qRgb(0,0,0);
			}
			else {
				*(pfirstassociatedrgb+row*width+col)=qRgb(255,255,255);
			}
		}
	}
}


void image::filterImage() {

	for(int row=0; row<height; row++)
	{
		for(int col=0; col<width; col++)
		{
			if(!this->checkThreshTag(img_array[row][col])) {
				img_array[row][col] = 0;
			}
		}
	}
}

bool image::checkThreshTag(int tag) {
	for(int i = 0; i < (int)(block_array.size()); i++) {
		if(block_array[i]->block_tag == tag) {
			return true;
		}
	}
	return false;
}

void image::drawCross(int x, int y) {
	
	if(x<4 || x > IMG_WIDTH-4 || y<4 || y > IMG_HEIGHT-4)
		return;

	for(int i = -3; i < 4; i++) {
		drawPixel(i+x,y);
	}
	for(int j = -3; j < 4; j++) {
		drawPixel(x,j+y);
	}

}

void image::drawNumber(int x, int y, int num) {

	if(x<5 || x > IMG_WIDTH-5 || y<5 || y > IMG_HEIGHT-5)
		return;

	for(int j = 0; j < 9; j++) {
		for(int i = 0; i < 7; i++) {
			switch(num){
				case(1):
					if(one_pixels[j][i]==1) {
						drawPixel(x+i-3,j+y-4);
					}
					break;
				case(2):
					if(two_pixels[j][i]==1) {
						drawPixel(x+i-3,j+y-4);
					}
					break;
				case(3):
					if(three_pixels[j][i]==1) {
						drawPixel(x+i-3,j+y-4);
					}
					break;
				case(4):
					if(four_pixels[j][i]==1) {
						drawPixel(x+i-3,j+y-4);
					}
					break;
				case(5):
					if(five_pixels[j][i]==1) {
						drawPixel(x+i-3,j+y-4);
					}
					break;
				case(6):
					if(six_pixels[j][i]==1) {
						drawPixel(x+i-3,j+y-4);
					}
					break;
				case(7):
					if(seven_pixels[j][i]==1) {
						drawPixel(x+i-3,j+y-4);
					}
					break;
				case(8):
					if(eight_pixels[j][i]==1) {
						drawPixel(x+i-3,j+y-4);
					}
					break;
				case(9):
					if(nine_pixels[j][i]==1) {
						drawPixel(x+i-3,j+y-4);
					}
					break;
				default:
					drawCross(x,y);
					break;
			}
		}
	}

}

// print blocks to console
void image::printBlocksToConsole() {

	for(int i = 0; i < (int)this->block_array.size(); i++) {
		console_printf("Block  #%d",i+1);
		this->block_array[i]->printWorldCoordinates();
		this->drawNumber((this->block_array[i]->centroid_x),(this->block_array[i]->centroid_y),i+1);
	}

}

image::~image() {
	img_array.clear();
	block_array.clear();
}






