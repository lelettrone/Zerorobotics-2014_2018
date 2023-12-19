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
Project: shoot and grav
Game: CosmoSPHERES_ALLIANCE
Created by: davidalcer
Last Modified: 2013-11-29 20:01:39.0
*/
class ZRUser01 : public ZRUser
{
public:

//BEGIN::PAGE::Comet
	float pos[3];
	
	
	void shoot()
	{
		if (time < 90)
		{
			cometState[0]=0.01*(90-time)*player;
			cometState[1]=0.8+0.026*(90-time);
			cometState[2]=-0.01*(90-time)*player;
		}
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
		
		
		
	    att[0]=cometState[0]-myState[0] + 3.5*(cometState[3]-myState[3]);  /////
			att[1]=cometState[1]-myState[1] + 3.5*(cometState[4]-myState[4]);  /////
			att[2]=cometState[2]-myState[2] + 3.5*(cometState[5]-myState[5]);   ////
		  
	
		  //att[0]=0.01*(cometState[0]-myState[0]) + 1*(cometState[3]-myState[3])/getDistance(cometState,myState); 
			//att[1]=0.01*(cometState[1]-myState[1]) + 1*(cometState[4]-myState[4])/getDistance(cometState,myState);
		  //att[2]=0.01*(cometState[2]-myState[2]) + 1*(cometState[5]-myState[5])/getDistance(cometState,myState); 
		
		
		  //att[0]=(diff[0]*diff[1]-diff[1]*diff[0]); 
			//att[1]=(diff[1]*diff[2]-diff[2]*diff[1]);
		  //att[2]=(diff[2]*diff[0]-diff[0]*diff[2]);
		
			api.setAttitudeTarget(att);
		//DEBUG(("%f", precise));
		//DEBUG(("%f       ", cometState[4]));
		//game.getCometState(0, cometState);
		//DEBUG(("%f", getDistance(dir,cometState)));
		//DEBUG(("%f     ", cometState[4]));
		if(getDistance(dir,cometState)<0.05f && time > 89) 
		{
			game.shootLaser();
		}
		
		if(time<90)
		{
		pos[0] = 0.35f * player; //////////////
		pos[1] = 0.55f;          //////////////////
		pos[2] = 0.10f * player; //////////////
		api.setPositionTarget(pos);
		}
		else
		{
			if(time<95)           //////////// 
			{
				pos[0] = -0.008f * player; //////////
				pos[1] = -0.015f;           /////////////
				pos[2] = +0.01f * player;  ///////////
				api.setVelocityTarget(pos);
			}
			else {comet();}
		}
	}
	
	
	
	void comet()
	{	
		float velTar[3];
		  game.getCometState(0, cometState);
				   api.getMyZRState(myState);
	         velTar[0] = cometState[3] + 0.1*(cometState[0] - myState[0] - 0.12*player); //////
	         velTar[1] = cometState[4] + 0.1*(cometState[1] - myState[1] + 0.05);        /////////
	         velTar[2] = cometState[5] + 0.1*(cometState[2] - myState[2] - 0.12*player);  ////////
				
				
	         if(getDistance(myState,cometState)<0.19)		
			// if we are very close to the comet we fly away a bit for avoiding a collision
	         {
	            velTar[0] -= 0.005*player;
						  velTar[2] -= 0.005*player;
	         }
	         api.setVelocityTarget(velTar);
	
	}

//END::PAGE::Comet
//BEGIN::PAGE::Funktionen
	float getDistance(float point1[], float point2[])
	{
		float result[3];
		mathVecSubtract(result, point2, point1, 3);
		return mathVecMagnitude(result, 3);
	}
	
	float max(float x, float y)
	{
		if(x > y)
		{
			return x;
		}
		
		return y;
	}
	
	float min(float x, float y)
	{
		if(x < y)
		{
			return x;
		}
		return y;
	}
	float abs(float number)
	{
		if(number>0)
		{
			return number;
		}
		else
		{
			number=-number; return number;
		}
	}
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

//END::PAGE::Funktionen
//BEGIN::PAGE::main
	int zustand;
	int time;
	int player;
	ZRState myState;
	float att[3];
	ZRState enemyState;
	int debrisPlayer;
	float precise;
	ZRState cometState;
	
	//float velTarget[3];
	
	bool haveDebris;
	//bool weAreBlue;
	
	void init()
	{
	  zustand=0;
	  time=0;
	  player=0;
		add = 0.0f;
	}
	
	void loop()
	{	
		time++;
		api.getMyZRState(myState);	
		api.getOtherZRState(enemyState);
		game.getCometState(0, cometState);
		if(player == 0.0f)
		{
			if(myState[0] > 0.0f)
			{
				player = 1.0f;	
			}
			else
			{
				player = -1.0f;
			}
			//debrisID = (-player+1)/2;
			//debrisPlayer = debrisID;
			DEBUG(("%f",player+0.0f));
		}
		
		
		
		if (game.haveItem(0,0.5-0.5*player))
		{
			zustand = 2;
		}
		
		if (zustand==0)
		{
			itempos[0]=0.5*player;
		itempos[1]=0.65;
		itempos[2]=0;
			getItem(itempos);
		}
				
		if (zustand==2)
		{
			shoot();	
		}
			
	}

//END::PAGE::main
//BEGIN::PAGE::Way to item
	float add;
	float itempos[3];
	
	void setZAttRate(float z)
	{
		float attitudeRate[3] = {0.0f, 0.0f, z};
		api.setAttRateTarget(attitudeRate);
	}
	
	void way (float* target)
	{	
		
		float flyto[3];
		mathVecSubtract(flyto, target, myState, 3);
		float deloc[3];
		mathVecNormalize(flyto, 3);
		flyto[0] = 0.8*flyto[0];
		flyto[1] = 0.8*flyto[1];
		flyto[2] = 0.8*flyto[2];
		for(int i=0; i<16; i++)
		{
			float xflyto[3];
			
			mathVecSubtract(xflyto, target, myState, 3);
			mathVecNormalize(xflyto, 3);
			xflyto[0] = 0.2*xflyto[0];
			xflyto[1] = 0.2*xflyto[1];
			xflyto[2] = 0.2*xflyto[2];
			mathVecAdd(xflyto, xflyto, myState, 3);
			if(getDistance(xflyto, deloc)<0.18)
			{
				if(xflyto[2]<deloc[2])
				{
					add = -0.4+getDistance(xflyto, deloc);
				}
				else
				{
					add = 0.4-getDistance(xflyto, deloc);
				}
				break;
			}
			
			game.getDebrisLocation(i, deloc);
		
			mathVecSubtract(xflyto, target, myState, 3);
			mathVecNormalize(xflyto, 3);
			xflyto[0] = 0.4*xflyto[0];
			xflyto[1] = 0.4*xflyto[1];
			xflyto[2] = 0.4*xflyto[2];
			mathVecAdd(xflyto, xflyto, myState, 3);
			if(getDistance(xflyto, deloc)<0.11)
			{
				if(xflyto[2]<deloc[2])
				{
					add = -0.3+getDistance(xflyto, deloc);
				}
				else
				{
					add = 0.3-getDistance(xflyto, deloc);
				}
				break;
			}
			
			if(getDistance(myState, deloc)<0.18)
			{
				if(myState[2]<deloc[2])
				{
					add = -0.3+getDistance(myState, deloc);
				}
				else
				{
					add = 0.3-getDistance(myState, deloc);
				}
				break;
			}
	
		}
		flyto[2] += add;
		if(add>0)
		{
			add -= 0.025;
		}
		else
		{
			add += 0.025;
		}
		if(getDistance(myState,target)<0.4)
		{
			api.setPositionTarget(target);
		}
		else
		{
			mathVecAdd(flyto, myState, flyto, 3);
			setConstantVelocityTo(flyto,0.04);
		}
	}
	
	
	int counterForCollectItem;
	
	void getItem(float* position)
		
	{	
		way(itempos);
		if( getDistance(myState, position) < 0.045f && mathVecMagnitude(&myState[3], 3) < 0.01f)
	  {
			if(counterForCollectItem > 10 && counterForCollectItem < 20)
			{
				setZAttRate(0.0f);
			}
	  	else
			{
				setZAttRate(0.4f);
			}
			counterForCollectItem++;
	  }
	  else
	  {
			counterForCollectItem = 0;
			setZAttRate(0.0f);
	  }
	}

//END::PAGE::Way to item

};

ZRUser *zruser01 = new ZRUser01;
