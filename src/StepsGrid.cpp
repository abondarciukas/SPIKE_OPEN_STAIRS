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

	col = 20;
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
	//Get interaction points

	/*for (int i = 0; i < _pts.size(); i++) {
		for (int p = 0; p < stepsGrid.getNumVertices(); p++) {
			ofPoint pt = stepsGrid.getVertex(p);
			float dist = pt.distance(_pts[i]);
			if (dist < 100.0) {
				ofPoint tv = pt - _pts[i];
				tv.limit(ofMap(dist, 100.0, 0.0, 1.0, 10.0));
				vel[p] = tv;
			}
		}
	}*/

	for (int p = 0; p < stepsGrid.getNumVertices(); p++) {
		ofPoint tvel = vel[p];
		ofPoint tcpos = stepsGrid.getVertex(p);

		for (int i = 0; i < _pts.size(); i++) {
			float pdist = tcpos.distance(_pts[i]);
			if (pdist < 100.0) {
				ofPoint tv = tcpos - _pts[i];
				tv.limit(ofMap(pdist, 100.0, 0.0, 1.0, 2.0));
				tvel += tv;
			}
		}

		//Set vertex colour
		ofFloatColor c;
		float hue = ofMap(tvel.length(), 10.0, 0.0, 0.0, 255);
		float bright = 255;
		float sat = ofMap(tvel.length(), 2.0,0.0,255,0);
		c = ofColor::fromHsb(hue, sat, bright);

		stepsGrid.setColor(p, c);

		ofPoint thead = pos[p] - tcpos;
		thead.limit(1.0);
		tvel += thead;
		
		for (int np = 0; np < stepsGrid.getNumVertices(); np++) {
			ofPoint npos = stepsGrid.getVertex(np);
			float ndist = npos.distance(tcpos);
			if (np != p && ndist < 120.0) {
				ofPoint nhead = tcpos - npos;
				nhead.limit(ofMap(ndist, 120.0, 0.0, 0.0, 1.0));
				tvel += nhead;
			}
		}
		//Boundary conditions
		/*if (tcpos.x < 0) {
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
		*/

		if (tvel.length() > 0.01) {
			tvel *= 0.965;
		}
		else {
			tvel = ofPoint(0, 0);
		}

		stepsGrid.setVertex(p, stepsGrid.getVertex(p) + tvel);

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
	ofPopStyle();
	ofPopMatrix();
	output.end();
}
