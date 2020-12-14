#include <cmath>
#include <vector>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include "Torus.h"
using namespace std;

Torus::Torus() {
	prec = 48;
	inner = 0.5f;
	outer = 0.2f;
	init();
}

Torus::Torus(float in, float out, int precIn) {
	prec = precIn;
	inner = in;
	outer = out;
	init();
}

float Torus::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Torus::init() {
	numVertices1 = (prec + 1) * (prec + 1);
	numIndices1 = prec * prec * 6;
	for (int i = 0; i < numVertices1; i++) { vertices1.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices1; i++) { texCoords1.push_back(glm::vec2()); }
	for (int i = 0; i < numVertices1; i++) { normals1.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices1; i++) { sTangents1.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices1; i++) { tTangents1.push_back(glm::vec3()); }
	for (int i = 0; i < numIndices1; i++) { indices1.push_back(0); }

	// calculate first ring
	for (int i = 0; i < prec + 1; i++) {
		float amt = toRadians(i*360.0f / prec);

		glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 initPos(rMat * glm::vec4(outer, 0.0f, 0.0f, 1.0f));

		vertices1[i] = glm::vec3(initPos + glm::vec3(inner, 0.0f, 0.0f));
		texCoords1[i] = glm::vec2(0.0f, ((float)i / (float)prec));

		rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 0.0f, 1.0f));
		tTangents1[i] = glm::vec3(rMat * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));

		sTangents1[i] = glm::vec3(glm::vec3(0.0f, 0.0f, -1.0f));
		normals1[i] = glm::cross(tTangents[i], sTangents[i]);
	}
	// rotate the first ring about Y to get the other rings
	for (int ring = 1; ring < prec + 1; ring++) {
		for (int i = 0; i < prec + 1; i++) {
			float amt = (float)toRadians((float)ring * 360.0f / (prec));

			glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			vertices1[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(vertices[i], 1.0f));

			texCoords1[ring*(prec + 1) + i] = glm::vec2((float)ring*2.0f / (float)prec, texCoords1[i].t);
			if (texCoords1[ring*(prec + 1) + i].s > 1.0) texCoords1[ring*(prec+1)+i].s -= 1.0f;

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			sTangents1[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(sTangents1[i], 1.0f));

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			tTangents1[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(tTangents1[i], 1.0f));

			rMat = glm::rotate(glm::mat4(1.0f), amt, glm::vec3(0.0f, 1.0f, 0.0f));
			normals1[ring*(prec + 1) + i] = glm::vec3(rMat * glm::vec4(normals1[i], 1.0f));
		}
	}
	// calculate triangle indices
	for (int ring = 0; ring < prec; ring++) {
		for (int i = 0; i < prec; i++) {
			indices1[((ring*prec + i) * 2) * 3 + 0] = ring*(prec + 1) + i;
			indices1[((ring*prec + i) * 2) * 3 + 1] = (ring + 1)*(prec + 1) + i;
			indices1[((ring*prec + i) * 2) * 3 + 2] = ring*(prec + 1) + i + 1;
			indices1[((ring*prec + i) * 2 + 1) * 3 + 0] = ring*(prec + 1) + i + 1;
			indices1[((ring*prec + i) * 2 + 1) * 3 + 1] = (ring + 1)*(prec + 1) + i;
			indices1[((ring*prec + i) * 2 + 1) * 3 + 2] = (ring + 1)*(prec + 1) + i + 1;
		}
	}
}
int Torus::getNumVertices1() { return numVertices1; }
int Torus::getNumIndices1() { return numIndices1; }
std::vector<int> Torus::getIndices1() { return indices1; }
std::vector<glm::vec3> Torus::getVertices1() { return vertices1; }
std::vector<glm::vec2> Torus::getTexCoords1() { return texCoords1; }
std::vector<glm::vec3> Torus::getNormals1() { return normals1; }
std::vector<glm::vec3> Torus::getStangents1() { return sTangents1; }
std::vector<glm::vec3> Torus::getTtangents1() { return tTangents1; }