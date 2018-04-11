#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	kinect.setup(0,2);
	ofSetFrameRate(60);
	ofBackground(0);
	/*footprint.loadImage("FOOTPRINT_01.png");
	footprint.setAnchorPercent(0.5, 0.5);
	float ratio = footprint.getHeight() / 166.6;
	footprint.resize(footprint.getWidth() / ratio, footprint.getHeight() / ratio);
	for (int i = 0; i < 1000; i++) {
		points.push_back(ofPoint(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight())));
	}
	delaunay.reset();
	delaunay.addPoints(points);
	delaunay.triangulate();
	mesh = delaunay.triangleMesh;
	mesh.enableColors();
	for (int i = 0; i < mesh.getNumVertices(); i++) {
		mesh.addColor(ofFloatColor(0.0, 0.0, 0.0));
	}*/
	//steps.setup(120);
	steps.setup(13);

	gui.setup();

	gui.add(kmin.setup("Clip min", 500, 500, 4500));
	gui.add(kmax.setup("Clip max", 2000, 500, 4500));
	gui.add(corner0.setup("Corner0", ofVec2f(0,0), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight())));
	gui.add(corner1.setup("Corner1", ofVec2f(ofGetWidth(), 0), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight())));
	gui.add(corner2.setup("Corner2", ofVec2f(0, ofGetHeight()), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight())));
	gui.add(corner3.setup("Corner3", ofVec2f(ofGetWidth(), ofGetHeight()), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight())));

	showGui = true;
	showKinect = false;

	//terrain.setup(5, 20);

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
	kinect.update();

	//terrain.update(kinect.tp);

	steps.update(kinect.tp);

	project.setVertex(0, ofVec3f(corner0->x, corner0->y, 0));
	project.setVertex(1, ofVec3f(corner1->x, corner1->y, 0));
	project.setVertex(2, ofVec3f(corner2->x, corner2->y, 0));
	project.setVertex(3, ofVec3f(corner3->x, corner3->y, 0));
	/*if (kinect.tp.size() > 0) {
		steps.getPoints(kinect.tp);
	}
	if (steps.stepsPos.size() > 0) {
		for (int i = 0; i < steps.stepsPos.size(); i++) {
			if (steps.life[i] >= 99) {
				wavePos.push_back(steps.stepsPos[i]);
				waveRadius.push_back(0);
				waveStrength.push_back(100);
			}
		}
	}*/
	/*if (kinect.tp.size() > 0) {
		for (int i = 0; i < kinect.tp.size(); i++) {
			if (prevPos.size() < kinect.tp.size()) {
				prevPos.push_front(kinect.tp[i]);
			}
			else {
				ofVec2f curPos = kinect.tp[i];
				if (prevPos.size() == 0 || prevPos.size() < i) {
					prevPos.push_front(curPos);
				}
				else {
					float dist = curPos.distance(prevPos[i]);
					if (dist > 80) {
						wavePos.push_front(kinect.tp[i]);
						waveRadius.push_front(0);
						waveStrength.push_front(100);
						ofVec3f theading = curPos - prevPos[i];
						theading.normalize();
						heading.push_front(theading);
					}
					prevPos[i] = curPos;
				}
			}
		}
	}*/
	//int vertNum = mesh.getNumVertices();
	//for (int i = 0; i < wavePos.size(); i++) {
	//	for (int v = 0; v < vertNum; v++) {
	//		ofVec3f verPos = mesh.getVertex(v);
	//		ofFloatColor cCol = mesh.getColor(v);
	//		float tDist = verPos.distance(wavePos[i]);
	//		if (tDist > waveRadius[i]-20 && tDist < waveRadius[i]+20) {
	//			//cCol = ofFloatColor(1.0);
	//			//verPos.z += ofMap(waveStrength[i], 0, 100, 0, 100);
	//			mesh.setVertex(v,verPos);
	//			cCol = ofFloatColor(ofMap(waveStrength[i], 0.0, 100.0, 0.0, 1.0));
	//			//cCol = ofFloatColor(1.0);
	//		}
	//		mesh.setColor(v, cCol);
	//	}
	//	waveRadius[i]+=10;
	//	waveStrength[i]-=2;
	//	if (waveStrength[i] < 10) {
	//		wavePos.erase(wavePos.begin() + i);
	//		waveRadius.erase(waveRadius.begin() + i);
	//		waveStrength.erase(waveStrength.begin() + i);
	//	}
	//}
	//for (int i = 0; i < vertNum; i++) {
	//	ofFloatColor cCol = mesh.getColor(i);
	//	mesh.setColor(i, cCol.lerp(ofColor::black, 0.2));
	//	ofVec3f verPos = mesh.getVertex(i);
	//	verPos.z = ofLerp(verPos.z, 0, 0.2);
	//	mesh.setVertex(i,verPos);
	//}
}

//--------------------------------------------------------------
void ofApp::draw(){
	//mesh.draw();
	
	//kinect.draw(0, 0);
	//for (int i = 0; i < steps.stepsPos.size(); i++) {
	//	ofPushStyle();
	//	ofSetColor(255, ofMap(steps.life[i], 0, 100, 0, 255));
	//	ofPushMatrix();
	//	ofTranslate(steps.stepsPos[i].x, steps.stepsPos[i].y);
	//	float rot = atan2f(steps.stepsHead[i].y, steps.stepsHead[i].x);
	//	//rot = tanf(sin(heading[i].y) / cos(heading[i].x));
	//	//rot = (rot > 0 ? rot : (2 * PI + rot)) * 360 / (2 * PI);
	//	ofRotate(ofRadToDeg(rot));
	//	footprint.draw(0, 0);
	//	ofPopStyle();
	//	ofPopMatrix();
	//}

	//kinect.drawPoints(0, 0);
	//terrain.draw();
	//terrain.output.getTextureReference().bind();
	steps.draw();
	steps.output.getTextureReference().bind();
	project.draw();
	steps.output.getTextureReference().unbind();
	//terrain.output.getTextureReference().unbind();
	
	//terrain.output.draw(0, 0);
	if (showKinect) {
		kinect.draw(0, 0);
	}
	if(showGui) gui.draw();
	
	
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