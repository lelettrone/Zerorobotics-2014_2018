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
Project: ROBOVALL-Cosmosphere3D
Game: CosmoSPHERES
Created by: martina.massucco
Last Modified: 2013-11-06 16:50:05.0
*/
class ZRUser01 : public ZRUser
{
public:

//BEGIN::PAGE::main
	#define color bool
	#define blue false
	#define red true
	
	unsigned int time; // Obviously, this variable indicates the current second.
	ZRState myState; // Data relating to the current state of the satellite.
	color spheres; // Which one SPHERES was randomly assigned to us.
	
	void init() {
		time = 0;
		phase = 0;
		dodock = false;
	}
	
	int phase; // This variable indicate which code fragment in the CASE part of the loop(); function has to be executed. Phases are executed sequentially and they are never repeated. They are not the same phases the game manual talk about.
	float intialMass;	// The mass of the satellite before colliding with debris (we hope we won't collide!).
	bool config2;	// The phase 2 makes some problems, so this variable indicates if we are in phase 2.
	bool dodock;	// Auxiliary variable used for docking.
	void loop() {
		api.getMyZRState(myState);
		switch(phase) {
			case 0: { // Phase 0: Initializing some variables after the creation of the environment
				if(myState[0]<0) {
					spheres = red;
				} else {
					spheres = blue;
				}
				intialMass = game.getMass();
				float debrisPosition[3];
				game.getDebrisLocation(0,debrisPosition);
				config2 = (0.09F<debrisPosition[0]&&debrisPosition[0]<0.11F);
				phase++;
				//break; // The break istruction is pointless becouse we can continue.
			}
	
			case 1: {	// Phase 1: Get to our item avoiding debris
				float distance[3];
				float itemPosition[3] ={((spheres==blue) ? 0.50F : -0.50F),0.65F,0.0F};
				mathVecSubtract(distance,myState,itemPosition,3);
				if((mathVecMagnitude(distance,3)>0.05F)&&time<90) {
					avoidDebris(itemPosition,0.8F,(config2 ? 0.3F : 0.8F));
					float target[3] = {myState[0], myState[1], 0.65F};
					directTowards(target);
					break;	// It pays to write break istructions in the main IF of the phase to avoid inactivity seconds.
				} else {
					phase++;
				}
			}
	
			case 2: { // Phase 2: Stop us
				float itemPosition[3] ={((spheres==blue) ? 0.50F : -0.50F),0.65F,0.0F};
				float distance[3];
				mathVecSubtract(distance,myState,itemPosition,3);
				if(mathVecMagnitude(distance,3)>0.05F) {	// By adding this IF we sort out a problem with the configuration 3.
					goToPoint(itemPosition,0.5F);
					break;
				} else if(mathVecMagnitude(myState+3,3)>(config2 ? 0.004F : 0.05F)) {
					float zeros[3] = {0.0F,0.0F,0.0F};
					api.setVelocityTarget(zeros);
					break;
				} else {
					phase++;
				}
			}
	
			case 3: { // Phase 3: Catch the item
				int item;
				if(spheres==blue) {
					item = 0;
				} else {
					item = 1;
				}
				float itemPosition[3] ={((spheres==blue) ? 0.50F : -0.50F),0.65F,0.0F};
				float distance[3];
				mathVecSubtract(distance,myState,itemPosition,3);
				if((!(game.haveItem(0,item)||game.haveItem(1,item)))&&time<90) {
					if(config2) {	// If the configurations is the number 2 we must execute different istructions.
						if(dodock) {
							dock();
						} else {
							if(mathVecMagnitude(distance,3)>0.025F) { // If we aren't enough close to the item yet?
								goToPoint(itemPosition,0.5F);
							} else if(mathVecMagnitude(myState+3,3)>0.004F) {
								float zeros[3] = {0.0F,0.0F,0.0F};
								api.setVelocityTarget(zeros);
							} else {
								dodock = true;
							}
						}
					} else {	// If the configuration is not the number 2?
						if(mathVecMagnitude(distance,3)>0.05F) { // f we aren't enough close to the item yet?
						goToPoint(itemPosition,0.5F);
						} else {
							float zeros[3] = {0.0F,0.0F,0.0F};
							api.setVelocityTarget(zeros);
							dock();
						}
					}
					break;
				}
				else {
					phase++;
				}
				break;
			}
	
			case 4: { // Phase 4: Reach the position from which we'll shot to the comet and start directing towards it
				if(time<90) {    // This phase lasts until the 90th second.
					float position[3] = {((spheres==blue) ? 0.25F : -0.25F),0.285F,((spheres==blue) ? -0.48F : 0.48F)};
					if(config2&&(spheres==red)) {
						position[1] = 0.25F;
					}
					goToPoint(position,0.6F);
					float cometState[6];
					game.getCometState(0,cometState); // The getCometState(); function returns the position in which the comet will "appear" if it hadn't "appeared" yet.
					directTowards(cometState);
					break;
				} else {
					phase++;
				}
			}
	
			case 5: { // Phase 5: Shot to the comet without moving (scores are better if we don't move).
				if(game.laserShotsRemaining()>0) { // If we still have some "shots" left (becouse otherwise trying to shot is unuserful)?
					float cometState[6];
					game.getCometState(0,cometState);
					cometState[1]-=0.1F;
					directTowards(cometState);
					game.shootLaser();
					float zeros[3] = {0.0F,0.0F,0.0F};
					api.setVelocityTarget(zeros);
					break;
				} else {
					phase++;
				}
			}
	
			case 6: { // Phase 6: Follow the comet
				if((game.getMass()>intialMass)&&!(config2&&(spheres==red))) {	// We follow the comet only if we had hurt some debris (score are better like this). Scores are better like this. If we hurt some debris the mass became higher.
					float cometState[6];
					game.getCometState(0,cometState);
					if(spheres==blue) {
						cometState[0]-=0.17F;
					} else {
						cometState[0]+=0.17F;
					}
					goToPoint(cometState,0.4F);
				}
				break;
			}
		}
		time++;
		mathVecMagnitude(myState+3,3);
		DEBUG(("%d",phase));
	}
	
	void dock() { // This function rotate the SPHERES to perform the docking. Of course, the satellite must be styll and close to the item.
		float round[3] = {((spheres==blue) ? -0.90F : 0.90F),0.0F,0.0F};
		api.setAttitudeTarget(round);
	}
	
	void directTowards(float *target) { // You use this function to direct the X-face of the satellite through a given point. The only one parameter is the position of the point.
		float attTarget[3];
		float satTargetRelVec[3];
		for (int i = 0; i<3; i++) {
			satTargetRelVec[i] = target[i] - myState[i];
			attTarget[i] = satTargetRelVec[i];
		}
		mathVecNormalize(attTarget, 3);
		api.setAttitudeTarget(attTarget);
	}
	
	void goToPoint(float *target,float maximumVelocity) {	// Alternative to the api.setPositionTarget(); The first parameter is the position you would like to get, you use the second parameter to regolate the velocity (and, so, the fuel consumption).
		float distance[3];
		mathVecSubtract(distance,target,myState,3);
		float stoppingDistance = maximumVelocity*maximumVelocity*35.0F;	// 35 is a value obtained empirically.
		if(mathVecMagnitude(distance,3)<stoppingDistance+0.01F) {	// If we need to start curbing?
			api.setPositionTarget(target);	// We use the setPositionTarget(); function, much more stable than a function manually-created.
		} else {	// Otherwise?
			float forza[3] = {0.0F,0.0F,0.0F};	// The default value of the force is 0. Like this if the condition of the below IF the SPHERES move thanks to inertia.
			if(mathVecMagnitude(myState+3,3)<maximumVelocity-0.015F) {	// I evaluate whether speeding up or move thanks to inertia.
				float max = fabsf(distance[0]);	// This auxiliary variable will contain the maximum component of the SPHERES distance between the target position.
				for (int i = 1; i<3; i++) {
					if (fabsf(distance[i])>max) {
						max = fabsf(distance[i]);
				}}
				float vettoreRapporti[3];	// This is an auxiliary vector, useful for the computing of the force vector.
				for (int i = 0; i<3; i++) {	
					vettoreRapporti[i] = distance[i]/max;
				}
				for (int i = 0; i<3; i++) {		// This FOR cicle applies the formula: f[3] = displacement[3] * K.
					forza[i] = vettoreRapporti[i]*0.38F;	// 0.38 is 0.55 divided by the third root of 3, or rather the maximum force each propeller can exert.
			}}
			api.setForces(forza);	// If the satellite must accelerate we'll exert the computed force, else the force will be null.
	}}
	
	void avoidDebris(float *target,float maximumVelocity,float multiplicativeFactor) { // You use this function to reach a given position without colliding with debris (it is not totally safe). The first parameter is the position you want reach; the second parameter is passed as second parameter to the goToPoint(); function (called by this function); the third parameter is an auxiliary value.
		float debrisPosition[3];	// Position of a debris.
		float distanzaDetrito[3];	// Distance between a debris.
		bool superato;	// The value of this variable is true if true if we have already "overcome" the current debris, else its value is false.
		float force[3] = {0.0F,0.0F,0.0F};	// The initial force is 0. If we are fare to debris no force will be exerted.
		for (int i = 0; i<16; i++) {	// This FOR cilcle "through" all the debris one at a time.
			game.getDebrisLocation(i,debrisPosition);
			mathVecSubtract(distanzaDetrito,debrisPosition,myState,3);
			superato = (distanzaDetrito[1]<0);
				if ((mathVecMagnitude(distanzaDetrito,3)<0.35F)&&(!superato)) {
					for (int j = 0; j<3; j++) {
						force[j] = force[j]+(-distanzaDetrito[j]*multiplicativeFactor);
					}
					force[1] = 0.0F; // The Y-component of the force is unuseful.
				}
		}
		api.setForces(force);
		goToPoint(target,maximumVelocity);
	}

//END::PAGE::main

};

ZRUser *zruser01 = new ZRUser01;
