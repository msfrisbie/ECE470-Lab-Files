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

// lablib.cpp

#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "lablib.h"
#include "imgclasses.h"

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
#define LIGHTING_OFFSET 0
#define NUM_OBJ_OFFSET 0



// used to force float operations to conserve float parameters
float f_div(float a, float b) {
   return (float)( (float)a / (float)b );
}

float f_mul(float a, float b) {
   return (float)( (float)a * (float)b );
}

float f_add(float a, float b) {
   return (float)( (float)a + (float)b );
}

float f_sub(float a, float b) {
   return (float)( (float)a - (float)b );
}

// corrects for division by zero errors
float fixEdge(float a) {
   if( a == (float)1 )
      a = (float)0.999999;
   if( a == (float)0 )
      a = (float)0.000001;
   return a;
}


//returns size of all neighbors to exhaustion
void objSize(int x, int y, int cval, int &size, int &xmax, int &xmin, int &ymax, int &ymin, int **image) {
	if(size == STACK_OVERFLOW_CAP) {
		cout << "Stack Overflow!!! Remove large objects from the image." << endl;
		return;
	}
	// check if at edge, if so, return 0;
	if(x < 0 || y < 0 || x > IMG_HEIGHT || y > IMG_WIDTH)
		return;


	// if the pixel being examined is white, return size 0
	if(image[x][y] == -1)
		return;
	// if it's black and unexamined yet, set it and recurse
	if(image[x][y] == 0) {
		image[x][y] = cval;
		size++;
		
		// checks if pixel is at object edge extremity
		if(x > xmax)
			xmax = x;
		if(y > ymax)
			ymax = y;
		if(x < xmin)
			xmin = x;
		if(y < ymin)
			ymin = y;

		// return 1 if everything around it has been counted or is white
		if(image[x+1][y] == 0)
			objSize(x+1,y,cval,size, xmax, xmin, ymax, ymin, image);
		if(image[x][y+1] == 0)
			objSize(x,y+1,cval,size, xmax, xmin, ymax, ymin, image);
		if(image[x-1][y] == 0)
			objSize(x-1,y,cval,size, xmax, xmin, ymax, ymin, image);
		if(image[x][y-1] == 0)
			objSize(x,y-1,cval,size, xmax, xmin, ymax, ymin, image);
		//return objSize(x+1,y,cval) + objSize(x,y+1,cval) + objSize(x-1,y,cval) + objSize(x,y-1,cval) + 1;
	}
	// else the pixel has already been visited and counted, return 0;
	else
		return;

	
	//console_printf("value: %d", image[5][5]);
	//x = image[0][0];
	//y = 0;
	//cval = 0;
	//return 0;
}

int getZval(int height, int width, int **img) {
	return 0;
}

int writeToFile(int height, int width, int **img) {

	ofstream myfile;
	myfile.open("out.txt");

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			//cout << (char)img[i][j];
			myfile << (char)img[i][j];
		}
		//cout << endl;
		myfile << "\n";
	}

	myfile.close();

	return 0;
}


void tagObjects(int height, int width, int **img) {
	
}

void echo(string h) {
	cout << h << endl;
}

void echo(char h) {
	cout << h << endl;
}

void echo(int h) {
	cout << h << endl;
}














