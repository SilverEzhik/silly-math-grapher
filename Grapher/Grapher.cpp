// Grapher.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

using namespace std;

//consts we'll use
const int PRECISION = 10;
const int PRECISION_POW = 1;
const int SCALE = 10;
const int imgRes = PRECISION * SCALE * 2 + 1;

float step = pow(0.1, PRECISION_POW);


int image[imgRes][imgRes]; //the array with image data, public access info because of reasons

void point(int x, int y, int color) //puts a point in the image array
{
	if (!((x >= 0) && (x <= imgRes)) || !((y >= 0) && (y <= imgRes))) return; //if image is out of bounds don't draw shit
	//if (color == 0xFF0000) cout << "; Point scale: " << x << ", " << y << '\n';
	image[y][x] = color;
} 
void graphPoint(float x, float y, int color) //converts a graph point to a normal point and puts it in the image array
{
	//cout << x << ", " << y;
	//round points
	x = roundf(x * pow(10, PRECISION_POW)) / pow(10, PRECISION_POW);
	y = roundf(y * pow(10, PRECISION_POW)) / pow(10, PRECISION_POW);
	//cout << "Rounded: " << x << ", " << y << '\n';
	int pointX, pointY;
	pointX = (x * PRECISION) + (PRECISION * SCALE);
	pointY = -(y * PRECISION) + (PRECISION * SCALE);
	if (fmod(y, 1) == 0 && fmod(x, 1) == 0)
	{
		//cout << "fmod(" << pointY << ", 1) == " << fmod(pointY, 1) << '\n';
		color = 0xFF0000;
	}
	point(pointX, pointY, color);
	
}
string convertColor(int input); //converts color from hex to a PBM string

//float getY[imgRes] (float X);

void writeImage(); //writes image to the file

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Drawing grid...";
	for (int a = 0; a < imgRes; a++)
	{
		for (int b = 0; b < imgRes; b++)
		{
			point(a, b, 0xFFFFFF);
			if (a % 10 == 0 || b % 10 == 0) point(a, b, 0xBFBFBF);
			if ((a % 100 == 0 || b % 100 == 0) && (a == 100 || b == 100))
			{
				point(a, b, 0x000000);
			}
			

		}
	}
	cout << " Done!\n";

	cout << "Drawing expression... ";
	for (float x = -SCALE; x <= (SCALE + step); x += 0.0001)
	{
		//circle:
		graphPoint(x,  sqrt(pow(4, 2) - pow(x, 2)), 0x000088);
		graphPoint(x, -sqrt(pow(4, 2) - pow(x, 2)), 0x000088);
		//x squared:
		//graphPoint(x, pow(x, 2), 0x000088);
		//sin x
		//graphPoint(x, sin(x), 0x000088);

	}

	cout << " Done!\n";

	writeImage();
	system("Pause");
	return 0;
}

void writeImage()
{
	cout << "Saving image...    ";
	ofstream output;
	output.open("output.pbm");
	output << "P3\n" << imgRes << ' ' << imgRes << "\n255\n"; //header

	int done = 0;

	for (int a = 0; a < imgRes; a++)
	{
		for (int b = 0; b < imgRes; b++)
		{
			output << convertColor(image[a][b]) << '\t';
		}
		output << '\n';

		done = (float)a / (float)imgRes * 100;
		if (done > 9) cout << '\b';
		cout << "\b\b" << done << '%';
	}
	cout << "\b\b\bDone!\n";
}

string convertColor(int input)
{
	int r;
	int g;
	int b;
	string output;

	r = ((input >> 16) & 0xFF); //how does this work tho?
	g = ((input >> 8) & 0xFF);
	b = ((input)& 0xFF);

	return to_string(r) + "\t" + to_string(g) + "\t" + to_string(b);
}
