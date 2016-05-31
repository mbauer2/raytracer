#include "Model.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>


using namespace std;

vector<string> readFile(string infile) {
	vector<string> linesOfFile;

	string line;

	ifstream file(infile);
	if (file.is_open()) {
		while (getline(file, line)) {
			linesOfFile.push_back(line);
		}
	}
	file.close();
	return linesOfFile;
}

void writeFile(vector<string> towrite, string outfile) {
		ofstream myfile(outfile);
		if (myfile.is_open()) {
			for (unsigned int i = 0; i < towrite.size(); i++) {
				myfile << towrite.at(i) << endl;
			}
		}
		myfile.close();
}

void reverseVector(vector<string> &lines) {
	reverse(lines.begin(), lines.end());
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		cout << "Wrong number of arguments." << endl;
		cout << argc << endl;
		system("pause");
		return -1;
	}

	string infile = argv[1];
	string outfile = argv[2];

	vector<string> linesOfFile = readFile(infile);

	Model model = Model(linesOfFile);

	float target1[] = { 1, 0, 0, 0 };
	float target2[] = { 100, 0, 0, 4 };
	float target3[] = { 4, 0, 2, 6 };
	float* target[3] = { target1, target2, target3 };

	Matrix s = Matrix(target, 3,4);

	s.print();
	cout << s.get(2,3) << endl;

	/*
	model.scaleVertices(7, 2, 1);
	model.translateVertices(30, 2, 1);
	model.rotateVertices( 4, 5, 6, 63);
	*/
	//model.printModelVertices();
	/*
	model.scaleVertices(1,1,1);
	model.calculateBounds();
	*/

	//model.printModelInfo();
	//writeFile(linesOfFile, outfile);




	system("pause"); // Replace with input loop

	return 1;
}

// Setup for testing matrix multiply
/*
float matrix1[] = {1, 0, 1};
float matrix2[] = {2, 0, 1};
float matrix3[] = {0, 1, 3};
float* matrix[3] = { matrix1, matrix2, matrix3 };

int mwidth = 3;
int mheight = 3;

float other1[] = { 1, 1, 1, 1 };
float other2[] = { 1, 0, 1, 2 };
float other3[] = { 1, 0, 1, 3 };
float* other_matrix[3] = { other1, other2, other3};

int owidth = 4;
int oheight = 3;

float target1[] = {0, 0, 0, 0};
float target2[] = {0, 0, 0, 0};
float target3[] = {0, 0, 0, 0};
float* target[3] = {target1, target2, target3};

Model::matrixMultiply(matrix, mwidth, mheight, other_matrix, owidth, oheight, target);

for (int i = 0; i<mheight; i++) {
for (int j = 0; j < owidth; j++) {
cout << target[i][j] << " ";
}
cout << endl;
}

float matrix1[] = { 1, 0, 1 };
float matrix2[] = { 2, 0, 1 };
float matrix3[] = { 0, 1, 3 };
float* matrix[3] = { matrix1, matrix2, matrix3 };

float vertex[3] = {1, 1, 1};

float res[3];

Model::matrixMultiplyVertex(matrix, 3, 3,vertex, res);

cout << res[0] << " " << res[1] << " " << res[2] << endl;

*/