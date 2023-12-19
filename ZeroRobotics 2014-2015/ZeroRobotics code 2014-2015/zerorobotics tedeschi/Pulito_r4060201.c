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
Project: Pulito
Game: CosmoSPHERES_ALLIANCE
Created by: anesthetize
Last Modified: 2013-12-20 20:14:13.0
*/
class ZRUser01 : public ZRUser
{
public:

//BEGIN::PAGE::Comet
	#define POSX -0.3
	#define POSY 0.5
	#define POSZ 0.05
	float pos[3];
	float constant;
	
	void shoot(){
		if (time == 90){
			cometState[0] = 0.01*(90-time)*player;
			cometState[1] = 0.8+0.026*(90-time);
			cometState[2] = -0.01*(90-time)*player;
		}
		
		
			pos[0] = POSX * player;    // -0.3  0.05
			pos[1] = POSY;            // 0.5   0.4
			pos[2] = POSZ * player;  // 0.05  -0.5
			api.setPositionTarget(pos);
		
		
		float d = getDistance(myState, cometState);
		//DEBUG(("%f", d));
		float dir[3];
			dir[0] = myState[6];
			dir[1] = myState[7];
			dir[2] = myState[8];
	
		mathVecNormalize(dir, 3);
	
		dir[0] *= d;
		dir[1] *= d;
		dir[2] *= d;
	
		mathVecAdd(dir, dir, myState, 3);
	
		att[0]=cometState[0]-myState[0] + constant*(cometState[3]-myState[3]);	//3.5
		att[1]=cometState[1]-myState[1] + constant*(cometState[4]-myState[4]);	//3.5
		att[2]=cometState[2]-myState[2] + constant*(cometState[5]-myState[5]);	//3.5
		api.setAttitudeTarget(att);
	
		if(getDistance(dir,cometState)<0.05f && time > 91) {
				game.shootLaser();
				DEBUG(("Shots Remaining: %d", game.laserShotsRemaining()));
			}
		
		if(90<time && time <100){
			precise += getDistance(dir,cometState);
		}
		
		
		if (time > 90) {
			float velocity[3];
	
			velocity[0] = -myState[3];
			velocity[1] = -myState[4];
			velocity[2] = -myState[5];
			api.setVelocityTarget(velocity);
			
			}
		
		
	}
	
	
	void comet(){
		setConstantVelocityTo(enemyState, 0.1);
	}

//END::PAGE::Comet
//BEGIN::PAGE::Function
	float getDistance(float point1[], float point2[]){
		
		float result[3];
		mathVecSubtract(result, point2, point1, 3);
		
		return mathVecMagnitude(result, 3);
		
	}
	
	       
	float max(float x, float y){
		return x > y ? x : y;
	}
	
	       
	float min(float x, float y){
		return x < y ? x : y;
	}
	    
	
	float abs(float number){
		
		if(number>0)
			return number;
		else{
			number=-number;
			return number;
		}
		
	}
	
	
	void setConstantVelocityTo(float point[], float k){
	
		float velocityVector[3];
	
		mathVecSubtract(velocityVector, point, myState, 3);
		mathVecNormalize(velocityVector, 3);
	
		velocityVector[0] *= k;	
		velocityVector[1] *= k;
		velocityVector[2] *= k;
	
		api.setVelocityTarget(velocityVector);
	
	}

//END::PAGE::Function
//BEGIN::PAGE::main
	// PULITO
	
	ZRState myState;
	ZRState enemyState;
	ZRState cometState;
	int phase;
	int time;
	int player;
	float att[3];
	float precise;
	bool haveDebris;
	
	void init(){
		
	  phase = 0;
	  time  = 0;
	  player = 0;
		add = 0.0f;
		costant = 3.85F;
		
	}
	
	
	void loop(){	
		
		time++;
		api.getMyZRState(myState);	
		api.getOtherZRState(enemyState);
		game.getCometState(0, cometState);
		
		switch (phase){
			case 0:
							player = myState[0] > 0.0f ? 1 : -1;
							itempos[0] = 0.5*player;
							itempos[1] = 0.65;
							itempos[2] = 0;
							DEBUG(("We are %s", player ? "Blue" : "Red"));
							phase++;
			break;
			
			case 1:
							getItem(itempos);
							if (game.haveItem(0,0.5-0.5*player))
								phase++;
			break;
			
			case 2:
							shoot();
			break;
	
		}
	
	}
	

//END::PAGE::main
//BEGIN::PAGE::main
	ZRState myState;
	ZRState enemyState;
	ZRState cometState;
	int phase;
	int time;
	int player;
	int hit;
	float att[3];
	float center[3];
	float precise;
	bool haveDebris;
	
	void init(){
		hit = 0;
	  phase = 0;
	  time  = 0;
	  player = 0;
		add = 0.0f;
		constant = 3.85F;
		center[0] = 0; center[1] = 0; center[2] = 0;
	
	}
	
	
	void loop(){	
		
		time++;
		api.getMyZRState(myState);	
		api.getOtherZRState(enemyState);
		game.getCometState(0, cometState);
		
		switch (phase){
			case 0:
							player = myState[0] > 0 ? 1 : -1;
							itempos[0] = 0.5*player;
							itempos[1] = 0.65;
							itempos[2] = 0;
							DEBUG(("We are %s", player ? "Blue" : "Red"));
							game.breakNet();
							phase++;
			break;
			
			case 1:
							getItem(itempos);
							if (game.haveItem(0,0.5-0.5*player))
								phase++;
			break;
			
			case 2:
							shoot();
			break;
			
		}
	
	
		// PROTECTION FROM GOING OUT	
		if ( myState[0] <= -0.52 || myState[0] >= 0.52 ) api.setPositionTarget(center);
		if ( myState[1] <= -0.68 || myState[1] >= 0.68 ) api.setPositionTarget(center);
		if ( myState[2] <= -0.52 || myState[2] >= 0.52 ) api.setPositionTarget(center);
		
		
	
	}

//END::PAGE::main
//BEGIN::PAGE::Way To Item
	float add;
	float itempos[3];
	
	void setZAttRate(float z){
		
		float attitudeRate[3] = {0.0f, 0.0f, z};
	
		api.setAttRateTarget(attitudeRate);
		
	}
	
	void way (float* target){
	
		float flyto[3];
		float deloc[3];
	
		mathVecSubtract(flyto, target, myState, 3);
		mathVecNormalize(flyto, 3);
		flyto[0] = 0.4*flyto[0];
		flyto[1] = 0.4*flyto[1];
		flyto[2] = 0.4*flyto[2];
	
		for(int i=0; i<16; i++){
			float xflyto[3];
	
			mathVecSubtract(xflyto, target, myState, 3);
			mathVecNormalize(xflyto, 3);
			xflyto[0] = 0.2*xflyto[0];
			xflyto[1] = 0.2*xflyto[1];
			xflyto[2] = 0.2*xflyto[2];
			mathVecAdd(xflyto, xflyto, myState, 3);
	
			if(getDistance(xflyto, deloc)<0.18){
				if(xflyto[2]<deloc[2])
					add = -0.4+getDistance(xflyto, deloc);
				else
					add = 0.4-getDistance(xflyto, deloc);
				break;
			}
	
			game.getDebrisLocation(i, deloc);
	
			mathVecSubtract(xflyto, target, myState, 3);
			mathVecNormalize(xflyto, 3);
			xflyto[0] = 0.4*xflyto[0];
			xflyto[1] = 0.4*xflyto[1];
			xflyto[2] = 0.4*xflyto[2];
			mathVecAdd(xflyto, xflyto, myState, 3);
	
			if(getDistance(xflyto, deloc)<0.11){
				if(xflyto[2] < deloc[2])
					add = -0.3+getDistance(xflyto, deloc);
				else
					add = 0.3-getDistance(xflyto, deloc);
				break;
			}
	
			if(getDistance(myState, deloc)<0.18){
				if(myState[2]<deloc[2])
					add = -0.3+getDistance(myState, deloc);
				else
					add = 0.3-getDistance(myState, deloc);
				break;
			}
	
		}
	
		flyto[2] += add;
	
		if(add>0)
			add -= 0.025;
		else
			add += 0.025;
	
		if(getDistance(myState,target)<0.4)
			api.setPositionTarget(target);
		else{
			mathVecAdd(flyto, myState, flyto, 3);
			setConstantVelocityTo(flyto,0.04);
		}
		
	}
	
	
	int counterForCollectItem;
	
	void getItem(float* position){
	
		way(itempos);
		if( getDistance(myState, position) < 0.045f && mathVecMagnitude(&myState[3], 3) < 0.01f){
			if(counterForCollectItem > 10 && counterForCollectItem < 20)
				setZAttRate(0.0f);
			else
				setZAttRate(0.4f);
	
			counterForCollectItem++;
		}
		else{
			counterForCollectItem = 0;
			setZAttRate(0.0f);
		}
		
	}

//END::PAGE::Way To Item

};

ZRUser *zruser01 = new ZRUser01;
