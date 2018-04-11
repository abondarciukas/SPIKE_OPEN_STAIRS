#include "FlowingBorders.h"



FlowingBorders::FlowingBorders()
{
}


FlowingBorders::~FlowingBorders()
{
}

void FlowingBorders::setup(int c, int r) {
	col = r;
	row = r;

	sx = ofGetWidth() / col;
	sy = ofGetHeight() / row;
	

	for (int y = 0; y < row; y++) {
		ofMesh temp;
		temp.enableColors();
		temp.enableIndices();
		temp.setupIndicesAuto();
		temp.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
		for (int x = 0; x < col; x++) {
			temp.addVertex(ofPoint(x*sx, (y*sy) - 10, 0));
			temp.addColor(ofColor(255,255,255));
			temp.addVertex(ofPoint(x*sx, (y*sy) + 10, 0));
			temp.addColor(ofColor(255,255,255));
		}
		mesh.push_back(temp);
	}
 }

void FlowingBorders::update() {

}

void FlowingBorders::draw() {
	for (ofMesh m : mesh) {
		m.draw();
	}
}
