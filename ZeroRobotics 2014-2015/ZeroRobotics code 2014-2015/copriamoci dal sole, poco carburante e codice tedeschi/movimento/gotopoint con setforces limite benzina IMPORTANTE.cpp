void goToPoint(float *target,float maximumVelocity) {	// Alternative to the api.setPositionTarget(); The first parameter is the position you would like to get, you use the second parameter to regolate the velocity (and, so, the fuel consumption).
		float distance[3];
		float myState[12];
		api.getMyZRState(myState);
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
	
	
	MAIN:
	
	float pos[3];
void init(){
    pos[0]=0.43;
    pos[1]=0;
    pos[2]=0;
}
void loop(){
	
	goToPoint(pos, 0.05);
}
