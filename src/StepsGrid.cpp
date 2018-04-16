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

	col = 40;
	row = 2 * nSteps;
	layer = nLayers;
	srow = h / row;
	scol = w / col;
	slayer = layer;

	shue = 40; //FFEA0D
	ehue = 160; //0F74FF
	huestep = (ehue - shue) / layer;

	posbuffersize = 100;

	row++;
	col++;
	layer++;

	output.clear();
	output.allocate(h, w, GL_RGB, 0);
	
	for (int y = 0; y < row; y++) {
		for (int x = 0; x < col; x++) {

			pos.push_back(ofPoint(x*scol, y*srow));
			vel.push_back(ofPoint(0, 0));

		}
	}

	/*stepsGrid.enableIndices();
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
	}*/

	//Building planes with colour going through Hue range (in Z);
	for (int y = 0; y < row; y++) {

		ofMesh temp;
		temp.setMode(OF_PRIMITIVE_TRIANGLES);
		temp.enableColors();
		temp.enableIndices();

		for (int z = 0; z < layer; z++) {
			for (int x = 0; x < col; x++) {

				int index = x + col * y;

				int hue = shue - (z * huestep);
				if (hue < 0) {
					hue = 255 - z * huestep;
				}
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

	for (int r = 0; r < row; r++) {
		vector<vector<ofPoint>> tl;
		for (int t = 0; t < posbuffersize; t++) {
			vector<ofPoint> tp;
			for (int x = 0; x < col; x++) {
				int index = x + col * r;
				tp.push_back(pos[index]);
			}
			tl.push_back(tp);
		}
		posbuf.push_back(tl);
	}

	
}

void StepsGrid::update(vector<ofPoint> &_pts) {

	//StepsGrid mesh
	/*for (int y = 0; y < row; y++) {
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
	}*/

	//New rows-planes update routine (re-write from scratch)
	for (int r = 0; r < row; r++) {
		for (int l = 0; l < layer; l++) {
			for (int x = 0; x < col; x++) {

				int index = x + col * r;
				ofPoint tcvel = vel[index];
				ofPoint tcpos = rows[r].getVertex(x);

				ofPoint thead = pos[index] - tcpos;
				thead.limit(0.2);
				tcvel.y += thead.y;

				//1. _pts only interact with layer 0
				if (layer == 0) {
					for (int p = 0; p < _pts.size(); p++) {
						float pdist = tcpos.distance(_pts[p]);
						if (pdist < 300.0) {
							ofPoint tv = pos[index] - _pts[p];
							tv.limit(ofMap(pdist, 200.0, 0.0, 0.0, 1.0));
							tcvel.y += tv.y;
						}
					}

					if (x > 0) {
						ofPoint tnpos = rows[r].getVertex(x - 1);

						ofPoint tnhead = tcpos - tnpos;
						tnhead.limit(0.5);
						tcvel.y -= tnhead.y;
					}
					if (x < col) {
						ofPoint tnpos = rows[r].getVertex(x + 1);
						ofPoint tnhead = tcpos - tnpos;
						tnhead.limit(0.5);
						tcvel.y -= tnhead.y;
					}
					if (r > 0) {
						ofPoint tnpos = rows[r-1].getVertex(x);
						ofPoint tnhead = tcpos - tnpos;
						float tdist = tnpos.distance(tcpos);
						if (tdist < srow / 2) {
							tnhead.limit(ofMap(tdist, 0.0, srow / 2, 0.5, 0.0));
							tcvel.y += tnhead.y;
						}
					}
					if (r < row) {
						ofPoint tnpos = rows[r+1].getVertex(x);
						ofPoint tnhead = tcpos - tnpos;
						float tdist = tnpos.distance(tcpos);
						if (tdist < srow / 2) {
							tnhead.limit(ofMap(tdist, 0.0, srow / 2, 0.5, 0.0));
							tcvel.y += tnhead.y;
						}
					}

					if (tcvel.length() > 0.02) {
						tcvel *= 0.95;
					}
					else {
						tcvel = ofPoint(0, 0);
					}

					rows[r].setVertex(x, tcpos + tcvel);

					vel[index].y = tcvel.y;

					//2. Layer 0 points position goes into posbuffer

					posbuf[r][x].insert(posbuf[r][x].begin(), rows[r].getVertex(x));
					posbuf[r][x].pop_back();
				}	
				else { //3. Rest of the layers are updated from posbuffer
					int index = x + col * l;
					int bufpos = floor(posbuffersize / layer);

					rows[r].setVertex(index, ofVec3f(posbuf[r][x][bufpos].x, posbuf[r][x][bufpos].x, layer * -slayer));
				}
			}
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
	for (ofMesh tm : rows) {
		tm.draw();
	}
	/*stepsGrid.draw();*/
	ofPopStyle();
	ofPopMatrix();
	output.end();
}
