//Begin page functions
float setConstantVelocityTo(float whereToGo[3], float velocity){
    float velocityVector[3];
    mathVecSubtract(velocityVector, whereToGo, myState, 3);
    mathVecNormalize(velocityVector, 3);
	for(int i=0;i<3;i++)
		velocityVector[i]*=velocity;
	api.setVelocityTarget(velocityVector);
}
float getDistance(float *a, float *b){
    float distance[3];
    mathVecSubtract(distance,a,b,3);
    return mathVecMagnitude(distance,3);
}

void mathVecMult (float *v, float *a, float k, int dim){
    int i;
    for (i=0; i<dim; i++){
        v[i]=a[i]*k;
    }
}

void goOnCircle(float radius,bool var,float centerX,float centerY, bool onZ){
    float Omega=0.14f;  //0.14
   
    float alpha=0.14f;// 0.14
    //float Omega = ((radius*2)/sinf(alpha/2));
    float cosalpha=cosf(alpha), sinalpha=sinf(alpha); 
    float stato[12];
    float centro[3] = {centerX,centerY,0.0f};
    api.getMyZRState(stato);
    float v[3], w[3];
    mathVecMult(v,stato,1,3);
    mathVecSubtract(v,v,centro,3);
    mathVecNormalize(v,3);
    if(!var){
        w[0]=centerX + (radius*(v[0]*cosalpha-v[1]*sinalpha));
        w[1]=centerY + (radius*(v[0]*sinalpha+v[1]*cosalpha));
    }else{
        w[0]=centerX + (radius*(v[1]*sinalpha+v[0]*cosalpha));
        w[1]=centerY + (radius*(v[1]*cosalpha-v[0]*sinalpha));
    }
    w[2]=0;

    mathVecSubtract(v,w,stato,3);
    mathVecNormalize(v,3);
    mathVecMult (v,v,Omega*radius,3);
    api.setVelocityTarget(v);
}
//End page functions
//Begin page main
float targetRate[3],myState[12];
bool isBlue,donefirstLegge;
short actualProcedure;
void init(){
	targetRate[0] = 0.00f;
	targetRate[1] = 0.00f;
	targetRate[2] = 0.00f;//57.2958*PI/180; //1rad
	api.getMyZRState(myState);
	actualProcedure = 0;
	isBlue = (myState[1]<0);
	donefirstLegge = false;
}

void loop(){
	api.getMyZRState(myState);
	DEBUG(("isBlue: %d x: %f  y: %f",isBlue,myState[0],myState[1]));
	float whereChangeLeggeOraria[3] = {-0.5 * (isBlue?1:-1),0.0f,0.0f};
	switch(actualProcedure){
	    case 0:
	        goOnCircle(0.343,true,-0.25 * (isBlue?1:-1),-0.25 * (isBlue?1:-1),false);
	        if(getDistance(myState,whereChangeLeggeOraria)<0.05){
	            actualProcedure = 1;
	            targetRate[2] = 57.2958*PI/180; //1rad/s
	        } 
	    break;
	    case 1:
	        float whereToGo[3] = {-0.04 * (isBlue?1:-1),-0.04 * (isBlue?1:-1),-0.5 * (isBlue?1:-1)};
	        float kVel = 0.04,velocity = (((getDistance(myState, whereToGo)*0.68f)/kVel)/100.0f);
	        setConstantVelocityTo(whereToGo, velocity);
        	DEBUG(("non eseguo"));
	    break;
	}
/*	if(getDistance(myState,whereChangeLeggeOraria)<0.05) donefirstLegge = true;
	
	(!donefirstLegge)? goOnCircle(0.343,true,-0.25 * (isBlue?1:-1),-0.25 * (isBlue?1:-1)) : DEBUG(("non eseguo")); ;
	*/
	//rotation
	api.setAttRateTarget(targetRate);
}

//End page main
