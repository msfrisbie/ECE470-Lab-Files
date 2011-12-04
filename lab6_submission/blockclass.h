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

// blockclass.h

#ifndef BLOCKCLASS_H
#define BLOCKCLASS_H

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>

#include "lablib.h"
#include "imgclasses.h"


class block {
	public:

	double cm_world_x,cm_world_y;
	int centroid_x,centroid_y,block_xmax,block_xmin,block_ymax,block_ymin,block_size,block_tag,world_x,world_y;

	block(int height, int width);

	void setWorldCoordinates();

	void printWorldCoordinates();

};

#endif
