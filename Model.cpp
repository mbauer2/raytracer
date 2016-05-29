
#include "Model.h"
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>
using namespace std;

void readHeader(vector<string>&, int*);
void tokenizeVertex(string, float*);
float** readVertices(vector<string>, int, int);
// Contains: list of strings for header
//           list of vertices
//           list of faces


Model::Model()
{

}

Model::Model(vector<string> lines) {
	int headerInfo[3];
	readHeader(lines, headerInfo);
	headerSize = headerInfo[0];
	numVertices = headerInfo[1];
	numFaces = headerInfo[2];

	currentAverage = new float[3];
	currentBounds = new float[6];

	if (headerSize + numVertices + numFaces == lines.size() && numVertices > 0) {
		for (int i = 0; i < headerSize; i++) {
			header.push_back(lines.at(i));
		}

		vertices = readVertices(lines, headerSize, numVertices);
		calculateBounds();
	}
	else {
		correct = false;
	}
}

Model::Model(int headerSize, vector<string> header, int numVertices, float** vertices, int numFaces, int** faces) {
	this->headerSize = headerSize;
	this->header = header;
	this->numVertices = numVertices;
	this->vertices = vertices;
	this->numFaces = numFaces;
	this->faces = faces;

}

Model::~Model()
{
	cout << "Model is being deleted." << endl;

	// Need to delete Vertices from heap
	for (int i = 0; i < numVertices; i++) {
		delete[] vertices[i];
	}
	delete[] vertices;
	vertices = NULL;

	// Delete other info
	delete[] currentAverage;
	delete[] currentBounds;
}

//Gets the current important info, later expand to read properties
void readHeader(vector<string>& lines, int* info) {
	for (unsigned int i = 0; i < lines.size(); i++) {
		string currString = lines.at(i);
		if (currString.compare("end_header") == 0) {
			info[0] = i + 1;
			break;
		}

		size_t found = currString.find("element vertex ");
		//cout << "First found: " << found << endl;
		if (found != std::string::npos) {
			currString.replace(found, 15, "");
			info[1] = stoi(currString);
			continue;
		}

		found = currString.find("element face ");
		//cout << "Second found: " << found << endl;
		if (found != std::string::npos) {
			currString.replace(found, 13, "");
			info[2] = stoi(currString);
			continue;
		}
	}
}

void tokenizeVertex(string line, float* target) {

	float newNums[3];

	istringstream ss(line);
	copy(
		istream_iterator <float>(ss),
		istream_iterator <float>(),
		newNums
	);

	target[0] = newNums[0];
	target[1] = newNums[1];
	target[2] = newNums[2];
}

float** readVertices(vector<string> lines, int startIndex, int numVertices) {
	float** toReturn = new float*[numVertices];

	int endIndex = startIndex + numVertices;

	for (unsigned int i = startIndex; i < endIndex; i++) {
		float* data = new float[3];
		tokenizeVertex(lines.at(i), data);
		toReturn[i - startIndex] = data;
	}

	return toReturn;
}

void Model::calculateBounds()
{
	float minX = 0;
	float maxX = 0;
	float minY = 0;
	float maxY = 0;
	float minZ = 0;
	float maxZ = 0;

	float xSum = 0;
	float ySum = 0;
	float zSum = 0;

	for (int i = 0; i < numVertices; i++) {
		//cout << currVertex[1] << endl;
		float* currVertex = vertices[i];
		if (currVertex[0] > maxX) {
			maxX = currVertex[0];
		}
		if (currVertex[0] < minX) {
			minX = currVertex[0];
		}
		if (currVertex[1] > maxY) {
			maxY = currVertex[1];
		}
		if (currVertex[1] < minY) {
			minY = currVertex[1];
		}
		if (currVertex[2] > maxZ) {
			maxZ = currVertex[2];
		}
		if (currVertex[2] < minZ) {
			minZ = currVertex[2];
		}
		xSum += currVertex[0];
		ySum += currVertex[1];
		zSum += currVertex[2];
	}

	currentAverage[0] = xSum/numVertices;
	currentAverage[1] = ySum / numVertices;
	currentAverage[2] = zSum / numVertices;

	currentBounds[0] = minX;
	currentBounds[1] = maxX;
	currentBounds[2] = minY;
	currentBounds[3] = maxY;
	currentBounds[4] = minZ;
	currentBounds[5] = maxZ;
}

void Model::printModelInfo() {

	cout << "Vertices: " << numVertices << endl;
	cout << "Faces: " << numFaces << endl;
	cout << "Average vertex: (" << currentAverage[0] << ","
		                        << currentAverage[1] << ","
		                        << currentAverage[2] << ")"
		 << endl;
	cout << "Min Bound: (" << currentBounds[0] << ","
		                   << currentBounds[2] << ","
		                   << currentBounds[4] << ")"
		<< endl;
	cout << "Max Bound: (" << currentBounds[1] << ","
		                   << currentBounds[3] << ","
		                   << currentBounds[5] << ")"
		<< endl;
}

