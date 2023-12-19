#define markDebris debrisMat[nr][0]=1103.12;debrisMat[nr][1]=1103.12;debrisMat[nr][2]=1103.12

int player,nr,state;
ZRState myState;
ZRState enemyState;
ZRState cometState;
int debrisPlayer;
bool haveDebris;
float loc[3];
float pos[3];
float debrisMat[16][3];
float initCometa[3];

void init() {
	float tmp[3];
	api.getMyZRState(myState);
	game.breakNet();
  // Are we blue or red?
	if(myState[0] > 0.0f){
		player = 1.0f;	        // we are blue
		initCometa[0] = 0.36;
		initCometa[1] = 0.8;
		initCometa[2] = 0.0;
	}
	else {
		player = -1.0f;         // we are red
		initCometa[0] = -0.36;
		initCometa[1] = 0.8;
		initCometa[2] = 0.0;
	}
	
	state = 0;
	
	for (int debrisID=0; debrisID<16; debrisID++){
		game.getDebrisLocation(debrisID, tmp);
		debrisMat[debrisID][0] = tmp[0];
		debrisMat[debrisID][1] = tmp[1];
		debrisMat[debrisID][2] = tmp[2];
	}
	
	initCometa[0] = 0.36;
	initCometa[1] = 0.8;
	initCometa[2] = 0.0;
	
	nr = nearestDebris();	
}

void loop()
{	
	api.getMyZRState(myState);
	for (int i=0; i<3; i++)
		pos[i] = myState[i];
	
	game.getCometState(0, cometState);
	
	
	if(api.getTime() < 70 || state < 2){
		collect(nr);
		if(game.haveDebris(0,nr)){
			markDebris;
			DEBUG(("Debris marked %d",nr));
			state++;
			nr = nearestDebris();
	}
	
	if(api.getTime() > 70 && api.getTime() < 90)
			api.setPositionTarget(initCometa);
	else 
			comet();
		

}

int collect(int nr){
	
	game.getDebrisLocation(nr, loc);
	DEBUG(("%d",nr));
	
	// if we have not reached the debris yet
	if(myState[1]+0.05 <= loc[1]){
		loc[1] = myState[1];
		loc[0] += 0.17f;
		// if we are far away, we fly to it
		
		if (getDistance(myState, loc) >= 0.05f)
		  api.setPositionTarget(loc);
		
		// if we are close we fly a tangent
		else {
		  loc[1] = 0.0f;
		  setConstantVelocityTo(loc, 0.05);
		}
	}
	
	// sets forces to the middle of the debris while flying a tangent if we are close enough
	// with this V we will fly in a circle around it, just like a satellite :) 
	else {
		game.startLasso(nr);
		mathVecSubtract(loc, loc, myState, 3);
		api.setForces(loc);
	}
			
	return 0;

}


void comet()
{
	
 if (api.getTime()<85)
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

// determines distance between two objects
float getDistance(float a[3], float b[3]){
	
	float lol;
	
	lol = sqrt( ((a[0]-b[0])*(a[0]-b[0])) +
							((a[1]-b[1])*(a[1]-b[1])) +
							((a[2]-b[2])*(a[2]-b[2])) );
				
	return lol;
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


int nearestDebris(){
	// Returns nearest debris using getDistance()
	// Works calculating what debris is nearest to both
	// the satellite and the comet initial coordinates
	// so we can get there before it enters the field
	int id = 0;
	
	// First condition always verified
	float minDist = 11.0;
	
	for(int i=0; i<16; i++){
		if((getDistance(debrisMat[i],pos) + getDistance(debrisMat[i],initCometa)) < minDist){
			minDist = getDistance(debrisMat[i],pos) + getDistance(debrisMat[i],initCometa);
			id = i;
		}
	}
	return id;		
}
