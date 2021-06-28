#ifndef MAP_H
#define MAP_H

#include <vector>

using  namespace std;

class Map {
		int size, X, Y;
		vector<vector<float>> elements;

	public:

		Map(int x, int y);

		int getX() {
			return X;
		};
		int getY() {
			return Y;
		};
		float getElement(int x, int y) {
			return elements[x][y];
		};
		void setElement(int x, int y, int val) {
			elements[x][y] = val;
		};

		// gerar heightmap
		void generate(int size, float roughness);

		void square(int x, int y, int size, int offset);
		void diamond(int x, int y, int size, int offset);

		void crop(int lower, int upper);

		//Smooths terrain using a mean/box filter
		void smooth(int filterSize, int passes);

};

#endif // MAP_H
