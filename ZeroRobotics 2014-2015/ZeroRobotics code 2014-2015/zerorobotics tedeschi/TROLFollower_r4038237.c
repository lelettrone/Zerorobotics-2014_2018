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
Project: TROLFollower
Game: CosmoSPHERES_ALLIANCE
Created by: martina.massucco
Last Modified: 2013-12-16 18:15:03.0
*/
class ZRUser01 : public ZRUser
{
public:

//BEGIN::PAGE::comet
	float velTar[3];
	float posTar[3];
	float cometPosition[3];
	ZRState enemyState;
	float pos[3];
	float att[3];
	
	void gravity()
	{ 
	   		float velTar[3];										
	
										
			// we use the velocity of the comet as our velocity to fly the same route
			// we want to be x = 0,14 + cs, y = cs, z = cs - 0.14 to the comet to have good gravitiy influence
			// if we are not on that point, we add a little bit to our velocity to get there 
		
				   game.getCometState(0, cometState);
				   api.getMyZRState(myState);
	  
		if (time < 90) {cometState[1]-=0.2;}
		
	         velTar[1] = cometState[4] + 0.1*(cometState[1] - myState[1] + 0.06);
			  	 velTar[2] = cometState[5]+ 0.1*(cometState[2] - myState[2]);
			  	 velTar[0] = cometState[3]+ 0.1*(cometState[0] - myState[0] + 0.185*player);
	       
				
	         if(getDistance(myState,cometState)<0.19)		
			// if we are very close to the comet we fly away a bit for avoiding a collision
	         {
						  velTar[2] -= 0.02*player;
	            velTar[0] += 0.02*player;
	         }
	         api.setVelocityTarget(velTar);
	}
	
	
	void shoot()
	{
		/*if (time < 91)
		{
			cometState[0]=0.16*player + 0.01*(90-time)*player;
			cometState[1]=0.8+0.026*(90-time);
			cometState[2]=-0.01*(90-time)*player;
		}*/
		
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
		if(getDistance(dir,cometState)<0.0675f && time > 90) 
		{
			game.shootLaser();
		}
		DEBUG(("%f",getDistance(dir,cometState)));
		
		float miss[3];
		
		miss[0] = 0;
		miss[1] = 0;
		miss[2] = 0;
		if (game.wasRecoilActive()) 
		{
			mathVecSubtract(miss, cometState, dir, 3);
		}
		
		if(time<85)
		{
		pos[0] = -0.04*player; 
		pos[1] = 0.50f;          
		pos[2] = 0.15f * player; 
		api.setPositionTarget(pos);
		}
		else
		{
			if(time<105)
			{
				pos[0] = -0.00002f * player + 2.0*miss[0]; //////////
				pos[1] = -0.01f + 2.0*miss[1];           /////////////
				pos[2] = +0.0000005f * player + 2.0*miss[2];  ///////////
				api.setVelocityTarget(pos);
			}
			else {comet();}
		}
	}
	
	
	
	void comet()
	{	
		 setConstantVelocityTo(enemyState, 0.1);
	}

//END::PAGE::comet
//BEGIN::PAGE::DoTheFollower
	void dotheFollower()
	{
		
		if(time < 40)
			// we follow the opponent if less than 30 seconds have passed
	    {
	        setPositionTargetFaster();
				DEBUG(("I'm a follower"));
	    }
	    else
	    {
	        if (time < 70)
	        {
	            if(getDistance(oppState, itempos) < 0.2)
							// if the opponent is close to his own item we try to push him out of bounds o
	            {
	                itempos[1] -= 0.1;
	                api.setPositionTarget(itempos);
								posTar[0]=+0.25*player;
	              posTar[1]=0.78;
	              posTar[2]=0.0;
								DEBUG(("I'm a follower"));
	            }
	            else
	            {
	                itempos[0] *= -1; //itempos is our Item
	                if(getDistance(oppState, itempos) < 0.2)
									// if the opponent is close to our item we try to push him out of bounds
	                {
	                    itempos[1] -= 0.1;
	                    api.setPositionTarget(itempos);
										posTar[0]=+0.25*player;
	                  posTar[1]=0.78;
	                  posTar[2]=0.0;
										DEBUG(("I'm a follower"));
	                }
	                else
	                {	
										DEBUG(("TROL"));
										p = 0.54f;
										/*itempos[0]= -0.0*player;
										itempos[1]= 0.65;
										itempos[2]= 0;*/
										getItem(itempos);
											// we redirect the comet away from our home base using an extra method
	                }
	            }
	        }
	        else gravity(); //gravitycomet(posTar);
					// we redirect the comet away from our home base using an extra method
	    }
	}

//END::PAGE::DoTheFollower
//BEGIN::PAGE::KillTheFollower
	float deloc [3];
	int closestDebris;
	float distance;
	
	void killtheFollower()
	{
			if(time < 50)
	    {
	    itempos[0] = 0;   
	  	itempos[1] = 0;
	  	itempos[2] = 0;	
			api.setPositionTarget(itempos);
	    }
	    else
	    {
				posTar[0]=+0.25*player;
	  		posTar[1]=0.65;
	  		posTar[2]=0.0;
	     	gravity();
					// we redirect the comet away from our home base using an extra method
	    }	
	}

//END::PAGE::KillTheFollower
//BEGIN::PAGE::main
	int time;
	float player;
	ZRState myState;
	ZRState oppState;
	ZRState cometState;
	float itempos[3];
	int follower;
	int oppstrategy;
	float p;
	
	 
	void init()
	{
	  	time = 0;
	  	api.getMyZRState(myState);       
																		 // for finding out if we are  blue or red  
	  	if(myState[0] > 0.0f)            
	  	{
	      	player = 1.0f;             // we are blue
	  	}
	  	else
	  	{
	      	player = -1.0f;            // we are red
	  	}
			follower = 1;
			oppstrategy = 0;
			posTar[0]=+0.25*player;
	  	posTar[1]=0.65;
	  	posTar[2]=0.0;
			p = 0.5F;
		
		
		add = 0.0f;
	}
	
	void loop()
	{   
	    time++;												 // increases time every second
	    api.getMyZRState(myState);
	    api.getOtherZRState(oppState);
	    game.getCometState(0, cometState);
	    
			/*itempos[0] = - 0.45 * player;   // Position of the Opponents Item
	  	itempos[1] = 0.65;
	  	itempos[2] = 0;*/
			itempos[0]=0.5*player;
			itempos[1]=0.65;
			itempos[2]=0;	
		
			spy();
			if(time == 7){
				if(follower == 1)
				{
					oppstrategy = 1;
				}
			}
		
		
		if (oppstrategy == 0)
		{
			dotheFollower();
		}
		
		if (oppstrategy == 1)
		{
			killtheFollower();
		}
			    
	}

//END::PAGE::main
//BEGIN::PAGE::movement
	
	
	void setPositionTargetFaster()
		// faster than setPositionTarget, always flies in the direction of the opponent
	{	
		api.getMyZRState(myState);
		api.getOtherZRState(oppState);
		if(getDistance(myState,oppState)<0.2)
		{
		}
		else{
		oppState[0]=oppState[0]+oppState[3]*6;
		oppState[1]=oppState[1]+oppState[4]*6;
		oppState[2]=oppState[2]+oppState[5]*6;
		}
		// the if-clauses ensure that we do not fly out of bounds while following the opponent
		if(oppState[0] > p) //0.54
	    {
	        oppState[0] = p;
	    }
	  if(oppState[0] < -p) //-0.54
	    { 
	        oppState[0] = -p;
	    }
	  if(oppState[1] > 0.7)
	    {
	        oppState[1] = 0.7;
	    }
	  if(oppState[1] < -0.7)
	    {
	        oppState[1] = -0.7;
	    }
	  if(oppState[2] > p) //0.54
	    {
	        oppState[2] = p;
	    }
	  if(oppState[2] < -p)//-0.54
	    {
	        oppState[2] = -p;
	    }
	 
		api.setPositionTarget(oppState);
	}
	float add;
	
	
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
		flyto[0] = 0.4*flyto[0];
		flyto[1] = 0.4*flyto[1];
		flyto[2] = 0.4*flyto[2];
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
		DEBUG(("I'm getting your item :P"));
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
			
			if (getDistance(myState, enemyState) < 0.2)
			{
				if (myState[0]!= round(myState[0], 0.6)||myState[1]!= round(myState[1], 0.75))
				{
					itempos[0]= 0.45*player; //0.45*player;
					itempos[1]=0.6;
				}
			}
				
	  }
	  else
	  {
			counterForCollectItem = 0;
			setZAttRate(0.0f);
			DEBUG(("don't hurt me please :("));
	  }
		api.setPositionTarget(itempos);
		//way(itempos);
	}

//END::PAGE::movement
//BEGIN::PAGE::utility
	float getDistance(float point1[], float point2[])
	// determines the distance between two points
	{
		float result[3];
		mathVecSubtract(result, point2, point1, 3);
		return mathVecMagnitude(result, 3);
	}
	
	void spy()
	{
		api.getOtherZRState(oppState);
		api.getOtherZRState(myState);
		float middle[3]; // the middlepoint between us and the enemy in the start position
		middle[0] = 0;
		middle[1] = -0.65;
		middle[2] = 0;
		if (time < 20)
		{
			// determines whether either we or the oponent move away from this point
			if (getDistance(middle, myState) > 0.2 || getDistance(middle, oppState) > 0.2)
			{
				// if they have, they cant be a follower, since otherwise they would move towards us
				follower = 0;
			}
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
	float round(float number, float limit)
	{
		if (number > limit)
			return limit;
		if (number < -limit)
			return -limit;
		return number;
	}

//END::PAGE::utility

};

ZRUser *zruser01 = new ZRUser01;
