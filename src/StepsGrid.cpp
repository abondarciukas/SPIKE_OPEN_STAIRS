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

	col = 50;
	row = 2 * nSteps;
	srow = h / row;
	scol = w / col;

	row++;
	col++;

	output.clear();
	output.allocate(h, w, GL_RGB, 0);
	
	for (int y = 0; y < row; y++) {
		for (int x = 0; x < col; x++) {
			pos.push_back(ofPoint(x*scol, y*srow));
			vel.push_back(ofPoint(0, 0));
		}
	}

	stepsGrid.enableIndices();
	stepsGrid.setMode(OF_PRIMITIVE_LINES);
	stepsGrid.addVertices(pos);

	for (int y = 0; y < row; y++) {
		for (int x = 0; x < col; x++) {
			int index = x + col * y;
			stepsGrid.addColor(ofFloatColor(1.0, 1.0, 1.0));
			if (x < col-1) {
				stepsGrid.addIndex(index);
				stepsGrid.addIndex(index+1);
			}
			if (y < row-1) {
				if (x == 0 || x == col-1) {
					stepsGrid.addIndex(index);
					stepsGrid.addIndex(index + col);
				}
			}
		}
	}
}

void StepsGrid::update(vector<ofPoint> &_pts) {

	for (int y = 0; y < row; y++) {
		for (int x = 0; x < col; x++) {
			int index = x + col * y;
			ofPoint tcvel = vel[index];
			ofPoint tcpos = stepsGrid.getVertex(index);

			ofPoint thead = pos[index] - tcpos;
			thead.limit(0.2);
			tcvel.y += thead.y;

			for (int i = 0; i < _pts.size(); i++) {
				float pdist = tcpos.distance(_pts[i]);
				if (pdist < 300.0) {
					ofPoint tv = pos[index] - _pts[i];
					tv.limit(ofMap(pdist, 200.0, 0.0, 0.0, 1.0));
					tcvel.y += tv.y;
				}
			}

			if (x > 0) {
				ofPoint tnpos = stepsGrid.getVertex(index - 1);

				ofPoint tnhead = tcpos - tnpos;
				tnhead.limit(0.5);
				tcvel.y -= tnhead.y;
			}
			if (x < col) {
				ofPoint tnpos = stepsGrid.getVertex(index + 1);
				ofPoint tnhead = tcpos - tnpos;
				tnhead.limit(0.5);
				tcvel.y -= tnhead.y;
			}
			if (y > 0) {
				ofPoint tnpos = stepsGrid.getVertex(index - col);
				ofPoint tnhead = tcpos - tnpos;
				float tdist = tnpos.distance(tcpos);
				if (tdist < srow/2) {
					tnhead.limit(ofMap(tdist, 0.0, srow/2, 0.5, 0.0));
					tcvel.y += tnhead.y;
				}
			}
			if (y < row) {
				ofPoint tnpos = stepsGrid.getVertex(index + col);
				ofPoint tnhead = tcpos - tnpos;
				float tdist = tnpos.distance(tcpos);
				if (tdist < srow/2) {
					tnhead.limit(ofMap(tdist, 0.0, srow/2, 0.5, 0.0));
					tcvel.y += tnhead.y;
				}
			}

			if (tcvel.length() > 0.02) {
				tcvel *= 0.95;
			}
			else {
				tcvel = ofPoint(0, 0);
			}

			stepsGrid.setVertex(index, tcpos + tcvel);

			ofFloatColor c;
			float hue = ofMap(tcvel.length(), 2.0, 0.0, 210.0, 255.0);
			float bright = 255;
			float sat = ofMap(tcvel.length(), 0.6, 0.2, 255, 0);
			c = ofColor::fromHsb(hue, sat, bright);

			stepsGrid.setColor(index, c);

			vel[index].y = tcvel.y;

		}
	}
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
	ofPopStyle();
	ofPopMatrix();
	output.end();
}
