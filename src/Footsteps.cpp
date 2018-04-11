#include "Footsteps.h"



Footsteps::Footsteps()
{
	
}


Footsteps::~Footsteps()
{
	
}

void Footsteps::setup(float cR) {
	checkRadius = cR;
}

void Footsteps::getPoints(vector<ofPoint> tPts) {
	if (tPts.size() > 0) {
		for (int i = 0; i < tPts.size(); i++) {
			if (tPts.size() > cPos.size()) {
				cout << "tPts > cPos, i = " + ofToString(i) << endl;
				cPos.push_back(tPts[i]);
				cout << "Got points" << endl;
			}
			else if (cPos.size() > pPos.size()) {
				cout << "cPos > pPos" << endl;
				pPos.push_back(cPos[i]);
				head.push_back(ofVec2f(0.0, 0.0));
				alive.push_back(0);
				cout << "Setting Previous Points" << endl;
			}
			else if (cPos.size() == pPos.size() && !alive[i])
			{
				cout << "cpos == pPos" << endl;
				ofVec2f tHead = cPos[i] - pPos[i];
				head[i] = tHead;
				alive[i] = 1;
				cout << "Got heading for points" << endl;
			}
		}
	}

	if (alive.size() == cPos.size()) {
		for (int i = 0; i < cPos.size(); i++) {
			if (alive[i] == 1 && stepsPos.size() < 1)
			{
				stepsPos.push_back(cPos[i]);
				stepsHead.push_back(head[i].normalize());
				life.push_back(100);

				head.erase(head.begin() + i);
				cPos.erase(cPos.begin() + i);
				pPos.erase(pPos.begin() + i);
				alive.erase(alive.begin() + i);
				cout << "Setup new step (start of deque)" << endl;
			}
			else if (stepsPos.size() > 0 && alive[i] == 1) {
				bool intersect = false;
				for (int s = 0; s < stepsPos.size(); s++) {
					if (cPos[i].distance(stepsPos[s]) < checkRadius) {
						intersect = true;
					}
				}
				if (!intersect) {
					stepsPos.push_back(cPos[i]);
					stepsHead.push_back(head[i].normalize());
					life.push_back(100);

					head.erase(head.begin() + i);
					cPos.erase(cPos.begin() + i);
					pPos.erase(pPos.begin() + i);
					alive.erase(alive.begin() + i);
				}
			}
			
		}
	}


	for (int i = 0; i < alive.size(); i++) {
		if (alive[i] == 1) {
			head.erase(head.begin() + i);
			cPos.erase(cPos.begin() + i);
			pPos.erase(pPos.begin() + i);
			alive.erase(alive.begin() + i);
			cout << "Deleting unused points" << endl;
		}
	}

	for (int i = 0; i < stepsPos.size(); i++) {
		if (life[i] > 0) {
			life[i]--;
		}
		else if(life[i] == 0){
			stepsPos.erase(stepsPos.begin() + i);
			stepsHead.erase(stepsHead.begin() + i);
			life.erase(life.begin() + i);
		}
		cout << "Checking if steps are alive" << endl;
	}
}