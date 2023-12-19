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
    for (int i=0; i<dim; i++) v[i]=a[i]*k;
}

void goOnCircle(float radius,bool var,float centerX,float centerY, bool onZ){
    float Omega=0.22f;  //0.14 //0.20
    float alpha=0.22f;// 0.14 //0.20
    //float Omega = ((radius*2)/sinf(alpha/2));
    float cosalpha=cosf(alpha), sinalpha=sinf(alpha); 
    float stato[12];
    
    float centro[3] = {centerX,(onZ)? 0.0f : centerY, (onZ) ? centerY : 0.0f};
    api.getMyZRState(stato);
    float v[3], w[3];
    mathVecMult(v,stato,1,3);
    mathVecSubtract(v,v,centro,3);
    mathVecNormalize(v,3);
    int asse = (onZ) ? 2 : 1 ;
    if(!var){
        w[0]=centerX + (radius*(v[0]*cosalpha-v[1]*sinalpha));
        w[asse]=centerY + (radius*(v[0]*sinalpha+v[asse]*cosalpha));
    }else{
        w[0]=centerX + (radius*(v[1]*sinalpha+v[0]*cosalpha));
        w[asse]=centerY + (radius*(v[asse]*cosalpha-v[0]*sinalpha));
    }
    w[(onZ) ? 1 : 2]=0;

    mathVecSubtract(v,w,stato,3);
    mathVecNormalize(v,3);
    mathVecMult (v,v,Omega*radius,3);
    api.setVelocityTarget(v);
}
//End page functions
//Begin page main
float targetRate[3],myState[12];
short actualProcedure;
void init(){
	api.getMyZRState(myState);
	for(int i = 0;i<3;i++) targetRate[i] = 0.00f; //57.2958*PI/180; //1rad
    actualProcedure = 0;
}

void loop(){
    api.getMyZRState(myState);
    if(actualProcedure == 0){
        float whereToGo[3] = {-0.5, -0.5, 0.0f },velocity = 0.067;//0.06
        setConstantVelocityTo(whereToGo, velocity);
        if(getDistance(myState,whereToGo)<0.05) actualProcedure = 1;
    }
    
    if(actualProcedure == 1){
        goOnCircle(0.22,true,-0.5 ,-0.25,false);//0.25 raggio
        float whereRotate[3] = {-0.5, 0.0, 0.0f };
        float whereToGo[3] = {-0.41, -0.02, 0.0f };
        if(getDistance(myState,whereRotate)<0.05){ targetRate[2] = (57.2958*PI/180) * -1;}
        if(getDistance(myState,whereToGo)<0.05){ actualProcedure = 2;}
        
    }
    
    if(actualProcedure==2){
        DEBUG(("ci sonoo "));
        float whereToGo[3] = {-0.057f, 0.0f,  -0.5f};
        //goOnCircle(0.45f,true,-0.45f ,-0.45f,true);
        float distanceToWhereGo = getDistance(myState, whereToGo);
        float kVel = 0.025;
        float velocity = (((distanceToWhereGo*0.68f)/kVel)/100.0f);
        setConstantVelocityTo(whereToGo, velocity);
    }
    api.setAttRateTarget(targetRate);
    //goOnCircle(0.26f,true,-0.16f ,-0.46f,true);
   // goOnCircle(0.25,true,0.5f,0.2f,true);
	    
	
}


//End page main
