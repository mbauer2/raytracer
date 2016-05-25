#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
			for (int i = 0; i < towrite.size(); i++) {
				myfile << towrite.at(i) << endl;
			}
		}
		myfile.close();
}

void reverseVector(vector<string> &lines) {
	reverse(lines.begin(), lines.end());
}

double* getModelInfo(vector<string> lines) {
	double info[3];

	return info;
}

int main(int argc, char *argv[] )
{
	if (argc != 3) {
		cout << "Wrong number of arguments." << endl;
		cout << argc << endl ;
		system("pause");
		return -1;
	}

	string infile = argv[1];
	string outfile = argv[2];

	vector<string> linesOfFile = readFile(infile);

	for (int i = 0; i < linesOfFile.size(); i++ ) {
		cout << linesOfFile.at(i)<< endl;
	}
	
	//reverseVector(linesOfFile);

	writeFile(linesOfFile, outfile);

	system("pause");
}