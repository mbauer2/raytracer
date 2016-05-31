#include "Model.h"
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>
#include <math.h>
using namespace std;

#define PI 3.14159265f

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


		float** vertexData = readVertices(lines, headerSize, numVertices);
		vertices = Matrix(vertexData, numVertices, 3);
		calculateBounds();
	}
	else {
		correct = false;
	}
}

Model::Model(int headerSize, vector<string> header, int numVertices, Matrix vertices, int numFaces, int** faces) {
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

	// Delete other info
	if (currentAverage !=NULL) {
		delete[] currentAverage;
		currentAverage = NULL;
	}
	if (currentBounds != NULL) {
		delete[] currentBounds;
		currentBounds = NULL;
	}
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

	for (int i = startIndex; i < endIndex; i++) {
		float* data = new float[3];
		tokenizeVertex(lines.at(i), data);
		toReturn[i - startIndex] = data;
	}

	return toReturn;
}

void Model::calculateBounds()
{
	float minX = vertices.get(0, 0);
	float maxX = vertices.get(0, 0);
	float minY = vertices.get(0, 1);
	float maxY = vertices.get(0, 1);
	float minZ = vertices.get(0, 2);
	float maxZ = vertices.get(0, 2);

	float xSum = 0;
	float ySum = 0;
	float zSum = 0;

	for (int i = 0; i < numVertices; i++) {
		//cout << currVertex[1] << endl;
		float currVertex[3];
		vertices.getRow(i, currVertex);
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
		cout << "[" << vertices.get(i,0) << ",   "
			<< vertices.get(i,1) << ",   "
			<< vertices.get(i,2) << "]"
			<< endl;
	}

	for (int i = numVertices -3; i < numVertices; i++) {
		cout << "[" << vertices.get(i,0) << ",   "
			<< vertices.get(i,1) << ",   "
			<< vertices.get(i,2) << "]"
			<< endl;
	}
}

Matrix Model::getRotationMatrix(float rx, float ry, float rz, float theta) {
	// Convert theta to radians
	theta = theta*PI / 180;

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
	float m[3] = { w[0], w[1], w[2] };
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
	float* rotateData[4] = { rd_row1, rd_row2, rd_row3, rd_row4 };
	Matrix rotateDataMatrix = Matrix(rotateData, 4, 4);

	//We need this matrix to perform the rotation by theta
	float rz_row1[4] = { cos(theta), -sin(theta), 0, 0 };
	float rz_row2[4] = { sin(theta), cos(theta),  0, 0 };
	float rz_row3[4] = { 0,          0,           1, 0 };
	float rz_row4[4] = { 0,          0,           0, 1 };
	float* rotateAboutZ[4] = { rz_row1, rz_row2, rz_row3, rz_row4 };
	Matrix rotateAboutZMatrix = Matrix(rotateAboutZ, 4, 4);

	// We need this matrix to reverse process of making Z the axis of rotation
	// Matrix transposed is inverse if orthonormal
	float rdt_row1[4] = { u[0], v[0], w[0], 0 };
	float rdt_row2[4] = { u[1], v[1], w[1], 0 };
	float rdt_row3[4] = { u[2], v[2], w[2], 0 };
	float rdt_row4[4] = { 0, 0, 0, 1 };
	float* rotateDataTranspose[4] = { rdt_row1, rdt_row2, rdt_row3, rdt_row4 };
	Matrix rotateDataTransposeMatrix = Matrix(rotateDataTranspose, 4, 4);


	rotateAboutZMatrix.multiply(rotateDataMatrix);
	rotateDataTransposeMatrix.multiply(rotateAboutZMatrix);

	return rotateDataTransposeMatrix;

}

Matrix Model::getTranslationMatrix(float tx, float ty, float tz) {
	float row1[4] = { 1, 0, 0, tx };
	float row2[4] = { 0, 1, 0, ty };
	float row3[4] = { 0, 0, 1, tz };
	float row4[4] = { 0, 0, 0, 1 };

	float* transformationMatrix[4] = { row1, row2, row3, row4 };

	return Matrix(transformationMatrix, 4, 4);
}

Matrix Model::getScaleMatrix(float sx, float sy, float sz) {
	float row1[4] = { sx, 0, 0, 0 };
	float row2[4] = { 0, sy, 0, 0 };
	float row3[4] = { 0, 0, sz, 0 };
	float row4[4] = { 0, 0, 0, 1 };

	float* transformationMatrix[4] = { row1, row2, row3, row4 };

	return Matrix(transformationMatrix, 4, 4);
}

void Model::applyTransformationToModel(Matrix transformationMatrix) {

	for (int i = 0; i < numVertices; i++) {
		float target[3];
		float currVertex[3];
		vertices.getRow(i, currVertex);
		transformationMatrix.matrixMultiplyVertex(currVertex, target);
		vertices.set(i, 0, target[0]);
		vertices.set(i, 1, target[1]);
		vertices.set(i, 2, target[2]);
	}
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
