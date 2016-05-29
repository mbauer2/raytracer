#ifndef MODEL_H_INCLUDE
#define MODEL_H_INCLUDE

#include <vector>
#include <string>

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
	Model(int, vector<string>, int, float**, int, int**);
	~Model();
	inline int getHeaderSize() { return headerSize; };
	inline int getNumVertices() { return numVertices; };
	inline int getNumFaces() { return numFaces; };
	inline bool getCorrect() { return correct; };

	void calculateBounds();

	void printModelInfo();
	void printModelVertices();

	void rotateVertices(float, float, float, float);
	void translateVertices(float, float, float);
	void scaleVertices(float, float, float);
	void applyTransformationToModel(float*, float*, float*, float*);

	static void matrixMultiplyVertex(float**, int, int, float*, float*);
	static void matrixMultiply(float**, int, int, float**, int, int, float**);
	static void normalizeVector(float, float, float, float*);
	static void crossVectors(float*, float*, float*);
	

private:
	int headerSize;
	vector<string> header;
	int numVertices;
	float** vertices;
	int numFaces;
	int** faces;
	float* currentAverage;
	float* currentBounds;

	bool correct = true;
};


#endif