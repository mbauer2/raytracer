#pragma once
class Matrix
{
public:
	Matrix();
	Matrix(int, float);
	Matrix(int, int, float);
	Matrix(float**, int, int);
	Matrix(const Matrix&);

	void print();
	float get(int, int) const;
	void getRow(int, float*) const;
	void set(int, int, float);
	void overwriteData(Matrix);

	inline int getWidth() const { return width; };
	inline int getHeight() const { return height; };

	void multiply(Matrix);
	void multiply(Matrix, Matrix&) const;
	void matrixMultiplyVertex(float*, float*) const;

	static void normalizeVector(float, float, float, float*);
	static void crossVectors(float*, float*, float*);

	~Matrix();

private:
	float** data;
	int width;
	int height;
};

