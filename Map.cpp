#include <iostream>
#include "Map.h"
#include <vector>
#include <time.h>

using namespace std;

Map::Map(int X, int Y) {
	this->X = X;
	this->Y = Y;
	this->size = X * Y;

	vector<float> tempvec;
	for (int i = 0; i != X; i++) {
		elements.push_back(tempvec);
		for (int j = 0; j != Y; j++) {
			elements[i].push_back(0);
		}
	}
}

void Map::square(int x, int y, int size, int offset) {
	elements[x][y] =
		(elements[x + size][y + size] +
		elements[x - size][y - size] +
		elements[x + size][y - size] +
		elements[x - size][y + size]) / 4 + offset;
}

void Map::diamond(int x, int y, int size, int offset) {
	if (y - size < 0) {
		elements[x][y] = 
			(elements[x][y + size] +
			elements[x + size][y] +
			elements[x - size][y]) / 3 + offset;
	} else if (y + size > this->Y - 1) {
		elements[x][y] = 
			(elements[x][y - size] +
			elements[x + size][y] +
			elements[x - size][y]) / 3 + offset;
	} else if (x - size < 0) {
		elements[x][y] = 
			(elements[x][y + size] +
			elements[x][y - size] +
			elements[x + size][y]) / 3 + offset;
	} else if (x + size > this->Y - 1) {
		elements[x][y] = 
			(elements[x][y + size] +
			elements[x][y - size] +
			elements[x - size][y]) / 3 + offset;
	} else {
		elements[x][y] = 
			(elements[x][y + size] +
			elements[x][y - size] +
			elements[x + size][y] +
			elements[x - size][y]) / 4 + offset;
	} 
}


void Map::generate(int size, float roughness) {

	srand(time(NULL));
	int squaresize = (size - 1) / 2;
	int d = roughness;

	while (squaresize > 0) {

		for (int y = 0; y < size / (squaresize * 2); y++) {
			for (int x = 0; x < size / (squaresize * 2); x++) {
				
				square((x * squaresize * 2) + squaresize,
					(y * squaresize * 2) + squaresize,
					squaresize,
					rand() % int(squaresize * roughness * 2) - squaresize * roughness);
			}
		}

		int rowtype = 1;
		for (int y = 0; y < size; y += squaresize) {
			if (rowtype == 1) {
				for (int x = squaresize; x < size; x += 2 * squaresize) {
					diamond(x, y, squaresize, 
						rand() % int(squaresize * roughness * 2) - squaresize * roughness);
				}
			}
			if (rowtype == -1) {
				for (int x = 0; x < size; x += squaresize * 2) {
					diamond(x, y, squaresize, 
						rand() % int(squaresize * roughness * 2) - squaresize * roughness);
				}
			}
			rowtype *= -1;
		}

		squaresize /= 2;
	}
}

void Map::crop(int lower, int upper) {
	for (int x = 0; x < this->X; x++) {
		for (int y = 0; y < this->Y; y++) {
			if (elements[x][y] > upper) elements[x][y] = upper;
			if (elements[x][y] < lower) elements[x][y] = lower;
		}
	}
}

void Map::smooth(int filterSize, int passes) {

	vector<vector<float>> newelements = elements;

	for (int i = 0; i != passes; i++) {

		for (int X = 0; X != this->X; X++) {
			for (int Y = 0; Y != this->Y; Y++) {

				float surroundingTotal = -elements[X][Y];
				int aroundNum = -1;

				for (int x = -filterSize; x <= filterSize; x++) {
					for (int y = -filterSize; y <= filterSize; y++) {
						if (Y + y > -1 && Y + y < this->Y && X + x > -1 && X + x < this->X) {
							surroundingTotal += elements[X + x][Y + y];
							aroundNum++;
						}
					}
				}
				newelements[X][Y] = (elements[X][Y] + (surroundingTotal / aroundNum)) / 2;
			}
		}
		elements = newelements;
	}
}
