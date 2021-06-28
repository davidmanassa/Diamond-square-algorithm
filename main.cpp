#include <iostream>
#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Windows.h>
#include <string>
#include <fstream>
#include "Utils.h"
#include "Matrices.h"
#include "Vectors.h"
#include "Map.h"
#include "Mesh.h"
#include "common/shader.hpp"
#include "loadShaders.h"
#include "Shaders.h"

using namespace std;

int main() {

	// Quanto maior o mapa, mais os detalhes
	// 2^n + 1
	const int mapsize = 512;
	const int start = 64;

	Map heightMap(mapsize + 1, mapsize + 1);

	// Definir os quatro cantos iniciais
	heightMap.setElement(0, 0, start);
	heightMap.setElement(0, mapsize, start);
	heightMap.setElement(mapsize, 0, start);
	heightMap.setElement(mapsize, mapsize, start);

	// Gerar mapa
	heightMap.generate(mapsize + 1, float(0.6)); // 0.6 rugosidade
	heightMap.crop(0, 255);
	heightMap.smooth(2, 2); // tamanho filtro, passagens

	// Definir os valores do heightmap para volores entre -1 e 1 para renderizar
	vector<GLfloat> heightdata;
	for (int y = 0; y != heightMap.getY(); y++) {
		for (int x = 0; x != heightMap.getX(); x++) {
			heightdata.push_back((GLfloat(heightMap.getElement(x, y))) / 255);
		}
	}

	Mesh mesh(mapsize, 2.0f);
	mesh.genTriangleMesh(heightdata);
	mesh.genTriangleNormals();

	// OPENGL
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Diamond Square Algorithm", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = true;

	if (glewInit() != GLEW_OK) { cout << "GLEW init failed"; return -1; }
	glfwSwapInterval(1);

	float size = 1.0f;
	float radx = 3.14 * 2;
	float rady = 3.14 * 2;
	float radz = 3.14 * 2;

	mat4 translate; translate.createTranslate(vec3(0, 0, -5));
	mat4 scale; scale.createScale(size, size, size);
	mat4 perspect; perspect.createPerspect(45, 16.0f / 9.0f, 0.1f, 100.0f);
	mat4 rotatex; rotatex.createRotation(vec3(1, 0, 0), 0);
	mat4 rotatey; rotatey.createRotation(vec3(0, 1, 0), 0);
	mat4 rotatez; rotatez.createRotation(vec3(0, 0, 1), 0);

	mat4 viewMatrix;
	mat4 modelMatrix;
	vec3 light(1, 1, 1);

	shader shaders("shader.vertshader", "shader.fragshader");
	mesh.initVertArray();

	int width, height, initWidth, initHeight;
	glfwGetFramebufferSize(window, &initWidth, &initHeight);

	while (!glfwWindowShouldClose(window)) {

		viewMatrix = perspect * translate * rotatex * rotatey * rotatez * scale;
		mat4 modelMatrix = translate * rotatex * rotatey * rotatez * scale;

		glfwGetFramebufferSize(window, &width, &height);
		glViewport((-initWidth + width) / 2, (-initHeight + height) / 2, 1280, 720);

		if (GetAsyncKeyState(VK_UP)) {
			radx += 0.02;
			rotatex.createRotation(vec3(1, 0, 0), radx);
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			radx -= 0.02;
			rotatex.createRotation(vec3(1, 0, 0), radx);
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			rady += 0.02;
			rotatey.createRotation(vec3(0, 1, 0), rady);
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			rady -= 0.02;
			rotatey.createRotation(vec3(0, 1, 0), rady);
		}
		if (GetAsyncKeyState(0x41)) {
			radz += 0.02;
			rotatez.createRotation(vec3(0, 0, 1), radz);
		}
		if (GetAsyncKeyState(0x44)) {
			radz -= 0.02;
			rotatez.createRotation(vec3(0, 0, 1), radz);
		}

		//Clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Z-Buffer
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		//Transforms triangle view
		shaders.setUniformMat4("view", viewMatrix);
		shaders.setUniformVec3("in_light", light);
		shaders.setUniformMat4("model", modelMatrix);

		//Use shader
		shaders.enable();

		mesh.draw(); // DRAW

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	mesh.disableVertArrays();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
