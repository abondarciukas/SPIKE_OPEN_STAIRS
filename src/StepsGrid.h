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

	int w, h, col, row, layer, scol, srow, slayer, posbuffersize, shue, ehue, huestep, interactionmode;

	ofMesh stepsGrid;

	vector<ofMesh> rows;

	vector<ofPoint> pos, vel;

	deque<deque<deque<ofPoint>>> posbuf;

	ofFbo output;

	//Step faces gradients and hue shift noise params

	ofMesh stepfaces;

	int sfcol, sfrow, sfscol, sfsrow;

	float gradnoisestep, gradnoisespeed;

	//------------------------------------------------

	

	//Noise parameters (global)

	float noisestep, noisespeed;

	//------------------------------------------------
};


/*

To-do list:

1. Add colour ways to GUI (multiple colour variations on the fly)
2. Add multiple white lines to highlight different colours, like topographical map
3. Make ripples lat longer, perhaps slower at the top, faster at the bottom
4. Limit step face colour shifts (3-4 main colours)
5. Add noise to the outline of the ripple, it looks too much like dropplet
6. Add subtle movement/noise to holding state

*/
