void setConstantVelocityTo(float point[], float k){
	    float myState[12];
		float velocityVector[3];
		api.getMyZRState(myState);
		mathVecSubtract(velocityVector, point, myState, 3);
		mathVecNormalize(velocityVector, 3);
               
		velocityVector[0] *= k;	
		velocityVector[1] *= k;
		velocityVector[2] *= k;
       
		api.setVelocityTarget(velocityVector);
	
}



void copriamoci_dal_sole(float velocity){
    float posizione[3];
    posizione[0]=0.5;
    posizione[1]=0;
    posizione[2]=0;
    setConstantVelocityTo(posizione,velocity);
    
}






