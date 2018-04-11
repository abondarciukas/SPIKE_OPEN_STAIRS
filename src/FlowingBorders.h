#pragma once
#include "ofMain.h"

class FlowingBorders
{
public:
	FlowingBorders();
	~FlowingBorders();

	void setup(int c, int r);
	void update();
	void draw();

	int col, row;
	int sx, sy;

	deque<ofVec3f> meshVert;

	deque<ofMesh> mesh;
};

