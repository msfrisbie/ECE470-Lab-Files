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

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>

#include "lablib.h"
#include "imgclasses.h"
#include "blockclass.h"

using namespace std;

int main() {

	image *img = new image;

	img->writeToTerminal();

	img->printBlockInfo();

	img->writeToFile();

}


