#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	kinect.setup(0, 4); //Kinect bot to top mapping
	ofSetFrameRate(60);
	ofBackground(0);
	steps.setup(13);

	gui.setup();

	gui.add(kmin.setup("Clip min", kinect.threshMin, 500, 4500));
	gui.add(kmax.setup("Clip max", kinect.threshMax, 500, 4500));
	gui.add(koffset.setup("Kinect offset", 0, -2000, 2000));
	gui.add(corner0.setup("Corner0", ofVec2f(0,0), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight())));
	gui.add(corner1.setup("Corner1", ofVec2f(ofGetWidth(), 0), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight())));
	gui.add(corner2.setup("Corner2", ofVec2f(0, ofGetHeight()), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight())));
	gui.add(corner3.setup("Corner3", ofVec2f(ofGetWidth(), ofGetHeight()), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight())));

	showGui = true;
	showKinect = false;	

	project.enableIndices();
	project.enableTextures();
	project.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

	project.addVertex(ofVec3f(0,0,0));
	project.addVertex(ofVec3f(ofGetWidth(), 0, 0));
	project.addVertex(ofVec3f(0, ofGetHeight(), 0));
	project.addVertex(ofVec3f(ofGetWidth(), ofGetHeight(), 0));

	project.addIndex(0);
	project.addIndex(1);
	project.addIndex(2);
	project.addIndex(3);

	project.addTexCoord(ofVec2f(0, 0));
	project.addTexCoord(ofVec2f(steps.output.getWidth(), 0));
	project.addTexCoord(ofVec2f(0, steps.output.getHeight()));
	project.addTexCoord(ofVec2f(steps.output.getWidth(), steps.output.getHeight()));

}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.offset = koffset;
	kinect.update();

	steps.update(kinect.tp);

	project.setVertex(0, ofVec3f(corner0->x, corner0->y, 0));
	project.setVertex(1, ofVec3f(corner1->x, corner1->y, 0));
	project.setVertex(2, ofVec3f(corner2->x, corner2->y, 0));
	project.setVertex(3, ofVec3f(corner3->x, corner3->y, 0));
}

//--------------------------------------------------------------
void ofApp::draw(){
	steps.draw();
	steps.output.getTextureReference().bind();
	project.draw();
	steps.output.getTextureReference().unbind();

	if (showKinect) {
		kinect.draw(0, 0);
		kinect.drawPoints(0, 0);
	}
	if (showGui) {
		gui.draw();
	}

	spout.sendTexture(steps.output.getTexture(), "Steps Grid");
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'x') {
		showGui = !showGui;
	}
	if (key == 'k') {
		kinect.setClip(kmin, kmax);
	}
	if (key == 's') {
		showKinect = !showKinect;
	}
	if (key == 'f') {
		ofPixels temp;
		steps.output.readToPixels(temp);
		ofSaveImage(temp, "PROJECTION.jpg", OF_IMAGE_QUALITY_BEST);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::exit() {
	kinect.close();
}