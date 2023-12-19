//Begin page funzioni
//Begin page funzioni
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
void mathVecMult (float *v, float *a, float k, int dim){
    int i;
    for (i=0; i<dim; i++){
        v[i]=a[i]*k;
    }
}
void goOnCircle(float radius,bool var){
    float Omega=0.14f;  //0.14
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
bool getColor(){
    float Blue[]={0.00f, -0.5f, 0.00f};
    float Red[]={0.00f, 0.5f, 0.00f};
    if(getDistance(myState,Blue)<=0.05f){
        return 0;
    }else if(getDistance(myState,Red)<=0.05f){
        return 1;
    }
}
//End page funzioni
//End page funzioni
//Begin page main
//Begin page main
enum Axis{x,y,z};
float myState[12];
float Points[2][2][3];
float Radius;
int Index;
int step;
void init(){
    Radius=0.646f; //0.65
    Points[0][1][x]=0.5f;
    Points[0][1][y]=-0.5f;//riduzione raggio sphere blue
    Points[0][1][z]=0.00f;
    
    Points[1][1][x]=-0.5f; 
    Points[1][1][y]=0.5f;//riduzione raggio sphere rosso
    Points[1][1][z]=0.00f;
    
    Points[0][0][x]=-0.19f; //terzo punto sphere blue
    Points[0][0][y]=0.70f;
    Points[0][0][z]=0.00f;
     
    Points[1][0][x]=0.19f;  //terzo punto sphere rosso
    Points[1][0][y]=-0.70f;
    Points[1][0][z]=0.00f;
    
    api.getMyZRState(myState); 
    Index = getColor();
    step=0;
    DEBUG(("Color: %d",Index));
    
}
void loop(){
    api.getMyZRState(myState);
     if(getDistance(myState,Points[Index][step])<0.05){
       Radius=0.67f; //0.45 oppure 0.457
      DEBUG(("Cambio Terzo Punto: %d",Index));
    }
 
    if(getDistance(myState,Points[Index][step+1])<0.05){
       Radius=0.475f; //0.45 oppure 0.457
      DEBUG(("Cambio: %d",Index));
    }
     goOnCircle(Radius,true);
}

//End page main
