#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"

class StepsGrid
{
public:
	StepsGrid();
	~StepsGrid();

	void setup(int nSteps, int nLayers);
	void update(vector<ofPoint> &_pts);
	void draw();

	int w, h, col, row, layer, scol, srow, slayer, posbuffersize;

	int shue, ehue, huestep;

	ofMesh stepsGrid;

	vector<ofMesh> rows;

	vector<ofPoint> pos, vel;

	vector<vector<vector<ofPoint>>> posbuf;

	ofFbo output;
};

