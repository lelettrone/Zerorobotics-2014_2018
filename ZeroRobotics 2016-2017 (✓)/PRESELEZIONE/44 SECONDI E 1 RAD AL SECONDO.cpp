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

void goTowards(float nextPosition[3],float myPosition[3],float myVelocity[3]){
    float velocity[3];
    
    mathVecSubtract(velocity,nextPosition,myPosition,3);
    mathVecSubtract(velocity,velocity,myVelocity,3);
    mathVecMult(velocity,velocity,30,3);
    api.setVelocityTarget(velocity);
}

void goOnCircle(float Omega,float radius,bool var,float centerX,float centerY, bool onZ){
    //float Omega=0.25f;  //0.14 //0.20
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
    //goTowards(w,stato,stato+3);
    mathVecSubtract(v,w,stato,3);
    mathVecNormalize(v,3);
    mathVecMult (v,v,Omega*radius,3);
    api.setVelocityTarget(v);
}
//End page functions
//Begin page main
float targetRate[3],myState[12];
bool isBlue;
short actualProcedure;
void init(){
	api.getMyZRState(myState);
	for(int i = 0;i<3;i++) targetRate[i] = 0.00f; //57.2958*PI/180; //1rad
    actualProcedure = 0;
    isBlue = (myState[1]<0);
}

void loop(){
    api.getMyZRState(myState);
    if(actualProcedure == 0){
        //float whereToGo[3] = {-0.46* (isBlue?1:-1), -0.5* (isBlue?1:-1), 0.0f },velocity = 0.072;//0.06
        float whereToGo[3] = {-0.5* (isBlue?1:-1), -0.51* (isBlue?1:-1), 0.0f },velocity = 0.072;//0.06
        setConstantVelocityTo(whereToGo, velocity);
        if(getDistance(myState,whereToGo)<0.05) actualProcedure = 1;
    }
    if(actualProcedure == 1){
        goOnCircle(0.25,0.22,true,-0.5* (isBlue?1:-1) ,-0.25* (isBlue?1:-1),false);//0.25 raggio
        float whereRotate[3] = {-0.5* (isBlue?1:-1), 0.0, 0.0f };
        float whereToGo[3] = {-0.41* (isBlue?1:-1), -0.02* (isBlue?1:-1), 0.0f };
        if(getDistance(myState,whereRotate)<0.05){ targetRate[2] = (58.5*PI/180) * -1;} //58.2958
        if(getDistance(myState,whereToGo)<0.05){ actualProcedure = 2;}
    }
    if(actualProcedure==2){
        float whereToGo[3] = {-0.082f* (isBlue?1:-1), 0.0f,  -0.5f* (isBlue?1:-1)};
        float distanceToWhereGo = getDistance(myState, whereToGo);
        float kVel = 0.025,velocity = (((distanceToWhereGo*0.68f)/kVel)/100.0f);
        setConstantVelocityTo(whereToGo, velocity);
       //goOnCircle(0.30,0.428,true,-0.46* (isBlue?1:-1) ,-0.46* (isBlue?1:-1),true);//0.25 raggio
    }
    api.setAttRateTarget(targetRate);
}


//End page main
