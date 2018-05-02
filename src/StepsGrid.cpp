#include "StepsGrid.h"

StepsGrid::StepsGrid()
{
}


StepsGrid::~StepsGrid()
{
}

void StepsGrid::setup(int nSteps, int nLayers) {
	w = 787;
	h = 3000;
	col = 49;
	row = 2 * nSteps;
	layer = nLayers;
	srow = h / row;
	scol = w / col;
	slayer = 1;
	interactionmode = 0; //0 - lines ripple when someone walks by, 1 - TBC, 2 - TBC.
	noisestep = 0.005; //Noise scale, smaller values smoother noise
	noisespeed = 0.00005;
	shue = 0;
	ehue = 220;
	huestep = ehue / layer;
	posbuffersize = 600;
	row++;
	col++;
	layer++;
	sgc1 = ofColor::fromHex(0x0700FF);
	sgc2 = ofColor::fromHex(0xF400FF);
	sgc3 = ofColor::fromHex(0x0C7A4D);
	sgc4 = ofColor::fromHex(0x00F6FE);
	//Gradient stuff:
	gradc1 = ofColor::fromHex(0xFD0047);
	gradc2 = ofColor::fromHex(0x691C71);
	gradc3 = ofColor::fromHex(0x01F4FD);
	gradc1.setSaturation(0);
	gradc2.setSaturation(0);
	gradc3.setSaturation(0);
	gradc1.setBrightness(200);
	gradc2.setBrightness(200);
	gradc3.setBrightness(200);
	sfcol = col-1;
	sfrow = row-1;
	sfscol = w / sfcol;
	sfsrow = h / sfrow;
	sfcol++;
	sfrow++;
	gradnoisestep = 0.0002;
	gradnoisespeed = 0.00005;
	//Sound grid setup
	trigcol = 1;
	trigrow = 10;
	trigscol = w / trigcol;
	trigsrow = h / trigrow;
	trigstartcol = trigscol / 2;
	trigstartrow = trigsrow / 2;
	//Buffer allocation
	output.clear();
	output.allocate(h, w, GL_RGBA);
	for (int y = 0; y < row; y++) {
		for (int x = 0; x < col; x++) {
			pos.push_back(ofPoint(x*scol, y*srow));
			vel.push_back(ofPoint(0, 0));
		}
	}
	//Step outline stuff
	for (int l = 0; l < layer/5; l++) {
		ofMesh tstepsGrid;
		tstepsGrid.enableIndices();
		tstepsGrid.setMode(OF_PRIMITIVE_LINES);
		tstepsGrid.addVertices(pos);
		for (int y = 0; y < row; y++) {
			for (int x = 0; x < col; x++) {
				int index = x + col * y;
				tstepsGrid.addColor(ofFloatColor(1.0, 1.0, 1.0));
				if (x < col - 1) {
					if (l % 3 == 0 && l != 0) {
						if (x % 2 == 0) {
							tstepsGrid.addIndex(index);
							tstepsGrid.addIndex(index + 1);
						}
					}
					else {
						tstepsGrid.addIndex(index);
						tstepsGrid.addIndex(index + 1);
					}

				}
				if (y < row - 1) {
					if(x == 0 || x == col - 1) {
						tstepsGrid.addIndex(index);
						tstepsGrid.addIndex(index + col);
					}
				}
			}
		}
		stepsGrid.push_back(tstepsGrid);
	}
	//Building planes with colour going through Hue range (in Z);
	for (int y = 0; y < row; y++) {
		ofMesh temp;
		temp.setMode(OF_PRIMITIVE_TRIANGLES);
		temp.enableColors();
		temp.enableIndices();
		for (int z = 0; z < layer; z++) {
			for (int x = 0; x < col; x++) {
				int index = x + col * y;
				int hue = shue + z * huestep;
				ofFloatColor tc = ofColor::fromHsb(hue, 255, 255, 255);
				temp.addColor(tc);
				temp.addVertex(ofVec3f(x * scol, y * srow, -slayer * z));
			}
		}
		rows.push_back(temp);
	}
	//Connecting the vertexes to form traingle mesh;
	for (int r = 0; r < row; r++) {
		for (int z = 0; z < layer-1; z++) {
			for (int x = 0; x < col-1; x++) {
				int index = x + col * z;
				//First triangle (current, current+up, current+right)
				rows[r].addIndex(index);
				rows[r].addIndex(index + col);
				rows[r].addIndex(index + 1);
				//Second triangle (current+right, current+layer, current+right+layer);
				rows[r].addIndex(index + 1);
				rows[r].addIndex(index + col);
				rows[r].addIndex(index + col+ 1);
			}
		}
	}
	//Position buffer of vertexes over time. Front layer of the planes' positions are pushed back
	for (int r = 0; r < row; r++) {
		deque<deque<ofPoint>> tl;
		for (int x = 0; x < col; x++) {
			deque<ofPoint> tp;
			for (int t = 0; t < posbuffersize; t++) {
				int index = x + col * r;
				tp.push_back(pos[index]);
			}
			tl.push_back(tp);
		}
		posbuf.push_back(tl);
	}
	//Build stepfaces
	stepfaces.setMode(OF_PRIMITIVE_TRIANGLES);
	stepfaces.enableColors();
	stepfaces.enableIndices();
	//Add vertices
	for (int y = 0; y < sfrow; y++) {
		for (int x = 0; x < sfcol; x++) {
			if (y == 0) {
				stepfaces.addColor(ofColor::black);
			}
			else if (y == 1) {
				stepfaces.addColor(ofColor::white);
			}
			else if (y % 2 == 0) {
				stepfaces.addColor(ofColor::black);
			}
			else {
				stepfaces.addColor(ofColor::white);
			}
			
			stepfaces.addVertex(ofVec3f(x * sfscol, y * sfsrow, 0));
		}
	}
	//Create triangles
	for (int y = 1; y < sfrow-1; y+=2) { 
		for (int x = 0; x < sfcol-1; x++) {
			int index = x + sfcol * y;
			stepfaces.addIndex(index);
			stepfaces.addIndex(index + 1);
			stepfaces.addIndex(index + sfcol);
			stepfaces.addIndex(index + 1);
			stepfaces.addIndex(index + sfcol);
			stepfaces.addIndex(index + sfcol + 1);
		}
	}
	//Sound stuff
	tracks.push_back("SOUND/ADARKPLACE.aif");
	tracks.push_back("SOUND/AFTERTHOUGHT.aif");
	tracks.push_back("SOUND/AIRLOCK.aif");
	tracks.push_back("SOUND/ARCTICGHOST.aif");
	tracks.push_back("SOUND/ASLEEPANDDREAMING.aif");
	tracks.push_back("SOUND/BIGROOMCLAVE.aif");
	tracks.push_back("SOUND/CHOIR.aif");
	tracks.push_back("SOUND/ICEFLOW.aif");
	tracks.push_back("SOUND/SPATIALVIBRATIONS.aif");
	tracks.push_back("SOUND/KUROSHIO.aif");
	tracks.push_back("SOUND/MILLS.aif");
	tracks.push_back("SOUND/MIX01.aif");
	tracks.push_back("SOUND/MIX02.aif");
	tracks.push_back("SOUND/SHIFTING.aif");
	tracks.push_back("SOUND/UNDERSCORE.aif");
	tracks.push_back("SOUND/CHIBA.aif");
	tracks.push_back("SOUND/CORALIX.aif");
	tracks.push_back("SOUND/SYMPHONY.aif");
	for (int y = 0; y < trigrow; y++) {
		for (int x = 0; x < trigcol; x++) {
			ofPoint temp = ofPoint(x * trigscol + trigstartcol, y * trigsrow + trigstartrow);
			triggerpos.push_back(temp);
			triggerlife.push_back(0);
			ofSoundPlayer tplayer;
			tplayer.load(tracks[floor(ofRandom(0, tracks.size()))]);
			player.push_back(tplayer);
		}
	}
}

void StepsGrid::update(vector<ofPoint> &_pts) {
	switch (interactionmode) //Multiple interaction choices (just in case)
	{
	default:
		break;
	case 0: 
		for (int r = 0; r < row; r++) {
			for (int x = 0; x < col; x++) {
				//1. _pts only interact with layer 0
				int index = x + col * r;
				ofPoint tcvel = vel[index];
				ofPoint tcpos = rows[r].getVertex(x);
				//Return to original position
				ofPoint thead = pos[index] - tcpos;
				thead.limit(0.0005);
				tcvel.y += thead.y;
				//Lines ripple when someone walks by
				for (int p = 0; p < _pts.size(); p++) {
					float pdist = tcpos.distance(_pts[p]);
					if (pdist < 250.0) {
						ofPoint tv = pos[index] - _pts[p];
						tv.limit(ofMap(pdist, 250.0, 0.0, 0.0, 0.09));
						tcvel.y += tv.y;
					}
				}
				if (x > 0) {
					ofPoint tnpos = rows[r].getVertex(x - 1);
					ofPoint tnhead = tcpos - tnpos;
					tnhead.limit(0.05);
					tcvel.y -= tnhead.y;
				}
				if (x < col) {
					ofPoint tnpos = rows[r].getVertex(x + 1);
					ofPoint tnhead = tcpos - tnpos;
					tnhead.limit(0.05);
					tcvel.y -= tnhead.y;
				}
				if (r > 0) {
					ofPoint tnpos = rows[r - 1].getVertex(x);
					ofPoint tnhead = tcpos - tnpos;
					float tdist = tnpos.distance(tcpos);
					if (tdist < srow / 2) {
						tnhead.limit(ofMap(tdist, 0.0, srow / 2, 0.5, 0.0));
						tcvel.y += tnhead.y;
					}
				}
				if (r < row - 1) {
					ofPoint tnpos = rows[r + 1].getVertex(x);
					ofPoint tnhead = tcpos - tnpos;
					float tdist = tnpos.distance(tcpos);
					if (tdist < srow / 2) {
						tnhead.limit(ofMap(tdist, 0.0, srow / 2, 0.5, 0.0));
						tcvel.y += tnhead.y;
					}
				}
				tcpos += tcvel;
				tcvel *= 0.996;
				rows[r].setVertex(x, tcpos);
				stepfaces.setVertex(index, tcpos);
				stepsGrid[0].setVertex(index, tcpos);
				vel[index].y = tcvel.y;
				//2. Layer 0 points position goes into posbuffer
				posbuf[r][x].push_front(rows[r].getVertex(x));
				posbuf[r][x].pop_back();
				//3. Rest of the layers are updated from the position buffer 
				for (int l = 1; l < layer; l++) {
					int pbindex = floor((posbuffersize / layer) * l);
					rows[r].setVertex(x + col * l, ofVec3f(posbuf[r][x][pbindex].x, posbuf[r][x][pbindex].y, rows[r].getVertex(x).z));
				}

				for (int l = 1; l < stepsGrid.size(); l++) {
					int pbindex = floor((posbuffersize / stepsGrid.size())*l);
					stepsGrid[l].setVertex(index, posbuf[r][x][pbindex]);
				}
			}
		}
		//Step face gradient stuff:
		for (int y = 0; y < sfrow; y++) {
			for (int x = 0; x < sfcol; x++) {
				int index = x + sfcol * y;
				if (y == 1 || y % 2 != 0) {
					float noiseval = ofNoise(x * sfscol * gradnoisestep, (y * sfsrow * gradnoisestep) + (ofGetElapsedTimeMillis() * gradnoisespeed));
					ofColor tc;
					if (noiseval < 0.5) {
						tc = gradc1;
						tc.lerp(gradc2, ofMap(noiseval, 0.0, 0.5, 0.0, 1.0));
					}
					else {
						tc = gradc2;
						tc.lerp(gradc3, ofMap(noiseval, 0.5, 1.0, 0.0, 1.0));
					}
					stepfaces.setColor(index, tc);
				}
			}
		}
		//Sound triggering
		for (int i = 0; i < triggerpos.size(); i++) {
			ofPoint tpos = triggerpos[i];
			for (int p = 0; p < _pts.size(); p++) {
				float dist = tpos.distance(_pts[p]);
				if (dist < 100 && !player[i].isPlaying()) {
					triggerlife[i] = 1000;
					player[i].load(tracks[floor(ofRandom(0, tracks.size()))]);
					player[i].play();
				}
			}
		}
		break;
	case 1: //Noise flow offset
		for (int r = 0; r < row; r++) {
			for (int x = 0; x < col; x++) {
				//1. _pts only interact with layer 0
				int index = x + col * r;
				ofPoint tcvel = vel[index];
				ofPoint tcpos = rows[r].getVertex(x);
				//Colour noise
				ofColor tc;
				float coffset = ofNoise(tcpos.x * gradnoisestep, (tcpos.y * gradnoisestep) + (ofGetElapsedTimeMillis() * gradnoisespeed));
				if (coffset < 0.25) {
					tc = sgc1;
					tc.lerp(sgc2, ofMap(coffset, 0.0, 0.25, 0.0, 1.0));
				}
				else if (coffset > 0.25 && coffset < 0.5) {
					tc = sgc2;
					tc.lerp(sgc3, ofMap(coffset, 0.25, 0.5, 0.0, 1.0));
				}
				else if (coffset > 0.5 && coffset < 0.75) {
					tc = sgc3;
					tc.lerp(sgc4, ofMap(coffset, 0.5, 0.75, 0.0, 1.0));
				}
				else {
					tc = sgc4;
					tc.lerp(sgc1, ofMap(coffset, 0.75, 1.0, 0.0, 1.0));
				}
				rows[r].setColor(x, tc);
				float offset = ofNoise((tcpos.x * noisestep), (tcpos.y * noisestep) + (ofGetElapsedTimeMillis() * noisespeed));
				if (offset < 0.1 || offset > 0.9) {
					offset = ofMap(offset, 0.0, 1.0, -0.1, 0.1);
				}
				else {
					offset = 0.0;
				}
				//Return to original position
				ofPoint thead = pos[index] - tcpos;
				thead *= 0.00005;
				tcvel.y += thead.y;
				for (int p = 0; p < _pts.size(); p++) {
					float pdist = tcpos.distance(_pts[p]);
					if (pdist < 250.0) {
						ofPoint tv = pos[index] - _pts[p];
						tv.limit(ofMap(pdist, 250.0, 0.0, 0.0, 0.3));
						tcvel.y += tv.y;
					}
				}
				if (x > 0) {
					ofPoint tnpos = rows[r].getVertex(x - 1);
					ofPoint tnhead = tcpos - tnpos;
					tnhead.limit(0.2);
					tcvel.y -= tnhead.y;
				}
				if (x < col) {
					ofPoint tnpos = rows[r].getVertex(x + 1);
					ofPoint tnhead = tcpos - tnpos;
					tnhead.limit(0.2);
					tcvel.y -= tnhead.y;
				}
				if (r > 0) {
					ofPoint tnpos = rows[r - 1].getVertex(x);
					ofPoint tnhead = tcpos - tnpos;
					float tdist = tnpos.distance(tcpos);
					if (tdist < srow / 2) {
						tnhead.limit(ofMap(tdist, 0.0, srow / 2, 2.0, 0.0));
						tcvel.y += tnhead.y;
					}
				}
				if (r < row - 1) {
					ofPoint tnpos = rows[r + 1].getVertex(x);
					ofPoint tnhead = tcpos - tnpos;
					float tdist = tnpos.distance(tcpos);
					if (tdist < srow / 2) {
						tnhead.limit(ofMap(tdist, 0.0, srow / 2, 2.0, 0.0));
						tcvel.y += tnhead.y;
					}
				}
				tcvel += ofPoint(0, offset);
				tcpos += tcvel;
				if (tcvel.length() > 0.0001) {
					tcvel *= 0.975;
				}
				else {
					tcvel = ofPoint(0, 0);
				}
				rows[r].setVertex(x, tcpos);
				stepfaces.setVertex(index, tcpos);
				stepsGrid[0].setVertex(index, tcpos);
				vel[index].y = tcvel.y;
				//2. Layer 0 points position goes into posbuffer
				posbuf[r][x].push_front(rows[r].getVertex(x));
				posbuf[r][x].pop_back();
				//3. Rest of the layers are updated from the position buffer 
				for (int l = 1; l < layer; l++) {
					coffset = ofNoise(tcpos.x * gradnoisestep, (tcpos.y * gradnoisestep) + (ofGetElapsedTimeMillis() * gradnoisespeed) + (l * 10));
					if (coffset < 0.25) {
						tc = sgc1;
						tc.lerp(sgc2, ofMap(coffset, 0.0, 0.25, 0.0, 1.0));
					}
					else if (coffset > 0.25 && coffset < 0.5) {
						tc = sgc2;
						tc.lerp(sgc3, ofMap(coffset, 0.25, 0.5, 0.0, 1.0));
					}
					else if (coffset > 0.5 && coffset < 0.75) {
						tc = sgc3;
						tc.lerp(sgc4, ofMap(coffset, 0.5, 0.75, 0.0, 1.0));
					}
					else {
						tc = sgc4;
						tc.lerp(sgc1, ofMap(coffset, 0.75, 1.0, 0.0, 1.0));
					}
					int pbindex = floor((posbuffersize / layer) * l);
					rows[r].setVertex(x + col * l, ofVec3f(posbuf[r][x][pbindex].x, posbuf[r][x][pbindex].y, rows[r].getVertex(x).z));
					rows[r].setColor(x + col * l, tc);
					stepsGrid[l].setVertex(index, posbuf[r][x][pbindex]);
					
				}
			}
		}
		//Step face gradient stuff:
		for (int y = 0; y < sfrow; y++) {
			for (int x = 0; x < sfcol; x++) {
				int index = x + sfcol * y;
				if (y == 1 || y % 2 != 0) {
					float noiseval = ofNoise(x * sfscol * gradnoisestep, (y * sfsrow * gradnoisestep) + (ofGetElapsedTimeMillis() * gradnoisespeed));
					ofColor tc;
					if (noiseval < 0.5) {
						tc = gradc1;
						tc.lerp(gradc2, ofMap(noiseval, 0.0, 0.5, 0.0, 1.0));
					}
					else {
						tc = gradc2;
						tc.lerp(gradc3, ofMap(noiseval, 0.5, 1.0, 0.0, 1.0));
					}
					stepfaces.setColor(index, tc);
				}
			}
		}
		//Sound triggering
		for (int i = 0; i < triggerpos.size(); i++) {
			ofPoint tpos = triggerpos[i];
			for (int p = 0; p < _pts.size(); p++) {
				float dist = tpos.distance(_pts[p]);
				if (dist < 100 && !player[i].isPlaying()) {
					triggerlife[i] = 1000;
					player[i].load(tracks[floor(ofRandom(0, tracks.size()))]);
					player[i].play();
				}
			}
		}
		break;
	case 2:
		break;
	}
}

void StepsGrid::draw() {
	output.begin();
	ofEnableAlphaBlending();
	ofBackground(0);
	ofPushMatrix();
	ofRotate(-90);
	ofTranslate(-w, 0);
	stepfaces.draw();
	for (ofMesh tm : rows) {
		tm.draw();
	}
	ofPushStyle();
	ofSetLineWidth(2);
	ofSetColor(255);
	for (int i = 0; i < stepsGrid.size(); i++) {
		stepsGrid[i].draw();
	}
	ofPopStyle();
	/*for (int i = 0; i < citiespos.size(); i++) {
		ofPushMatrix();
		ofTranslate(citiespos[i]);
		ofRotate(citiesrot[i]);
		ofPushStyle();
		if (citieslife[i] > 500) {
			ofSetColor(255,ofMap(citieslife[i], 600, 500, 0, 255));
		}
		else if (citieslife[i] < 100) {
			ofSetColor(255,ofMap(citieslife[i], 100, 0, 255, 0));
		}
		else {
			ofSetColor(255,255);
		}
		font.drawString(citiesnames[citiesid[i]], 0, 0);
		ofPopStyle();
		ofPopMatrix();
	}*/
	ofPopMatrix();
	output.end();
}
