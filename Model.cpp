#include "Model.h"
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>
#include <math.h>
using namespace std;

#define PI 3.14159265

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
	float minX = vertices[0][0];
	float maxX = vertices[0][0];
	float minY = vertices[0][1];
	float maxY = vertices[0][1];
	float minZ = vertices[0][2];
	float maxZ = vertices[0][2];

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

void Model::printModelVertices() {
	for (int i = 0; i < 3; i++) {
		cout << "[" << vertices[i][0] << ",   "
			<< vertices[i][1] << ",   "
			<< vertices[i][2] << "]"
			<< endl;
	}

	for (int i = numVertices -3; i < numVertices; i++) {
		cout << "[" << vertices[i][0] << ",   "
			<< vertices[i][1] << ",   "
			<< vertices[i][2] << "]"
			<< endl;
	}
}

void Model::rotateVertices(float rx, float ry, float rz, float theta) {
    // Convert theta to radians
	theta = theta*PI / 180.0;

	// Normalize axis of rotation
	float w[3];
	normalizeVector(rx, ry, rz, w);

	//Find smallest value of w to make non-parallel vector
	int min_w = 0;
	for (int i = 1; i < 3; i++) {
		if (w[i] < min_w) {
			min_w = i;
		}
	}

	//Non-parallel vector
	float m[3] = {w[0], w[1], w[2]};
	m[min_w] = 1;
	normalizeVector(m[0], m[1], m[2], m);

	// Cross product gives perpendicular vector to w
	float u[3];
	crossVectors(w, m, u);
	normalizeVector(u[0], u[1], u[2], u);

	// Cross product gives perpendicular vector to w and u
	float v[3];
	crossVectors(w, u, v);


	//We need this matrix to make Z the axis of rotation, it is orthnormal
	float rd_row1[4] = { u[0], u[1], u[2], 0 };
	float rd_row2[4] = { v[0], v[1], v[2], 0 };
	float rd_row3[4] = { w[0], w[1], w[2], 0 };
	float rd_row4[4] = { 0, 0, 0, 1 };
	float* rotateDataMatrix[4] = { rd_row1, rd_row2, rd_row3, rd_row4 };

	//We need this matrix to perform the rotation by theta
	float rz_row1[4] = { cos(theta), -sin(theta), 0, 0 };
	float rz_row2[4] = { sin(theta), cos(theta),  0, 0 };
	float rz_row3[4] = { 0,          0,           1, 0 };
	float rz_row4[4] = { 0,          0,           0, 1 };
	float* rotateAboutZMatrix[4] = { rz_row1, rz_row2, rz_row3, rz_row4 };

	// We need this matrix to reverse process of making Z the axis of rotation
	// Matrix transposed is inverse if orthonormal
	float rdt_row1[4] = { u[0], v[0], w[0], 0 };
	float rdt_row2[4] = { u[1], v[1], w[1], 0 };
	float rdt_row3[4] = { u[2], v[2], w[2], 0 };
	float rdt_row4[4] = { 0, 0, 0, 1 };
	float* rotateDataTransposeMatrix[4] = { rdt_row1, rdt_row2, rdt_row3, rdt_row4 };

	float mrow1[] = { 0, 0, 0, 0 };
	float mrow2[] = { 0, 0, 0, 0 };
	float mrow3[] = { 0, 0, 0, 0 };
	float mrow4[] = { 0, 0, 0, 0 };
	float* middleMatrix[4] = {mrow1, mrow2, mrow3, mrow4};
	matrixMultiply(rotateAboutZMatrix, 4, 4, rotateDataMatrix, 4, 4, middleMatrix);
	
	float frow1[] = { 0, 0, 0, 0 };
	float frow2[] = { 0, 0, 0, 0 };
	float frow3[] = { 0, 0, 0, 0 };
	float frow4[] = { 0, 0, 0, 0 };
	float* finalRotationMatrix[4] = {frow1, frow2, frow3, frow4};
	matrixMultiply(rotateDataTransposeMatrix, 4, 4, middleMatrix, 4, 4, finalRotationMatrix);

	applyTransformationToModel(finalRotationMatrix[0], finalRotationMatrix[1], finalRotationMatrix[2], finalRotationMatrix[3]);
}

void Model::translateVertices(float tx, float ty, float tz) {
	// Can either mulitpy by matrix or just add values here

	float row1[4] = {1, 0, 0, tx};
	float row2[4] = { 0, 1, 0, ty };
	float row3[4] = { 0, 0, 1, tz };
	float row4[4] = { 0, 0, 0, 1 };

	applyTransformationToModel(row1, row2, row3, row4);

}

void Model::scaleVertices(float sx, float sy, float sz) {
	float row1[4] = { sx, 0, 0, 0 };
	float row2[4] = { 0, sy, 0, 0 };
	float row3[4] = { 0, 0, sz, 0 };
	float row4[4] = { 0, 0, 0, 1 };

	applyTransformationToModel(row1, row2, row3, row4);
}

void Model::applyTransformationToModel(float* row1, float* row2, float* row3, float* row4) {

	float* transformationMatrix[4] = { row1, row2, row3, row4 };

	for (int i = 0; i < numVertices; i++) {
		float target[3];
		matrixMultiplyVertex(transformationMatrix, 4, 4, vertices[i], target);
		vertices[i][0] = target[0];
		vertices[i][1] = target[1];
		vertices[i][2] = target[2];
	}
}

//These don't really belong here
void Model::matrixMultiplyVertex(float** matrix, int mwidth, int mheight, float* vertex, float* target) {
	//Checking dimensions is important here, currently assuming correct input

	//Transpose vertex for multiplication, add 1 for homogenous
	float row1[1] = { vertex[0] };
	float row2[1] = { vertex[1] };
	float row3[1] = { vertex[2] };
	float row4[1] = { 1 };

	float* newVertex[4] = { row1, row2, row3, row4};

	float new_target1[] = {0};
	float new_target2[] = {0};
	float new_target3[] = {0};
	float new_target4[] = {0};
	float* new_target[4] = { new_target1, new_target2, new_target3, new_target4 };

	matrixMultiply(matrix, mwidth, mheight, newVertex, 1, 4, new_target);

	target[0] = new_target[0][0];
	target[1] = new_target[1][0];
	target[2] = new_target[2][0];

}

//These don't really belong here
void Model::matrixMultiply(float** matrix, int mwidth, int mheight, float** other_matrix, int owidth, int oheight, float** target) {

	for (int i = 0; i < mheight; i++) {
		for (int j = 0; j < owidth; j++) {
			for (int l = 0; l < mwidth; l++) {
				target[i][j] += matrix[i][l] * other_matrix[l][j];
			}
		}
	}
}

//These don't really belong here
void Model::normalizeVector(float x, float y, float z, float* target) {
	float length = x*x + y*y + z*z;
	length = sqrt(length);
	target[0] = x / length;
	target[1] = y / length;
	target[2] = z / length;
}

//These don't really belong here
void Model::crossVectors(float* u, float* v, float* target) {
	target[0] = u[1] * v[2] - u[2] * v[1];
	target[1] = u[2] * v[0] - u[0] * v[2];
	target[2] = u[0] * v[1] - u[1] * v[0];
}
