#include "KinectData.h"



KinectData::KinectData()
{
	
}


KinectData::~KinectData()
{
}

void KinectData::setup(int id, int map) {
	deviceId = id;
	kinectMap = map;

	kw = 640;
	kh = 480;

	mx = 787;
	my = 3000;

	roisx = 0;
	roisy = 215;
	roiex = kw;
	roiey = 265;

	threshMin = 500;
	threshMax = 3600;

	kinect.setDepthClipping(threshMin, threshMax);
	kinect.init(deviceId);
	kinect.open();
}

void KinectData::setClip(int c, int f) {
	nearClip = c;
	farClip = f;

	kinect.setDepthClipping(nearClip, farClip);
}

void KinectData::update() {
	kinect.update();

	kinectDepthPixels = kinect.getDepthPixels();
	kinectColourPixels = kinect.getPixels();

	for (int y = 0; y < kh; y++) {
		for (int x = 0; x < kw; x++) {
			int index = x + kw * y;
			if (x < roisx || x > roiex || y < roisy || y > roiey) {
				kinectDepthPixels.setColor(index,0);
			}
		}
	}

	cvDepthGrey.setFromPixels(kinectDepthPixels);
	//cvDepthGrey.mirror(true, false);
	cvDepthFloat.setFromPixels(cvDepthGrey.getPixels());

	cvContour.findContours(cvDepthGrey, 250, 240000, 10, false, true);

	tp.clear();

	if (cvContour.nBlobs > 0) {
		for (int b = 0; b < cvContour.nBlobs; b++) {

			int px, py, pz, kz;

			ofPoint blob = cvContour.blobs[b].centroid;

			float dist = kinect.getDistanceAt(blob.x, blob.y);

			int cutoff = ofMap(dist, threshMin, threshMax, 320, 50);

			if (blob.x > 320 - cutoff && blob.x < 320 + cutoff) {

				switch (kinectMap) {

				case 0: //Regular mapping
					kz = kinect.getDistanceAt(blob.x, blob.y);
					px = blob.x;
					py = blob.y;
					pz = kz;
					break;

				case 1: //Map Z to X (left to rigt) and Y to X
					kz = ofMap(kinect.getDistanceAt(blob.x, blob.y), threshMin, threshMax, 0, mx);
					px = kz;
					py = blob.x;
					pz = kz;
					break;

				case 2: //Map Z to X (right to left) and Y to X
					kz = ofMap(kinect.getDistanceAt(blob.x, blob.y), threshMin, threshMax, mx, 0);
					px = kz;
					py = blob.x;
					pz = kz;
					break;
					

				case 3: // Flip Y and Z (top to bottom)
					kz = ofMap(kinect.getDistanceAt(blob.x, blob.y), threshMin, threshMax, 0, my);
					px = blob.x;
					py = kz;
					pz = blob.y;
					break;

				case 4: // Flip Y and Z (bottom to top)
					kz = ofMap(kinect.getDistanceAt(blob.x, blob.y), threshMin, threshMax, my, 0);
					px = blob.x;
					py = kz;
					pz = blob.y;
					break;

				}

				tp.push_back(ofPoint(px, py));
			}
		}
	}
}

void KinectData::draw(int x, int y) {
	cvDepthGrey.draw(x, y);
	cvContour.draw(x, y);
}

void KinectData::drawPoints(int x, int y) {
	ofPushMatrix();
	ofTranslate(x, y);
	for (ofVec2f p : tp) {
		ofPushStyle();
		ofSetCircleResolution(4);
		ofSetColor(255);
		ofDrawCircle(p, 5);
		ofPopStyle();
	}
	ofPopMatrix();
}

void KinectData::close() {
	kinect.close();
}
