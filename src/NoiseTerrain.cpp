#include "NoiseTerrain.h"

NoiseTerrain::NoiseTerrain()
{
}


NoiseTerrain::~NoiseTerrain()
{
}

void NoiseTerrain::setup(int nCont, int nStep) {
	w = ofGetHeight();
	h = ofGetWidth();
	shader.load("shader.vert", "shader.frag");
	fbo.clear();
	fbo.allocate(w, h, GL_LUMINANCE, 0);
	output.clear();
	output.allocate(ofGetWidth(), ofGetHeight(), GL_RGB, 0);
	noiseGrey.allocate(w, h);
	int cthresh = 115;
	fbo.begin();
	shader.begin();
	ofRect(0, 0, w, h);
	shader.end();
	fbo.end();
	ofPixels temp;
	fbo.readToPixels(temp, 0);
	for (int i = 0; i < nCont; i++) {
		noiseGrey.setFromPixels(temp);
		noiseGrey.threshold(cthresh);
		noiseConts.findContours(noiseGrey, 500, 400000, 50, false, true);
		for (int c = 0; c < noiseConts.nBlobs; c++) {
			ofMesh temp;
			temp.enableColors();
			temp.enableIndices();
			temp.enableNormals();
			temp.setMode(OF_PRIMITIVE_LINE_LOOP);
			temp.addVertices(noiseConts.blobs[c].pts);
			terrain.push_back(temp);

			vector<ofPoint> tpos, tvel;
			tpos = noiseConts.blobs[c].pts;
			tvel.assign(noiseConts.blobs[c].nPts, ofPoint(0, 0));
			pos.push_back(tpos);
			vel.push_back(tvel);
		}
		cthresh += nStep;
	}

}

void NoiseTerrain::update(vector<ofPoint> &_pts) {
	//Get interaction points

	for (int i = 0; i < _pts.size(); i++) {
		for (int t = 0; t < terrain.size(); t++) {
			for (int p = 0; p < terrain[t].getNumVertices(); p++) {
				ofPoint pt = terrain[t].getVertex(p);
				float dist = pt.distance(_pts[i]);
				if (dist < 10.0) {
					ofPoint tv = pt - _pts[i];
					tv.limit(10.0);
					vel[t][p] = tv;
				}
			}
		}
	}

	for (int t = 0; t < terrain.size(); t++) {
		for (int p = 0; p < terrain[t].getNumVertices(); p++) {
			terrain[t].setVertex(p, terrain[t].getVertex(p) + vel[t][p]);
			ofPoint tvel = vel[t][p];
			if (tvel.length() > 0.01) {
				tvel *= 0.97;
			}
			else {
				tvel = ofPoint(0,0);
			}
			ofPoint tcpos = terrain[t].getVertex(p);
			ofPoint thead = pos[t][p] - tcpos;
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
			vel[t][p] = tvel;
		}
	}

	//Check if timeout

	//Start going back to original place to form the map again
}

void NoiseTerrain::draw() {
	output.begin();
	ofBackground(0);
	ofPushMatrix();
	ofRotate(-90);
	ofTranslate(-ofGetHeight(), 0);
	for (ofMesh m : terrain) {
		ofPushStyle();
		ofSetLineWidth(1);
		ofSetColor(255);
		m.draw();
		ofPopStyle();
	}
	ofPopMatrix();
	output.end();
}
