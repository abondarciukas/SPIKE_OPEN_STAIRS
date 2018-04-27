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

	int w, h, col, row, layer, posbuffersize, shue, ehue, huestep, interactionmode, sfcol, sfrow, trigcol, trigrow;
	float scol, srow, slayer, sfscol, sfsrow, gradnoisestep, gradnoisespeed, noisestep, noisespeed, trigscol, trigsrow, trigstartcol, trigstartrow;
	vector<ofMesh> stepsGrid, rows;
	ofMesh stepfaces;
	vector<ofPoint> pos, vel, triggerpos;
	deque<deque<deque<ofPoint>>> posbuf;
	ofColor sgc1, sgc2, sgc3, sgc4, gradc1, gradc2, gradc3;
	vector<ofSoundPlayer> player;
	vector<string> tracks;
	vector<int> triggerlife;
	ofFbo output;
};
