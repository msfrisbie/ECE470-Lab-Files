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

// lablib.h

#ifndef LABLIB_H
#define LABLIB_H

#include <string>

float f_div(float a, float b);

float f_mul(float a, float b);

float f_add(float a, float b);

float f_sub(float a, float b);

float fixEdge(float a);

void objSize(int x, int y, int cval, int &size, int &xmax, int &xmin, int &ymax, int &ymin, int **image);

int getZval(int height, int width, int **img);

int writeToFile(int height, int width, int **img);

void tagObjects(int height, int width, int **img);

void echo(std::string h);

void echo(char h);

void echo(int h);

#endif

