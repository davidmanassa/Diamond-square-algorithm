#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <string>
#include "common/shader.hpp"
#include "Matrices.h"
#include "Vectors.h"
#include "Map.h"

class Mesh{
	
	vector<vec3> triNormals;

public:
	GLuint vbomesh, normalBuffer, vaoID;

	vector<GLfloat> mesh, normals;

	int sideNum; //Number of squares on side of mesh
	GLfloat meshSize;
	int mapSize;


	Mesh(int mapSize, int meshSize);

	void genTriangleMesh(vector<GLfloat> heightData);
	void genTriangleNormals();

	void initVertArray(); // buffer attributes
	void disableVertArrays();
	void draw();

};