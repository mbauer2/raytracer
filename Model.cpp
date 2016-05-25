#include "Model.h"
#include <iostream>



using namespace std;

Model::Model()
{
	this->lines = 0;
	this->isPolygon = false;

}

Model::Model(int lines, bool isPoly)
{
	this->lines = lines;
	this->isPolygon = isPoly;

}


Model::~Model()
{
	cout << "Model is being deleted." << endl;

}
