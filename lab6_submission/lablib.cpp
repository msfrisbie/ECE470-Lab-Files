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

// lablib.cpp

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>

#include "rhino.h"
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

#define sin_t -0.0995
#define n_sin_t 0.0995
#define cos_t 0.995
#define n_cos_t -0.995

#define beta 6.6000

#define x_cm_offset 3.4
#define y_cm_offset -9.55

/////////////// FROM LAB 4 /////////////////////////////////
#define PI 3.14159265
#define i1058 (float)1058
#define i23 (float)23
#define i16 (float)16
#define i2  (float)2
#define in2 (float)-2
#define in1 (float)-1

#define f_ratio (float)4.333333333
#define e_ratio (float)-8.666666666
#define d_ratio (float)8.333333333
#define c_ratio (float)8.888888888
#define b_ratio (float)3.111111111

//                F E D C B
float theta[5] = {0,0,0,0,0};
int enc[5] =     {0,0,0,0,0};
float IK[5] =    {0,0,0,0,0};
char encoder_name[5] = {'F','E','D','C','B'};
bool offset = true;

// merely to assuage any zero-based confusion
float* t1 = &theta[0];
float* t2 = &theta[1];
float* t3 = &theta[2];
float* t4 = &theta[3];
float* t5 = &theta[4];

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
	}
	// else the pixel has already been visited and counted, return 0;
	else {
		return;
	}

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

void echo(string h) {
	cout << h << endl;
}

void echo(char h) {
	cout << h << endl;
}

void echo(int h) {
	cout << h << endl;
}

int lab_movex(float xworld, float yworld, float zworld, float pitch, float roll)
// Computes of the the elbow-up inverse kinematics solutions
{
	console_printf("here!");

	double x_grip,y_grip,z_grip,w_grip,w_wrist,z_wrist,theta5,theta4,theta3,theta2,theta1; 

	// First, convert xworld, yworld, zworld to coordinates in the base frame
	if(offset) {
		x_grip = xworld + 11.5;
		y_grip = yworld - 15.0;
		z_grip = zworld + 0;
	}
	else {
		x_grip = xworld;
		y_grip = yworld;
		z_grip = zworld;
	}

	// Now the wrist roll
	theta5 = roll;
	
	// the waist angle
	theta1 = degree((float)atan2(y_grip,x_grip));

	w_grip = sqrt((x_grip*x_grip)+(y_grip*y_grip));

	w_grip -= cos(rad(pitch));
	z_grip -= sin(rad(pitch));
	
	w_wrist = (float)w_grip - i16*(sin(rad(pitch)));
	z_wrist = (float)z_grip + i16*(cos(rad(pitch)));

	// to account for z offset from base
	z_wrist -= 27.0;

	float len = (float)sqrt(w_wrist*w_wrist + z_wrist*z_wrist);

	theta3 = (float)2*degree(asin((len/(float)2)/i23)) - (float)180;
	theta2 = degree(asin(z_wrist/len)) + ((float)90 - (float)degree(asin((len/(float)2)/i23)));
	theta4 = pitch;

	// set pointers to avoid this error nonsense
	*t1 = theta1;
	*t2 = theta2;
	*t3 = theta3;
	*t4 = theta4;
	*t5 = theta5;

	float enc_B, enc_C, enc_D, enc_E, enc_F;

	// waist
	enc_F = f_ratio * fixDegrees(*t1);
	// shoulder
	enc_E = e_ratio * fixDegrees(*t2-125);
	// elbow
	enc_D = d_ratio * fixDegrees(*t2+*t3);
	// wrist flex
	enc_C = c_ratio * fixDegrees(/* *t2+*t3+ */*t4);
	// wrist rotate
	enc_B = b_ratio * fixDegrees(*t5);

	for(int i=0;i<5;i++)
		printf("%d: %f\n", i, IK[i]); 

	rhino_mmove((int)enc_B, (int)enc_C, (int)enc_D, (int)enc_E, (int)enc_F);


	return 0;
}


// converts degrees to radian value
float rad(float deg)
{
	return deg*((float)PI)/((float)180);
}

// converts radians to degrees
float degree(float rad)
{
	return rad*((float)180)/((float)PI);
}

// fixes degree values before sending to encoder equations
float fixDegrees(float degree)
{
	// why absolute value...
	return (float)(((int)degree)%360);
}

void convertToWorldCoordinates(double &x, double &y) {

	double world_y = (double)((double)(x-6.0)*cos_t)+(double)(y*sin_t);
	double world_x = (int)((double)(x-6.0)*n_sin_t)+(int)(y*cos_t);

	world_x = (double)world_x/(double)beta;
	world_y = (double)world_y/(double)beta;

	world_x += x_cm_offset;
	world_y += y_cm_offset;

	world_y *= 1.05;

	x = world_x;
	y = world_y;

	return;

}










