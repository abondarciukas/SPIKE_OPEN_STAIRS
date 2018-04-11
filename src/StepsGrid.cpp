#include "StepsGrid.h"

StepsGrid::StepsGrid()
{
}


StepsGrid::~StepsGrid()
{
}

void StepsGrid::setup(int nSteps) {
	w = 787;
	h = 3000;

	col = 2;
	row = 2 * nSteps;
	srow = h / row;
	scol = w;

	output.clear();
	output.allocate(h, w, GL_RGB, 0);
	
	for (int y = 0; y < row+1; y++) {
		for (int x = 0; x < col; x++) {
			pos.push_back(ofPoint(x*scol, y*srow));
			vel.push_back(ofPoint(0, 0));
		}
	}

	stepsGrid.enableIndices();
	stepsGrid.setMode(OF_PRIMITIVE_LINES);
	stepsGrid.addVertices(pos);

	for (int y = 0; y < row+1; y++) {
		for (int x = 0; x < col; x++) {
			int index = x + col * y;
			if (x < col-1) {
				stepsGrid.addIndex(index);
				stepsGrid.addIndex(index+1);
			}
			if (y < row) {
				stepsGrid.addIndex(index);
				stepsGrid.addIndex(index + col);
			}
		}
	}
}

void StepsGrid::update(vector<ofPoint> &_pts) {
	//Get interaction points

	for (int i = 0; i < _pts.size(); i++) {
		for (int p = 0; p < stepsGrid.getNumVertices(); p++) {
			ofPoint pt = stepsGrid.getVertex(p);
			float dist = pt.distance(_pts[i]);
			if (dist < 10.0) {
				ofPoint tv = pt - _pts[i];
				tv.limit(10.0);
				vel[p] = tv;
			}
		}
	}

	for (int p = 0; p < stepsGrid.getNumVertices(); p++) {
		stepsGrid.setVertex(p, stepsGrid.getVertex(p) + vel[p]);
		ofPoint tvel = vel[p];
		if (tvel.length() > 0.01) {
			tvel *= 0.97;
		}
		else {
			tvel = ofPoint(0,0);
		}
		ofPoint tcpos = stepsGrid.getVertex(p);
		ofPoint thead = pos[p] - tcpos;
		thead.limit(0.05);
		tvel += thead;
		if (tcpos.x < 0) {
			tvel.x = 1;
		}
		if (tcpos.x > w) {
			tvel.x = -1;
		}
		if (tcpos.y < 0) {
			tvel.y = 1;
		}
		if (tcpos.y > h) {
			tvel.y = -1;
		}
		vel[p] = tvel;
	}

	//Check if timeout

	//Start going back to original place to form the map again
}

void StepsGrid::draw() {
	output.begin();
	ofBackground(0);
	ofPushMatrix();
	ofRotate(-90);
	ofTranslate(-w, 0);
	ofPushStyle();
	ofSetLineWidth(5);
	ofSetColor(255);
	stepsGrid.draw();
	for (ofPoint p : pos) {
		ofDrawCircle(p, 10);
	}
	ofPopStyle();
	ofPopMatrix();
	output.end();
}
