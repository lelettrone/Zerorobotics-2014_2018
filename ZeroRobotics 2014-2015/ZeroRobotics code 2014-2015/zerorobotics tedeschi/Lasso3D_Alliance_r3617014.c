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
Project: Lasso3D_Alliance
Game: CosmoSPHERES
Created by: katze
Last Modified: 2013-11-03 19:49:46.0
*/
class ZRUser01 : public ZRUser
{
public:

//BEGIN::PAGE::Comet
	float pos[3];
	
	void comet()
	{
		
	 if (time<85)
	 {
			// if we still got time, the satellite goes to the comet's entrance position
		  pos[0] = -0.15*player; 
		  pos[1] = 0.8;
		 	pos[2] = 0;
		  api.setPositionTarget(pos);
	 }
	 else
	 {
			// follows the comet with some distance
		 	cometState[0] = cometState[0] - 0.15 * player;
		 	cometState[1] = cometState[1] - 0.35;
			cometState[2] = cometState[2] - 0.1 * player;
		 api.setPositionTarget(cometState);
	 }
	}
	

//END::PAGE::Comet
//BEGIN::PAGE::Debris
	void collect(int nr)
	{
		game.getDebrisLocation(nr, loc);
		
		// if we have not reacher the debris yet
		if(myState[1]+0.05 <= loc[1])
		{
			loc[1] = myState[1];
		  loc[0] += 0.17f;
			// if we are far away, we fly to it
		  if (getDistance(myState, loc) >= 0.05f)
		  {
			  api.setPositionTarget(loc);
		  }
			
			// if we are close we fly a tangent
		  else
		  {
			  loc[1] = 0.0f;
			  setConstantVelocityTo(loc, 0.05);
		  }
		}
		
		// sets forces to the middle of the debris while flying a tangent if we are close enough
		// with this V we will fly in a circle around it, just like a satellite :) 
		else
		{
		  game.startLasso(nr);
			mathVecSubtract(loc, loc, myState, 3);
			api.setForces(loc);
		}
		
		// if we have collected the first debris (0 or 1), we try to collect the next on our side
		if (game.haveDebris(0,debrisID) ||game.haveDebris(1,debrisID) )
		{
			debrisID++;
			debrisID++;
		}
		
		// we finish collecting after three debris
		if (debrisID > 3)
		{
			state = 1;
		}
	}

//END::PAGE::Debris
//BEGIN::PAGE::main
	int state; 
	int time;
	int player;
	ZRState myState;
	ZRState enemyState;
	ZRState cometState;
	int debrisPlayer;
	bool haveDebris;
	int debrisID;
	float loc[3];
	
	void init()
	{
	  state=0;
	  time=0;
		api.getMyZRState(myState);
	  // Are we blue or red?
		if(myState[0] > 0.0f)
		{
			player = 1.0f;	        // we are blue
		}
		else
		{
			player = -1.0f;         // we are red
		}
		debrisID = (-player+1)/2; // 0 or 1 (first debris on our side)
	}
	
	void loop()
	{	
		time++;
		api.getMyZRState(myState);
		game.getCometState(0, cometState);
		
		
		if (time > 90)
		{
			state = 1;
		}
		
		
		if (state==0)
		{
			collect(debrisID); // uses Lasso
		}
		
		if (state==1)
		{
			comet();	 // just a simple comet-deflection method based on gravitation
		}
			
	}

//END::PAGE::main
//BEGIN::PAGE::Utility
	// determines distance between two objects
	float getDistance(float point1[], float point2[])
	{
		float result[3];
		mathVecSubtract(result, point2, point1, 3);
		return mathVecMagnitude(result, 3);
	}
	
	// flys to point with a velocity of "k"
	void setConstantVelocityTo(float point[], float k)
	{
		float velocityVector[3];
		mathVecSubtract(velocityVector, point, myState, 3);
		mathVecNormalize(velocityVector, 3);
		
		velocityVector[0] *= k;
		velocityVector[1] *= k;
		velocityVector[2] *= k;
	
		api.setVelocityTarget(velocityVector);		
	}

//END::PAGE::Utility

};

ZRUser *zruser01 = new ZRUser01;
