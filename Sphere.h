#pragma once
#include <vector>
#include <glm/glm.hpp>
class Sphere
{
private:
	int numVertices;
	int numIndices;
	std::vector<int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;

	void init(int);
	float toRadians(float degree);

public:
	Sphere();
	Sphere(int prec);
	int getNumVertices();
	int getNumIndices();
	std::vector<glm::vec3> getVertices();
	std::vector<int> getIndices();
	std::vector<glm::vec2> getTexCoords();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec3> getTangents();
};

