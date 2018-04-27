#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

class KinectData
{
public:
	KinectData();
	~KinectData();
	void setup(int id, int map);
	void setClip(int c, int f);
	void update();
	void draw(int x, int y);
	void drawPoints(int x, int y);
	void close();

	ofxKinect kinect;
	ofPixels kinectDepthPixels;
	ofPixels kinectColourPixels;
	ofTexture kinectDepthTexture;
	ofTexture kinectColourTexture;
	ofxCvGrayscaleImage cvDepthGrey;
	ofxCvFloatImage cvDepthFloat;
	ofxCvContourFinder cvContour;
	vector<ofPoint> tp;
	int kinectMap, deviceId, kw, kh, nearClip, farClip, roisx, roisy, roiex, roiey, mx, my, offset;
	float threshMin, threshMax;

};

