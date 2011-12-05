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

// vision_labs.cpp

#include "labs.h"
#include "console.h"
#include "rhino.h"
#include "lab6_oop/lablib.h"
#include "lab6_oop/imgclasses.h"
#include "lab6_oop/blockclass.h"
#include <stdio.h>

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

int DEBUG_LABELS=false; // if set to "true" will generate a text file in your directory displaying the contents of the pixel label array 

void thresholdImage(QImage *gray, QImage *threshed)
// For use with Lab 5
// Take a grayscale bitmap and threshold it.
// The image pointer has dimension width by height pixels.
{
	int   total_pixels;
	uchar gray_level;							// think of the uchar datatype as an integer that occupies only one byte in memory.
	uchar *p_first_gray_byte, *p_gray_byte; 
	QRgb  *p_first_thresh_rgb;
   int histogram[NUM_VALUES];
   //int gray_level = 0;
   float prob[NUM_VALUES];
   float q0[NUM_VALUES];
   float q1[NUM_VALUES];
   float sigma0[NUM_VALUES];
   float sigma1[NUM_VALUES];
   float sigma_w[NUM_VALUES];
   float u0[NUM_VALUES];
   float u1[NUM_VALUES];
   float var_b[NUM_VALUES];
   int i,j;//,k;
   float mean = 0;
   int z_sum[NUM_VALUES];
   int z_sum2[NUM_VALUES];
   //int it_sum = 0;

   total_pixels     = gray->numBytes()/4;         // total number of pixels in image
   p_first_gray_byte     = gray->bits();         // address of first byte of pixel data in image gray
   p_first_thresh_rgb = (QRgb*)threshed->bits();// address of first RGBA triplet in image threshed

   int zt = 0; // threshold grayscale value 

   // 1. build a histogram for the gray image

   //printf("////////////////////////////////////////////////\n");

   // initialize as empty
   for(i=0;i<NUM_VALUES;i++) {
      histogram[i]=0;
      prob[i]=0;
      q0[i]=0;
      q1[i]=0;
      u0[i]=0;
      u1[i]=0;
      sigma0[i]=0;
      sigma1[i]=0;
      sigma_w[i]=0;
      var_b[i]=0;
	  z_sum[i]=0;
	  z_sum2[i]=0;
   }

	//int height = gray->height();
	//int width = gray->width();

   // occurences of z value
   for(i=0;i<total_pixels;i++) {
      gray_level = *(p_first_gray_byte + 4*i);
      histogram[gray_level]++;
   }

   // print nonzero histogram values
   //for(i=0;i<NUM_VALUES;i++)
      //if(histogram[i]!=0)
         //printf("histogram[%d]: %d\n", i, histogram[i]);

   // calculate probability of z value
   for(i=0;i<NUM_VALUES;i++) {
      prob[i] = f_div(histogram[i],total_pixels);
      //if(prob[i]!=0)
         //printf("prob[%d]: %f\n", i, prob[i]);
   }

   // calculate mean
   for(i=0;i<NUM_VALUES;i++) {
      mean = f_add( mean, f_mul(i,prob[i]) );
   }

   //printf("mean = %f\n",mean);

   // iterate through 256 possible z values
   for(i=0;i<NUM_VALUES;i++) {
      // calculate z_sum,z_sum2 for current value of zt
	  for(j=0;j<i+1;j++) {
		 z_sum[i] += histogram[j];
	  }
	  for(j=i+1;j<NUM_VALUES;j++) {
		 z_sum2[i] += histogram[j];
	  }

	  // calculate q0,q1 for current value of zt
	  q0[i] = f_div(z_sum[i], total_pixels);
	  q1[i] = f_div(z_sum2[i], total_pixels);

	  if(i==30)
		i=i;

	  // correct for cases of 0,1
	  q0[i] = fixEdge(q0[i]);
	  q1[i] = fixEdge(q1[i]);
   
	  // calculate u0,u1 for current value of zt
	  for(j=0;j<NUM_VALUES;j++) {
		 //u0[i] += (float)j*(float)(prob[j]/q0[i]);
		 //u1[i] += (float)j*(float)(prob[j]/q1[i]);
		 u0[i] = f_add(u0[i], f_mul(j, f_div(prob[j], q0[i])));
		 u1[i] = f_add(u1[i], f_mul(j, f_div(prob[j], q1[i])));
	  }

	  // variable used for j - u in sigma calculations
	  int diff;

	  // calculate sigma0,sigma1 for current value of zt
	  for(j=0;j<NUM_VALUES;j++) {
		 if(i<(j+1)) {
			diff = f_sub( j, u1[i] );
			//sigma1[i] += ((float)j - u1[i])*((float)j - u1[i])*prob[j]/q1[i];
			sigma1[i] = f_add(sigma1[i], f_mul( f_mul(diff, diff), f_div( prob[j], q0[i] ) ) );
			//printf("sigma_w[%d] = %f\n", i, sigma_w[i]);
		 }
		 else {
			diff = f_sub( j, u0[i] );
			//sigma0[i] += ((float)j - u0[i])*((float)j - u0[i])*prob[j]/q0[i];
			sigma0[i] = f_add(sigma0[i], f_mul( f_mul(diff, diff), f_div( prob[j], q1[i] ) ) );
		 }
	  }

	  //printf("sigma0: %f q0: %f sigma1: %f q1: %f\n", sigma0[i], q0[i], sigma1[i], q1[i]);
	  //sigma_w[i] = q0[i]*sigma0[i] + q1[i]*sigma1[i];
	  sigma_w[i] = f_add( f_mul( q0[i], sigma0[i] ), f_mul( q1[i], sigma1[i] ) );

	  //printf("sigma_w[%d] = %f\n", i, sigma_w[i]);

	  if(i==200)
		 i=i;
   }

	float min_val;

	for(i=0;i<NUM_VALUES;i++) {
		if(i==0)
			min_val = sigma_w[0];
		if(sigma_w[i] < min_val) {
			min_val = sigma_w[i];
			zt = i;
		}
	}

	//int rand = *(p_first_gray_byte + 4*32) % 10;
	//rand -= 5;


	zt = zt + LIGHTING_OFFSET;

	//zt=140;



	// 2. select value of zt that minimizes the within-group variance
	//zt = 50;  	// blantantly wrong, just here as an example
	
	// threshold the image
	for(int i=0; i<total_pixels; i++)
	{
		p_gray_byte = p_first_gray_byte+i*4;
		gray_level = *(p_gray_byte);		
		if(gray_level>zt) *(p_first_thresh_rgb+i)=0xffffffff; // set rgb to 255 (white)
		else             *(p_first_thresh_rgb+i)=0x00000000; // set rgb to 0   (black)
	}
}




void associateObjects(QImage *threshed, QImage *associated)
// For use with Lab 5
// Assign a unique color to each object in the thresholded image,
// and change all pixels in each object to the color.
{

	image *img = new image(threshed,associated);

	// draw grid on image
	img->drawGrid();

	// draw cross at center of image
	//img->drawCross(160,120);

	// print block coordinates and draw their crosses
	img->printBlocksToConsole();

}

void lab_pick(int row, int column, QRgb pix)
// For use with Lab 6.
// Pick up a block by clicking on the object in the processed image.
{
	console_printf("Picking up the block at (%d,%d); color is (%d,%d,%d)",
		row, column, qRed(pix), qGreen(pix), qBlue(pix));

	double xCood = (double)column;
	double yCood = (double)row;

	convertToWorldCoordinates(xCood, yCood);
	
	lab_movex((float)xCood,(float)yCood,3,0,100);
	rhino_grip();
	rhino_softhome();
}


void lab_place(int row, int column, QRgb pix)
// For use with Lab 6
// If the robot is holding a block, place it at the designated row and column.
{
	console_printf("Placing the block at (%d,%d); color is (%d,%d,%d)",
		row, column, qRed(pix), qGreen(pix), qBlue(pix));

	double xCood = (double)column;
	double yCood = (double)row;

	convertToWorldCoordinates(xCood, yCood);

	lab_movex((float)xCood,(float)yCood,3,0,100);
	rhino_ungrip();
	rhino_softhome();
}


void lab_main()
{
	// nothing here; leave it blank!
}


void lab_help()
{
	console_printf("find   get color information for a pixel in any of the four images\n");
	console_printf("pick   click on an object to direct the Rhino to pick it up\n");
	console_printf("place  click on a location for the Rhino to place the object in its gripper\n");
}
