#pragma once
#include "ofMain.h"

class Footsteps
{
public:
	Footsteps();
	~Footsteps();
	void setup(float cR);
	void getPoints(vector<ofPoint> tPts);

	float checkRadius;
	vector<ofPoint> head, cPos, pPos;
	vector<int> alive;

	vector<ofPoint> stepsPos, stepsHead;
	vector<int> life;
};

