#define markDebris {debrisMat[nearestDebris()][0] = 1103.12;debrisMat[nearestDebris()][1] = 1103.12;debrisMat[nearestDebris()][2] = 1103.12;}
#define blue false
#define red true

ZRState myState;					// Used for getMyZRState()
float pos[3];						// Satellites position
float debrisMat[16][3];		// Matrix containing position of debris						
float cometa[6];					// Holds Comets initial position
float initCometa[3];			// Where we position before comet runs in the field
float center[3];
int nr;	
bool  color;							// True if Blue, False if Red

void init(){
	float tmp[3];
	
	// Fills debrisMat, to be done only once
	for (int debrisID=0; debrisID<16; debrisID++){
		game.getDebrisLocation(debrisID, tmp);
		debrisMat[debrisID][0] = tmp[0];
		debrisMat[debrisID][1] = tmp[1];
		debrisMat[debrisID][2] = tmp[2];
	}
	
	center[0] = 0.0;
	center[1] = 0.0;
	center[2] = 0.0;
	initCometa[0] = 0.32;
	initCometa[1] = 0.8;
	initCometa[2] = 0.0;
	
	if (myState[0]>0){
		color = blue;
	} else color = false;
	
	
}


void loop(){
	
	// Updates our position stored in pos.
	api.getMyZRState(myState);	
	for (int i=0; i<3; i++)
		pos[i] = myState[i];
		
	nr = nearestDebris();
	
	// We have time to get some delicious Debris!
	if(api.getTime() < 70){
		if(myState[1]+0.05 <= debrisMat[nr][1]) {
			debrisMat[nr][1] = myState[1];
			debrisMat[nr][0] += 0.17f;
			
			// if we are far away, we fly to it
			if (getDistance(myState, debrisMat[nr]) >= 0.05f) {
				api.setPositionTarget(debrisMat[nr]);
			} else { 
				//if we are close we fly a tangent
				debrisMat[nr][1] = 0.0f;
				setConstantVelocityTo(debrisMat[nr], 0.05);
			}
		} else{
			while(!game.haveDebris(0,nr){
				lassobaby(nr);
				DEBUG(("Called lassobaby()"));
			}
			if(game.haveDebris(0,nr))
				markDebris		//That debris is never chosen again
		} 
	} 
	
	else {
		// Time is running out, we get in position
		
		if(api.getTime() > 70 && api.getTime() < 90) {
			api.setPositionTarget(initCometa);
		} 
		else {
			// If we remain in the field, follow the comet
			if(pos[0] < 0.50 && pos[1] > -0.65 && pos[2] < 0.50){
	  		game.getCometState(0,cometa);
				cometa[0] += 0.20;
				api.setPositionTarget(cometa);
			}
			else {
				api.setPositionTarget(center);
			}
			
		}
	}
}

// Simply calculates distance from a to b using Euclidean distance 
float getDistance(float a[3], float b[3]){
	
	float lol;
	
	lol = sqrt( ((a[0]-b[0])*(a[0]-b[0])) +
							((a[1]-b[1])*(a[1]-b[1])) +
							((a[2]-b[2])*(a[2]-b[2])) );
				
	return lol;
}


int nearestDebris(){
	// Returns nearest debris using getDistance()
	// Works calculating what debris is nearest to both
	// the satellite and the comet initial coordinates
	// so we can get there before it enters the field
	
	int id = 0;
	float minDist = 11.0; // Why 11? field cannot be that big so first 
												// condition is always verified, also is my anniversary :)
	
	for(int i=0; i<16; i++){
		if((getDistance(debrisMat[i],pos) + getDistance(debrisMat[i],initCometa)) < minDist){
			minDist = getDistance(debrisMat[i],pos) + getDistance(debrisMat[i],initCometa);
			id = i;
			break;
		}
	}

	return id;		
}


void lassobaby(int nr) {
		game.startLasso(nr);
		mathVecSubtract(debrisMat[nr], debrisMat[nr], myState, 3);
		api.setForces(debrisMat[nr]);
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
