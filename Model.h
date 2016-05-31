#ifndef MODEL_H_INCLUDE
#define MODEL_H_INCLUDE

#include <vector>
#include <string>
#include "Matrix.h"

using std::vector;
using std::string;

#pragma once

// Contains: list of strings for header
//           list of vertices
//           list of faces

class Model
{
public:

	Model();
	Model(vector<string>);
	Model(int, vector<string>, int, Matrix, int, int**);
	~Model();
	inline int getHeaderSize() { return headerSize; };
	inline int getNumVertices() { return numVertices; };
	inline int getNumFaces() { return numFaces; };
	inline bool getCorrect() { return correct; };

	void calculateBounds();

	void printModelInfo();
	void printModelVertices();

	Matrix getRotationMatrix(float, float, float, float);
	Matrix getTranslationMatrix(float, float, float);
	Matrix getScaleMatrix(float, float, float);
	void applyTransformationToModel(Matrix);

	static void matrixMultiply(float**, int, int, float**, int, int, float**);
	static void normalizeVector(float, float, float, float*);
	static void crossVectors(float*, float*, float*);
	

private:
	int headerSize;
	vector<string> header;
	int numVertices;
	Matrix vertices;
	int numFaces;
	int** faces;
	float* currentAverage;
	float* currentBounds;

	bool correct = true;
};


#endif