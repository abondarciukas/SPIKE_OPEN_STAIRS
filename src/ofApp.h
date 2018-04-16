#pragma once

#include "ofMain.h"
#include "KinectData.h"
#include "FlowingBorders.h"
#include "ofxDelaunay.h"
#include "Footsteps.h"
#include "ofxGui.h"
#include "NoiseTerrain.h"
#include "StepsGrid.h"
#include "ofxSpout2Sender.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit();

		KinectData kinect;
		FlowingBorders borders;
		ofImage footprint;
		ofxDelaunay delaunay;
		vector<ofPoint> points;

		StepsGrid steps;
		ofMesh project;

		ofxVec2Slider corner0, corner1, corner2, corner3;

		ofxFloatSlider kmin, kmax;
		ofxIntSlider koffset;

		ofxPanel gui;

		bool showGui, showKinect;

		ofxSpout2::Sender spout;
};
