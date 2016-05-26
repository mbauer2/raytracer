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

void tokenizeLine(string line, float* target) {
	float OutputVertices[3];
	
	// conversion here:
	istringstream ss(line);
	copy(
		istream_iterator <float>(ss),
		istream_iterator <float>(),
		OutputVertices
	);

	target[0] = OutputVertices[0];
	target[1] = OutputVertices[1];
	target[2] = OutputVertices[2];
}

void getModelInfo(vector<string> lines, float* info) {
	float numVertices = 0;
	float numFaces = 0;
	float minX = 0;
	float maxX = 0;
	float minY = 0;
	float maxY = 0;
	float minZ = 0;
	float maxZ = 0;

	float xSum = 0;
	float ySum = 0;
	float zSum = 0;

	bool inHeader = true;
	int endHeaderIndex = 0;

	//	std::vector<int>::iterator it;
	//	it = find(lines.begin, lines.end, "end_header");
	//	if (it != lines.end) {
	//	
	//	}

	for (unsigned int i = 0; i < lines.size(); i++) {
		string currString = lines.at(i);
		if (currString.compare("end_header") == 0) {
			endHeaderIndex = i;
			break;
		}

		size_t found = currString.find("element vertex ");
		//cout << "First found: " << found << endl;
		if (found != std::string::npos) {
			currString.replace(found, 15, "");
			numVertices = stof(currString);
			continue;
		}

		found = currString.find("element face ");
		//cout << "Second found: " << found << endl;
		if (found != std::string::npos) {
			currString.replace(found, 13, "");
			numFaces = stof(currString);
			continue;
		}
	}
	cout << "Header end index: " << endHeaderIndex << endl;
	if (endHeaderIndex + 1 + numVertices + numFaces == lines.size() && numVertices > 0) {

		float currVertex[3];

		tokenizeLine(lines.at(endHeaderIndex + 1), currVertex);

		minX = currVertex[0];
		maxX = currVertex[0];
		minY = currVertex[1];
		maxY = currVertex[1];
		minZ = currVertex[2];
		maxZ = currVertex[2];

		//cout << currVertex[1] << endl;

		for (int i = endHeaderIndex + 1; i <= endHeaderIndex + numVertices; i++) {
			tokenizeLine(lines.at(i), currVertex);
			//cout << currVertex[1] << endl;
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
	}
	else {
		// Error: invalid input file
		info[0] = NAN;
		cout << numVertices << endl;
		cout << numFaces << endl;
		cout << endHeaderIndex << endl;
		return;
	}

	info[0] = numVertices;
	info[1] = numFaces;

	info[2] = xSum / numVertices;
	info[3] = ySum / numVertices;
	info[4] = zSum / numVertices;

	info[5] = minX;
	info[6] = maxX;
	info[7] = minY;
	info[8] = maxY;
	info[9] = minZ;
	info[10] = maxZ;

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
	
	float info[11];
	getModelInfo(linesOfFile, info);
	for (int i = 0; i < 11; i++) {
		cout << info[i] << endl;
	}

	//reverseVector(linesOfFile);

	writeFile(linesOfFile, outfile);

	system("pause");

	return 25;
}