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

// blockclass.cpp

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

#include "lablib.h"
#include "imgclasses.h"

#define sin_t -0.0995
#define n_sin_t 0.0995
#define cos_t 0.995
#define n_cos_t -0.995

#define beta 6.6000

#define x_cm_offset 3.4
#define y_cm_offset -9.55

using namespace std;


block::block(int height, int width) {
	block_size = 0;
	block_xmax = 0;
	block_xmin = width;
	block_ymax = 0;
	block_ymin = height;

}

void block::setWorldCoordinates() {

	// camera x and y are flipped for world
	this->world_y = (int)(this->centroid_x * cos_t) + (int)(this->centroid_y * sin_t);
	this->world_x = (int)(this->centroid_x * n_sin_t) + (int)(this->centroid_y * cos_t);

	this->cm_world_x = (double)this->world_x/beta;
	this->cm_world_y = (double)this->world_y/beta;

	this->cm_world_x += x_cm_offset;
	this->cm_world_y += y_cm_offset;

	this->cm_world_y *= 1.05;

}

void block:: printWorldCoordinates() {
	console_printf("   cm x: %f",this->cm_world_x);
	console_printf("   cm y: %f",this->cm_world_y);
}
