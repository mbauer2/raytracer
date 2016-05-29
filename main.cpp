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

int main(int argc, char *argv[] )
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
	model.printModelInfo();
	//writeFile(linesOfFile, outfile);

	system("pause");

	return 1;
}