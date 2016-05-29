#include "Matrix.h"
#include <iterator>
using namespace std;

Matrix::Matrix()
{
}

// Creates a diagonal matrix with diagonal_value along diagonal
Matrix::Matrix(int height, float diagonal_value) {
	this->height = height;
	this->width = height;
	for (int i = 0; i < height; i++) {
		float* line_data = new float[width];
		for (int j = 0; j < width; j++) {
			if (i == j) {
				line_data[j] = diagonal_value;
			}
			else {
				line_data[j] = 0;
			}
		}
		data[i] = line_data;
	}

}

Matrix::Matrix(int height, int width, float initial_value) {
	this->height = height;
	this->width = width;
	data = new float*[height];
	for (int i = 0; i < height; i++) {
		float* line_data = new float[width];
		for (int j = 0; j < width; j++) {
			line_data[j] = initial_value;
		}
		data[i] = line_data;
	}
}

Matrix::~Matrix()
{
	for (int i = 0; i < height; i++) {
		delete[] data[i];
	}
	delete[] data;
	data = NULL;
}

float Matrix::get(int row, int column) {
	return data[row][column];
}

void Matrix::set(int row, int column, float val) {
	data[row][column] = val;
}

void Matrix::overwriteData(Matrix other) {
	// Assuming indeces are the same, if not... bad
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			data[i][j] = other.get(i,j);
		}
	}
}

// Modifies this
void Matrix::multiply(Matrix other_matrix) {
	// Assuming indeces are compatible, if not... bad
	Matrix target = Matrix(height, width, 0);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < other_matrix.getWidth(); j++) {
			for (int l = 0; l < width; l++) {
				float newVal = get(i,j) + get(i,l)*other_matrix.get(l,j);
				target.set(i, j, newVal);
			}
		}
	}

	overwriteData(target);
}

// Does not modify this
void Matrix::multiply(Matrix other_matrix, Matrix target) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < other_matrix.getWidth(); j++) {
			for (int l = 0; l < width; l++) {
				float newVal = get(i, j) + get(i, l)*other_matrix.get(l, j);
				target.set(i, j, newVal);
			}
		}
	}
}

void Matrix::matrixMultiplyVertex(Matrix vertex, Matrix& target) {
	//Checking dimensions is important here, currently assuming correct input

	//Transpose vertex for multiplication, add 1 for homogenous
	Matrix newVertex = Matrix(4, 1, 0);
	newVertex.set(0, 0, vertex.get(0, 0));
	newVertex.set(1, 0, vertex.get(0, 1));
	newVertex.set(2, 0, vertex.get(0, 2));
	newVertex.set(3, 0, vertex.get(0, 3));

	Matrix newTarget = Matrix(4, 1, 0);

	multiply(newVertex, newTarget);

	target.set(0, 0, newTarget.get(0, 0));
	target.set(0, 1, newTarget.get(1, 0));
	target.set(0, 2, newTarget.get(2, 0));
}


//These don't really belong here
void Matrix::normalizeVector(float x, float y, float z, float* target) {
	float length = x*x + y*y + z*z;
	length = sqrt(length);
	target[0] = x / length;
	target[1] = y / length;
	target[2] = z / length;
}

//These don't really belong here
void Matrix::crossVectors(float* u, float* v, float* target) {
	target[0] = u[1] * v[2] - u[2] * v[1];
	target[1] = u[2] * v[0] - u[0] * v[2];
	target[2] = u[0] * v[1] - u[1] * v[0];
}