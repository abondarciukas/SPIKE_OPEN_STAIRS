#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"

class StepsGrid
{
public:
	StepsGrid();
	~StepsGrid();

	void setup(int nSteps);
	void update(vector<ofPoint> &_pts);
	void draw();

	int w, h, col, row, scol, srow;

	ofMesh stepsGrid;

	vector<ofPoint> pos, vel;

	vector<vector<ofPoint>> posbuf;

	ofFbo output;
};

