#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"

class NoiseTerrain
{
public:
	NoiseTerrain();
	~NoiseTerrain();

	void setup(int nCont, int nStep);
	void update(vector<ofPoint> &_pts);
	void draw();

	int w, h;

	vector<ofMesh> terrain;

	ofShader shader;
	ofFbo fbo;
	ofxCvGrayscaleImage noiseGrey;
	ofxCvContourFinder noiseConts;

	vector<vector<ofPoint>> pos, vel;

	ofFbo output;
};

