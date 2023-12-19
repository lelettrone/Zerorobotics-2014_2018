//Begin page funzioni
void goTarget(float Target[3]){ //MADE BY RICCIO-ALBERTO 01/07/2015
    float myState[12];
    api.getMyZRState(myState);
    float myVelocity[]={myState[3],myState[4],myState[5]};
    float myPosition[]={myState[0],myState[1],myState[2]};
    float idealVector[3],realVector[3];
    mathVecSubtract(idealVector,Target,myPosition,3);
    mathVecSubtract(realVector,idealVector,myVelocity,3);
    mathVecNormalize(realVector,3);
    mathVecMult(realVector,realVector,0.075f,3);
    api.setVelocityTarget(realVector);
}

void setConstantVelocityTo(float point[], float k)
{
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
void mathVecMult (float *v, float *a, float k, int dim){
    int i;
    for (i=0; i<dim; i++){
        v[i]=a[i]*k;
    }
}

/*void mygoOnCircle(float Ro){ //MADE BY CESARANO 
    float doveandare[3];
    float w[3];
    myalpha=0.14f;
    doveandare[0]=Ro*(sinf(myalpha));
    doveandare[1]=Ro*(cosf(myalpha));
    doveandare[2]=0.0f;
    mathVecSubtract(w,doveandare,myState,3);
    mathVecNormalize(w,3);
    mathVecMult (w,w,0.07f,3);
    api.setVelocityTarget(w);
}*/

void goOnCircle(float radius,bool var){
    float alpha=0.14f;// 0.14
    float cosalpha=cosf(alpha),sinalpha=sinf(alpha); 
    float stato[12];
    api.getMyZRState(stato);
    float v[3], w[3];
    mathVecMult(v,stato,1,3);
    mathVecNormalize(v,3);
    if(!var){
    w[0]=radius*(v[0]*cosalpha-v[1]*sinalpha);
    w[1]=radius*(v[0]*sinalpha+v[1]*cosalpha);
    }else{
    w[0]=radius*(v[1]*sinalpha+v[0]*cosalpha);
    w[1]=radius*(v[1]*cosalpha-v[0]*sinalpha);
    }
    w[2]=0;
    mathVecSubtract(v,w,stato,3);
    mathVecNormalize(v,3);
    mathVecMult (v,v,Omega*radius,3);
    api.setVelocityTarget(v);
}
float getDistance(float a[13], float b[13])
{
	float distance = sqrt(((a[0]-b[0])*(a[0]-b[0]))+
					((a[1]-b[1])*(a[1]-b[1])) +
					((a[2]-b[2])*(a[2]-b[2])));
	return distance;
}


bool getColor(){//MADE BY CESARANO
    return ((myState[y] <0.0f) ? 0 : 1);
}

//End page funzioni
//Begin page main
enum Axis{x,y,z}; 
float myState[12],Points[2][4][3],forza[3],Radius,Omega;
int Index,step;
bool fatto;
void init(){
    api.getMyZRState(myState);
    Omega=0.1350f; //0.1356f
    //Radius=0.6435f; //0.65 0.646 6435    0.646f
    
    Index = getColor(); //get color sphere
    step=0;
    (Index == 0 ? Radius=0.6435f : Radius=0.64335f);
    fatto=false;
    
    Points[0][0][x]=-0.19f; 
    Points[0][0][y]=0.70f;   //terzo punto sphere blue
    Points[0][0][z]=0.00f;
    
    Points[0][1][x]=0.5f;
    Points[0][1][y]=-0.5f;   //riduzione raggio sphere blue
    Points[0][1][z]=0.00f; 
    
    Points[0][2][x]=-0.0f;
    Points[0][2][y]=1.0f;
    Points[0][2][z]=0.0f;
    
    Points[0][3][x]=0.00f;
    Points[0][3][y]=-0.37;
    Points[0][3][z]=0.0f;
    
    Points[1][0][x]=0.19f;  //terzo punto sphere rosso
    Points[1][0][y]=-0.70f;
    Points[1][0][z]=0.00f;
    
    Points[1][1][x]=-0.5f; 
    Points[1][1][y]=0.5f;
    Points[1][1][z]=0.00f;
    
    Points[1][2][x]=-0.0f;
    Points[1][2][y]=-1.0f;
    Points[1][2][z]=0.0f;
    
    Points[1][3][x]=0.0f;
    Points[1][3][y]=0.37;
    Points[1][3][z]=0.0f;
}

void loop(){
    api.getMyZRState(myState);
    if(getDistance(myState,Points[Index][step])<0.05f){ 
       Omega=0.1390f;
       (Index == 0 ? Radius=0.6655f : Radius=0.6650f);
    }
    
    if(getDistance(myState,Points[Index][step+1])<0.115f)// 125  0.078 0.115f
       fatto=true;
       
    (fatto ? goTarget(Points[Index][step+3]) : goOnCircle(Radius,true));
}
//End page main
