#pragma once
class Matrix
{
public:
	Matrix();
	Matrix(int, float);
	Matrix(int, int, float);

	float get(int, int);
	void set(int, int, float);
	void overwriteData(Matrix);

	inline float getWidth() { return width; };
	inline float getHeight() { return height; };

	void multiply(Matrix);
	void multiply(Matrix, Matrix);
	void matrixMultiplyVertex(Matrix, Matrix&);

	static void normalizeVector(float, float, float, float*);
	static void crossVectors(float*, float*, float*);

	~Matrix();

private:
	float** data;
	int width;
	int height;
};

