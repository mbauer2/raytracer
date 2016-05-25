#pragma once
class Model
{
public:

	inline int getLines() { return lines; };
	inline bool getIsNum() { return isPolygon; };

	Model();
	Model(int, bool);
	~Model();

private:
	int lines;
	bool isPolygon;
};

