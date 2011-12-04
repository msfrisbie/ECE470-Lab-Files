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

	A derivation of this code was submitted for a class grade in the Fall 2011 ECE470 course.  
	As such, it is expected that the school retains a repository of all submitted code, and 
	actively checks against them in an effort to deter cheating by means of copying code 
	from other students.  

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

#include "lablib.h"
#include "imgclasses.h"
#include "blockclass.h"

using namespace std;


// classes

image::image() {

	string line;

	ifstream myfile("image.txt");

	while(!myfile.is_open()) {}

	getline(myfile,line);
	width = atoi(line.c_str());

	getline(myfile,line);
	height = atoi(line.c_str());

	img_array.resize(height);
	for(int i = 0; i < height; i++) {
		img_array[i].resize(width);
	}

	for(int j = 0; j < height; j++) {

		getline(myfile,line);

		for(int i = 0; i < width; i++) {

			if(line[i] == '1') {
				img_array[j][i] = '1';
			}
			else {
				img_array[j][i] = '0';
			}
		}

	}

	myfile.close();

	this->findAndTagBlocks();
}


void image::findAndTagBlocks() {
	int xmin, xmax,ymin,ymax,size;
	int tag = 64;
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			// found new image
			if(img_array[j][i] == '1') {
				size = 0;
				xmax = 0;
				xmin = width;
				ymax = 0;
				ymin = height;
				tag++;
				
				block *newblock = new block;
				block_array.push_back(newblock);

				tagBlock(j,i,tag,size,xmax,xmin,ymax,ymin);
				//cout << "object " << tag << ":" << endl;
				//echo(size);
				//echo(xmin);
				//echo(xmax);
				//echo(ymin);
				//echo(ymax);	
				newblock->block_size = size;		
				newblock->block_xmin = xmin;	
				newblock->block_xmax = xmax;	
				newblock->block_ymin = ymin;	
				newblock->block_ymax = ymax;	
				newblock->block_tag = tag;		// this is the 'primary key'!

				setCentroid(newblock);
			}
		}
	}
}

void image::tagBlock(int x,int y,int tag, int &size, int &xmax, int &xmin, int &ymax, int &ymin) {

	if((x == -1) || (y == -1) || (y == (width)) || (x == (height)))
		return;

	if(img_array[x][y] == '0')
		return;

	if(img_array[x][y] == '1') {
		size++;
		img_array[x][y] = tag;

		if(y<xmin)
			xmin = y;
		else if(y>xmax)
			xmax = y;
		if(x<ymin)
			ymin = x;
		else if(x>ymax)
			ymax = x;

		if(x<height-1)
			tagBlock(x+1,y,tag,size,xmax,xmin,ymax,ymin);
		if(y<width-1)
			tagBlock(x,y+1,tag,size,xmax,xmin,ymax,ymin);
		if(x>0)
			tagBlock(x-1,y,tag,size,xmax,xmin,ymax,ymin);
		if(y>0)
			tagBlock(x,y-1,tag,size,xmax,xmin,ymax,ymin);
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
			if(img_array[i][j]!='0')
				myfile << img_array[i][j];
			else
				myfile << '-';
		}
		myfile << "\n";
	}

	myfile.close();
}

void image::writeToTerminal() {

	//char buf;

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			
			//buf = (char)img_array[i][j];

			if(img_array[i][j]!='0')
				cout << img_array[i][j];
			else
				cout << '-';
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
	//cout << xsum << ": ";
	b->centroid_x = (int)f_div(ysum,(b->block_size));
	b->centroid_y = (int)f_div(xsum,(b->block_size));
	img_array[b->centroid_y][b->centroid_x] = '*';


}

void image::printBlockInfo() {

	for(int i = 0; i < (int)this->block_array.size(); i++) {
		cout << "Block " << (char)block_array[i]->block_tag << ": "<< endl;
		cout << "   Block size: " << block_array[i]->block_size << endl;
		cout << "   X(min,max): " << block_array[i]->block_xmin << "," << block_array[i]->block_xmax << endl;
		cout << "   Y(min.max): " << block_array[i]->block_ymin << "," << block_array[i]->block_ymax << endl;
		cout << "   Centroid X: " << block_array[i]->centroid_x << endl;
		cout << "   Centroid Y: " << block_array[i]->centroid_y << endl;
	}

}

image::~image() {
	img_array.clear();
	block_array.clear();
}






