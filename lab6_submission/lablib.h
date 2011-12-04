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

//int getZval(int height, int width, int **img);

int writeToFile(int height, int width, int **img);

//void tagObjects(int height, int width, int **img);

void echo(std::string h);

void echo(char h);

void echo(int h);

int lab_movex(float xworld, float yworld, float zworld, float pitch, float roll);

float rad(float deg);

float degree(float rad);

float fixDegrees(float degree);

void convertToWorldCoordinates(double &x, double &y);

#endif

