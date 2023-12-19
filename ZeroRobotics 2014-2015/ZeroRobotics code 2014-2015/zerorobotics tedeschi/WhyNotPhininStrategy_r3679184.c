#include <string.h>
#include <math.h>
#include "ZRGame.h"
#include "ZR_API.h"
#include "spheres_types.h"
#include "spheres_constants.h"
#include "ctrl_attitude.h"
#include "ctrl_position.h"
#include "find_state_error.h"
#include "math_matrix.h"
#include "ZRUser.hpp"

#undef ZRSIMULATION

static ZeroRoboticsGame &game = ZeroRoboticsGame::instance();
static ZeroRoboticsAPI &api = ZeroRoboticsAPI::instance();

/*
Project: WhyNotPhininStrategy
Game: CosmoSPHERES
Created by: martina.massucco
Last Modified: 2013-11-10 14:01:38.0
*/
class ZRUser01 : public ZRUser
{
public:

//BEGIN::PAGE::main
	#define color bool
	#define blue false
	#define red true
	
	
	unsigned int time; 
	ZRState myState; 
	color spheres; 
	int phase; 
	float debrisId;
	float debrisIndex;
	float debrisLocation[3];
	bool side;
	
	void init(){
		time = 0;
		phase = 0;
		debrisIndex = 0;
		side = false;
	}
	
	void loop(){
		api.getMyZRState(myState);
		switch(phase) {
			case 0: { // Phase 0: Initializing some variables after the creation of the environment
				if(myState[0]<0) {
					spheres = red;
				} else {
					spheres = blue;
				}
				phase++;
			}
			case 1: { //We get only two debris
				if(spheres==blue) {
					debrisId = debrisIndex*2;
				} else {
					debrisId = debrisIndex*2+1;
				}
				if(game.haveDebris(0, debrisId)) {
					if (debrisIndex == 4){
						phase++;
					} else {
						debrisIndex = 4;
					}
				} else {
					game.getDebrisLocation(debrisId, debrisLocation);
					api.setPositionTarget(debrisLocation);
				}
				break;	
			}
	
			case 2:{ //go to the item
				float itemPosition[3] ={((spheres==blue) ? 0.50F : -0.50F),0.65F,0.0F};
				float distance[3];
				mathVecSubtract(distance,myState,itemPosition,3);
				if(mathVecMagnitude(distance,3)>0.05F) {	
					api.setPositionTarget(itemPosition);
					break;
				} else if(mathVecMagnitude(myState+3,3) >  0.01F) {
					float zeros[3] = {0.0F,0.0F,0.0F};
					api.setVelocityTarget(zeros);
					break;
				} else {
					phase++;
				}
			}
	
			case 3: { //take the item
				float itemPosition[3] ={((spheres==blue) ? 0.50F : -0.50F),0.65F,0.0F};
				int item;
				if(spheres==blue) {
					item = 0;
				} else {
					item = 1;
				}	
				api.setPositionTarget(itemPosition);
				if((!(game.haveItem(0,item)||game.haveItem(1,item)))&&time<90){
					dock();
				} else {
					phase++;
				}
				break;
			}
			case 4: {//direct towards the comet
				float shotPoint[3] = {((spheres == blue) ? 0.40F : -0.40F), 0.50F, ((spheres == blue) ? 0.25 : -0.25) };
				float cometState[3];
				game.getCometState(0,cometState);
				if (time < 90){
					api.setPositionTarget(shotPoint);
				} else{
					phase++;
				}
					directTowards(cometState);
					break;
				}
	
		case 5: {
				float cometState[6];
				game.getCometState(0,cometState);
				if(game.laserShotsRemaining()>0) {  //shoot the comet and get closer
					cometState[1]-=0.1F;
					directTowards(cometState);
					game.shootLaser();
					cometState[0] = (spheres == blue) ? cometState[0] + 0.15F :  cometState[0] - 0.1F;
					cometState[1] = (spheres == blue) ? cometState[1] + 0.15F : cometState[1] + 0.1F;
					cometState[2] =  (spheres == blue) ? cometState[2] - 0.1F :  cometState[2] + 0.1F;
					//these are the better points we found in which shot the laser
					api.setPositionTarget(cometState);
				} else if (!side){ //if we are behind the comet, go to the side of it
					if (spheres == blue) {
						/*cometState[1] = cometState[1] - 0.1F; //for the blue sphere we don't need this passage 
						cometState[2] = cometState[2] - 0.1F;		// go behind the comet
						if (myState[0] <= cometState[0] - 0.02F){*/
							side = true;
						//}
					GoToPoint(cometState);
					} else {
						cometState[1] = cometState[1] + 0.08F; //go behind the comet
						cometState[2] = cometState[2] + 0.1F;
						if (myState[0] <= cometState[0] + 0.02F){ //sometimes the sphere hits the comet
							side = true;
						}
						GoToPoint(cometState); //
					}
					
				} else { // go to the side of the sphere
					if (spheres == blue) { 
						cometState[0] = cometState[0] - 0.15F;
						cometState[2] = cometState[2] - 0.1F;
					} else {
						cometState[0] = cometState[0] + 0.15F;
						cometState[2] = cometState[2] + 0.1F;
					}
					GoToPoint(cometState);
				}
			
			DEBUG(("%d",side));
				}
		}
		time++;
	}
	
	void dock() { 
		float round[3] = {((spheres==blue) ? -0.90F : 0.90F),0.0F,0.0F};
		api.setAttitudeTarget(round);
	}
	
	void directTowards(float *target) { 
		float attTarget[3];
		float satTargetRelVec[3];
		for (int i = 0; i<3; i++) {
			satTargetRelVec[i] = target[i] - myState[i];
			attTarget[i] = satTargetRelVec[i];
		}
		mathVecNormalize(attTarget, 3);
		api.setAttitudeTarget(attTarget);
	}
	
	void GoToPoint(float target[])
	{
		float velocity[3];
		float slow = 0.3f;
		float displacement[3];
		int i;
		api.getMyZRState(myState);
		for(i=0;i<3;i++){
			displacement[i] = target[i] - myState[i];
		}
		mathVecSubtract(velocity, displacement, myState + 3, 3);
		for(i=0;i<3;i++){
		velocity[i] = velocity[i] * slow;
		}
		api.setVelocityTarget(velocity);
	}

//END::PAGE::main

};

ZRUser *zruser01 = new ZRUser01;
